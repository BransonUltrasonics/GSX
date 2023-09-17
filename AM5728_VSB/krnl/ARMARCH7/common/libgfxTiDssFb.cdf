Symbol vmPageSizeGet {
}
Symbol memalign {
}
Symbol vmStateSet {
}
Symbol vmTranslate {
}
Symbol free {
}
Symbol pmapGlobalMap {
}
Symbol __stderr {
}
Symbol fprintf {
}
Symbol strcmp {
}
Symbol sscanf {
}
Symbol strchr {
}
Symbol strlen {
}
Symbol strncmp {
}
Symbol bcopy {
}
Symbol bzero {
}
Symbol errnoSet {
}
Symbol printf {
}
Symbol vxbFdtDevMatch {
}
Symbol semMInitialize {
}
Symbol __errno {
}
Symbol semTake {
}
Symbol snprintf {
}
Symbol semGive {
}
Symbol calloc {
}
Symbol vxbDevSoftcSet {
}
Symbol iosDrvInstall {
}
Symbol iosDevAdd {
}
Symbol iosDevDelete {
}
Symbol iosDrvRemove {
}
Symbol __ctype_toupper {
}
Symbol gfxTiDssInit {
}
Symbol strstr {
}
Symbol gfxTiDssSetVideoModeEx {
}
Symbol gfxTiDssSetFbAddr {
}
Symbol gfxTiDssHwInit {
}
Symbol vxbDevProbe_desc {
}
Symbol vxbDevAttach_desc {
}
Symbol vxbFdtFbdevDrv {
}
Symbol __dataset__driver_vxbFdtFbdevDrv {
}
Symbol virtClkAddr {
}
Symbol virtDispcAddr {
}
Symbol virtDssAddr {
}
Symbol virtHdmiAddr {
}
Symbol virtHdmiPhyAddr {
}
Symbol virtHdmiPllAddr {
}
Symbol hardCodeHdmi {
}
Symbol gHdmiConfig_480x800 {
}
Symbol taskDelay {
}
Symbol virtHdmiCoreAddr {
}
Symbol gfxTiAm35xxEnable {
}
Symbol gfxTiAm37xxEnable {
}
Symbol gfxTiAm35xxLcdPowerOn {
}
Symbol gfxTiAm35xxLcdBLPowerOn {
}
Symbol gfxTiAm35xxLcdBLPowerOff {
}
Symbol gfxTiAm35xxLcdPowerOff {
}
Symbol gfxTiAm37xxDviDisable {
}
Symbol gfxTiAm37xxLcdVgaModeEnable {
}
Symbol gfxTiAm37xxLcdVgaModeDisable {
}
Symbol gfxTiAm37xxLcdEnable {
}
Symbol gfxTiAm37xxLcdDisable {
}
Symbol gfxTiAm37xxDviEnable {
}
Symbol getGfxPos {
}
Symbol setGfxPos {
}
Symbol virtMuxAddr {
}
Symbol gfxTiDssI2cWrite {
}
Symbol gfxTiDssI2cRead {
}
ObjectFile libgfxTiDssFb.a::gfxTiDssIosDrv.o {
	NAME Object file gfxTiDssIosDrv.o from archive libgfxTiDssFb.a
	EXPORTS gfxTiDssInit vxbFdtFbdevDrv __dataset__driver_vxbFdtFbdevDrv
	IMPORTS vmPageSizeGet memalign vmStateSet vmTranslate free pmapGlobalMap __stderr fprintf strcmp sscanf strchr strlen strncmp bcopy bzero errnoSet printf vxbFdtDevMatch semMInitialize __errno semTake snprintf semGive calloc vxbDevSoftcSet iosDrvInstall iosDevAdd iosDevDelete iosDrvRemove __ctype_toupper strstr gfxTiDssSetVideoModeEx gfxTiDssSetFbAddr gfxTiDssHwInit vxbDevProbe_desc vxbDevAttach_desc
	DECLARES 
	USES 
}
Module gfxTiDssIosDrv.o {
	OBJECT += libgfxTiDssFb.a::gfxTiDssIosDrv.o
}
ObjectFile libgfxTiDssFb.a::gfxTiDss.o {
	NAME Object file gfxTiDss.o from archive libgfxTiDssFb.a
	EXPORTS hardCodeHdmi gHdmiConfig_480x800 gfxTiDssHwInit gfxTiDssSetVideoModeEx gfxTiDssSetFbAddr getGfxPos setGfxPos
	IMPORTS pmapGlobalMap __stderr fprintf taskDelay printf bzero gfxTiAm35xxEnable gfxTiAm37xxEnable bcopy gfxTiAm35xxLcdPowerOn gfxTiAm35xxLcdBLPowerOn gfxTiAm35xxLcdBLPowerOff gfxTiAm35xxLcdPowerOff gfxTiAm37xxDviDisable gfxTiAm37xxLcdVgaModeEnable gfxTiAm37xxLcdVgaModeDisable gfxTiAm37xxLcdEnable gfxTiAm37xxLcdDisable gfxTiAm37xxDviEnable
	DECLARES virtClkAddr virtDispcAddr virtDssAddr virtHdmiAddr virtHdmiPhyAddr virtHdmiPllAddr virtHdmiCoreAddr virtMuxAddr
	USES 
}
Module gfxTiDss.o {
	OBJECT += libgfxTiDssFb.a::gfxTiDss.o
}
ObjectFile libgfxTiDssFb.a::gfxTiAm35xx.o {
	NAME Object file gfxTiAm35xx.o from archive libgfxTiDssFb.a
	EXPORTS gfxTiAm35xxEnable gfxTiAm35xxLcdPowerOff gfxTiAm35xxLcdPowerOn gfxTiAm35xxLcdBLPowerOff gfxTiAm35xxLcdBLPowerOn
	IMPORTS 
	DECLARES 
	USES 
}
Module gfxTiAm35xx.o {
	OBJECT += libgfxTiDssFb.a::gfxTiAm35xx.o
}
ObjectFile libgfxTiDssFb.a::gfxTiAm37xx.o {
	NAME Object file gfxTiAm37xx.o from archive libgfxTiDssFb.a
	EXPORTS gfxTiAm37xxEnable gfxTiAm37xxDviDisable gfxTiAm37xxDviEnable gfxTiAm37xxLcdDisable gfxTiAm37xxLcdEnable gfxTiAm37xxLcdVgaModeDisable gfxTiAm37xxLcdVgaModeEnable
	IMPORTS gfxTiDssI2cWrite gfxTiDssI2cRead taskDelay
	DECLARES 
	USES 
}
Module gfxTiAm37xx.o {
	OBJECT += libgfxTiDssFb.a::gfxTiAm37xx.o
}
Library libgfxTiDssFb.a {
	MODULES libgfxTiDssFb.a::gfxTiDssIosDrv.o libgfxTiDssFb.a::gfxTiDss.o libgfxTiDssFb.a::gfxTiAm35xx.o libgfxTiDssFb.a::gfxTiAm37xx.o
}
