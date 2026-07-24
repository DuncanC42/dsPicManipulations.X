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
#include "i2c_guard.h"
#include "ssd1306.h"
#include <stdio.h>
#define FCY 100000000UL
#include <libpic30.h>
/*
    Main application
*/

#define VCNL4200_ADDR     0x51
#define VCNL4200_PS_CONF  0x03
#define VCNL4200_PS_DATA  0x08

static void VCNL4200_Init(void)
{
    uint8_t cfg[3] = { VCNL4200_PS_CONF, 0x08, 0x00 };   /* PS_SD=0 -> capteur actif */
    if (!I2C_WaitIdle()) { return; }
    I2C1_Write(VCNL4200_ADDR, cfg, sizeof(cfg));
    (void)I2C_WaitIdle();
}

static uint16_t VCNL4200_ReadReg(uint8_t reg)
{
    uint8_t rx[2] = {0, 0};
    if (!I2C_WaitIdle()) { return 0; }
    I2C1_WriteRead(VCNL4200_ADDR, &reg, 1, rx, 2);   /* repeated start */
    (void)I2C_WaitIdle();
    return (uint16_t)(rx[0] | ((uint16_t)rx[1] << 8));   /* LSB d'abord */
}

int main(void)
{
    SYSTEM_Initialize();
    SSD1306_Init();
    SSD1306_Clear();
    VCNL4200_Init();

    while (1)
    {
        uint16_t prox = VCNL4200_ReadReg(VCNL4200_PS_DATA);

        char buffer[16];
        sprintf(buffer, "PROX: %4u   ", prox);
        SSD1306_SelectPage(0);
        SSD1306_WriteString(buffer);

        __delay_ms(50);
    }
}