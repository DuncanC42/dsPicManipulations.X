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
 * @brief    Sets the RC4 GPIO Pin which has a custom name of LED0 to High
 * @pre      The RC4 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define LED0_SetHigh()          (_LATC4 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC4 GPIO Pin which has a custom name of LED0 to Low
 * @pre      The RC4 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED0_SetLow()           (_LATC4 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC4 GPIO Pin which has a custom name of LED0
 * @pre      The RC4 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED0_Toggle()           (_LATC4 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC4 GPIO Pin which has a custom name of LED0
 * @param    none
 * @return   none  
 */
#define LED0_GetValue()         _RC4

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC4 GPIO Pin which has a custom name of LED0 as Input
 * @param    none
 * @return   none  
 */
#define LED0_SetDigitalInput()  (_TRISC4 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC4 GPIO Pin which has a custom name of LED0 as Output
 * @param    none
 * @return   none  
 */
#define LED0_SetDigitalOutput() (_TRISC4 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Initializes the PINS module
 * @param    none
 * @return   none  
 */
void PINS_Initialize(void);



#endif
