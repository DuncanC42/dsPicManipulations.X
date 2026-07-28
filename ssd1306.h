/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  

#ifndef SSD1306_H
#define SSD1306_H

#include "mcc_generated_files/system/system.h"
//#include "mcc_generated_files/i2c_host/i2c1.h"
#include <string.h>
#include <xc.h> // include processor files - each processor file is guarded. 
#include "font.h"
//#include "mcc_generated_files/i2c_host/i2c1.h"

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#define SUCCESS 1
    
//SSD1306 128x64 Definition
#define SSD1306_I2C_ADDRESS 0x3D
#define SSD1306_LCDWIDTH                  128
#define SSD1306_LCDHEIGHT                 64
#define SSD1306_CLEAR_SIZE                1024


#define SSD1306_COMMAND                     0x00
#define SSD1306_DATA                        0xC0
#define SSD1306_DATA_CONTINUE               0x40


/* SSD1306 Command Set*/
// Fundamental Commands
#define SSD1306_SET_CONTRAST_CONTROL                    0x81
#define SSD1306_DISPLAY_ALL_ON_RESUME                   0xA4
#define SSD1306_DISPLAY_ALL_ON                          0xA5
#define SSD1306_NORMAL_DISPLAY                          0xA6
#define SSD1306_INVERT_DISPLAY                          0xA7
#define SSD1306_DISPLAY_OFF                             0xAE
#define SSD1306_DISPLAY_ON                              0xAF
#define SSD1306_NOP                                     0xE3

// Scrolling Commands
#define SSD1306_HORIZONTAL_SCROLL_RIGHT                 0x26
#define SSD1306_HORIZONTAL_SCROLL_LEFT                  0x27
#define SSD1306_HORIZONTAL_SCROLL_VERTICAL_AND_RIGHT    0x29
#define SSD1306_HORIZONTAL_SCROLL_VERTICAL_AND_LEFT     0x2A
#define SSD1306_DEACTIVATE_SCROLL                       0x2E
#define SSD1306_ACTIVATE_SCROLL                         0x2F
#define SSD1306_SET_VERTICAL_SCROLL_AREA                0xA3

// Addressing Setting Commands
#define SSD1306_SET_LOWER_COLUMN                        0x00
#define SSD1306_SET_HIGHER_COLUMN                       0x10
#define SSD1306_MEMORY_ADDR_MODE                        0x20
#define SSD1306_SET_COLUMN_ADDR                         0x21
#define SSD1306_SET_PAGE_ADDR                           0x22

// Hardware Configuration Commands
#define SSD1306_SET_START_LINE                          0x40
#define SSD1306_SET_SEGMENT_REMAP                       0xA0
#define SSD1306_SET_MULTIPLEX_RATIO                     0xA8
#define SSD1306_COM_SCAN_DIR_INC                        0xC0
#define SSD1306_COM_SCAN_DIR_DEC                        0xC8
#define SSD1306_SET_DISPLAY_OFFSET                      0xD3
#define SSD1306_SET_COM_PINS                            0xDA
#define SSD1306_CHARGE_PUMP                             0x8D

// Timing & Driving Scheme Setting Commands
#define SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO             0xD5
#define SSD1306_SET_PRECHARGE_PERIOD                    0xD9
#define SSD1306_SET_VCOM_DESELECT                       0xDB


void SSD1306_SendCommand(uint8_t command);
void SSD1306_SendData(uint8_t data);
void SSD1306_Init();
void SSD1306_WriteString(char *characters);
void SSD1306_WriteCharacter(char character);
void SSD1306_Clear();
void SSD1306_ClearLine(uint8_t page_num);
void SSD1306_SelectPage(uint8_t page_num);
const uint8_t* GetFontCharacter(uint8_t character);
void SSD1306_DrawBitmap(const uint8_t* bitmap);


#ifdef	__cplusplus
}
#endif /* __cplusplus */



#endif //SSD1306_H
