#ifndef __ECATSLAVE_SLAVE_APP__
#define __ECATSLAVE_SLAVE_APP__

#include <stdint.h>
#include <vxworksCommon.h>
/* Macros */

/* Macros for Logging SDO upload / download request message */
#define WELD_RECIPE_OBJ_IND 0x210A
#define MAX_PRINT_WELD_RECIPE_FIELD_SIZE 100

//#define WELD_RECIPE_CONSOLE_LOGGING 
#define NULL 0



/* Structures */
typedef unsigned char 	Uint8 ;
typedef char			Int8 ;
typedef short             Int16;
typedef int            Int32;
typedef unsigned short  Uint16;
typedef unsigned long   Uint32;
typedef float           Float32;
typedef double     Float64;

typedef uint8_t Uint8_t ;

#endif
