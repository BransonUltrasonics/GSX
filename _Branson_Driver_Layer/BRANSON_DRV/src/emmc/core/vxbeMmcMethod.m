#include <vxWorks.h>
#include <hwif/vxBus.h>
#include <vxbeMmcLib.h>

METHOD STATUS vxbSdspecinfoGet(VXB_DEV_ID pDev, void ** pHostSpec, VXB_SD_CMDISSUE_FUNC * pCmdIssue); default[ERROR] # "Get vxBus EMMC spec info method"
