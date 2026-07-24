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
 * @brief    Sets the RB7 GPIO Pin which has a custom name of LED_B to High
 * @pre      The RB7 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define LED_B_SetHigh()          (_LATB7 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB7 GPIO Pin which has a custom name of LED_B to Low
 * @pre      The RB7 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED_B_SetLow()           (_LATB7 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RB7 GPIO Pin which has a custom name of LED_B
 * @pre      The RB7 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED_B_Toggle()           (_LATB7 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RB7 GPIO Pin which has a custom name of LED_B
 * @param    none
 * @return   none  
 */
#define LED_B_GetValue()         _RB7

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB7 GPIO Pin which has a custom name of LED_B as Input
 * @param    none
 * @return   none  
 */
#define LED_B_SetDigitalInput()  (_TRISB7 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB7 GPIO Pin which has a custom name of LED_B as Output
 * @param    none
 * @return   none  
 */
#define LED_B_SetDigitalOutput() (_TRISB7 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB8 GPIO Pin which has a custom name of LED_G to High
 * @pre      The RB8 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define LED_G_SetHigh()          (_LATB8 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB8 GPIO Pin which has a custom name of LED_G to Low
 * @pre      The RB8 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED_G_SetLow()           (_LATB8 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RB8 GPIO Pin which has a custom name of LED_G
 * @pre      The RB8 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED_G_Toggle()           (_LATB8 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RB8 GPIO Pin which has a custom name of LED_G
 * @param    none
 * @return   none  
 */
#define LED_G_GetValue()         _RB8

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB8 GPIO Pin which has a custom name of LED_G as Input
 * @param    none
 * @return   none  
 */
#define LED_G_SetDigitalInput()  (_TRISB8 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB8 GPIO Pin which has a custom name of LED_G as Output
 * @param    none
 * @return   none  
 */
#define LED_G_SetDigitalOutput() (_TRISB8 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB9 GPIO Pin which has a custom name of LED_R to High
 * @pre      The RB9 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define LED_R_SetHigh()          (_LATB9 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB9 GPIO Pin which has a custom name of LED_R to Low
 * @pre      The RB9 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED_R_SetLow()           (_LATB9 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RB9 GPIO Pin which has a custom name of LED_R
 * @pre      The RB9 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED_R_Toggle()           (_LATB9 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RB9 GPIO Pin which has a custom name of LED_R
 * @param    none
 * @return   none  
 */
#define LED_R_GetValue()         _RB9

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB9 GPIO Pin which has a custom name of LED_R as Input
 * @param    none
 * @return   none  
 */
#define LED_R_SetDigitalInput()  (_TRISB9 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB9 GPIO Pin which has a custom name of LED_R as Output
 * @param    none
 * @return   none  
 */
#define LED_R_SetDigitalOutput() (_TRISB9 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Initializes the PINS module
 * @param    none
 * @return   none  
 */
void PINS_Initialize(void);



#endif
