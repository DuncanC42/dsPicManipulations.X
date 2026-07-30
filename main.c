/*
© [2026] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/
#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/system/pins.h"
#include "mcc_generated_files/spi_host/spi1.h"
#include "mcc_generated_files/adc/adc1.h"
#include "mcc_generated_files/timer/sccp1.h"
#include "mcc_generated_files/uart/uart1.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define FCY 100000000UL
#include <libpic30.h>

/* ---------- DAC (MCP4821) ---------- */
#define DAC_CTRL_BITS 0x3000

static void DAC_Write12(uint16_t value)
{
    uint16_t command  = DAC_CTRL_BITS | (value & 0x0FFF);
    uint8_t  highByte = (uint8_t)(command >> 8);
    uint8_t  lowByte  = (uint8_t)(command & 0xFF);
    DAC_CS_SetLow();
    while (!SPI1_IsTxReady()) {}
    (void)SPI1_ByteExchange(highByte);
    while (!SPI1_IsTxReady()) {}
    (void)SPI1_ByteExchange(lowByte);
    DAC_CS_SetHigh();
}

/* ---------- Buffer audio, entierement en RAM (pas d'EEPROM) ---------- */
#define SAMPLE_RATE   4000UL
#define BUFFER_SIZE   7000      /* ~1.5 s a 4 kHz : 6000 octets, large marge sur les ~7 Ko libres */

static uint8_t audioBuf[BUFFER_SIZE];
static volatile uint16_t sampleIndex    = 0;
static volatile uint16_t recordedLength = 0;
static volatile bool     recording = false;
static volatile bool     playing   = false;

void TimerTick(void)   /* appelee toutes les 250 us (1/4000 s) */
{
    if (recording)
    {
        ADC1_SoftwareTriggerEnable();
        uint16_t raw = ADC1_ConversionResultGet(MIC_OUT);
        audioBuf[sampleIndex++] = (uint8_t)(raw >> 4);
        if (sampleIndex >= BUFFER_SIZE) { recording = false; recordedLength = BUFFER_SIZE; }
    }
    else if (playing)
    {
        uint16_t value12 = (uint16_t)audioBuf[sampleIndex++] << 4;
        DAC_Write12(value12);
        if (sampleIndex >= recordedLength) { playing = false; }
    }
}

/* ---------- UART ---------- */
static void UART_SendString(const char *s)
{
    while (*s) { while (!UART1_IsTxReady()) {} UART1_Write((uint8_t)*s++); }
}

#define LINE_WIDTH 32
static char    rxBuf[LINE_WIDTH + 1];
static uint8_t rxIdx = 0;

static void HandleCommand(const char *cmd)
{
    if (strcmp(cmd, "REC") == 0)
    {
        if (!recording && !playing)
        {
            sampleIndex = 0;
            recording   = true;
            UART_SendString("REC start\r\n");
        }
    }
    else if (strcmp(cmd, "STOP") == 0)
    {
        if (recording)
        {
            recordedLength = sampleIndex;
            recording      = false;
            UART_SendString("REC stop\r\n");
        }
    }
    else if (strcmp(cmd, "PLAY") == 0)
    {
        if (!recording && !playing && recordedLength > 0)
        {
            sampleIndex = 0;
            playing     = true;
            UART_SendString("PLAY start\r\n");
        }
    }
    else
    {
        UART_SendString("? commandes: REC, STOP, PLAY\r\n");
    }
}

int main(void)
{
    bool wasRecording = false, wasPlaying = false;

    SYSTEM_Initialize();
    SPKR_EN_SetHigh();
    ADC1_Enable();
    SPI1_Open(HOST_CONFIG);
    Timer1_TimeoutCallbackRegister(TimerTick);
    Timer1_Start();

    UART_SendString("Pret. Commandes : REC, STOP, PLAY.\r\n");

    while (1)
    {
        if (UART1_IsRxReady())
        {
            uint8_t c = UART1_Read();
            while (!UART1_IsTxReady()) {}
            UART1_Write(c);   /* echo local */

            if (c == '\r' || c == '\n')
            {
                if (rxIdx > 0)
                {
                    rxBuf[rxIdx] = '\0';
                    UART_SendString("\r\n");
                    HandleCommand(rxBuf);
                    rxIdx = 0;
                }
            }
            else if (rxIdx < LINE_WIDTH)
            {
                rxBuf[rxIdx++] = (char)c;
            }
        }

        if (!recording && wasRecording)
        {
            recordedLength = sampleIndex;
            UART_SendString("REC stop (buffer plein)\r\n");
        }
        wasRecording = recording;

        if (!playing && wasPlaying) { UART_SendString("PLAY stop (fin)\r\n"); }
        wasPlaying = playing;

        __delay_ms(2);
    }
}