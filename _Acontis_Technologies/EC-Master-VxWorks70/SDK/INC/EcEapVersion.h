/*-----------------------------------------------------------------------------
 * EcEapVersion.h               
 * Copyright                acontis technologies GmbH, D-88250 Weingarten, Germany
 * Description              EC-EAP stack version information.
 *---------------------------------------------------------------------------*/

/* Version information */
#define EAP_VERS_MAJ             0      /* major version */
#define EAP_VERS_MIN             8      /* minor version */
#define EAP_VERS_SERVICEPACK     5      /* service pack */
#define EAP_VERS_BUILD           0      /* build number */

#define EAP_VERSION ((EAP_VERS_MAJ << 3*8)              \
                    | (EAP_VERS_MIN << 2*8)              \
                    | (EAP_VERS_SERVICEPACK << 1*8)      \
                    | (EAP_VERS_BUILD << 0*8)            \
                    )

#define EAP_FILEVERSION      EAP_VERS_MAJ,EAP_VERS_MIN,EAP_VERS_SERVICEPACK,EAP_VERS_BUILD
#define EAP_FILEVERSIONSTR   "0.8.5.0\0"

#define EAP_PRODVERSION      EAP_FILEVERSION
#define EAP_PRODVERSIONSTR   EAP_FILEVERSIONSTR
#define EAP_COPYRIGHT        "Copyright acontis technologies GmbH @ 2015\0"