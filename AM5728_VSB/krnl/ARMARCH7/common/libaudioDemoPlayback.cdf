Symbol audPlay {
}
Symbol taskSpawn {
}
Symbol vxCpuConfiguredGet {
}
Symbol taskCpuAffinitySet {
}
Symbol taskDelete {
}
Symbol taskActivate {
}
Symbol open {
}
Symbol printf {
}
Symbol audioWavHeaderRead {
}
Symbol bzero {
}
Symbol ioctl {
}
Symbol bcopy {
}
Symbol __udiv32 {
}
Symbol calloc {
}
Symbol lseek {
}
Symbol read {
}
Symbol write {
}
Symbol close {
}
Symbol free {
}
ObjectFile libaudioDemoPlayback.a::audioDemoPlayback.o {
	NAME Object file audioDemoPlayback.o from archive libaudioDemoPlayback.a
	EXPORTS audPlay
	IMPORTS taskSpawn vxCpuConfiguredGet taskCpuAffinitySet taskDelete taskActivate open printf audioWavHeaderRead bzero ioctl bcopy __udiv32 calloc lseek read write close free
	DECLARES 
	USES 
}
Module audioDemoPlayback.o {
	OBJECT += libaudioDemoPlayback.a::audioDemoPlayback.o
}
Library libaudioDemoPlayback.a {
	MODULES libaudioDemoPlayback.a::audioDemoPlayback.o
}
