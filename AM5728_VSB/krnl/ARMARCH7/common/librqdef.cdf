Symbol readyQInit {
}
Symbol vxCpuConfigured {
}
Symbol readyQSmpBMap {
}
Symbol readyQSmpHead {
}
Symbol qPriBMapInit {
}
Symbol readyQReservationCheck {
}
Symbol _func_readyQReservationCheck {
}
Symbol _func_readyQRotateOrderListGet {
}
Symbol readyQCpuInit {
}
Symbol readyQTaskInit {
}
Symbol vxCpuEnabled {
}
Symbol vxCpuSetReserved {
}
Symbol vxAtomic32Get {
}
Symbol vxAtomic32Or {
}
Symbol readyQPut {
}
Symbol vxCpuIndexGet {
}
Symbol ffs32Lsb {
}
Symbol vxKernelVars {
}
Symbol readyQRemove {
}
Symbol readyQResort {
}
Symbol readyQRotate {
}
Symbol readyQPreemptCheck {
}
Symbol vxAtomic32And {
}
Symbol readyQTaskGet {
}
Symbol readyQNextCpuGet {
}
Symbol readyQNodeToTcb {
}
ObjectFile librqdef.a::readyQDeltaLib.o {
	NAME Object file readyQDeltaLib.o from archive librqdef.a
	EXPORTS readyQInit readyQReservationCheck readyQCpuInit readyQTaskInit readyQPut readyQRemove readyQResort readyQRotate readyQPreemptCheck readyQTaskGet readyQNextCpuGet readyQNodeToTcb
	IMPORTS vxCpuConfigured readyQSmpBMap readyQSmpHead qPriBMapInit _func_readyQReservationCheck _func_readyQRotateOrderListGet vxCpuEnabled vxCpuSetReserved vxAtomic32Get vxAtomic32Or vxCpuIndexGet ffs32Lsb vxKernelVars vxAtomic32And
	DECLARES 
	USES 
}
Module readyQDeltaLib.o {
	OBJECT += librqdef.a::readyQDeltaLib.o
}
Library librqdef.a {
	MODULES librqdef.a::readyQDeltaLib.o
}
