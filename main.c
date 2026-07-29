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
#include "mcc_generated_files/uart/uart1.h"
#include <stdio.h>
#define FCY 100000000UL
#include <libpic30.h>

#define EE_CMD_READ      0x03
#define EE_CMD_WRITE     0x02
#define EE_CMD_WREN      0x06
#define EE_CMD_RDSR      0x05
#define EE_CMD_JEDEC_ID  0x9F

static void EE_ReadId(uint8_t id[3])
{
    EE_CS_SetLow();
    SPI1_ByteExchange(EE_CMD_JEDEC_ID);
    id[0] = SPI1_ByteExchange(0x00);
    id[1] = SPI1_ByteExchange(0x00);
    id[2] = SPI1_ByteExchange(0x00);
    EE_CS_SetHigh();
}

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

static void EE_WriteByte(uint32_t addr, uint8_t data)
{
    EE_CS_SetLow();
    SPI1_ByteExchange(EE_CMD_WREN);
    EE_CS_SetHigh();

    EE_CS_SetLow();
    SPI1_ByteExchange(EE_CMD_WRITE);
    SPI1_ByteExchange((uint8_t)(addr >> 16));
    SPI1_ByteExchange((uint8_t)(addr >> 8));
    SPI1_ByteExchange((uint8_t)addr);
    SPI1_ByteExchange(data);
    EE_CS_SetHigh();

    EE_WaitReady();
}

static uint8_t EE_ReadByte(uint32_t addr)
{
    EE_CS_SetLow();
    SPI1_ByteExchange(EE_CMD_READ);
    SPI1_ByteExchange((uint8_t)(addr >> 16));
    SPI1_ByteExchange((uint8_t)(addr >> 8));
    SPI1_ByteExchange((uint8_t)addr);
    uint8_t data = SPI1_ByteExchange(0x00);
    EE_CS_SetHigh();
    return data;
}

static void UART_SendString(const char *s)
{
    while (*s != '\0')
    {
        while (!UART1_IsTxReady()) { }
        UART1_Write((uint8_t)*s++);
    }
}

int main(void)
{
    SYSTEM_Initialize();
    SPI1_Open(HOST_CONFIG);
    DAC_CS_SetHigh();   /* deselectionne explicitement le DAC pour ne pas polluer le bus */
    uint8_t id[3];
    EE_HOLD_SetHigh();   /* desactive la pause : la puce doit toujours ecouter le bus */
    EE_ReadId(id);
    char msg[48];
    sprintf(msg, "EEPROM ID: %02X %02X %02X\r\n", id[0], id[1], id[2]);
    UART_SendString(msg);

    EE_WriteByte(0x000000, 0x42);
    uint8_t readBack = EE_ReadByte(0x000000);
    sprintf(msg, "Write 0x42, read 0x%02X -> %s\r\n", readBack,
            (readBack == 0x42) ? "OK" : "ECHEC");
    UART_SendString(msg);

    while (1) { }
}