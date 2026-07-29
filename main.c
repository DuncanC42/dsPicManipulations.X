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
#include <stdbool.h>
#define FCY 100000000UL
#include <libpic30.h>

#define EE_CMD_WREN   0x06
#define EE_CMD_WRITE  0x02
#define EE_CMD_RDSR   0x05

#define PAGE_SIZE        256
#define RECORD_SECONDS   12
#define SAMPLE_RATE      8000UL
#define TOTAL_SAMPLES    (RECORD_SECONDS * SAMPLE_RATE)   /* 96000 */
#define TOTAL_PAGES      (TOTAL_SAMPLES / PAGE_SIZE)       /* 375, exact */

static uint8_t bufA[PAGE_SIZE], bufB[PAGE_SIZE];
static uint8_t *fillBuf  = bufA;
static uint8_t *flushBuf = bufB;
static volatile uint16_t fillIndex = 0;
static volatile bool     pageReady = false;
static volatile bool     recording = false;

static void EE_WaitReady(void)
{
    uint8_t status;
    do {
        EE_CS_SetLow();
        SPI1_ByteExchange(EE_CMD_RDSR);
        status = SPI1_ByteExchange(0x00);
        EE_CS_SetHigh();
    } while (status & 0x01);
}

static void EE_WritePage(uint32_t addr, const uint8_t *data)
{
    EE_CS_SetLow();
    SPI1_ByteExchange(EE_CMD_WREN);
    EE_CS_SetHigh();

    EE_CS_SetLow();
    SPI1_ByteExchange(EE_CMD_WRITE);
    SPI1_ByteExchange((uint8_t)(addr >> 16));
    SPI1_ByteExchange((uint8_t)(addr >> 8));
    SPI1_ByteExchange((uint8_t)addr);
    for (uint16_t i = 0; i < PAGE_SIZE; i++) { SPI1_ByteExchange(data[i]); }
    EE_CS_SetHigh();

    EE_WaitReady();
}

static void UART_SendString(const char *s)
{
    while (*s != '\0')
    {
        while (!UART1_IsTxReady()) { }
        UART1_Write((uint8_t)*s++);
    }
}

void SampleTick(void)   /* appelee par Timer1 toutes les 125 us */
{
    if (!recording) { return; }

    ADC1_SoftwareTriggerEnable();
    uint16_t raw = ADC1_ConversionResultGet(MIC_OUT);
    fillBuf[fillIndex++] = (uint8_t)(raw >> 4);   /* 12 -> 8 bits, centre sur 128 */

    if (fillIndex >= PAGE_SIZE)
    {
        fillIndex = 0;
        uint8_t *tmp = fillBuf; fillBuf = flushBuf; flushBuf = tmp;
        pageReady = true;
    }
}

int main(void)
{
    uint8_t  prevMask    = 0;
    uint16_t pagesWritten = 0;

    SYSTEM_Initialize();
    ADC1_Enable();
    SPI1_Open(HOST_CONFIG);
    Timer1_TimeoutCallbackRegister(SampleTick);
    Timer1_Start();

    UART_SendString("Pret. SW1 = start/stop (12s max).\r\n");

    while (1)
    {
        uint8_t mask = 0;
        if (!SW_1_GetValue()) { mask |= 0x01; }
        uint8_t pressed = mask & (uint8_t)~prevMask;

        if (pressed & 0x01)
        {
            __delay_ms(20);                       /* anti-rebond : meme principe que le tout premier lab GPIO */
            uint8_t confirm = 0;
            if (!SW_1_GetValue()) { confirm |= 0x01; }

            if (confirm & 0x01)                   /* toujours presse 20ms plus tard = vrai appui */
            {
                if (!recording)
                {
                    recording    = true;
                    pagesWritten = 0;
                    fillIndex    = 0;
                    UART_SendString("REC start\r\n");
                }
                else
                {
                    recording = false;
                    UART_SendString("REC stop (bouton)\r\n");
                }
            }
        }

        prevMask = mask;
        __delay_ms(5);
    }
}