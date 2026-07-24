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
#include "mcc_generated_files/i2c_host/i2c1.h"
#include "mcc_generated_files/uart/uart1.h"
#include "i2c_guard.h"
#include "ssd1306.h"
#include <stdio.h>
#include <string.h>
#define FCY 100000000UL
#include <libpic30.h>

/*
    Main application
*/
/* ---------- VCNL4200 proximity sensor ---------- */
#define VCNL4200_ADDR     0x51
#define VCNL4200_PS_CONF  0x03   /* command code: PS_CONF1 (low) + PS_CONF2 (high) */
#define VCNL4200_PS_DATA  0x08   /* command code: proximity output, 16-bit, LSB first */
 
/* ---------- Timing ---------- */
#define SEND_PERIOD_MS    250    /* how often the board pushes a reading out */
#define LOOP_DELAY_MS     10     /* keeps RX polling responsive between sends */
 
/* ---------- OLED ---------- */
#define LINE_WIDTH        20     /* line capacity in this font, ~20 chars */
 
static void VCNL4200_Init(void)
{
    /* PS_CONF1 = 0x08 : PS_SD = 0 (sensor active), 8T integration time
       PS_CONF2 = 0x00 : 12-bit output (0-4095), interrupt disabled (we poll) */
    uint8_t cfg[3] = { VCNL4200_PS_CONF, 0x08, 0x00 };
    if (!I2C_WaitIdle()) { return; }
    I2C1_Write(VCNL4200_ADDR, cfg, sizeof(cfg));
    (void)I2C_WaitIdle();
}
 
static uint16_t VCNL4200_ReadReg(uint8_t reg)
{
    uint8_t rx[2] = { 0, 0 };
    if (!I2C_WaitIdle()) { return 0; }
    I2C1_WriteRead(VCNL4200_ADDR, &reg, 1, rx, 2);   /* repeated start is mandatory here */
    (void)I2C_WaitIdle();
    return (uint16_t)(rx[0] | ((uint16_t)rx[1] << 8));   /* LSB first */
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
    char     rxBuffer[LINE_WIDTH + 1];
    uint8_t  rxIdx      = 0;
    uint16_t elapsedMs  = 0;
 
    SYSTEM_Initialize();
    SSD1306_Init();
    SSD1306_Clear();
    VCNL4200_Init();
 
    OLED_ShowLine(0, "PROX: ---", 9);
    OLED_ShowLine(2, "MSG: (none yet)", 15);
    UART_SendString("Board ready. Streaming proximity readings.\r\n");
    UART_SendString("Type a line and press Enter to show it on screen.\r\n");
 
    while (1)
    {
        /* ---- direction 1: board -> PC, unsolicited, on a timer ---- */
        elapsedMs += LOOP_DELAY_MS;
        if (elapsedMs >= SEND_PERIOD_MS)
        {
            elapsedMs = 0;
            uint16_t prox = VCNL4200_ReadReg(VCNL4200_PS_DATA);
 
            char msg[24];
            sprintf(msg, "PROX: %4u\r\n", prox);
            UART_SendString(msg);
 
            char oledMsg[16];
            sprintf(oledMsg, "PROX: %4u", prox);
            OLED_ShowLine(0, oledMsg, (uint8_t)strlen(oledMsg));
        }
 
        /* ---- direction 2: PC -> board, whenever it happens to arrive ---- */
        if (UART1_IsRxReady())
        {
            uint8_t c = UART1_Read();
 
            while (!UART1_IsTxReady()) { }   /* local echo: screen has none by default */
            UART1_Write(c);
 
            if (c == '\r' || c == '\n')
            {
                if (rxIdx > 0)
                {
                    OLED_ShowLine(2, rxBuffer, rxIdx);
                    UART_SendString("\r\n[shown on screen]\r\n");
                    rxIdx = 0;
                }
            }
            else if (rxIdx < LINE_WIDTH)
            {
                rxBuffer[rxIdx++] = (char)c;
            }
            else
            {
                UART_SendString("\r\n[line too long for the screen]\r\n");
                rxIdx = 0;
            }
        }
 
        __delay_ms(LOOP_DELAY_MS);
    }
}
