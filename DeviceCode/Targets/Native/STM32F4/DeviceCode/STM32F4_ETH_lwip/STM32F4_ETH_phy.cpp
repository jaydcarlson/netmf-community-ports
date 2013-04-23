/*
 * Copyright 2011 CSA Engineering AG
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*********************************************************************************************
 * @file    STM32F4_ETH_phy.cpp
 * @brief   PHY (TERIDIAN 78Q2123) driver. 
 * @author  CSA Engineering AG, Switzerland, www.csa.ch, info@csa.ch
 * @date    March 2012
 ********************************************************************************************/
  
//--------------------------------------------------------------------------------------------
// System and local includes
//--------------------------------------------------------------------------------------------
 
#include <tinyhal.h>

#include "STM32F4_ETH_phy.h"

//--------------------------------------------------------------------------------------------
// Constant defines
//--------------------------------------------------------------------------------------------

#define PHY_CONTROL_REGISTER            0U              // Control register
#define PHY_RESET                       0x8000U         // Reset
#define PHY_ANEGEN                      0x1000U         // Auto-negotiation enable 
#define PHY_PWRDN                       0x0800U         // Power-down
#define PHY_STATUS_REGISTER             1U              // Status register
#define PHY_LINK                        0x0004U         // Link status
#define PHY_ANEGC                       0x0020U         // Auto-negotiation complete
#define PHY_DIAGNOSTIC_REGISTER         18U             // Diagnostic register
#define PHY_ANEGF                       0x1000U         // Auto-negotiation fail indication
#define PHY_LINK_TIMEOUT                0x0003FFFFU     // PHY link timeout
#define PHY_RESET_DELAY                 0x000FFFFFU     // PHY reset delay
#define PHY_AUTO_NEGOTIATION_TIMEOUT    0x00000FFFU     // Auto negotiation timeout

//--------------------------------------------------------------------------------------------
// Local declarations
//--------------------------------------------------------------------------------------------

static BOOL (*readPhyRegister)(const uint32_t miiAddress, uint16_t *const miiData);
static BOOL (*writePhyRegister)(const uint32_t miiAddress, const uint16_t miiData);

//--------------------------------------------------------------------------------------------
// Functions definitions
//--------------------------------------------------------------------------------------------
/**
 * Save the function to read the PHY.
 * @param pRead pointer to the read PHY callback function.
 */
void initReadPhyCallback(pRead readCallback)
{
    readPhyRegister = readCallback;
}

//--------------------------------------------------------------------------------------------
/**
 * Save the function to write to the PHY.
 * @param pWrite pointer to the write PHY callback function.
 */
void initWritePhyCallback(pWrite writeCallback)
{
    writePhyRegister = writeCallback;
}

//--------------------------------------------------------------------------------------------
/**
 * Reset the PHY.
 * @return Error status.
 *   @retval TRUE if PHY reset successful.
 *   @retval FALSE otherwise.
 */
BOOL eth_phyReset()
{
    volatile uint32_t nWait = 0U;
    
    // Check callback
    if (!writePhyRegister)
    {
        return FALSE;
    }
    
    // Perform reset
    if (!writePhyRegister(PHY_CONTROL_REGISTER, PHY_RESET))
    {
        return FALSE;
    }
    
    // Wait for completion
    while (nWait < PHY_RESET_DELAY)
    {
        nWait++;
    }
  
    return TRUE;
    
}

//--------------------------------------------------------------------------------------------
/**
 * Test whether a valid PHY link is established.
 * @param isCallBlocking indicates whether the function should block (until a link is detected 
 *        or a timeout is elapsed)
 * @return The PHY link status.
 *   @retval TRUE if valid link detected.
 *   @retval FALSE otherwise.
 */
BOOL eth_isPhyLinkValid(BOOL isCallBlocking)
{
    volatile uint32_t nWait = 0U;
    uint16_t status = 0U;

    // Check callback
    if (!readPhyRegister)
    {
        return FALSE;
    }
    
    // Read status register until a valid link is detected or a timeout is elapsed
    do 
    {
        readPhyRegister(PHY_STATUS_REGISTER, &status);
        nWait++;
    } 
    while ( isCallBlocking &&
            ((status & PHY_LINK) != PHY_LINK) &&
            (nWait++ < PHY_LINK_TIMEOUT) );
    
    // Check the link
    return (status & PHY_LINK) == PHY_LINK;
}

 
//--------------------------------------------------------------------------------------------
/**
 * Enable auto negotiation and wait for completion.
 * @return Error status.
 *   @retval TRUE if auto negotiation successful.
 *   @retval FALSE otherwise.
 */
BOOL eth_enableAutoNegotiation()
{
    volatile uint32_t nWait = 0U;
    uint16_t status = 0U;
    uint16_t diagnostic = 0U;
    
    // Check callback
    if (!writePhyRegister || !readPhyRegister)
    {
        return FALSE;
    }
    
    // Enable auto-negotiation
    if (!writePhyRegister(PHY_CONTROL_REGISTER, PHY_ANEGEN))
    {
        return FALSE;
    }
    
    // Wait for completion
    do
    {
        readPhyRegister(PHY_STATUS_REGISTER, &status);
        nWait++;
    }
    while ( ((status & PHY_ANEGC) != PHY_ANEGC) &&
            (nWait++ < PHY_AUTO_NEGOTIATION_TIMEOUT) );
    
    // Check auto negotiation completed
    if ((status & PHY_ANEGC) != PHY_ANEGC)
    {
        return FALSE;
    }
    
    // Check common technology found
    readPhyRegister(PHY_DIAGNOSTIC_REGISTER, &diagnostic);
    if ((diagnostic & PHY_ANEGF) == PHY_ANEGF)
    {
        return FALSE;
    }
    
    return TRUE;
}

//--------------------------------------------------------------------------------------------
/**
 * Power up the PHY.
 * @param isPowerUp the power state.
 *  @arg TRUE if the PHY must be powered up.
 *  @arg FALSE if the PHY must be powered down.
 * @return Error status.
 *   @retval TRUE if PHY powered up succesfully.
 *   @retval FALSE otherwise.
 */
BOOL eth_powerUpPhy(BOOL isPowerUp)
{
    uint16_t ctrlReg;

    // Check callback
    if (!writePhyRegister || !readPhyRegister)
    {
        return FALSE;
    }
    
    // Read the current value of the PHY control register
    if (!readPhyRegister(PHY_CONTROL_REGISTER, &ctrlReg))
    {
        return FALSE;
    }
    
    // Update the PWRDN bit
    if (isPowerUp)
    {
        // Power up the PHY
        ctrlReg &= ~PHY_PWRDN;
    }
    else
    {
        // Power down the PHY
        ctrlReg |= PHY_PWRDN;
    }
    
    // Write the updated PHY control register
    if (!writePhyRegister(PHY_CONTROL_REGISTER, ctrlReg))
    {
        return FALSE;
    }
    
    return TRUE;
}

//--------------------------------------------------------------------------------------------
