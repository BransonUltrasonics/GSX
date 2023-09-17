Symbol rfsread {
}
Symbol semTake {
}
Symbol vxBlkIO {
}
Symbol semGive {
}
Symbol bcopy {
}
Symbol romfs_ntoh {
}
Symbol rfs_open {
}
Symbol malloc {
}
Symbol memcpy {
}
Symbol printf {
}
Symbol strncmp {
}
Symbol free {
}
Symbol rfs_read {
}
Symbol rfs_seek {
}
Symbol rfs_close {
}
Symbol rfs_write {
}
Symbol rfs_stat {
}
Symbol bzero {
}
Symbol rfs_ls {
}
Symbol romfsBlkDevCreate {
}
Symbol __div32 {
}
Symbol __udiv32 {
}
Symbol poolItemGet {
}
Symbol errnoSet {
}
Symbol poolItemReturn {
}
Symbol romfsFsInit {
}
Symbol romfsFsDrvNum {
}
Symbol iosDrvInstall {
}
Symbol printErr {
}
Symbol iosDevFind {
}
Symbol iosDevDelete {
}
Symbol poolDelete {
}
Symbol romfsFsDevCreate {
}
Symbol poolCreate {
}
Symbol semMCreate {
}
Symbol iosDevAdd {
}
Symbol fsPathAddedEventRaise {
}
Symbol romfsVolumeCreate {
}
ObjectFile libfsRomfs.a::romfs.o {
	NAME Object file romfs.o from archive libfsRomfs.a
	EXPORTS rfsread romfs_ntoh rfs_open rfs_read rfs_seek rfs_close rfs_write rfs_stat rfs_ls
	IMPORTS semTake vxBlkIO semGive bcopy malloc memcpy printf strncmp free bzero
	DECLARES 
	USES 
}
Module romfs.o {
	OBJECT += libfsRomfs.a::romfs.o
}
ObjectFile libfsRomfs.a::romfsDrv.o {
	NAME Object file romfsDrv.o from archive libfsRomfs.a
	EXPORTS romfsBlkDevCreate
	IMPORTS __div32 malloc bzero bcopy __udiv32
	DECLARES 
	USES 
}
Module romfsDrv.o {
	OBJECT += libfsRomfs.a::romfsDrv.o
}
ObjectFile libfsRomfs.a::romfsLib.o {
	NAME Object file romfsLib.o from archive libfsRomfs.a
	EXPORTS romfsFsInit romfsFsDrvNum romfsFsDevCreate vxBlkIO romfsVolumeCreate
	IMPORTS poolItemGet errnoSet rfs_open poolItemReturn rfs_close rfs_read rfsread memcpy rfs_seek rfs_stat iosDrvInstall printErr iosDevFind iosDevDelete poolDelete free malloc bzero poolCreate semMCreate iosDevAdd fsPathAddedEventRaise __udiv32 romfsBlkDevCreate
	DECLARES 
	USES 
}
Module romfsLib.o {
	OBJECT += libfsRomfs.a::romfsLib.o
}
Library libfsRomfs.a {
	MODULES libfsRomfs.a::romfs.o libfsRomfs.a::romfsDrv.o libfsRomfs.a::romfsLib.o
}
