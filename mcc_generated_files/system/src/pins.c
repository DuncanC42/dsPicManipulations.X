 /**
 * PINS Generated Driver Source File 
 * 
 * @file      pins.c
 *            
 * @ingroup   pinsdriver
 *            
 * @brief     This is the generated driver source file for PINS driver.
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

// Section: Includes
#include <xc.h>
#include <stddef.h>
#include "../pins.h"

// Section: File specific functions

// Section: Driver Interface Function Definitions
void PINS_Initialize(void)
{
    /****************************************************************************
     * Setting the Output Latch SFR(s)
     ***************************************************************************/
    LATA = 0x0000U;
    LATB = 0x0000U;
    LATC = 0x0440U;
    LATD = 0x0000U;

    /****************************************************************************
     * Setting the GPIO Direction SFR(s)
     ***************************************************************************/
    TRISA = 0x001FU;
    TRISB = 0xBFFFU;
    TRISC = 0x3B36U;
    TRISD = 0x2502U;


    /****************************************************************************
     * Setting the Weak Pull Up and Weak Pull Down SFR(s)
     ***************************************************************************/
    CNPUA = 0x0004U;
    CNPUB = 0x0000U;
    CNPUC = 0x2000U;
    CNPUD = 0x0002U;
    CNPDA = 0x0000U;
    CNPDB = 0x0000U;
    CNPDC = 0x0000U;
    CNPDD = 0x0000U;


    /****************************************************************************
     * Setting the Open Drain SFR(s)
     ***************************************************************************/
    ODCA = 0x0000U;
    ODCB = 0x0000U;
    ODCC = 0x0000U;
    ODCD = 0x0000U;


    /****************************************************************************
     * Setting the Analog/Digital Configuration SFR(s)
     ***************************************************************************/
    ANSELA = 0x001BU;
    ANSELB = 0x039FU;
    ANSELC = 0x0000U;
    ANSELD = 0x2400U;

    /****************************************************************************
     * Set the PPS
     ***************************************************************************/
     __builtin_write_RPCON(0x0000); // unlock PPS

        RPINR20bits.SDI1R = 0x0031U; //RC1->SPI1:SDI1;
        RPINR18bits.U1RXR = 0x003BU; //RC11->UART1:U1RX;
        RPOR8bits.RP48R = 0x0005U;  //RC0->SPI1:SDO1;
        RPOR13bits.RP58R = 0x0001U;  //RC10->UART1:U1TX;
        RPINR20bits.SCK1R = 0x0032U;  //RC2->SPI1:SCK1IN;
        RPOR9bits.RP50R = 0x0006U;  //RC2->SPI1:SCK1OUT;

     __builtin_write_RPCON(0x0800); // lock PPS


}

