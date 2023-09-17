Symbol boardLibInit {
}
Symbol datasetIterate {
}
Symbol boardInit {
}
Symbol boardReset {
}
Symbol boardInfoShow {
}
Symbol boardFlagCheck {
}
Symbol sysUsDelay {
}
Symbol sysMsDelay {
}
Symbol sysDelay {
}
Symbol sysToMonitor {
}
Symbol sysProcNumGet {
}
Symbol sysProcNum {
}
Symbol sysProcNumSet {
}
Symbol sysModel {
}
Symbol sysMemSizeGet {
}
Symbol sysMemDescGet {
}
Symbol sysWarmBootLibInit {
}
Symbol sysWarmBootFunc {
}
Symbol warmBootLibInit {
}
Symbol warmBootStart {
}
Symbol warmBootDataEndGet {
}
Symbol warmBootDataAlign {
}
Symbol wrs_kernel_bss_end {
}
Symbol warmBootCRC {
}
Symbol warmBootBackup {
}
Symbol wrs_kernel_data_start {
}
Symbol wrs_kernel_data_end {
}
Symbol bcopy {
}
Symbol wrs_kernel_text_start {
}
Symbol wrs_kernel_text_end {
}
Symbol warmBootCheck {
}
Symbol warmBootRestore {
}
Symbol vmTranslate {
}
Symbol vxFdtDtbAddressGet {
}
ObjectFile libboard.a::boardLib.o {
	NAME Object file boardLib.o from archive libboard.a
	EXPORTS boardLibInit boardInit boardReset boardInfoShow boardFlagCheck sysUsDelay sysMsDelay sysDelay sysToMonitor sysProcNumGet sysProcNumSet sysModel sysMemSizeGet
	IMPORTS datasetIterate sysProcNum sysMemDescGet
	DECLARES sysWarmBootLibInit sysWarmBootFunc
	USES 
}
Module boardLib.o {
	OBJECT += libboard.a::boardLib.o
}
ObjectFile libboard.a::warmBootLib.o {
	NAME Object file warmBootLib.o from archive libboard.a
	EXPORTS warmBootLibInit warmBootStart warmBootDataEndGet warmBootCRC warmBootBackup warmBootCheck warmBootRestore
	IMPORTS sysWarmBootFunc warmBootDataAlign wrs_kernel_bss_end wrs_kernel_data_start wrs_kernel_data_end bcopy wrs_kernel_text_start wrs_kernel_text_end vmTranslate vxFdtDtbAddressGet
	DECLARES 
	USES 
}
Module warmBootLib.o {
	OBJECT += libboard.a::warmBootLib.o
}
Library libboard.a {
	MODULES libboard.a::boardLib.o libboard.a::warmBootLib.o
}
