/* ntp_mem.h - Wind River NTP memory library */

/*
 ******************************************************************************
 *                     INTERPEAK API HEADER FILE
 *
 *   Document no: @(#) $Name: VXWORKS_ITER33_2015071414 $ $RCSfile: ntp_mem.h,v $ $Revision: 1.1.10.2 $
 *   $Source: /home/interpeak/CVSRoot/ipntp/include/ntp_mem.h,v $
 *   $Author: lyang3 $ $Date: 2015-03-31 09:17:18 $
 *   $State: Exp $ $Locker:  $
 *
 *   INTERPEAK_COPYRIGHT_STRING
 *   Design and implementation by Windriver
 ******************************************************************************
 */
#ifndef NTP_MEM_H
#define NTP_MEM_H

/*
 ****************************************************************************
 * 1                    DESCRIPTION
 ****************************************************************************
 * Public header file of NTP memory.
 */

/*
DESCRIPTION
This library contains the APIs provided by the WindRiver NTP.
INCLUDE FILES: 
*/


/*
 ****************************************************************************
 * 2                    CONFIGURATION
 ****************************************************************************
 */

/*
 ****************************************************************************
 * 3                    INCLUDE FILES
 ****************************************************************************
 */
#include <ipcom_type.h>
#include <ipcom_time.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 ****************************************************************************
 * 4                    DEFINES
 ****************************************************************************
 */

/*
 ****************************************************************************
 * 5                    TYPES
 ****************************************************************************
 */

/*
 ****************************************************************************
 * 6                    FUNCTIONS
 ****************************************************************************
 */

int   ntp_mempool_init(void);
void  ntp_mempool_free(void);
/*void* emalloc(Ip_size_t size);*/
void  efree(void* p);


#ifdef __cplusplus
}
#endif

#endif


/*
 ****************************************************************************
 *                      END OF FILE
 ****************************************************************************
 */
