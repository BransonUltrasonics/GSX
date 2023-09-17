/*-----------------------------------------------------------------------------
 * SCUsrConfig.h
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              EtherCAT Master demo header
 *---------------------------------------------------------------------------*/

#ifndef __SCUSRCONFIG__
#define __SCUSRCONFIG__

/*-INCLUDES------------------------------------------------------------------*/

#include EC_PACKED_INCLUDESTOP
/*-MACROS--------------------------------------------------------------------*/
#define MAX_OD_SUBINDEX             20

/* PDO SDO Test Scenarios Define*/
#define NUM_PDO_SCENARIOS   2
#define NUM_SDO_SCENARIOS   9

#define PARAM_INVALID				-1			/* Invalid Data					*/
#define PARAM_ZERO					0

#define RxPDO_SUPPORT
#define TxPDO_SUPPORT
//#define SDO_REQUEST

#define MAX_SLAVES			6				/* Max slaves					*/
#define SIXTEEN_BITS		16				/* Macro for two bytes 			*/

/* SDO Print Macro Parameters */
#define MAX_DATA_STRING_LENGTH 100
#define MAX_BYTE_STRING_LENGTH 6

#define TXPDO_INPUT_SIZE 	14				/* TxPDO input size in bytes 	*/
#define RXPDO_OUTPUT_SIZE 	10				/* RxPDO output size in bytes 	*/


//#define COE_SDO_SLAVE_MBX_SIZE                 0x12
//#define COE_SDO_SLAVE_MBX_SIZE                 0xC8		// Max MailBox Buffer size is 200Bytes (Ultrasonic)
#define COE_SDO_SLAVE_MBX_SIZE                 0x100		


/* Transfer ID parameters */

#define TFR_ID_SLAVE_ID_SHIFT 16
#define TFR_ID_SLAVE_ID_MASK 0x00FF0000
#define TFR_ID_SUBINDEX_SHIFT 24
#define TFR_ID_SUBINDEX_MASK 0x7F000000
#define TFR_ID_ACCESS_MASK 0x80000000

/* Weld Recipe Parameters  */
#define EL437x_INDEX_FIRST                 0x1000
#define EL437x_INDEX_WELD_RECIPE             0x210A
#define EL437x_SUBINDEX_ENTRIES                  0
#define EL437x_SUBINDEX_WELD_DELAY               1
#define EL437x_SUBINDEX_WELD_TIME                2
#define EL437x_SUBINDEX_INTENSITY                3
#define EL437x_SUBINDEX_CTRL_RECIPE_OVERRIDES    4
#define EL437x_SUBINDEX_INTENSITY_FACTOR         5
#define EL437x_SUBINDEX_PART_REFLECTIVITY_FACTOR 6
#define EL437x_SUBINDEX_TOOL_REFLECTIVITY_FACTOR 7

/* Data Length */
#define EL437x_SUBINDEX_ENTRIES_SIZE							2
#define EL437x_SUBINDEX_WELD_DELAY_SIZE 				2
#define EL437x_SUBINDEX_WELD_TIME_SIZE           		4
#define EL437x_SUBINDEX_INTENSITY_SIZE           		1
#define EL437x_SUBINDEX_CTRL_RECIPE_OVERRIDES_SIZE   	1
#define EL437x_SUBINDEX_INTENSITY_FACTOR_SIZE        	2
#define EL437x_SUBINDEX_PART_REFLECTIVITY_FACTOR_SIZE 	2
#define EL437x_SUBINDEX_TOOL_REFLECTIVITY_FACTOR_SIZE 	2
#define EL437x_WELD_RECIPE_DOWNLOAD_TOTAL_SIZE			14

/* Offset */
#define EL437x_SUBINDEX_ENTRIES_OFFSET						0
#define EL437x_SUBINDEX_WELD_DELAY_OFFSET					EL437x_SUBINDEX_ENTRIES_SIZE
#define EL437x_SUBINDEX_WELD_TIME_OFFSET					EL437x_SUBINDEX_WELD_DELAY_OFFSET+	EL437x_SUBINDEX_WELD_DELAY_SIZE
#define EL437x_SUBINDEX_INTENSITY_OFFSET					EL437x_SUBINDEX_WELD_TIME_OFFSET+EL437x_SUBINDEX_WELD_TIME_SIZE
#define EL437x_SUBINDEX_CTRL_RECIPE_OVERRIDES_OFFSET		EL437x_SUBINDEX_INTENSITY_OFFSET+EL437x_SUBINDEX_INTENSITY_SIZE
#define EL437x_SUBINDEX_INTENSITY_FACTOR_OFFSET				EL437x_SUBINDEX_CTRL_RECIPE_OVERRIDES_OFFSET+EL437x_SUBINDEX_CTRL_RECIPE_OVERRIDES_SIZE
#define EL437x_SUBINDEX_PART_REFLECTIVITY_FACTOR_OFFSET		EL437x_SUBINDEX_INTENSITY_FACTOR_OFFSET+EL437x_SUBINDEX_INTENSITY_FACTOR_SIZE
#define EL437X_SUBINDEX_TOOL_REFLECTIVITY_FACTOR_OFFSET		EL437x_SUBINDEX_PART_REFLECTIVITY_FACTOR_OFFSET+EL437x_SUBINDEX_PART_REFLECTIVITY_FACTOR_SIZE

/* Weld Recipe SDO Print Macro Parameters */
#define MAX_DATA_STRING_LENGTH 100
#define MAX_BYTE_STRING_LENGTH 6

/* Unique ID parameters */
#define EL437x_UNIQUE_ID_SIZE 2
#define EL437x_SDO_DATA_OFFSET 2
/*-DEFINES-------------------------------------------------------------------*/
typedef struct s_RxPDO {
EC_T_BYTE   bylaserTempCh1;
EC_T_BYTE   bylaserTempCh2;
EC_T_BYTE   bylaserTempCh3;
EC_T_BYTE   bylaserTempCh4;
EC_T_BYTE   bylaserTempCh5;
} t_RxPDO;

typedef struct s_OD_OffsetMapping {
EC_T_WORD       wObjectIndex;       /* holds the object Number */
EC_T_BYTE       byIndexOffset[MAX_OD_SUBINDEX];
} t_OD_OffsetMapping;

/*-GLOBAL DECLARATIONS------------------------------------------------------*/

/*-FUNCTION DECLARATIONS------------------------------------------------------*/
EC_T_DWORD  getObjectSubIndexOffset(EC_T_DWORD dwODIndex, EC_T_DWORD dwODSubIndex) ;

#endif

/*-END OF SOURCE FILE--------------------------------------------------------*/
