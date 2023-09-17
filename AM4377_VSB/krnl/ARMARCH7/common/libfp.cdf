Symbol scalb {
}
Symbol drem {
}
Symbol _Raise_Inexact {
}
Symbol fpTypeGet {
}
Symbol getInf {
}
Symbol getNan {
}
Symbol hypot {
}
Symbol finite {
}
Symbol copysign {
}
Symbol logb {
}
Symbol sqrt {
}
Symbol cabs {
}
Symbol z_abs {
}
Symbol fioBaseLibInit {
}
Symbol printErr {
}
Symbol _func_printErr {
}
Symbol fioFormatV {
}
Symbol memchr {
}
Symbol strlen {
}
Symbol __urem64 {
}
Symbol __udiv64 {
}
Symbol _func_fioFltFormatRtn {
}
Symbol fieldSzIncludeSign {
}
Symbol printf {
}
Symbol fioBufPrint {
}
Symbol oprintf {
}
Symbol sprintf {
}
Symbol fioBufPut {
}
Symbol printExc {
}
Symbol intContext {
}
Symbol taskIdCurrent {
}
Symbol kernelState {
}
Symbol _func_printExcPrintHook {
}
Symbol _func_excJobAdd {
}
Symbol vmLibInfo {
}
Symbol sysExcMsg {
}
Symbol __udiv32 {
}
Symbol evtAction {
}
Symbol taskUnsafe {
}
Symbol _func_shellExcPrint {
}
Symbol taskIdSelf {
}
Symbol _func_kprintf {
}
Symbol taskClassId {
}
Symbol eventPointSwHandle {
}
Symbol snprintf {
}
Symbol fioSnBufPut {
}
Symbol bcopy {
}
Symbol _func_consoleOut {
}
Symbol _func_kwrite {
}
Symbol fioFltInstall {
}
Symbol _func_fioFltScanRtn {
}
Symbol floatInit {
}
Symbol modf {
}
Symbol __rem32 {
}
Symbol __div32 {
}
Symbol strcpy {
}
Symbol __ieee754_fmod {
}
Symbol __ieee754_pow {
}
Symbol __ieee754_sqrt {
}
Symbol fabs {
}
Symbol scalbn {
}
Symbol __ieee754_remainder {
}
Symbol rint {
}
Symbol fioLibInit {
}
Symbol voprintf {
}
Symbol fdprintf {
}
Symbol vprintf {
}
Symbol vfdprintf {
}
Symbol vsprintf {
}
Symbol vsnprintf {
}
Symbol fioRead {
}
Symbol read {
}
Symbol fioRdString {
}
Symbol __ctype {
}
Symbol scanCharSet {
}
Symbol __mul64 {
}
Symbol scanField {
}
Symbol fioScanV {
}
Symbol sscanf {
}
ObjectFile libfp.a::support.o {
	NAME Object file support.o from archive libfp.a
	EXPORTS scalb drem _Raise_Inexact
	IMPORTS 
	DECLARES 
	USES 
}
Module support.o {
	OBJECT += libfp.a::support.o
}
ObjectFile libfp.a::fpType.o {
	NAME Object file fpType.o from archive libfp.a
	EXPORTS fpTypeGet getInf getNan
	IMPORTS 
	DECLARES 
	USES 
}
Module fpType.o {
	OBJECT += libfp.a::fpType.o
}
ObjectFile libfp.a::cabs.o {
	NAME Object file cabs.o from archive libfp.a
	EXPORTS hypot cabs z_abs
	IMPORTS finite copysign logb _Raise_Inexact sqrt
	DECLARES 
	USES 
}
Module cabs.o {
	OBJECT += libfp.a::cabs.o
}
ObjectFile libfp.a::fioBaseLib.o {
	NAME Object file fioBaseLib.o from archive libfp.a
	EXPORTS fioBaseLibInit printErr fioFormatV fieldSzIncludeSign printf fioBufPrint oprintf sprintf fioBufPut printExc snprintf fioSnBufPut
	IMPORTS _func_printErr memchr strlen __urem64 __udiv64 _func_fioFltFormatRtn intContext taskIdCurrent kernelState _func_excJobAdd vmLibInfo sysExcMsg __udiv32 evtAction taskUnsafe _func_shellExcPrint taskIdSelf _func_kprintf taskClassId eventPointSwHandle bcopy _func_consoleOut _func_kwrite
	DECLARES _func_printExcPrintHook
	USES 
}
Module fioBaseLib.o {
	OBJECT += libfp.a::fioBaseLib.o
}
ObjectFile libfp.a::floatLib.o {
	NAME Object file floatLib.o from archive libfp.a
	EXPORTS fioFltInstall floatInit
	IMPORTS _func_fioFltFormatRtn _func_fioFltScanRtn modf __rem32 __div32 fpTypeGet strcpy
	DECLARES 
	USES 
}
Module floatLib.o {
	OBJECT += libfp.a::floatLib.o
}
ObjectFile libfp.a::floatFuncBind.o {
	NAME Object file floatFuncBind.o from archive libfp.a
	EXPORTS _func_fioFltFormatRtn _func_fioFltScanRtn
	IMPORTS 
	DECLARES 
	USES 
}
Module floatFuncBind.o {
	OBJECT += libfp.a::floatFuncBind.o
}
ObjectFile libfp.a::fioFuncBind.o {
	NAME Object file fioFuncBind.o from archive libfp.a
	EXPORTS 
	IMPORTS 
	DECLARES _func_printErr
	USES 
}
Module fioFuncBind.o {
	OBJECT += libfp.a::fioFuncBind.o
}
ObjectFile libfp.a::e_fmod.o {
	NAME Object file e_fmod.o from archive libfp.a
	EXPORTS __ieee754_fmod
	IMPORTS 
	DECLARES 
	USES 
}
Module e_fmod.o {
	OBJECT += libfp.a::e_fmod.o
}
ObjectFile libfp.a::e_pow.o {
	NAME Object file e_pow.o from archive libfp.a
	EXPORTS __ieee754_pow
	IMPORTS __ieee754_sqrt fabs getNan scalbn
	DECLARES 
	USES 
}
Module e_pow.o {
	OBJECT += libfp.a::e_pow.o
}
ObjectFile libfp.a::e_remainder.o {
	NAME Object file e_remainder.o from archive libfp.a
	EXPORTS __ieee754_remainder
	IMPORTS __ieee754_fmod fabs
	DECLARES 
	USES 
}
Module e_remainder.o {
	OBJECT += libfp.a::e_remainder.o
}
ObjectFile libfp.a::s_rint.o {
	NAME Object file s_rint.o from archive libfp.a
	EXPORTS rint
	IMPORTS 
	DECLARES 
	USES 
}
Module s_rint.o {
	OBJECT += libfp.a::s_rint.o
}
ObjectFile libfp.a::s_scalbn.o {
	NAME Object file s_scalbn.o from archive libfp.a
	EXPORTS scalbn
	IMPORTS copysign
	DECLARES 
	USES 
}
Module s_scalbn.o {
	OBJECT += libfp.a::s_scalbn.o
}
ObjectFile libfp.a::e_sqrt.o {
	NAME Object file e_sqrt.o from archive libfp.a
	EXPORTS __ieee754_sqrt
	IMPORTS getNan
	DECLARES 
	USES 
}
Module e_sqrt.o {
	OBJECT += libfp.a::e_sqrt.o
}
ObjectFile libfp.a::fioLib.o {
	NAME Object file fioLib.o from archive libfp.a
	EXPORTS fioLibInit voprintf fdprintf vprintf vfdprintf vsprintf vsnprintf fioRead fioRdString scanCharSet scanField fioScanV sscanf
	IMPORTS printErr _func_printErr fioBufPrint fioFormatV fioBufPut fioSnBufPut read __ctype __mul64 _func_fioFltScanRtn
	DECLARES 
	USES 
}
Module fioLib.o {
	OBJECT += libfp.a::fioLib.o
}
Library libfp.a {
	MODULES libfp.a::support.o libfp.a::fpType.o libfp.a::cabs.o libfp.a::fioBaseLib.o libfp.a::floatLib.o libfp.a::floatFuncBind.o libfp.a::fioFuncBind.o libfp.a::e_fmod.o libfp.a::e_pow.o libfp.a::e_remainder.o libfp.a::s_rint.o libfp.a::s_scalbn.o libfp.a::e_sqrt.o libfp.a::fioLib.o
}
