#ifndef __INCvxbPruh
#define __INCvxbPruh 

#ifdef __cplusplus
extern "C" {
#endif

#include <hwif/vxBus.h>
#include <hwif/util/vxbIsrDeferLib.h>

struct pru
    {
    UINT8 val;
    };

#define PRU_NAME            "pru"

typedef struct vxbPruFunctionality
    {
    char   pruName[MAX_DRV_NAME_LEN + 1];

    STATUS (*pruRead) (VXB_DEV_ID pInst, UINT32 addr, UINT8 *buf, UINT32 len,
                      UINT32* realByte);
    STATUS (*pruWrite)(VXB_DEV_ID pInst, UINT32 addr, UINT8 *buf, UINT32 len,
                      UINT32* realByte);
    } VXB_PRU_FUNC;

struct vxbPruHandle
    {
    struct vxbPruFunctionality *pPruFunc;
    VXB_DEV_ID                  pInst;
    };

/* IMPORT */

IMPORT STATUS vxbPruWrite (VXB_DEV_ID pInst, UINT32 addr, UINT8 *buf, UINT32 len,
                      UINT32* realByte);
IMPORT STATUS vxbPruRead (VXB_DEV_ID pInst, UINT32 addr, UINT8 *buf, UINT32 len,
                      UINT32* realByte);
IMPORT UINT32 pruDrvNextUnit ();
IMPORT STATUS pruDevCreate (char * name, VXB_DEV_ID pDev, VXB_PRU_FUNC * pCandidate);
IMPORT STATUS pruDevRemove (char * name);

#ifdef __cplusplus
}
#endif

#endif /* __INCvxbPruh */
