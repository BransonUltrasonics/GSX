Symbol umaskLibInit {
}
Symbol umask {
}
Symbol taskIdCurrent {
}
Symbol kernelId {
}
Symbol umaskGet {
}
Symbol umaskSet {
}
Symbol globalUmaskGet {
}
Symbol umaskScLibInit {
}
Symbol _umaskSc {
}
ObjectFile libumask.a::umaskLib.o {
	NAME Object file umaskLib.o from archive libumask.a
	EXPORTS umaskLibInit umask umaskGet umaskSet globalUmaskGet
	IMPORTS taskIdCurrent kernelId
	DECLARES 
	USES 
}
Module umaskLib.o {
	OBJECT += libumask.a::umaskLib.o
}
ObjectFile libumask.a::umaskScLib.o {
	NAME Object file umaskScLib.o from archive libumask.a
	EXPORTS umaskScLibInit _umaskSc
	IMPORTS umask
	DECLARES 
	USES 
}
Module umaskScLib.o {
	OBJECT += libumask.a::umaskScLib.o
}
Library libumask.a {
	MODULES libumask.a::umaskLib.o libumask.a::umaskScLib.o
}
