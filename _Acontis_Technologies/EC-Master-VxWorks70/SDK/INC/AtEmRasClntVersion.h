/*-----------------------------------------------------------------------------
 * AtEmRasClntVersion.h      file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Willig, Andreas
 * Description              description of file
 * Date                     2007/5/4::7:25
 *---------------------------------------------------------------------------*/

#ifndef __ATEMRASCLNTVERSION_H__
#define __ATEMRASCLNTVERSION_H__     1

/*-DEFINES-------------------------------------------------------------------*/

#define ATEMRASCLNT_VERS_MAJ             1   /* major version */
#define ATEMRASCLNT_VERS_MIN             9   /* minor version */
#define ATEMRASCLNT_VERS_SERVICEPACK     2   /* service pack */
#define ATEMRASCLNT_VERS_BUILD          99   /* build number */

#define ATEMRASCLNT_VERSION (                   \
      (ATEMRASCLNT_VERS_MAJ << 3*8)             \
    | (ATEMRASCLNT_VERS_MIN << 2*8)             \
    | (ATEMRASCLNT_VERS_SERVICEPACK << 1*8)     \
    | (ATEMRASCLNT_VERS_BUILD << 0*8)           \
                        )

#define ATEMRASCLNT_FILEVERSION     ATEMRASCLNT_VERS_MAJ,ATEMRASCLNT_VERS_MIN,ATEMRASCLNT_VERS_SERVICEPACK,ATEMRASCLNT_VERS_BUILD
#define ATEMRASCLNT_FILEVERSIONSTR  "1.8.1.99\0"

#endif /* __ATEMRASCLNTVERSION_H__ */

/*-END OF SOURCE FILE--------------------------------------------------------*/
