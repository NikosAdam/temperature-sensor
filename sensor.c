#include	<pic.h>


 //----------------------CONFIGURE DEVICE--------------------

	__CONFIG(INTIO & WDTDIS & PWRTDIS & MCLRDIS &
			UNPROTECT & BORDIS & IESODIS & FCMDIS);

//-----------------------DATA MEMORY------------------------

unsigned char LED_Output = 0;	//assigned to PORTC to light
								//connected LEDs



//----------------------PROGRAM MEMORY----------------------


/** PRIVATE PROTOTYPES *********************************************/
void Initialize(void);
void Get_Inputs(void);
void Decide(void);
void Do_Outputs(void);
void Timing(void);





/**SUPPORT ROUTINES*************************************************/

/*---------------------------------------------------------
	Subroutine: Delay_1mS
	Parameters: none
	Returns:nothing
	Synopsys:Creates a 1mS delay when called
---------------------------------------------------------*/
void Delay_1mS(void)
{
	unsigned int delay_var = 98;
	//Keep looping until the delay_var is
	// equal to zero (should take 1mS)
	while(--delay_var);
}


/**INTERRUPT CODE*************************************************/




/**SOFTWARE CONTROL LOOP CODE*************************************/
/*----------------------------------------------------------
	Subroutine: main
	Parameters: none
	Returns:	nothing
	Synopsys:	Main program function
-----------------------------------------------------------*/
void main(void)
{

	Initialize(); //Initialize the relevant registers
	while(1)
	{
		Get_Inputs();
		Decide();
		Do_Outputs();
	}

}

/*----------------------------------------------------------
	Subroutine: INITIALIZE
	Parameters: none
	Returns:	nothing
	Synopsys:	Initializes all registers
				associated with the application by calling
				initialization functions for each peripheral
----------------------------------------------------------*/
void Initialize(void)
{
	//Configure Port:
	//Disable pin output driver (See TRIS register)
	TRISB4 = 1;
	// Configure pin as analog
	ANS10 = 1;
	//Configure RC0, RC1, RC2 and RC3 as digital output
	PORTC = 0;
	TRISC0 = 0;
	TRISC1 = 0;
	TRISC2 = 0;
	TRISC3 = 0;
	ANS4 = 0;
	ANS5 = 0;
	ANS6 = 0;
	ANS7 = 0;
	//Configure the ADC module:
	//Select ADC conversion clock Frc
	ADCS0 = 1;
	ADCS1 = 1;
	ADCS2 = 1;
	//Configure voltage reference using VDD
	VCFG = 0;
	//Select ADC input channel Pin 13 (RB4/AN10)
	CHS0 = 0;
	CHS1 = 1;
	CHS2 = 0;
	CHS3 = 1;
	//Select result format left justified
	ADFM = 0;
	//Turn on ADC module
	ADON = 1;
}

/*----------------------------------------------------------
	Subroutine: Get_Inputs
	Parameters: none
	Returns:	nothing
	Synopsys:	Obtains all off-chip inputs
----------------------------------------------------------*/
void Get_Inputs(void)
{
	//Perform an ADC of potentiometer connected to pin 13
	//Wait the required acquisition time
	Delay_1mS();
	//Start conversion by setting the GO/DONE bit.
	GODONE = 1;
	//Wait for ADC conversion to complete
	//Polling the GO/DONE bit
	// 0 = ADC completed
	// 1 = ADC in progress
	while(GODONE == 1);
}

/*----------------------------------------------------------
	Subroutine: Decide
	Parameters: none
	Returns:	nothing
	Synopsys:	Function that calls all sub-routines for
				processing the input data
----------------------------------------------------------*/
void Decide(void)
{
	
	LED_Output = ADRESH >> 4; 	//Shifts the bits in ADRESL 4 bits
								//to the right
}

/*----------------------------------------------------------
	Subroutine: Do_Outputs
	Parameters: none
	Returns:	nothing
	Synopsys:	Ouputs to off-chip circuitry based off of
				Decide function
----------------------------------------------------------*/
void Do_Outputs(void)
{
	//Assign contents of LED_Output to PORTC to light the connected
	//LEDs
	PORTC = LED_Output;
}
