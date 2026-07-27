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
/*
 * 07_MCP9808_app -- reads the on-board MCP9808 temperature sensor and
 * streams it to the PC over UART, mirrored on the OLED.
 * dsPIC33CK64MC105 Curiosity Nano + Nano Explorer, MCC Melody, XC16
 *
 * Required MCC modules:
 *   - I2C1_Host : SCL1 -> RC9, SDA1 -> RC8   (sensor + OLED share this bus)
 *   - UART1     : U1TX -> RC10, U1RX -> RC11, 115200 8N1, polling (no ISR)
 *
 * PC side: screen /dev/ttyACM0 115200 (or any 115200 8N1 terminal)
 */

#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/system/pins.h"
#include "mcc_generated_files/i2c_host/i2c1.h"
#include "mcc_generated_files/uart/uart1.h"
#include "i2c_guard.h"
#include "ssd1306.h"
#include <stdio.h>
#include <string.h>
#define FCY 100000000UL
#include <libpic30.h>

/* ---------- MCP9808 temperature sensor ---------- */
#define MCP9808_ADDR        0x1C
#define MCP9808_REG_TA      0x05   /* Ambient Temperature, read-only, 16-bit, MSB first */
#define MCP9808_REG_MFG_ID  0x06   /* Manufacturer ID, read-only, expected 0x0054 */

/* ---------- Timing ---------- */
#define SEND_PERIOD_MS    250    /* how often the board pushes a reading out */
#define LOOP_DELAY_MS     10     /* keeps the loop responsive */

/* ---------- OLED ---------- */
#define LINE_WIDTH        20     /* line capacity in this font, ~20 chars */

static uint16_t MCP9808_ReadReg(uint8_t reg)
{
    uint8_t rx[2] = { 0, 0 };
    if (!I2C_WaitIdle()) { return 0xFFFF; }
    I2C1_WriteRead(MCP9808_ADDR, &reg, 1, rx, 2);
    if (!I2C_WaitIdle()) { return 0xFFFF; }
    if (I2C1_ErrorGet() != I2C_HOST_ERROR_NONE) { return 0xFFFF; }   /* no reply from 0x1C */
    return (uint16_t)(((uint16_t)rx[0] << 8) | rx[1]);
}

/* Raw Ta register -> hundredths of a degree C (2537 means 25.37 C).
   Bits 15-13 are alert flags (masked off), bits 12-0 are a 13-bit
   two's complement value in units of 1/16 C. */
static int16_t MCP9808_ToCentiCelsius(uint16_t raw)
{
    uint8_t upper = (uint8_t)((raw >> 8) & 0x1F);
    int16_t value = (int16_t)(((uint16_t)upper << 8) | (raw & 0xFF));

    if (value & 0x1000) { value -= 0x2000; }   /* sign-extend the 13-bit value */

    /* value*100 can exceed 32767 (16-bit int on this platform) for normal
       room temperatures -> widen to 32-bit before multiplying */
    return (int16_t)(((int32_t)value * 100) / 16);
}

/* ---------- UART ---------- */
static void UART_SendString(const char *s)
{
    while (*s != '\0')
    {
        while (!UART1_IsTxReady()) { }
        UART1_Write((uint8_t)*s++);
    }
}

/* ---------- OLED ---------- */
static void OLED_ShowLine(uint8_t page, const char *text, uint8_t len)
{
    char line[LINE_WIDTH + 1];
    uint8_t i = 0;

    for (; i < len && i < LINE_WIDTH; i++) { line[i] = text[i]; }
    for (; i < LINE_WIDTH; i++)            { line[i] = ' '; }   /* clear any longer previous line */
    line[LINE_WIDTH] = '\0';

    SSD1306_SelectPage(page);
    SSD1306_WriteString(line);
}

/* ---------- Main ---------- */
int main(void)
{
    uint16_t elapsedMs = 0;

    SYSTEM_Initialize();
    SSD1306_Init();
    SSD1306_Clear();

    /* Sanity check: confirms the sensor is alive before trusting its data */
    uint16_t mfgId = MCP9808_ReadReg(MCP9808_REG_MFG_ID);

    char idMsg[24];
    sprintf(idMsg, "MFG ID: 0x%04X\r\n", mfgId);
    UART_SendString(idMsg);

    char idLine[16];
    sprintf(idLine, "ID: 0x%04X", mfgId);
    OLED_ShowLine(0, idLine, (uint8_t)strlen(idLine));
    __delay_ms(1000);

    while (1)
    {
        elapsedMs += LOOP_DELAY_MS;
        if (elapsedMs >= SEND_PERIOD_MS)
        {
            elapsedMs = 0;

            uint16_t raw   = MCP9808_ReadReg(MCP9808_REG_TA);
            int16_t  centi = MCP9808_ToCentiCelsius(raw);
            int16_t  whole = centi / 100;
            int16_t  frac  = centi % 100;
            if (frac < 0) { frac = -frac; }

            char msg[24];
            sprintf(msg, "TEMP: %d.%02d C\r\n", whole, frac);
            UART_SendString(msg);

            char oledMsg[16];
            sprintf(oledMsg, "TEMP: %d.%02d C", whole, frac);
            OLED_ShowLine(0, oledMsg, (uint8_t)strlen(oledMsg));
        }

        __delay_ms(LOOP_DELAY_MS);
    }
}