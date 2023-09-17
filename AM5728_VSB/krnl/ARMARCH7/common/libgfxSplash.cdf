Symbol __urem32 {
}
Symbol memcpy {
}
Symbol wrSplashInflatedSize {
}
Symbol memset {
}
Symbol bzero {
}
Symbol gfxFbSplashBlit1 {
}
Symbol malloc {
}
Symbol wrSplashDeflatedSize {
}
Symbol wrSplashDeflatedData {
}
Symbol __div32 {
}
Symbol free {
}
ObjectFile libgfxSplash.a::gfxSplash.o {
	NAME Object file gfxSplash.o from archive libgfxSplash.a
	EXPORTS gfxFbSplashBlit1
	IMPORTS __urem32 memcpy wrSplashInflatedSize memset bzero malloc wrSplashDeflatedSize wrSplashDeflatedData __div32 free
	DECLARES 
	USES 
}
Module gfxSplash.o {
	OBJECT += libgfxSplash.a::gfxSplash.o
}
Library libgfxSplash.a {
	MODULES libgfxSplash.a::gfxSplash.o
}
