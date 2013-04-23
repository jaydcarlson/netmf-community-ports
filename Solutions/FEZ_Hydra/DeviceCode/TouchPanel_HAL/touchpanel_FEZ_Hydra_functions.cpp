////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#include <GHI\Include\GHI_Touch_AnalogIn.h>
#include <pal\TouchPanel\touchpanel_driver.h>

#define TOUCHSCREEN_ADC_CONTROLLER_CHANNEL_DISABLE_REGISTER				(*(volatile unsigned long *)0xFFFD0014) // TSADCC_MR

// analog touch

struct AT_PinsConfig
{
	// GPIO pins
	BYTE YU;
	BYTE YD;
	BYTE YU_state;
	BYTE XL;
	BYTE XR;
	BYTE XL_state;

	// Analog read pins
	BYTE XA;	// XL, or XR analog pin to read Y!
	BYTE YA;	
};

// calibration
#define MAX_CAL_POINTS	5
short AT_CAL_EMX_sx[MAX_CAL_POINTS] = {160, 32, 32, 288, 288}, AT_CAL_EMX_sy[MAX_CAL_POINTS] = {120, 24, 216, 216, 24},
	  AT_CAL_EMX_cx[MAX_CAL_POINTS] = {483, 179, 184, 775, 790}, AT_CAL_EMX_cy[MAX_CAL_POINTS] = {467, 217, 738, 736, 225};

//const AT_PinsConfig _pinsEMX = {5, 73, FALSE, 8, 72, FALSE, 0, 1};	// EMX
//const AT_PinsConfig _pinsHydra = {((0 * 32) + 20), ((1 * 32) + 1), FALSE, ((0 * 32) + 18), ((1 * 32) + 28), FALSE, 1, 3};
const AT_PinsConfig _pinsHydra = {20, 33, FALSE, 18, 60, FALSE, 1, 3};

AT_PinsConfig _pins;

#define AT_MIN_POINT_VALID	30
#define AT_POLL_TIME	(1000 * 100)//(1000 * 200 * 5)//(1000 * 100)

BYTE wasDown;
GPIO_INTERRUPT_SERVICE_ROUTINE IsrProc;
HAL_COMPLETION touchCompletion;
extern TOUCH_PANEL_SamplingSettings g_TouchPanel_Sampling_Settings;
UINT32 AT_AVERAGE_COUNT = 0;
BOOL useDefaultTouch = TRUE;


#define _ANALOG_SETUP_DELAY 30
#define _ANALOG_READ_DELAY 5


void AT_ReadXY(int *x, int *y)
{
	int total, i;

	// setup X
	CPU_GPIO_EnableOutputPin(_pins.XL, _pins.XL_state);
	CPU_GPIO_EnableOutputPin(_pins.XR, !_pins.XL_state);

	//if(GHI_OSH_HAL_AnalogIn_GetPin(_pins.YA) != _pins.YU)
	if(AD_GetPinForChannel((ANALOG_CHANNEL)_pins.YA) != _pins.YU) // YA --- .NetMF Analog Channel 1
		CPU_GPIO_EnableInputPin(_pins.YU, FALSE, NULL, GPIO_INT_NONE, RESISTOR_DISABLED);
	else
		CPU_GPIO_EnableInputPin(_pins.YD, FALSE, NULL, GPIO_INT_NONE, RESISTOR_DISABLED);

	//GHI_OSH_HAL_AnalogIn_Init(_pins.YA);
	AD_Initialize((ANALOG_CHANNEL)_pins.YA, 0); // YA --- .NetMF Analog Channel 1 *** 0 is not used 
	HAL_Time_Sleep_MicroSeconds_InterruptEnabled(_ANALOG_SETUP_DELAY);
	*x = AD_Read((ANALOG_CHANNEL)_pins.YA);
	//GHI_OSH_HAL_AnalogIn_Read(_pins.YA, (UINT32*)x);
	
	total = 0;
	for(i = 0; i < AT_AVERAGE_COUNT; i++)
	{
		*x = AD_Read((ANALOG_CHANNEL)_pins.YA);
		//HAL_Time_Sleep_MicroSeconds_InterruptEnabled(_ANALOG_READ_DELAY);
		//GHI_OSH_HAL_AnalogIn_Read(_pins.YA, (UINT32*)x);
		total += *x;
	}
	*x = total / AT_AVERAGE_COUNT;

	CPU_GPIO_DisablePin(_pins.YU, RESISTOR_PULLUP, 0, GPIO_ALT_PRIMARY);
	TOUCHSCREEN_ADC_CONTROLLER_CHANNEL_DISABLE_REGISTER = (1 << (unsigned int)_pins.YA);
	//GHI_OSH_HAL_AnalogIn_Uninit(_pins.YA);

	// setup Y
	CPU_GPIO_EnableOutputPin(_pins.YU, _pins.YU_state);
	CPU_GPIO_EnableOutputPin(_pins.YD, !_pins.YU_state);

	//if(GHI_OSH_HAL_AnalogIn_GetPin(_pins.XA) != _pins.XL)
	if(AD_GetPinForChannel((ANALOG_CHANNEL)_pins.XA) != _pins.XL) // XA --- .NetMF Analog Channel 2
		CPU_GPIO_EnableInputPin(_pins.XL, FALSE, NULL, GPIO_INT_NONE, RESISTOR_DISABLED);
	else
		CPU_GPIO_EnableInputPin(_pins.XR, FALSE, NULL, GPIO_INT_NONE, RESISTOR_DISABLED);

	//GHI_OSH_HAL_AnalogIn_Init(_pins.XA);
	AD_Initialize((ANALOG_CHANNEL)_pins.XA, 0); // XA --- .NetMF Analog Channel 2 *** 0 is not used
	HAL_Time_Sleep_MicroSeconds_InterruptEnabled(_ANALOG_SETUP_DELAY);
	*y = AD_Read((ANALOG_CHANNEL)_pins.XA);
	//GHI_OSH_HAL_AnalogIn_Read(_pins.XA, (UINT32*)y);

	total = 0;
	for(int i = 0; i < AT_AVERAGE_COUNT;i++)
	{
		*y = AD_Read((ANALOG_CHANNEL)_pins.XA);
		//HAL_Time_Sleep_MicroSeconds_InterruptEnabled(_ANALOG_READ_DELAY);
		//GHI_OSH_HAL_AnalogIn_Read(_pins.XA, (UINT32*)y);
		total += *y;
	}
	*y = total / AT_AVERAGE_COUNT;


	// was pressed??
	if(*x < AT_MIN_POINT_VALID || *y < AT_MIN_POINT_VALID)
	{
		*x = *y = -1;
	}

	CPU_GPIO_DisablePin(_pins.XL, RESISTOR_PULLUP, 0, GPIO_ALT_PRIMARY);
	TOUCHSCREEN_ADC_CONTROLLER_CHANNEL_DISABLE_REGISTER = (1 << (unsigned int)_pins.XA); // Disables the channel in the register | 2 is Actual Processor Channel
	//GHI_OSH_HAL_AnalogIn_Uninit(_pins.XA);
}

void AT_Completion(void *arg)
{
	int x, y;
	AT_ReadXY(&x, &y);

	if(wasDown)
	{
		// released?
		if(x < 0)
		{
			wasDown = FALSE;
			IsrProc(0,wasDown, 0);
		}
	}
	else
	{
		// pressed?
		if(x > 0)
		{
			wasDown = TRUE;
			IsrProc(0,wasDown, 0);
		}
	}

	if (!touchCompletion.IsLinked())
	{
		touchCompletion.EnqueueDelta(AT_POLL_TIME);
	}   
}

void GHI_OSH_HAL_Touch_UseDefault(BOOL useDefault)
{
	useDefaultTouch = useDefault;
}

BOOL HAL_TOUCH_PANEL_Enable( GPIO_INTERRUPT_SERVICE_ROUTINE touchIsrProc )
{   
	GLOBAL_LOCK(irq);

	if(!useDefaultTouch)
		return TRUE;

	_pins = _pinsHydra;
		AT_AVERAGE_COUNT = 16; // 4 works goon only on 320x240

		TouchPanel_Driver::SetCalibration(MAX_CAL_POINTS, AT_CAL_EMX_sx, AT_CAL_EMX_sy, AT_CAL_EMX_cx, AT_CAL_EMX_cy);



	IsrProc = touchIsrProc;

	if(CPU_GPIO_PinIsBusy(_pins.XL) ||
		CPU_GPIO_PinIsBusy(_pins.XR) ||
		CPU_GPIO_PinIsBusy(_pins.YU) ||
		CPU_GPIO_PinIsBusy(_pins.YD)
		)
		return FALSE;

	CPU_GPIO_ReservePin(_pins.XL, TRUE);
	CPU_GPIO_ReservePin(_pins.XR, TRUE);
	CPU_GPIO_ReservePin(_pins.YU, TRUE);
	CPU_GPIO_ReservePin(_pins.YD, TRUE);

	wasDown = FALSE;
	
	touchCompletion.InitializeForISR(AT_Completion, NULL);  
	if (!touchCompletion.IsLinked())
	{
		touchCompletion.EnqueueDelta(AT_POLL_TIME);
	}   

	return TRUE;
}

BOOL HAL_TOUCH_PANEL_Disable()
{
	GLOBAL_LOCK(irq);

	if(!useDefaultTouch)
		return TRUE;

	if(touchCompletion.IsLinked())
    {
        touchCompletion.Abort();
    }
	
	CPU_GPIO_DisablePin(_pins.YU, RESISTOR_PULLUP, 0, GPIO_ALT_PRIMARY);
	CPU_GPIO_DisablePin(_pins.YD, RESISTOR_PULLUP, 0, GPIO_ALT_PRIMARY);
	CPU_GPIO_DisablePin(_pins.XL, RESISTOR_PULLUP, 0, GPIO_ALT_PRIMARY);
	CPU_GPIO_DisablePin(_pins.XR, RESISTOR_PULLUP, 0, GPIO_ALT_PRIMARY);
	
	CPU_GPIO_ReservePin(_pins.YU, FALSE);
	CPU_GPIO_ReservePin(_pins.YD, FALSE);
	CPU_GPIO_ReservePin(_pins.XL, FALSE);
	CPU_GPIO_ReservePin(_pins.XR, FALSE);

	return TRUE;
}

void HAL_TOUCH_PANEL_GetPoint( TOUCH_PANEL_SAMPLE_FLAGS* pTipState, int* pSource, int* pUnCalX, int* pUnCalY )
{
    *pTipState = 0;
    *pUnCalX = 0;
    *pUnCalY = 0;
    *pSource = 0;

	if(!useDefaultTouch)
	{
		*pUnCalX = -1;
        *pUnCalY = -1;

		return ;
	}

    static bool  stylusDown = false;

    /// Apparently there's a lot of noise from the touch hardware. We will take several
    /// independent measures to compensate for them:
    /// 1. Settle down time (instead of reading right away, wait few moments) --> ReadsToIgnore
    /// 2. Read multiple samples (read a number of them, and then take average) --> ReadsPerSample.
    ///

    //UINT16 i = 0;
    //UINT16 totalReads = g_TouchPanel_Sampling_Settings.ReadsToIgnore + g_TouchPanel_Sampling_Settings.ReadsPerSample;

    int x;
    int y;

    INT32 validReadCount = 0;
	{
		GLOBAL_LOCK(irq);

		AT_ReadXY(&x, &y);
	}

	if(x >= 0 && y >= 0)
		validReadCount = 1;
    
    if (stylusDown)
    {
        *pTipState |= TouchSamplePreviousDownFlag;
    }

    if (validReadCount > 0)
    {
        *pTipState |= TouchSampleValidFlag;
        *pUnCalX = x;
        *pUnCalY = y;
        *pTipState |= TouchSampleDownFlag;
        stylusDown = true;
    }
    else
    {
        *pUnCalX = -1;
        *pUnCalY = -1;
        stylusDown = false;       
    }    
}

BOOL CalibrationPointGet(TOUCH_PANEL_CALIBRATION_POINT *pTCP)
{

    INT32   cDisplayWidth  = pTCP->cDisplayWidth;
    INT32   cDisplayHeight = pTCP->cDisplayHeight;

    int CalibrationRadiusX = cDisplayWidth  / 20;
    int CalibrationRadiusY = cDisplayHeight / 20;

    switch (pTCP -> PointNumber)
    {
    case    0:
        pTCP->CalibrationX = cDisplayWidth  / 2;
        pTCP->CalibrationY = cDisplayHeight / 2;
        break;

    case    1:
        pTCP->CalibrationX = CalibrationRadiusX * 2;
        pTCP->CalibrationY = CalibrationRadiusY * 2;
        break;

    case    2:
        pTCP->CalibrationX = CalibrationRadiusX * 2;
        pTCP->CalibrationY = cDisplayHeight - CalibrationRadiusY * 2;
        break;

    case    3:
        pTCP->CalibrationX = cDisplayWidth  - CalibrationRadiusX * 2;
        pTCP->CalibrationY = cDisplayHeight - CalibrationRadiusY * 2;
        break;

    case    4:
        pTCP->CalibrationX = cDisplayWidth - CalibrationRadiusX * 2;
        pTCP->CalibrationY = CalibrationRadiusY * 2;
        break;

    default:
        pTCP->CalibrationX = cDisplayWidth  / 2;
        pTCP->CalibrationY = cDisplayHeight / 2;
        
        return FALSE;
    }
    
    return TRUE;
}


HRESULT HAL_TOUCH_PANEL_GetDeviceCaps(unsigned int iIndex, void* lpOutput)
{
    if ( lpOutput == NULL )
    {
        return FALSE;
    }

    switch ( iIndex )
    {
    case TOUCH_PANEL_SAMPLE_RATE_ID:
        {
            TOUCH_PANEL_SAMPLE_RATE  *pTSR  = (TOUCH_PANEL_SAMPLE_RATE*)lpOutput;    

            pTSR->SamplesPerSecondLow       = g_TouchPanel_Sampling_Settings.SampleRate.SamplesPerSecondLow;
            pTSR->SamplesPerSecondHigh      = g_TouchPanel_Sampling_Settings.SampleRate.SamplesPerSecondHigh;
            pTSR->CurrentSampleRateSetting  = g_TouchPanel_Sampling_Settings.SampleRate.CurrentSampleRateSetting;
            pTSR->MaxTimeForMoveEvent_ticks = g_TouchPanel_Sampling_Settings.SampleRate.MaxTimeForMoveEvent_ticks;
        }
        break; 

    case TOUCH_PANEL_CALIBRATION_POINT_COUNT_ID:
        {
            TOUCH_PANEL_CALIBRATION_POINT_COUNT *pTCPC = (TOUCH_PANEL_CALIBRATION_POINT_COUNT*)lpOutput;            

            pTCPC->flags              = 0;
            pTCPC->cCalibrationPoints = 5;
        }
        break;

    case TOUCH_PANEL_CALIBRATION_POINT_ID:        
        return(CalibrationPointGet((TOUCH_PANEL_CALIBRATION_POINT*)lpOutput));

    default:        
        return FALSE;
    }

    return TRUE;
}

