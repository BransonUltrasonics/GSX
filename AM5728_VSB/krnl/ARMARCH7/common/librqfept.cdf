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
Symbol readyQCpuInit {
}
Symbol readyQTaskInit {
}
Symbol taskStackAllot {
}
Symbol readyQPut {
}
Symbol vxCpuSetReserved {
}
Symbol vxKernelVars {
}
Symbol vxCpuIndexGet {
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
Symbol vxAtomic32Get {
}
Symbol vxAtomic32Or {
}
Symbol vxCpuEnabled {
}
Symbol readyQNextCpuGet {
}
Symbol readyQNodeToTcb {
}
Symbol readyQReservationCheck {
}
ObjectFile librqfept.a::readyQFeptLib.o {
	NAME Object file readyQFeptLib.o from archive librqfept.a
	EXPORTS readyQInit readyQCpuInit readyQTaskInit readyQPut readyQRemove readyQResort readyQRotate readyQPreemptCheck readyQTaskGet readyQNextCpuGet readyQNodeToTcb readyQReservationCheck
	IMPORTS vxCpuConfigured readyQSmpBMap readyQSmpHead qPriBMapInit taskStackAllot vxCpuSetReserved vxKernelVars vxCpuIndexGet vxAtomic32And vxAtomic32Get vxAtomic32Or vxCpuEnabled
	DECLARES 
	USES 
}
Module readyQFeptLib.o {
	OBJECT += librqfept.a::readyQFeptLib.o
}
Library librqfept.a {
	MODULES librqfept.a::readyQFeptLib.o
}
