////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) GHI Electronics, LLC.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <ThirdParty\GHI\Include\GHI_OSHW_HAL.h>

#define TIMEOUT_STICK 2000 // millisecon

#define I2CDELAY(x) // depends on clock Khz...

#define ClearSCL() MakePinOutput(_i2c->scl)
#define ClearSDA() MakePinOutput(_i2c->sda)

BOOL _start;
GHI_OSHW_HAL_SoftwareI2C *_i2c;

void MakePinOutput(GPIO_PIN pin)
{
	CPU_GPIO_EnableOutputPin(pin, FALSE);
}

void MakePinInput(GPIO_PIN pin)
{
	CPU_GPIO_EnableInputPin(pin, FALSE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP);
}

BOOL ReadPinState(GPIO_PIN pin)
{
	CPU_GPIO_EnableInputPin(pin, FALSE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP);
	return CPU_GPIO_GetPinState(pin);
}

BOOL ReadBit()
{
    // "ReadSDA" makes SDA an input - processor lets go of pin and internal
    //  pull-up resistor makes it high.  Now slave can drive the pin.
    ReadPinState(_i2c->sda);

    I2CDELAY(i2cSpeed);

    // Clock stretching - Makes SCL an input and pull-up resistor makes
    //  the pin high.  Slave device can pull SCL low to extend clock cycle.
    long endStretch = 0;//Utility.GetMachineTime().Ticks + timeOutTicks;
    while (ReadPinState(_i2c->scl)==0 && endStretch <TIMEOUT_STICK)
    {
		// How long have we been stuck in the while loop?
		//if (Utility.GetMachineTime().Ticks >= endStretch)
		//    throw new TimeOutException();    // Too long, so bail out by throwing an exception.
		HAL_Time_Sleep_MicroSeconds_InterruptEnabled(1); // 1 microsecond;
		endStretch++;
     }
     // At this point, SCL is high and SDA is valid - so read the bit.
    BYTE bit = ReadPinState(_i2c->sda);

    I2CDELAY(i2cSpeed);

    ClearSCL();     // Pull the serial clock line low ...

    return bit;     //  and return.
}

BOOL WriteBit(BOOL bit)
{
    if (bit)
    {
		ReadPinState(_i2c->sda);      // Make SDA an input ... so pin is pulled up.
    }
    else
    {
		ClearSDA();     // Make SDA an output ... so pin is pulled low.
    }
    I2CDELAY(i2cSpeed);
     // Clock stretching - Makes SCL an input and pull-up resistor makes
     //  the pin high.  Slave device can pull SCL low to extend clock cycle.
    long endStretch = 0;//Utility.GetMachineTime().Ticks + TIMEOUT_STICK;
    while (!ReadPinState(_i2c->scl)&& endStretch <TIMEOUT_STICK)
    {
		// How long have we been stuck in the while loop?
		//if (Utility.GetMachineTime().Ticks >= endStretch)
		//   throw new TimeOutException();    // Too long, so bail out by throwing an exception.
		HAL_Time_Sleep_MicroSeconds_InterruptEnabled(1); // 1 microsecond;
		endStretch++;
    }
    // SCL is high and SDA is valid ...
    //  Check that nobody else is driving SDA
    if (bit && !ReadPinState(_i2c->sda))
    {
		return FALSE;// Lost arbitration
    }

    I2CDELAY(i2cSpeed);
    ClearSCL();

    return TRUE;    // Success!
}

BOOL SendStartCondition()
{
    if (_start)
    {
		// set SDA to 1
		ReadPinState(_i2c->sda);
		I2CDELAY(i2cSpeed);
		//
		// Clock stretching - Makes SCL an input and pull-up resistor makes
		//  the pin high.  Slave device can pull SCL low to extend clock cycle.
		long endStretch = 0;//Utility.GetMachineTime().Ticks + TIMEOUT_STICK;
		while (!ReadPinState(_i2c->scl) && endStretch <TIMEOUT_STICK )
		{
			// How long have we been stuck in the while loop?
			//if (Utility.GetMachineTime().Ticks >= endStretch)
			//throw new TimeOutException();    // Too long, so bail out by throwing an exception.
			HAL_Time_Sleep_MicroSeconds_InterruptEnabled(1); // 1 microsecond;
			endStretch++;
		}
    }

    if (!ReadPinState(_i2c->sda))
    {
		return FALSE;
    }

     // SCL is high, set SDA from 1 to 0
     ClearSDA();
     I2CDELAY(i2cSpeed);
     ClearSCL();

     _start = TRUE;

     return TRUE;
}

BOOL SendStopCondition()
 {
     // set SDA to 0
    ClearSDA();
    I2CDELAY(i2cSpeed);
    //
    // Clock stretching - Makes SCL an input and pull-up resistor makes
    //  the pin high.  Slave device can pull SCL low to extend clock cycle.
    long endStretch = 0;//Utility.GetMachineTime().Ticks + TIMEOUT_STICK;
    while (!ReadPinState(_i2c->scl)&& endStretch <TIMEOUT_STICK)
    {
		 // How long have we been stuck in the while loop?
		 //if (Utility.GetMachineTime().Ticks >= endStretch)
		 //    throw new TimeOutException();    // Too long, so bail out by throwing an exception.
		HAL_Time_Sleep_MicroSeconds_InterruptEnabled(1); // 1 microsecond;
		endStretch++;

    }
     //
     // SCL is high, set SDA from 0 to 1
    if (!ReadPinState(_i2c->sda))
    {
		return FALSE;
    }

     I2CDELAY(i2cSpeed);
     _start = FALSE;

     return TRUE;
 }

BOOL Transmit(BOOL sendStartCondition, BOOL sendStopCondition, BYTE ByteToSend)
{			
	UINT32 bit;
	BOOL nack;
	if (sendStartCondition)
	{
		SendStartCondition();
	}
	
    for (bit = 0; bit < 8; bit++)
    {
		WriteBit((ByteToSend & 0x80) != 0);

		ByteToSend <<= 1;
    }
    
    nack = ReadBit();
     //
	if (sendStopCondition)
	{
		SendStopCondition();
	}
     // Return value is "TRUE" for NAK
     //  "FALSE" for ACK.
     return nack;
}

BYTE Receive(BOOL sendAcknowledgeBit, BOOL sendStopCondition)
{
    BYTE d = 0;
    //BOOL b;
	UINT32 bit=0;
    for (bit = 0; bit < 8; bit++)
    {
		d <<= 1;

		//b = ReadBit();
		if ( ReadBit())
			d |= 1;
	}
		 //
	WriteBit(!sendAcknowledgeBit);
		 //
	if (sendStopCondition)
	{
		SendStopCondition();
    }
     //
    return d;
 }

 UINT32 WriteBytes( BYTE *data, UINT32 length)
 {
	if (length == 0) return 0;
    UINT32 numWrite=0;
    UINT32 i = 0;
    if (!Transmit(TRUE, FALSE, (BYTE)((_i2c->address) << 1)))
    {
		for (i = 0; i < length - 1; i++)
		{
			if (!Transmit(FALSE, FALSE, data[i]))
			{
				numWrite++;
			}

		}
    }
    if (!Transmit(FALSE, TRUE, data[i])) numWrite++;
    return numWrite;
 }

UINT32 ReadBytes(BYTE *data, UINT32 length)
{
	if (length == 0) return 0;
    UINT32 numRead = 0;
    UINT32 i = 0;
    if (Transmit(TRUE, FALSE, (BYTE)(((_i2c->address) << 1) | 1)))
    {
		for (i = 0; i < length - 1; i++)
		{
			data[i] = Receive(TRUE, FALSE);
			numRead++;
		}
    }

    data[i] = Receive(FALSE, TRUE);
	numRead++;

    return numRead;
}

BOOL SoftwareI2C_Initialize(GHI_OSHW_HAL_SoftwareI2C *i2c)
{
	_i2c = i2c;
	
	_start = FALSE;
	MakePinInput(_i2c->sda);
	MakePinInput(_i2c->scl);

	return TRUE;
}

BOOL GHI_OSHW_HAL_SoftwareI2C_WriteRead(GHI_OSHW_HAL_SoftwareI2C *i2c, BYTE *writeBuffer, UINT32 writeLength, BYTE *readBuffer, UINT32 readLength, UINT32 *numWritten, UINT32 *numRead)
{
	*numWritten = *numRead = 0;

	SoftwareI2C_Initialize(i2c);
	UINT32 i   = 0;
	BYTE write =0;
	BYTE read  =0;
	// For write 
    if (writeLength >0) 
    {
		if (!Transmit(TRUE, FALSE, (BYTE)((_i2c->address) << 1))) // Write address for write
		{
			for (i = 0; i < writeLength - 1; i++)
			{
				if (!Transmit(FALSE, FALSE, writeBuffer[i])) // Write data 
				{
					(write)++;
				}

			}
		}
		// if (!Transmit(FALSE, TRUE, writeBuffer[i])) (write)++; // Write last byte - send stop condition
			// *numWritten = write;
		// 26-07-2012
		// Fix issue: Remove Stop condition if user want to Write-Read in one transaction
		if (!Transmit(FALSE, (readLength==0), writeBuffer[i])) (write)++; 
		*numWritten = write;
		
    }
	// For Read
    if (readLength > 0) 
    {
		if (!Transmit(TRUE, FALSE, (BYTE)(((_i2c->address) << 1) | 1))) // Write address for Read
		{
			for (i = 0; i < readLength - 1; i++)
			{
				readBuffer[i] = Receive(TRUE, FALSE);
				(read)++;
			}
		}
		readBuffer[i] = Receive(FALSE, TRUE); // Read  last byte
		(read)++;
		*numRead = read;
    }

	return (write+read) == (writeLength+readLength);
}