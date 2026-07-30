/*
 * UART-driven voice recorder (dictaphone) - RAM buffer version.
 *
 * Target : dsPIC33CK64MC105 Curiosity Nano + Curiosity Nano Explorer
 * FCY    : 100 MHz   |   Toolchain : XC16 + MCC Melody
 *
 * Timing : SCCP1 ("Timer1") period = 24999 -> 250 us -> 4 kHz sample rate.
 *          If you change the period in MCC, change SAMPLE_RATE below too.
 *
 * Audio  : 8-bit unsigned mono, stored raw in RAM (no EEPROM in this step).
 *
 * A 500 ms full-scale beep is emitted at startup. It proves, before you say
 * a single word into the microphone, that power, SPI, the DAC and the audio
 * amplifier are all alive. Because StartBeep() is blocking, the "Ready."
 * banner below is only printed AFTER the beep has finished: seeing the
 * banner means the beep really was generated.
 *
 * UART1 commands (115200 8N1), one per line, terminated by CR or LF:
 *   REC     start recording (stops by itself when the buffer is full)
 *   STOP    stop recording, or abort playback
 *   PLAY    replay the last recording
 *   STATUS  print state, recorded length and duration
 *   LEVEL   print min / max / average of the recorded buffer (mic sanity check)
 *   TONE    write a fixed mid-scale value to the DAC (output path sanity check)
 *   BEEP    re-run the startup beep on demand
 */

#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/system/pins.h"
#include "mcc_generated_files/spi_host/spi1.h"
#include "mcc_generated_files/adc/adc1.h"
#include "mcc_generated_files/timer/sccp1.h"
#include "mcc_generated_files/uart/uart1.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#define FCY 100000000UL
#include <libpic30.h>

/* ------------------------------------------------------------------ */
/* Audio format                                                        */
/* ------------------------------------------------------------------ */
#define SAMPLE_RATE   4000U       /* must match the SCCP1 period set in MCC */
#define BUFFER_SIZE   6000U       /* 1.5 s at 4 kHz. The device has only     */
                                  /* 8 kB of RAM: leave ~2 kB for the stack. */
static uint8_t audioBuf[BUFFER_SIZE];

/* ------------------------------------------------------------------ */
/* DAC (MCP4821)                                                       */
/* ------------------------------------------------------------------ */
#define DAC_CTRL_BITS  0x3000U    /* channel A, gain x1 (0..2.048 V), active */
#define DAC_MIDSCALE   2048U      /* 0 V of audio = half of the DAC range    */

static void DAC_Write12(uint16_t value)
{
    uint16_t command = DAC_CTRL_BITS | (value & 0x0FFFU);

    DAC_CS_SetLow();
    (void)SPI1_ByteExchange((uint8_t)(command >> 8));
    (void)SPI1_ByteExchange((uint8_t)(command & 0xFFU));
    DAC_CS_SetHigh();
}

/* ------------------------------------------------------------------ */
/* Microphone (ADC1, shared core, AN7 = MIC_OUT, common software trigger) */
/* ------------------------------------------------------------------ */
/*
 * Correct Microchip sequence for a software-triggered shared-core channel:
 *   1. arm the common trigger
 *   2. wait for the channel ready flag
 *   3. release the trigger (it is level sensitive, not a pulse)
 *   4. read the result buffer
 * The guard counter makes sure a hardware problem can never lock the ISR.
 * Total cost is about 1.5 us, i.e. under 1 % of the 250 us tick.
 */
static uint16_t MIC_Read(void)
{
    uint16_t guard = 0U;

    ADC1_SoftwareTriggerEnable();
    while (!ADC1_IsConversionComplete(MIC_OUT) && (guard < 1000U))
    {
        guard++;
    }
    ADC1_SoftwareTriggerDisable();

    return ADC1_ConversionResultGet(MIC_OUT);
}

/* ------------------------------------------------------------------ */
/* Startup / cable test beep                                           */
/* ------------------------------------------------------------------ */
/*
 * Full-scale swing on purpose: this is the same amplitude as the step-2
 * tone that was validated by ear. A reduced swing is easily inaudible
 * when the amplifier gain knob is not turned up.
 */
#define BEEP_HALF_PERIOD_TICKS  5U     /* 4000 / (2*5) = 400 Hz            */
#define BEEP_DURATION_TICKS     2000U  /* 2000 * 250 us = 500 ms           */
#define BEEP_LOW                0U     /* full-scale square wave: 4.1 Vpp  */
#define BEEP_HIGH               4095U  /* after the amplifier             */

/* ------------------------------------------------------------------ */
/* Application state                                                   */
/* ------------------------------------------------------------------ */
/*
 * Ownership rules (this is what makes the code safe without disabling
 * interrupts anywhere):
 *   - main() is the ONLY writer of recordedLength.
 *   - main() only ever moves state out of ST_IDLE; the ISR only ever
 *     moves it back to ST_IDLE.
 *   - sampleIndex is written by the ISR while running, and by main()
 *     only while state == ST_IDLE.
 * All of these are single 16-bit accesses, which are atomic on this core.
 */
enum AppState
{
    ST_IDLE = 0,
    ST_RECORDING,
    ST_PLAYING,
    ST_BEEP
};

static volatile enum AppState state          = ST_IDLE;
static volatile uint16_t      sampleIndex    = 0U;
static volatile uint16_t      recordedLength = 0U;

/* Beep counters: shared between StartBeep() and the ISR, hence volatile. */
static volatile uint16_t beepTicks = 0U;
static volatile uint16_t beepPhase = 0U;
static volatile bool     beepHigh  = false;

/* Called by the SCCP1 timeout interrupt every 250 us. */
void SampleTick(void)
{
    if (state == ST_RECORDING)
    {
        uint16_t raw = MIC_Read();                          /* 12-bit */
        audioBuf[sampleIndex] = (uint8_t)(raw >> 4);        /* -> 8-bit */
        sampleIndex++;

        if (sampleIndex >= BUFFER_SIZE)
        {
            state = ST_IDLE;                                /* buffer full */
        }
    }
    else if (state == ST_PLAYING)
    {
        DAC_Write12((uint16_t)audioBuf[sampleIndex] << 4);  /* 8 -> 12 bit */
        sampleIndex++;

        if (sampleIndex >= recordedLength)
        {
            DAC_Write12(DAC_MIDSCALE);                      /* no DC left */
            state = ST_IDLE;
        }
    }
    else if (state == ST_BEEP)
    {
        beepPhase++;
        if (beepPhase >= BEEP_HALF_PERIOD_TICKS)
        {
            beepPhase = 0U;
            beepHigh  = !beepHigh;
            DAC_Write12(beepHigh ? BEEP_HIGH : BEEP_LOW);
        }

        beepTicks++;
        if (beepTicks >= BEEP_DURATION_TICKS)
        {
            DAC_Write12(DAC_MIDSCALE);                      /* no DC left */
            state = ST_IDLE;
        }
    }
}

/*
 * Blocking on purpose: 500 ms is short, it keeps the state transitions in
 * main() simple, and it turns the banner printed afterwards into proof
 * that the beep was actually generated.
 */
static void StartBeep(void)
{
    beepPhase = 0U;
    beepHigh  = false;
    beepTicks = 0U;
    state     = ST_BEEP;

    while (state != ST_IDLE)
    {
    }
}

/* ------------------------------------------------------------------ */
/* UART helpers                                                        */
/* ------------------------------------------------------------------ */
static void UART_SendString(const char *s)
{
    while (*s != '\0')
    {
        while (!UART1_IsTxReady())
        {
        }
        UART1_Write((uint8_t)*s++);
    }
}

/* Small integer printer: avoids pulling sprintf and its stack usage in. */
static void UART_SendUint(uint32_t value)
{
    char    digits[11];
    uint8_t n = 0U;

    do
    {
        digits[n++] = (char)('0' + (value % 10U));
        value /= 10U;
    } while (value != 0U);

    while (n > 0U)
    {
        n--;
        while (!UART1_IsTxReady())
        {
        }
        UART1_Write((uint8_t)digits[n]);
    }
}

/* ------------------------------------------------------------------ */
/* Commands                                                            */
/* ------------------------------------------------------------------ */
#define LINE_WIDTH 32
static char    rxBuf[LINE_WIDTH + 1];
static uint8_t rxIdx = 0U;

static void PrintStatus(void)
{
    UART_SendString("state=");
    switch (state)
    {
        case ST_RECORDING: UART_SendString("RECORDING"); break;
        case ST_PLAYING:   UART_SendString("PLAYING");   break;
        case ST_BEEP:      UART_SendString("BEEP");      break;
        default:           UART_SendString("IDLE");      break;
    }
    UART_SendString(" index=");
    UART_SendUint(sampleIndex);
    UART_SendString(" recorded=");
    UART_SendUint(recordedLength);
    UART_SendString(" samples (");
    UART_SendUint(((uint32_t)recordedLength * 1000UL) / SAMPLE_RATE);
    UART_SendString(" ms) capacity=");
    UART_SendUint(BUFFER_SIZE);
    UART_SendString("\r\n");
}

/*
 * LEVEL is the diagnostic that tells us whether the microphone path works.
 *   spread == 0            -> the ADC is not converting at all
 *   spread of 1..3 LSB     -> the mic is alive but far too quiet (check GAIN)
 *   avg near 0 or near 255 -> the mic bias or the ADC channel is wrong
 *   spread == 255          -> clipping, lower the gain or move further away
 * A normal speaking voice should give a spread of 100 to 200 counts.
 */
static void PrintLevel(void)
{
    uint16_t i;
    uint8_t  minVal = 255U;
    uint8_t  maxVal = 0U;
    uint32_t sum    = 0UL;

    if (recordedLength == 0U)
    {
        UART_SendString("nothing recorded yet\r\n");
        return;
    }

    for (i = 0U; i < recordedLength; i++)
    {
        uint8_t s = audioBuf[i];
        if (s < minVal) { minVal = s; }
        if (s > maxVal) { maxVal = s; }
        sum += s;
    }

    UART_SendString("min=");
    UART_SendUint(minVal);
    UART_SendString(" max=");
    UART_SendUint(maxVal);
    UART_SendString(" spread=");
    UART_SendUint((uint32_t)(maxVal - minVal));
    UART_SendString(" avg=");
    UART_SendUint(sum / recordedLength);
    UART_SendString("\r\n");
}

static void HandleCommand(const char *cmd)
{
    if (strcmp(cmd, "REC") == 0)
    {
        if (state == ST_IDLE)
        {
            sampleIndex    = 0U;
            recordedLength = 0U;
            state          = ST_RECORDING;
            UART_SendString("REC start\r\n");
        }
        else
        {
            UART_SendString("busy\r\n");
        }
    }
    else if (strcmp(cmd, "STOP") == 0)
    {
        if (state == ST_RECORDING)
        {
            state = ST_IDLE;              /* stop the ISR first ...        */
            recordedLength = sampleIndex; /* ... then latch the length     */
            UART_SendString("REC stop, ");
            UART_SendUint(recordedLength);
            UART_SendString(" samples\r\n");
        }
        else if (state == ST_PLAYING)
        {
            state = ST_IDLE;
            DAC_Write12(DAC_MIDSCALE);
            UART_SendString("PLAY aborted\r\n");
        }
        else
        {
            UART_SendString("already idle\r\n");
        }
    }
    else if (strcmp(cmd, "PLAY") == 0)
    {
        if ((state == ST_IDLE) && (recordedLength > 0U))
        {
            sampleIndex = 0U;
            state       = ST_PLAYING;
            UART_SendString("PLAY start\r\n");
        }
        else if (recordedLength == 0U)
        {
            UART_SendString("nothing recorded yet\r\n");
        }
        else
        {
            UART_SendString("busy\r\n");
        }
    }
    else if (strcmp(cmd, "STATUS") == 0)
    {
        PrintStatus();
    }
    else if (strcmp(cmd, "LEVEL") == 0)
    {
        PrintLevel();
    }
    else if (strcmp(cmd, "TONE") == 0)
    {
        /* Output path check: forces a steady 1.024 V on the DAC output. */
        if (state == ST_IDLE)
        {
            DAC_Write12(DAC_MIDSCALE);
            UART_SendString("DAC set to mid-scale\r\n");
        }
        else
        {
            UART_SendString("busy\r\n");
        }
    }
    else if (strcmp(cmd, "BEEP") == 0)
    {
        if (state == ST_IDLE)
        {
            StartBeep();
            UART_SendString("beep done\r\n");
        }
        else
        {
            UART_SendString("busy\r\n");
        }
    }
    else
    {
        UART_SendString("commands: REC STOP PLAY STATUS LEVEL TONE BEEP\r\n");
    }
}

/* ------------------------------------------------------------------ */
/* Main                                                               */
/* ------------------------------------------------------------------ */
int main(void)
{
    enum AppState prevState = ST_IDLE;

    SYSTEM_Initialize();

    SPKR_EN_SetHigh();
    DAC_CS_SetHigh();
    ADC1_Enable();
    SPI1_Open(HOST_CONFIG);
    DAC_Write12(DAC_MIDSCALE);

    Timer1_TimeoutCallbackRegister(SampleTick);
    Timer1_Start();

    /* Blocking: if the banner below appears, the beep really happened. */
    StartBeep();

    UART_SendString("\r\nReady. Commands: REC STOP PLAY STATUS LEVEL TONE BEEP\r\n");

    while (1)
    {
        /* ---- UART line assembler (polled, no delay: never miss a byte) ---- */
        if (UART1_IsRxReady())
        {
            uint8_t c = UART1_Read();

            while (!UART1_IsTxReady())
            {
            }
            UART1_Write(c);                       /* local echo */

            if ((c == '\r') || (c == '\n'))
            {
                if (rxIdx > 0U)
                {
                    rxBuf[rxIdx] = '\0';
                    UART_SendString("\r\n");
                    HandleCommand(rxBuf);
                    rxIdx = 0U;
                }
            }
            else if (rxIdx < LINE_WIDTH)
            {
                rxBuf[rxIdx++] = (char)c;
            }
        }

        /* ---- React to transitions decided by the ISR ---- */
        if ((state == ST_IDLE) && (prevState != ST_IDLE))
        {
            if (prevState == ST_RECORDING)
            {
                recordedLength = sampleIndex; /* main() is the only writer */
                UART_SendString("REC stop (buffer full), ");
                UART_SendUint(recordedLength);
                UART_SendString(" samples\r\n");
            }
            else if (prevState == ST_PLAYING)
            {
                UART_SendString("PLAY end\r\n");
            }
            /* ST_BEEP needs no message: StartBeep() is blocking. */
        }
        prevState = state;
    }
}