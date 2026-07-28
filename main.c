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
//#include "i2c_guard.h"
//#include "ssd1306.h"
#include <stdio.h>
#include <string.h>
#define FCY 100000000UL
#include <libpic30.h>

#define DAC_CTRL_BITS  0x3000   /* bit15=0(write) bit14=x GA=1(x1) SHDN=1(actif) */

static void DAC_Write12(uint16_t value)
{
    uint16_t command  = DAC_CTRL_BITS | (value & 0x0FFF);
    uint8_t  highByte = (uint8_t)(command >> 8);
    uint8_t  lowByte  = (uint8_t)(command & 0xFF);

    DAC_CS_SetLow();
    while (!SPI1_IsTxReady()) { }
    (void)SPI1_ByteExchange(highByte);
    while (!SPI1_IsTxReady()) { }
    (void)SPI1_ByteExchange(lowByte);
    DAC_CS_SetHigh();
}


int main(void)
{
    SYSTEM_Initialize();
    SPI1_Open(HOST_CONFIG);

    while (1)
    {
        DAC_Write12(0);
        DAC_Write12(4095);
    }
}