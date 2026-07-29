/**
 * PINS Generated Driver Header File 
 * 
 * @file      pins.h
 *            
 * @defgroup  pinsdriver Pins Driver
 *            
 * @brief     The Pin Driver directs the operation and function of 
 *            the selected device pins using dsPIC MCUs.
 *
 * @skipline @version   Firmware Driver Version 1.0.2
 *
 * @skipline @version   PLIB Version 1.4.1
 *
 * @skipline  Device : dsPIC33CK64MC105
*/

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

#ifndef PINS_H
#define PINS_H
// Section: Includes
#include <xc.h>

// Section: Device Pin Macros

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB14 GPIO Pin which has a custom name of DAC_CS to High
 * @pre      The RB14 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define DAC_CS_SetHigh()          (_LATB14 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB14 GPIO Pin which has a custom name of DAC_CS to Low
 * @pre      The RB14 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define DAC_CS_SetLow()           (_LATB14 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RB14 GPIO Pin which has a custom name of DAC_CS
 * @pre      The RB14 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define DAC_CS_Toggle()           (_LATB14 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RB14 GPIO Pin which has a custom name of DAC_CS
 * @param    none
 * @return   none  
 */
#define DAC_CS_GetValue()         _RB14

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB14 GPIO Pin which has a custom name of DAC_CS as Input
 * @param    none
 * @return   none  
 */
#define DAC_CS_SetDigitalInput()  (_TRISB14 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB14 GPIO Pin which has a custom name of DAC_CS as Output
 * @param    none
 * @return   none  
 */
#define DAC_CS_SetDigitalOutput() (_TRISB14 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC3 GPIO Pin which has a custom name of EE_CS to High
 * @pre      The RC3 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define EE_CS_SetHigh()          (_LATC3 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC3 GPIO Pin which has a custom name of EE_CS to Low
 * @pre      The RC3 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define EE_CS_SetLow()           (_LATC3 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC3 GPIO Pin which has a custom name of EE_CS
 * @pre      The RC3 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define EE_CS_Toggle()           (_LATC3 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC3 GPIO Pin which has a custom name of EE_CS
 * @param    none
 * @return   none  
 */
#define EE_CS_GetValue()         _RC3

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC3 GPIO Pin which has a custom name of EE_CS as Input
 * @param    none
 * @return   none  
 */
#define EE_CS_SetDigitalInput()  (_TRISC3 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC3 GPIO Pin which has a custom name of EE_CS as Output
 * @param    none
 * @return   none  
 */
#define EE_CS_SetDigitalOutput() (_TRISC3 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC6 GPIO Pin which has a custom name of SPKR_EN to High
 * @pre      The RC6 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define SPKR_EN_SetHigh()          (_LATC6 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC6 GPIO Pin which has a custom name of SPKR_EN to Low
 * @pre      The RC6 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define SPKR_EN_SetLow()           (_LATC6 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC6 GPIO Pin which has a custom name of SPKR_EN
 * @pre      The RC6 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define SPKR_EN_Toggle()           (_LATC6 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC6 GPIO Pin which has a custom name of SPKR_EN
 * @param    none
 * @return   none  
 */
#define SPKR_EN_GetValue()         _RC6

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC6 GPIO Pin which has a custom name of SPKR_EN as Input
 * @param    none
 * @return   none  
 */
#define SPKR_EN_SetDigitalInput()  (_TRISC6 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC6 GPIO Pin which has a custom name of SPKR_EN as Output
 * @param    none
 * @return   none  
 */
#define SPKR_EN_SetDigitalOutput() (_TRISC6 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC7 GPIO Pin which has a custom name of EE_HOLD to High
 * @pre      The RC7 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define EE_HOLD_SetHigh()          (_LATC7 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC7 GPIO Pin which has a custom name of EE_HOLD to Low
 * @pre      The RC7 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define EE_HOLD_SetLow()           (_LATC7 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC7 GPIO Pin which has a custom name of EE_HOLD
 * @pre      The RC7 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define EE_HOLD_Toggle()           (_LATC7 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC7 GPIO Pin which has a custom name of EE_HOLD
 * @param    none
 * @return   none  
 */
#define EE_HOLD_GetValue()         _RC7

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC7 GPIO Pin which has a custom name of EE_HOLD as Input
 * @param    none
 * @return   none  
 */
#define EE_HOLD_SetDigitalInput()  (_TRISC7 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC7 GPIO Pin which has a custom name of EE_HOLD as Output
 * @param    none
 * @return   none  
 */
#define EE_HOLD_SetDigitalOutput() (_TRISC7 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Initializes the PINS module
 * @param    none
 * @return   none  
 */
void PINS_Initialize(void);



#endif
