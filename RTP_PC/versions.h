/*
  * version.h
 *
 *  Created on: Sep 6, 2017
 *      Author: DShilonie
 */

#ifndef VERSION_H_
#define VERSION_H_

/*
 * System version components
 */
typedef enum {
	FW_VERSION_SC,
	FW_VERSION_AC,
	FW_VERSION_PC,

	FW_VERSION_MAX
} FW_VER_IDX;

typedef enum {
	FW_CRC_SC,
	FW_CRC_AC,
	FW_CRC_PC,

	FW_CRC_MAX
} FW_CRC_IDX;

typedef enum {
	FW_INFO_SC,
	FW_INFO_AC,
	FW_INFO_PC,

	FW_INFO_MAX
} FW_INFO_IDX;

/*
 * Type of version
 */
typedef enum {
	VERSION_MAJOR,
	VERSION_MINOR,
	VERSION_BUILD,

	VERSION_FIELD_MAX
} VER_FIELD_IDX;

/*
 * Provides global access to version info, no need to instantiate.
 */
class FWVersion
{
public:
	
	FWVersion ();
	~FWVersion ();
	
	static UINT32 	Get (FW_VER_IDX idx, VER_FIELD_IDX field);
	static void 	Set (FW_VER_IDX idx, VER_FIELD_IDX field, UINT32 val);

private:

	static UINT32 versions[FW_VERSION_MAX][VERSION_FIELD_MAX];	
};

class FWCrc
{
public:
	
	FWCrc ();
	~FWCrc ();
	
	static UINT16 	Get (FW_CRC_IDX);
	static void 	Set (FW_CRC_IDX idx, UINT16 val);

private:

	static UINT16 crc[FW_CRC_MAX];	
};

class FWInfo
{
public:
	
	FWInfo ();
	~FWInfo ();
	
	static UINT32 	Get (FW_INFO_IDX);
	static void 	Set (FW_INFO_IDX idx, UINT32 val);

private:

	static UINT32 info[FW_CRC_MAX];	
};

#endif /* VERSION_H_ */
