/*-----------------------------------------------------------------------------
 * Copyright    acontis technologies GmbH, Weingarten, Germany
 * Response     Paul Bussmann
 *---------------------------------------------------------------------------*/

#ifndef INC_ECTEXT
#define INC_ECTEXT     1

/*-FUNCTIONS-----------------------------------------------------------------*/
const EC_T_CHAR* EcErrorText(   EC_T_DWORD  dwTextId        );
const EC_T_CHAR* EcNotifyText(  EC_T_DWORD  dwNotifyCode    );

EC_T_DWORD EcConvertEcErrorToAdsError(EC_T_DWORD dwErrorCode);
EC_T_DWORD EcConvertAdsErrorToEcError(EC_T_DWORD dwAdsError);

EC_T_DWORD EcConvertEcErrorToFoeError(EC_T_DWORD dwErrorCode);
EC_T_DWORD EcConvertFoeErrorToEcError(EC_T_DWORD dwFoeError);

EC_T_DWORD EcConvertEcErrorToSoeError(EC_T_DWORD dwErrorCode);
EC_T_DWORD EcConvertSoeErrorToEcError(EC_T_DWORD dwSoeError);

EC_T_DWORD EcConvertEcErrorToCoeError(EC_T_DWORD dwErrorCode);
EC_T_DWORD EcConvertCoeErrorToEcError(EC_T_DWORD dwCoeError);
#endif /* INC_ECTEXT */
