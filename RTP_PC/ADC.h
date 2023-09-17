/*
 * ADC.h
 *
 *  Created on: Jul 3, 2017
 *      Author: hasanchez
 */

#ifndef SRC_ADC_H_
#define SRC_ADC_H_



#define NUM_TRANSFERS 1
//CFG_OVERWRITE | INCC_UPDIFFPAIR_INXREF_GND | 7_CHANNELS | BW_FULL| REF_EREF_IBUFF_TEMPDISABLED | SEQ_SCAN_WITHOUTTEMPERATIRE | RB_CFGDISBALE
#define ADC_CONFIG 0xE9FC

#define PDT 3
#define DT  5
#define ADC_LEN  2
#define FPGA_LEN 4

#define CONFIG_OF_CHANNELS 4
#define CHANNELS_AVAILABLE (CONFIG_OF_CHANNELS + 1)
#define SIZE_OF_CONFIGS 2
#define DONT_CARE_BITS 2

// The next 2 values are for the scaling of the ADC input resistors. This is to ensure that 10V = 100%
// PACC board and DCX equivalent circuits used a divider of 7/6.
#define ADC_NUM_FACTOR     13
#define ADC_DENOM_FACTOR   10
#define ADC_RIGHT_SHIFT_BITS(x)  (x >> 5)
#define FANENABLE_TEMP  60    //60 degC

enum ADC_CH_SELECT
{
	ADC_AMPLITUDE,
	ADC_POWER,
	ADC_DCC,
	ADC_TEMP,
	ADC_PHASE,
	ADC_SBEAM,
	BATT_VOLT

};


/*The ADC_DATA will be mapped on memory as follow:
 
 ADC_DATA:
 ---------------------------
 | Amplitude   or DataADC[0] |     16 bits
 | Power       or DataADC[1] |     16 bits
 | DCC         or DataADC[2] |     16 bits
 | Temperature or DataADC[3] |     16 bits
 | Phase       or DataADC[4] |     16 bits
 ----------------------------

With the union we give freedom to reference to that particular piece of memory 
as element of an array or as single parameter.

E.g. The next two lines are doing the same but in different ways. 
x.SingleParam.Amplitude =100;
x.DataADC[0]=100;
 
 it is required to easy handling of data in this case to load the memory using a conditional for.*/
typedef union 
{
	 struct 
	{
		UINT16	Amplitude;
		UINT16 	Power;
		UINT16	DCC;
		UINT16	Temperature;
		UINT16	Phase;
	}SingleParam;
	 
	UINT16 DataADC[CHANNELS_AVAILABLE];
}ADC_DATA;



class ADC
{
	public:
		ADC();
		virtual ~ADC();
		static void ReadADC(void);
		static void SetADCSequence(void);
		static void  ScaleADCValues(void);
		
		static UINT32 	GetAmplitude 	();
		static UINT32 	GetPower 		();
		static UINT32 	GetDCC 			();
		static UINT32 	GetPhase 		();
		static UINT16   GetPhaseInDeg   ();
		
		static UINT32 	GetAmpPercent	();
		static UINT32   GetDCCPercent   ();
		static UINT32 	GetTotalPwr 	();
		static UINT32  GetTemperatureInDegC	();
		static void    SetADCReInitRequest(UINT8);
		static UINT8   IsADCReInitRequested();
		
	private:
		//Values Actually loaded from the
		
		static ADC_DATA  ADC_Data;
		// ADC values. These values are scaled 
		static UINT32 Amplitude;
		static UINT32 Power;
		static UINT32 DCC;
		static UINT32 Phase;
		static UINT32 Temperature;
		static UINT8  ReInitRequestFlag;
	
};




#endif /* SRC_ADC_H_ */
