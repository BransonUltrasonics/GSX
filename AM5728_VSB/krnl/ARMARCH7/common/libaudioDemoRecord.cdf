Symbol audRec {
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
Symbol bzero {
}
Symbol open {
}
Symbol printf {
}
Symbol ioctl {
}
Symbol bcopy {
}
Symbol calloc {
}
Symbol read {
}
Symbol write {
}
Symbol __div32 {
}
Symbol audioWavHeaderWrite {
}
Symbol close {
}
Symbol free {
}
ObjectFile libaudioDemoRecord.a::audioDemoRecord.o {
	NAME Object file audioDemoRecord.o from archive libaudioDemoRecord.a
	EXPORTS audRec
	IMPORTS taskSpawn vxCpuConfiguredGet taskCpuAffinitySet taskDelete taskActivate bzero open printf ioctl bcopy calloc read write __div32 audioWavHeaderWrite close free
	DECLARES 
	USES 
}
Module audioDemoRecord.o {
	OBJECT += libaudioDemoRecord.a::audioDemoRecord.o
}
Library libaudioDemoRecord.a {
	MODULES libaudioDemoRecord.a::audioDemoRecord.o
}
