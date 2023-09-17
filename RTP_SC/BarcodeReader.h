/*
 * BarcodeReader.h
 *
 *  Created on: Apr 16, 2018
 *      Author: ee209315
 */

#ifndef BARCODEREADER_H_
#define BARCODEREADER_H_

/* Header files and Macros */
#include  "Common.h"
#include <evdevLib.h>

#define BARCODE_SCANNER_DEVICE_NAME		"/usb2Kbd/0"
#define BARCODE_SCANNER_DEVICE_MODE		0644
#define MAX_NUM_OF_BYTES_TO_READ		100
#define SCANNER_CONNECT_DELAY			10


/* Structure variables */
typedef struct BarcodeMessage
{
	INT32	msgID;
	char	Buffer[BARCODE_DATA_SIZE];	
}BarcodeMessage;


class BarcodeReader
{
public:	
	BarcodeReader();
	~BarcodeReader();	
	STATUS		BarcodeScannerConnect();
	bool 		GetScannerConnectStatus();
	void 		GetBarcodeScannerFd(BOOL &scannerFd);
	void 		SendToMsgQ(BarcodeMessage& message, MSG_Q_ID msgQID);

private:
	int			barcodeScannerFd;
	int			msgCount;
	EV_DEV_MSG	evdevMsg;
	char		devName[EV_DEV_NAME_LEN + 1];
    UINT32		devCap;
    UINT32		kbdMode;
	bool		m_bBarcodeScannerStatus;	
};



#endif /* BARCODEREADER_H_ */
