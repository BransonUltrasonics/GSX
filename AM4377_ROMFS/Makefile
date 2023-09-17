# Wind River Workbench generated Makefile.
# Do not edit!!!

TRACE=0
TRACEON=$(TRACE:0=@)
TRACE_FLAG=$(TRACEON:1=)

all : subdirs_all main_all
	@echo "make: built file system contents"

main_all : _clean
	$(TRACE_FLAG)echo "building file system contents";\
	mkdir -p Contents
	$(TRACE_FLAG)cp -f "C:/ScriptGSX2-3/GSX2-3Stream/BuildOutput/Custom/Debug/PC/RTP_PC.vxe" "Contents/RTP_PC.vxe"
	$(TRACE_FLAG)cp -f "C:/ScriptGSX2-3/GSX2-3Stream/_Support_Tools/BootStrap/AC/ForceConstants.txt" "Contents/ForceConstants.txt"
	$(TRACE_FLAG)cp -f "C:/ScriptGSX2-3/GSX2-3Stream/BuildOutput/Custom/Debug/PC/AM4377_DKM_ESS.out" "Contents/AM4377_DKM_ESS.out"
	$(TRACE_FLAG)cp -f "C:/ScriptGSX2-3/GSX2-3Stream/_Support_Tools/BootStrap/PC/TimerIntervalRecipefromsc.txt" "Contents/TimerIntervalRecipefromsc.txt"
	$(TRACE_FLAG)mkromfs -o content.romfs Contents

clean : subdirs_clean _clean

_clean :
	$(TRACE_FLAG)echo "make: removing file system contents" ;\
	rm -rf content.romfs Contents

subdirs_all :

subdirs_clean :
