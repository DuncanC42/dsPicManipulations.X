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
#include "mcc_generated_files/adc/adc1.h"
#include "i2c_guard.h"
#include "ssd1306.h"
#include <stdio.h>
#define FCY 100000000UL
#include <libpic30.h>

#define SAMPLES_PER_WINDOW  200   /* nombre de lectures par fenetre d'observation */

int main(void)
{
    SYSTEM_Initialize();
    ADC1_Enable();
    SSD1306_Init();
    SSD1306_Clear();

    while (1)
    {
        uint16_t minVal = -1;   /* valeur la plus haute en non signé (65535) */
        uint16_t maxVal = 0; /* sera forcément remplacé à la premiere lecture */

        for (uint16_t i = 0; i < SAMPLES_PER_WINDOW; i++)
        {
            ADC1_SoftwareTriggerEnable();
            __delay_us(50);
            uint16_t sample = ADC1_ConversionResultGet(MIC_OUT);

            if (sample < minVal) { minVal = sample; }
            if (sample > maxVal) { maxVal = sample; }
        }

        uint16_t swing = maxVal - minVal ;   /* le niveau qui nous interesse vraiment */

        char buffer[16];
        sprintf(buffer, "MIC: %4u   ", swing);
        SSD1306_SelectPage(0);
        SSD1306_WriteString(buffer);
    }
}