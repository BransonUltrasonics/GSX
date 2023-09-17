Symbol bzero {
}
Symbol select {
}
Symbol memdemo_fibonacci_example {
}
Symbol mybuf_strdup {
}
Symbol printf {
}
Symbol strlen {
}
Symbol mybuf_alloc {
}
Symbol __div32 {
}
Symbol __rem32 {
}
Symbol strncpy {
}
Symbol mybuf_free {
}
Symbol memdemo {
}
Symbol __stdout {
}
Symbol fflush {
}
Symbol malloc {
}
Symbol cafe_user_event_unsigned {
}
Symbol free {
}
Symbol memcpy {
}
ObjectFile libruntime_analysis_usereventdemo.a::memdemo.o {
	NAME Object file memdemo.o from archive libruntime_analysis_usereventdemo.a
	EXPORTS memdemo_fibonacci_example memdemo
	IMPORTS bzero select mybuf_strdup printf strlen mybuf_alloc __div32 __rem32 strncpy mybuf_free __stdout fflush
	DECLARES 
	USES 
}
Module memdemo.o {
	OBJECT += libruntime_analysis_usereventdemo.a::memdemo.o
}
ObjectFile libruntime_analysis_usereventdemo.a::mybuffer.o {
	NAME Object file mybuffer.o from archive libruntime_analysis_usereventdemo.a
	EXPORTS mybuf_alloc mybuf_free mybuf_strdup
	IMPORTS malloc cafe_user_event_unsigned free strlen memcpy
	DECLARES 
	USES 
}
Module mybuffer.o {
	OBJECT += libruntime_analysis_usereventdemo.a::mybuffer.o
}
Library libruntime_analysis_usereventdemo.a {
	MODULES libruntime_analysis_usereventdemo.a::memdemo.o libruntime_analysis_usereventdemo.a::mybuffer.o
}
