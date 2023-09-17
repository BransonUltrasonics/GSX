Symbol audioCoreInit {
}
Symbol calloc {
}
Symbol dllCreate {
}
Symbol semBCreate {
}
Symbol iosDrvInstall {
}
Symbol semDelete {
}
Symbol dllDelete {
}
Symbol iosDrvRemove {
}
Symbol free {
}
Symbol audioCoreRegCodec {
}
Symbol strlen {
}
Symbol sysClkRateGet {
}
Symbol semTake {
}
Symbol dllAdd {
}
Symbol semGive {
}
Symbol cacheDmaFree {
}
Symbol cacheDmaMalloc {
}
Symbol bzero {
}
Symbol msgQSend {
}
Symbol __udiv32 {
}
Symbol msgQReceive {
}
Symbol bcopy {
}
Symbol audioCoreUnregTransceiver {
}
Symbol snprintf {
}
Symbol iosDevFind {
}
Symbol strcmp {
}
Symbol iosDevDelete {
}
Symbol msgQDelete {
}
Symbol taskDelete {
}
Symbol dllRemove {
}
Symbol audioCoreUnregCodec {
}
Symbol audioCoreRegTransceiver {
}
Symbol iosDevAdd {
}
Symbol msgQCreate {
}
Symbol taskSpawn {
}
Symbol __div32 {
}
Symbol cacheDmaFuncs {
}
Symbol cacheLib {
}
Symbol audioWavHeaderReadBuf {
}
Symbol memcmp {
}
Symbol audioWavHeaderRead {
}
Symbol lseek {
}
Symbol read {
}
Symbol audioWavHeaderWriteBuf {
}
Symbol memcpy {
}
Symbol audioWavHeaderWrite {
}
Symbol write {
}
ObjectFile libaudioLib.a::audioLibCore.o {
	NAME Object file audioLibCore.o from archive libaudioLib.a
	EXPORTS audioCoreInit audioCoreRegCodec audioCoreUnregTransceiver audioCoreUnregCodec audioCoreRegTransceiver
	IMPORTS calloc dllCreate semBCreate iosDrvInstall semDelete dllDelete iosDrvRemove free strlen sysClkRateGet semTake dllAdd semGive cacheDmaFree cacheDmaMalloc bzero msgQSend __udiv32 msgQReceive bcopy snprintf iosDevFind strcmp iosDevDelete msgQDelete taskDelete dllRemove iosDevAdd msgQCreate taskSpawn __div32 cacheDmaFuncs cacheLib
	DECLARES 
	USES 
}
Module audioLibCore.o {
	OBJECT += libaudioLib.a::audioLibCore.o
}
ObjectFile libaudioLib.a::audioLibWav.o {
	NAME Object file audioLibWav.o from archive libaudioLib.a
	EXPORTS audioWavHeaderReadBuf audioWavHeaderRead audioWavHeaderWriteBuf audioWavHeaderWrite
	IMPORTS memcmp __udiv32 lseek read bzero memcpy write
	DECLARES 
	USES 
}
Module audioLibWav.o {
	OBJECT += libaudioLib.a::audioLibWav.o
}
Library libaudioLib.a {
	MODULES libaudioLib.a::audioLibCore.o libaudioLib.a::audioLibWav.o
}
