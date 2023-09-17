<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="17008000">
	<Item Name="My Computer" Type="My Computer">
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="Default_TestMode_40KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_40KHZ_AUTO/Default_TestMode_40KHZ/Default_TestMode_40KHZ.vi"/>
		<Item Name="TestMode_Above50_40KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_40KHZ_AUTO/TestMode_Above50_40KHZ/TestMode_Above50_40KHZ.vi"/>
		<Item Name="TestMode_Above100_40KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_40KHZ_AUTO/TestMode_Above100_40KHZ/TestMode_Above100_40KHZ.vi"/>
		<Item Name="TestMode_Above500_40KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_40KHZ_AUTO/TestMode_Above500_40KHZ/TestMode_Above500_40KHZ.vi"/>
		<Item Name="TestMode_Boundary_Maxoneless_40KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_40KHZ_AUTO/TestMode_Boundary_Maxoneless_40KHZ/TestMode_Boundary_Maxoneless_40KHZ.vi"/>
		<Item Name="TestMode_Boundary_Maxonemore_40KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_40KHZ_AUTO/TestMode_Boundary_Maxonemore_40KHZ/TestMode_Boundary_Maxonemore_40KHZ.vi"/>
		<Item Name="TestMode_Boundary_Minoneless_40KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_40KHZ_AUTO/TestMode_Boundary_Minoneless_40KHZ/TestMode_Boundary_Minoneless_40KHZ.vi"/>
		<Item Name="TestMode_Boundary_Minonemore_40KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_40KHZ_AUTO/TestMode_Boundary_Minonemore_40KHZ/TestMode_Boundary_Minonemore_40KHZ.vi"/>
		<Item Name="TestMode_MaxAmpLpC1_40KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_40KHZ_AUTO/TestMode_MaxAmpLpC1_40KHZ/TestMode_MaxAmpLpC1_40KHZ.vi"/>
		<Item Name="TestMode_MaxAmpLpC2_40KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_40KHZ_AUTO/TestMode_MaxAmpLpC2_40KHZ/TestMode_MaxAmpLpC2_40KHZ.vi"/>
		<Item Name="TestMode_MaxPhLp_40KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_40KHZ_AUTO/TestMode_MaxPhLp_40KHZ/TestMode_MaxPhLp_40KHZ.vi"/>
		<Item Name="TestMode_MaxPhLpCF_40KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_40KHZ_AUTO/TestMode_MaxPhLpCF_40KHZ/TestMode_MaxPhLpCF_40KHZ.vi"/>
		<Item Name="TestMode_MaxRecipe_40KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_40KHZ_AUTO/TestMode_MaxRecipe_40KHZ/TestMode_MaxRecipe_40KHZ.vi"/>
		<Item Name="TestMode_MinAmpLpC1_40KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_40KHZ_AUTO/TestMode_MinAmpLpC1_40KHZ/TestMode_MinAmpLpC1_40KHZ.vi"/>
		<Item Name="TestMode_MinAmpLpC2_40KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_40KHZ_AUTO/TestMode_MinAmpLpC2_40KHZ/TestMode_MinAmpLpC2_40KHZ.vi"/>
		<Item Name="TestMode_MinPhLp_40KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_40KHZ_AUTO/TestMode_MinPhLp_40KHZ/TestMode_MinPhLp_40KHZ.vi"/>
		<Item Name="TestMode_MinPhLpCF_40KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_40KHZ_AUTO/TestMode_MinPhLpCF_40KHZ/TestMode_MinPhLpCF_40KHZ.vi"/>
		<Item Name="TestMode_MinRecipe_40KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_40KHZ_AUTO/TestMode_MinRecipe_40KHZ/TestMode_MinRecipe_40KHZ.vi"/>
		<Item Name="TestMode_Rcp50_40KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_40KHZ_AUTO/TestMode_Rcp50_40KHZ/TestMode_Rcp50_40KHZ.vi"/>
		<Item Name="TestMode_RcpBelow50_40KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_40KHZ_AUTO/TestMode_RcpBelow50_40KHZ/TestMode_RcpBelow50_40KHZ.vi"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="8.6CompatibleGlobalVar.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/8.6CompatibleGlobalVar.vi"/>
				<Item Name="Application Directory.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Application Directory.vi"/>
				<Item Name="Bit-array To Byte-array.vi" Type="VI" URL="/&lt;vilib&gt;/picture/pictutil.llb/Bit-array To Byte-array.vi"/>
				<Item Name="BuildHelpPath.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/BuildHelpPath.vi"/>
				<Item Name="Built App File Layout.vi" Type="VI" URL="/&lt;vilib&gt;/AppBuilder/Built App File Layout.vi"/>
				<Item Name="Calc Long Word Padded Width.vi" Type="VI" URL="/&lt;vilib&gt;/picture/bmp.llb/Calc Long Word Padded Width.vi"/>
				<Item Name="Check Color Table Size.vi" Type="VI" URL="/&lt;vilib&gt;/picture/jpeg.llb/Check Color Table Size.vi"/>
				<Item Name="Check Data Size.vi" Type="VI" URL="/&lt;vilib&gt;/picture/jpeg.llb/Check Data Size.vi"/>
				<Item Name="Check File Permissions.vi" Type="VI" URL="/&lt;vilib&gt;/picture/jpeg.llb/Check File Permissions.vi"/>
				<Item Name="Check if File or Folder Exists.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/libraryn.llb/Check if File or Folder Exists.vi"/>
				<Item Name="Check Path.vi" Type="VI" URL="/&lt;vilib&gt;/picture/jpeg.llb/Check Path.vi"/>
				<Item Name="Check Special Tags.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Check Special Tags.vi"/>
				<Item Name="Clear Errors.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Clear Errors.vi"/>
				<Item Name="Close Registry Key.vi" Type="VI" URL="/&lt;vilib&gt;/registry/registry.llb/Close Registry Key.vi"/>
				<Item Name="Color to RGB.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/colorconv.llb/Color to RGB.vi"/>
				<Item Name="compatOverwrite.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/compatOverwrite.vi"/>
				<Item Name="Convert property node font to graphics font.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Convert property node font to graphics font.vi"/>
				<Item Name="Create ActiveX Event Queue.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/ax-events.llb/Create ActiveX Event Queue.vi"/>
				<Item Name="Create Error Clust.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/ax-events.llb/Create Error Clust.vi"/>
				<Item Name="Create Mask By Alpha.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Create Mask By Alpha.vi"/>
				<Item Name="Destroy ActiveX Event Queue.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/ax-events.llb/Destroy ActiveX Event Queue.vi"/>
				<Item Name="Details Display Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Details Display Dialog.vi"/>
				<Item Name="DialogType.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/DialogType.ctl"/>
				<Item Name="DialogTypeEnum.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/DialogTypeEnum.ctl"/>
				<Item Name="Directory of Top Level VI.vi" Type="VI" URL="/&lt;vilib&gt;/picture/jpeg.llb/Directory of Top Level VI.vi"/>
				<Item Name="Draw Flattened Pixmap.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Draw Flattened Pixmap.vi"/>
				<Item Name="Error Cluster From Error Code.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Cluster From Error Code.vi"/>
				<Item Name="Error Code Database.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Code Database.vi"/>
				<Item Name="ErrWarn.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/ErrWarn.ctl"/>
				<Item Name="Escape Characters for HTTP.vi" Type="VI" URL="/&lt;vilib&gt;/printing/PathToURL.llb/Escape Characters for HTTP.vi"/>
				<Item Name="EventData.ctl" Type="VI" URL="/&lt;vilib&gt;/Platform/ax-events.llb/EventData.ctl"/>
				<Item Name="eventvkey.ctl" Type="VI" URL="/&lt;vilib&gt;/event_ctls.llb/eventvkey.ctl"/>
				<Item Name="Find Tag.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Find Tag.vi"/>
				<Item Name="FixBadRect.vi" Type="VI" URL="/&lt;vilib&gt;/picture/pictutil.llb/FixBadRect.vi"/>
				<Item Name="Flip and Pad for Picture Control.vi" Type="VI" URL="/&lt;vilib&gt;/picture/bmp.llb/Flip and Pad for Picture Control.vi"/>
				<Item Name="Format Message String.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Format Message String.vi"/>
				<Item Name="General Error Handler Core CORE.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/General Error Handler Core CORE.vi"/>
				<Item Name="General Error Handler.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/General Error Handler.vi"/>
				<Item Name="Generate Temporary File Path.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/libraryn.llb/Generate Temporary File Path.vi"/>
				<Item Name="Get LV Class Default Value.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/LVClass/Get LV Class Default Value.vi"/>
				<Item Name="Get String Text Bounds.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Get String Text Bounds.vi"/>
				<Item Name="Get Text Rect.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Get Text Rect.vi"/>
				<Item Name="GetHelpDir.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/GetHelpDir.vi"/>
				<Item Name="GetRTHostConnectedProp.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/GetRTHostConnectedProp.vi"/>
				<Item Name="imagedata.ctl" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/imagedata.ctl"/>
				<Item Name="Longest Line Length in Pixels.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Longest Line Length in Pixels.vi"/>
				<Item Name="LVBoundsTypeDef.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/miscctls.llb/LVBoundsTypeDef.ctl"/>
				<Item Name="LVRectTypeDef.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/miscctls.llb/LVRectTypeDef.ctl"/>
				<Item Name="NI_FileType.lvlib" Type="Library" URL="/&lt;vilib&gt;/Utility/lvfile.llb/NI_FileType.lvlib"/>
				<Item Name="NI_HTML.lvclass" Type="LVClass" URL="/&lt;vilib&gt;/Utility/NIReport.llb/HTML/NI_HTML.lvclass"/>
				<Item Name="NI_LVConfig.lvlib" Type="Library" URL="/&lt;vilib&gt;/Utility/config.llb/NI_LVConfig.lvlib"/>
				<Item Name="NI_PackedLibraryUtility.lvlib" Type="Library" URL="/&lt;vilib&gt;/Utility/LVLibp/NI_PackedLibraryUtility.lvlib"/>
				<Item Name="NI_report.lvclass" Type="LVClass" URL="/&lt;vilib&gt;/Utility/NIReport.llb/NI_report.lvclass"/>
				<Item Name="NI_ReportGenerationCore.lvlib" Type="Library" URL="/&lt;vilib&gt;/Utility/NIReport.llb/NI_ReportGenerationCore.lvlib"/>
				<Item Name="NI_Standard Report.lvclass" Type="LVClass" URL="/&lt;vilib&gt;/Utility/NIReport.llb/Standard Report/NI_Standard Report.lvclass"/>
				<Item Name="Not Found Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Not Found Dialog.vi"/>
				<Item Name="OccFireType.ctl" Type="VI" URL="/&lt;vilib&gt;/Platform/ax-events.llb/OccFireType.ctl"/>
				<Item Name="Open Registry Key.vi" Type="VI" URL="/&lt;vilib&gt;/registry/registry.llb/Open Registry Key.vi"/>
				<Item Name="Path to URL.vi" Type="VI" URL="/&lt;vilib&gt;/printing/PathToURL.llb/Path to URL.vi"/>
				<Item Name="Read PNG File.vi" Type="VI" URL="/&lt;vilib&gt;/picture/png.llb/Read PNG File.vi"/>
				<Item Name="Read Registry Value DWORD.vi" Type="VI" URL="/&lt;vilib&gt;/registry/registry.llb/Read Registry Value DWORD.vi"/>
				<Item Name="Read Registry Value Simple STR.vi" Type="VI" URL="/&lt;vilib&gt;/registry/registry.llb/Read Registry Value Simple STR.vi"/>
				<Item Name="Read Registry Value Simple U32.vi" Type="VI" URL="/&lt;vilib&gt;/registry/registry.llb/Read Registry Value Simple U32.vi"/>
				<Item Name="Read Registry Value Simple.vi" Type="VI" URL="/&lt;vilib&gt;/registry/registry.llb/Read Registry Value Simple.vi"/>
				<Item Name="Read Registry Value STR.vi" Type="VI" URL="/&lt;vilib&gt;/registry/registry.llb/Read Registry Value STR.vi"/>
				<Item Name="Read Registry Value.vi" Type="VI" URL="/&lt;vilib&gt;/registry/registry.llb/Read Registry Value.vi"/>
				<Item Name="Registry Handle Master.vi" Type="VI" URL="/&lt;vilib&gt;/registry/registry.llb/Registry Handle Master.vi"/>
				<Item Name="Registry refnum.ctl" Type="VI" URL="/&lt;vilib&gt;/registry/registry.llb/Registry refnum.ctl"/>
				<Item Name="Registry RtKey.ctl" Type="VI" URL="/&lt;vilib&gt;/registry/registry.llb/Registry RtKey.ctl"/>
				<Item Name="Registry SAM.ctl" Type="VI" URL="/&lt;vilib&gt;/registry/registry.llb/Registry SAM.ctl"/>
				<Item Name="Registry Simplify Data Type.vi" Type="VI" URL="/&lt;vilib&gt;/registry/registry.llb/Registry Simplify Data Type.vi"/>
				<Item Name="Registry View.ctl" Type="VI" URL="/&lt;vilib&gt;/registry/registry.llb/Registry View.ctl"/>
				<Item Name="Registry WinErr-LVErr.vi" Type="VI" URL="/&lt;vilib&gt;/registry/registry.llb/Registry WinErr-LVErr.vi"/>
				<Item Name="Search and Replace Pattern.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Search and Replace Pattern.vi"/>
				<Item Name="Set Bold Text.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Set Bold Text.vi"/>
				<Item Name="Set String Value.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Set String Value.vi"/>
				<Item Name="Simple Error Handler.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Simple Error Handler.vi"/>
				<Item Name="STR_ASCII-Unicode.vi" Type="VI" URL="/&lt;vilib&gt;/registry/registry.llb/STR_ASCII-Unicode.vi"/>
				<Item Name="TagReturnType.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/TagReturnType.ctl"/>
				<Item Name="Three Button Dialog CORE.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Three Button Dialog CORE.vi"/>
				<Item Name="Three Button Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Three Button Dialog.vi"/>
				<Item Name="Trim Whitespace.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Trim Whitespace.vi"/>
				<Item Name="Wait On ActiveX Event.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/ax-events.llb/Wait On ActiveX Event.vi"/>
				<Item Name="Wait types.ctl" Type="VI" URL="/&lt;vilib&gt;/Platform/ax-events.llb/Wait types.ctl"/>
				<Item Name="whitespace.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/whitespace.ctl"/>
				<Item Name="Write BMP Data To Buffer.vi" Type="VI" URL="/&lt;vilib&gt;/picture/bmp.llb/Write BMP Data To Buffer.vi"/>
				<Item Name="Write BMP Data.vi" Type="VI" URL="/&lt;vilib&gt;/picture/bmp.llb/Write BMP Data.vi"/>
				<Item Name="Write BMP File.vi" Type="VI" URL="/&lt;vilib&gt;/picture/bmp.llb/Write BMP File.vi"/>
				<Item Name="Write JPEG File.vi" Type="VI" URL="/&lt;vilib&gt;/picture/jpeg.llb/Write JPEG File.vi"/>
				<Item Name="Write PNG File.vi" Type="VI" URL="/&lt;vilib&gt;/picture/png.llb/Write PNG File.vi"/>
			</Item>
			<Item Name="Advapi32.dll" Type="Document" URL="Advapi32.dll">
				<Property Name="NI.PreserveRelativePath" Type="Bool">true</Property>
			</Item>
			<Item Name="Common_Socket_Cmd_Sub_VI.vi" Type="VI" URL="../../Generic_Common/Common_Socket_Cmd_Sub_VI.vi"/>
			<Item Name="Common_Socket_Cmd_Sub_VI_TestMode.vi" Type="VI" URL="../../Generic_Common/Test_Mode/Common_Socket_Cmd_Sub_VI_TestMode.vi"/>
			<Item Name="Create_Error_String_Sub_VI.vi" Type="VI" URL="../../Generic_Common/Create_Error_String_Sub_VI.vi"/>
			<Item Name="Global_Read_INI_Sub_VI.vi" Type="VI" URL="../../Generic_Common/Global_Read_INI_Sub_VI.vi"/>
			<Item Name="kernel32.dll" Type="Document" URL="kernel32.dll">
				<Property Name="NI.PreserveRelativePath" Type="Bool">true</Property>
			</Item>
			<Item Name="Local_Read_INI_Sub_VI.vi" Type="VI" URL="../../Generic_Common/Local_Read_INI_Sub_VI.vi"/>
			<Item Name="Log_Msg_Sub_VI.vi" Type="VI" URL="../../Generic_Common/Log_Msg_Sub_VI.vi"/>
			<Item Name="Sending_TestMode_Recipe_Sub_VI.vi" Type="VI" URL="../../Generic_Common/Test_Mode/Sending_TestMode_Recipe_Sub_VI.vi"/>
			<Item Name="Sending_TestMode_Recipe_With_TestMode_Cmd_SUB_VI.vi" Type="VI" URL="../../Generic_Common/Test_Mode/Sending_TestMode_Recipe_With_TestMode_Cmd_SUB_VI.vi"/>
			<Item Name="TestMode_Local_Read_INI_Sub_VI.vi" Type="VI" URL="../../Generic_Common/Test_Mode/TestMode_Local_Read_INI_Sub_VI.vi"/>
			<Item Name="TestMode_Operation_SUB_VI.vi" Type="VI" URL="../../Generic_Common/Test_Mode/TestMode_Operation_SUB_VI.vi"/>
			<Item Name="TestMode_RangeValidation.vi" Type="VI" URL="../../Generic_Common/SEEK/TestMode_RangeValidation.vi"/>
			<Item Name="Write_Html_Report_Sub_VI.vi" Type="VI" URL="../../Generic_Common/Write_Html_Report_Sub_VI.vi"/>
		</Item>
		<Item Name="Build Specifications" Type="Build">
			<Item Name="Default_TestMode_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{226C34E1-F4C2-474B-823B-FB42244DF85E}</Property>
				<Property Name="App_INI_GUID" Type="Str">{ADEC0E8C-18D1-4B03-A5E0-8B3B254940C2}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{C9707E02-16DB-43FD-AB36-C95054C65631}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Default_TestMode_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/Default_TestMode_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{AE39BC50-C691-4756-A47B-7C51C2654AA2}</Property>
				<Property Name="Bld_version.build" Type="Int">4</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Default_TestMode_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/Default_TestMode_40KHZ/Default_TestMode_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/Default_TestMode_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{72D7F7CD-6E66-4C50-B303-686C73FEDE53}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/Default_TestMode_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Default_TestMode_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Default_TestMode_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">Default_TestMode_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{B01B13B1-37BD-4D46-98D8-1BCE53F318D6}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Default_TestMode_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_Above50_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{73D13D34-56A6-4FA2-B94C-1FAFEB631836}</Property>
				<Property Name="App_INI_GUID" Type="Str">{766DFC84-43B8-422D-A992-BCA52ED20A43}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{114D7ED9-EB04-4B72-8851-539431988997}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_Above50_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_Above50_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{A3EDEAFB-C8B9-4998-A662-ADE2979CE6A0}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_Above50_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_Above50_40KHZ/TestMode_Above50_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_Above50_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{72D7F7CD-6E66-4C50-B303-686C73FEDE53}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_Above50_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_Above50_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_Above50_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_Above50_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{CB3B9A9B-7D04-4DFA-A035-6AB0731A7660}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_Above50_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_Above100_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{3C84CAFA-C70A-4393-AAE2-EC81B6DC7692}</Property>
				<Property Name="App_INI_GUID" Type="Str">{BD912293-AEDD-414B-B33C-B6ADF91F0D3F}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{110631E3-2F4B-4F97-BDE0-E8FB1F7DFFA9}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_Above100_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_Above100_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{BC812283-86EE-4FC9-8271-6EFBAD534097}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_Above100_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_Above100_40KHZ/TestMode_Above100_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_Above100_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{72D7F7CD-6E66-4C50-B303-686C73FEDE53}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_Above100_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_Above100_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_Above100_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_Above100_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{A6FA9510-9826-414B-AFAD-D0FF6DC6E83E}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_Above100_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_Above500_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{986DE85D-DCD7-45D8-9C89-8B78F1469DC8}</Property>
				<Property Name="App_INI_GUID" Type="Str">{D1E7A4EC-B9B0-4031-A8F4-1372E85FBEE0}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{3C9224D2-4618-4FD2-ABFD-B933EFACD72D}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_Above500_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_Above500_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{D845EEA6-33E0-4ED6-9F21-E8C6AEF8E324}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_Above500_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_Above500_40KHZ/TestMode_Above500_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_Above500_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{72D7F7CD-6E66-4C50-B303-686C73FEDE53}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_Above500_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_Above500_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_Above500_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_Above500_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{C71C3622-CF22-433C-8099-44A848AC0690}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_Above500_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_Boundary_Maxoneless_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{2D086F83-3062-49EA-A30E-AB0572DD5BF2}</Property>
				<Property Name="App_INI_GUID" Type="Str">{B6BE6BD4-5CDA-4948-9463-A81E9ECF3042}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{8A9B0863-7350-4692-864C-07B67DA44F80}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_Boundary_Maxoneless_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_Boundary_Maxoneless_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{6CCEA6E4-0AF9-4622-80EF-84504856A606}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_Boundary_Maxoneless_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_Boundary_Maxoneless_40KHZ/TestMode_Boundary_Maxoneless_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_Boundary_Maxoneless_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{72D7F7CD-6E66-4C50-B303-686C73FEDE53}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_Boundary_Maxoneless_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_Boundary_Maxoneless_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_Boundary_Maxoneless_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_Boundary_Maxoneless_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{9E2678FC-2946-411F-8C0E-C5B756A2335A}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_Boundary_Maxoneless_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_Boundary_Maxonemore_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{A7B45571-BA8B-47D8-8DFB-D26FFA7A79EF}</Property>
				<Property Name="App_INI_GUID" Type="Str">{DB825534-BECF-4B16-85DE-C3EE1E667A03}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{744A6367-E976-40F3-839C-A485CBE185F0}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_Boundary_Maxonemore_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_Boundary_Maxonemore_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{0F45BF47-8974-4A3D-9784-C9B2BC1E0907}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_Boundary_Maxonemore_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_Boundary_Maxonemore_40KHZ/TestMode_Boundary_Maxonemore_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_Boundary_Maxonemore_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{72D7F7CD-6E66-4C50-B303-686C73FEDE53}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_Boundary_Maxonemore_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_Boundary_Maxonemore_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_Boundary_Maxonemore_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_Boundary_Maxonemore_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{D95450BE-2B3A-4F80-9151-42E51711BA33}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_Boundary_Maxonemore_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_Boundary_Minoneless_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{7F8FFCA5-0813-4D8D-8EBB-4F57F78FD864}</Property>
				<Property Name="App_INI_GUID" Type="Str">{75EB4A0D-0ADC-4932-9C01-2A4679D89B11}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{11A73A49-0199-4275-A3D3-1CFB5A881551}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_Boundary_Minoneless_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_Boundary_Minoneless_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{9297C70A-31A7-4D3E-BCD0-3872A263722A}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_Boundary_Minoneless_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_Boundary_Minoneless_40KHZ/TestMode_Boundary_Minoneless_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_Boundary_Minoneless_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{72D7F7CD-6E66-4C50-B303-686C73FEDE53}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_Boundary_Minoneless_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_Boundary_Minoneless_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_Boundary_Minoneless_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_Boundary_Minoneless_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{900C8A33-93FC-41A0-AFF4-D922B43A36BD}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_Boundary_Minoneless_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_Boundary_Minonemore_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{6747532E-4E8C-4007-8C7B-DDB2CB81180F}</Property>
				<Property Name="App_INI_GUID" Type="Str">{6B3A5369-6050-43A1-B87D-00E139474AD9}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{40E8CADE-7BAA-414B-B2BF-25AA08E27004}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_Boundary_Minonemore_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_Boundary_Minonemore_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{3C2498C8-28CA-4F46-814D-CD6450225B6A}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_Boundary_Minonemore_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_Boundary_Minonemore_40KHZ/TestMode_Boundary_Minonemore_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_Boundary_Minonemore_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{72D7F7CD-6E66-4C50-B303-686C73FEDE53}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_Boundary_Minonemore_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_Boundary_Minonemore_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_Boundary_Minonemore_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_Boundary_Minonemore_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{8FF560D7-8B8F-431C-9979-E0527ABC1ECA}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_Boundary_Minonemore_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MaxAmpLpC1_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{BD68BEEE-9D73-42EC-97C9-2AED8B3C6677}</Property>
				<Property Name="App_INI_GUID" Type="Str">{ACB00BEB-88B2-4D50-9CBA-8EC615FDA418}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{47CA17FE-9A59-4BA2-85EC-B06E10D15FDF}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MaxAmpLpC1_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MaxAmpLpC1_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{F97D04A8-D7A9-4D46-9CEA-6B4F76CA22DA}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MaxAmpLpC1_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MaxAmpLpC1_40KHZ/TestMode_MaxAmpLpC1_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MaxAmpLpC1_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{72D7F7CD-6E66-4C50-B303-686C73FEDE53}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MaxAmpLpC1_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MaxAmpLpC1_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MaxAmpLpC1_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MaxAmpLpC1_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{017095E7-1687-4F66-B233-CE2BD730F83D}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MaxAmpLpC1_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MaxAmpLpC2_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{24BF6622-0E27-4B65-9E95-0F45912746A4}</Property>
				<Property Name="App_INI_GUID" Type="Str">{E5F4F6CE-294B-4CF2-921D-279F3FCE2249}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{99A131E2-5FCC-4F3E-8606-55FAB24B335B}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MaxAmpLpC2_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MaxAmpLpC2_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{69BAE8B8-5890-4675-822B-FA066B4EB631}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MaxAmpLpC2_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MaxAmpLpC2_40KHZ/TestMode_MaxAmpLpC2_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MaxAmpLpC2_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{72D7F7CD-6E66-4C50-B303-686C73FEDE53}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MaxAmpLpC2_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MaxAmpLpC2_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MaxAmpLpC2_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MaxAmpLpC2_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{5968747D-D7E7-43BC-ADA0-DEBA499D5EAA}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MaxAmpLpC2_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MaxPhLp_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{A301599E-D24E-4D6A-8F41-855952AF7920}</Property>
				<Property Name="App_INI_GUID" Type="Str">{F76DF5B0-BBD5-499E-BB89-20C751D658CE}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{36B5BC94-804B-4E56-9F83-61884F0A7DC4}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MaxPhLp_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MaxPhLp_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{2E6F52E6-B20B-4BA4-BFB5-A40F43EFCD1F}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MaxPhLp_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MaxPhLp_40KHZ/TestMode_MaxPhLp_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MaxPhLp_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{72D7F7CD-6E66-4C50-B303-686C73FEDE53}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MaxPhLp_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MaxPhLp_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MaxPhLp_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MaxPhLp_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{F00C1B81-21A2-4AD7-8CCC-2C9C3DE98576}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MaxPhLp_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MaxPhLpCF_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{230C4388-A7B5-4848-95C7-2398B9CB6CB0}</Property>
				<Property Name="App_INI_GUID" Type="Str">{663BF2A8-37AB-4D32-A967-49BF36A1C7CB}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{AD9A6D28-48F8-4692-B993-9120A8BA25BD}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MaxPhLpCF_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MaxPhLpCF_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{1E8C684E-08B5-4201-A635-C73416D5A72F}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MaxPhLpCF_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MaxPhLpCF_40KHZ/TestMode_MaxPhLpCF_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MaxPhLpCF_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{72D7F7CD-6E66-4C50-B303-686C73FEDE53}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MaxPhLpCF_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MaxPhLpCF_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MaxPhLpCF_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MaxPhLpCF_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{AD9A1C04-D4DA-4B56-9523-F10D3B285237}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MaxPhLpCF_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MaxRecipe_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{869AFAFB-373D-492E-A84C-564503122615}</Property>
				<Property Name="App_INI_GUID" Type="Str">{855829B0-3AA7-445D-8145-A404E5455E3C}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{BFFA79F2-8885-4BCF-B07B-C18A169C955E}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MaxRecipe_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MaxRecipe_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{87FFC2C4-42EB-43C9-937C-51F89CC031B9}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MaxRecipe_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MaxRecipe_40KHZ/TestMode_MaxRecipe_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MaxRecipe_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{72D7F7CD-6E66-4C50-B303-686C73FEDE53}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MaxRecipe_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MaxRecipe_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MaxRecipe_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MaxRecipe_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{106DBDD3-8198-4A73-9343-E9D466706420}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MaxRecipe_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MinAmpLpC1_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{50DA0D42-3B91-4690-BFF2-1ABD02101FB6}</Property>
				<Property Name="App_INI_GUID" Type="Str">{A966FF9B-3807-410F-8C95-E092F9537246}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{458587B4-73C3-475E-8840-05F495706632}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MinAmpLpC1_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MinAmpLpC1_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{E7D2C067-F431-4BBD-A5C3-A40E2978D80E}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MinAmpLpC1_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MinAmpLpC1_40KHZ/TestMode_MinAmpLpC1_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MinAmpLpC1_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{72D7F7CD-6E66-4C50-B303-686C73FEDE53}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MinAmpLpC1_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MinAmpLpC1_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MinAmpLpC1_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MinAmpLpC1_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{D4C5E5F0-7F28-43F2-A205-A9FDDD8D4D7B}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MinAmpLpC1_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MinAmpLpC2_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{B9FDC9E5-0975-4BFD-81B6-D4152B6D9219}</Property>
				<Property Name="App_INI_GUID" Type="Str">{CA8F434B-6867-4E44-9AF5-BD01AC4448E5}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{2EA6F466-4ECF-4B80-AF19-13669C80079A}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MinAmpLpC2_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MinAmpLpC2_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{7309F83D-90BA-4F8C-AC62-02639C8B7C52}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MinAmpLpC2_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MinAmpLpC2_40KHZ/TestMode_MinAmpLpC2_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MinAmpLpC2_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{72D7F7CD-6E66-4C50-B303-686C73FEDE53}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MinAmpLpC2_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MinAmpLpC2_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MinAmpLpC2_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MinAmpLpC2_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{CAB4C699-FE43-41B3-9229-65925B33599D}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MinAmpLpC2_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MinPhLp_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{E3F827F2-974C-4652-A6CE-731FC628F654}</Property>
				<Property Name="App_INI_GUID" Type="Str">{5CB0CC2F-48CD-4D78-A15B-A276E53D6F10}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{63A6D6DC-3662-4D5A-88C3-62F518635CB0}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MinPhLp_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MinPhLp_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{B367633D-9787-4B0A-9FCF-4F277C594134}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MinPhLp_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MinPhLp_40KHZ/TestMode_MinPhLp_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MinPhLp_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{72D7F7CD-6E66-4C50-B303-686C73FEDE53}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MinPhLp_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MinPhLp_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MinPhLp_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MinPhLp_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{A6B7A39C-EFFE-4780-8B2E-A175982B6EB7}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MinPhLp_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MinPhLpCF_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{9D9225D1-8391-490D-93DE-583AEA393C16}</Property>
				<Property Name="App_INI_GUID" Type="Str">{7C5F58E9-0197-438D-BAF5-9CF3D83A3829}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{551BD59B-ECF1-4938-BD03-8734B461BFB3}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MinPhLpCF_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MinPhLpCF_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{21EC244D-8AA7-4BE0-B9AF-DD6C598C91D3}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MinPhLpCF_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MinPhLpCF_40KHZ/TestMode_MinPhLpCF_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MinPhLpCF_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{72D7F7CD-6E66-4C50-B303-686C73FEDE53}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MinPhLpCF_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MinPhLpCF_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MinPhLpCF_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MinPhLpCF_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{F20C49A4-8A7A-4E72-B095-F70041BD0148}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MinPhLpCF_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MinRecipe_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{95CA4031-9156-446B-94A4-F9BECA336546}</Property>
				<Property Name="App_INI_GUID" Type="Str">{C506EA14-044E-49E9-84F0-EAF866512D9F}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{9ED0447C-898A-4ECC-80FC-1A6306E6D40E}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MinRecipe_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MinRecipe_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{BD425166-6396-49AA-A546-D7522B525DFF}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MinRecipe_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MinRecipe_40KHZ/TestMode_MinRecipe_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_MinRecipe_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{72D7F7CD-6E66-4C50-B303-686C73FEDE53}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MinRecipe_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MinRecipe_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MinRecipe_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MinRecipe_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{33503B80-48C5-47E6-9104-5E979C4C3293}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MinRecipe_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_Rcp50_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{FCBDC331-C2E8-4DE0-BB05-0F9814B5FAED}</Property>
				<Property Name="App_INI_GUID" Type="Str">{2BB8046D-BD6B-4993-AD5D-573BEAE84AAF}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{C332D5F2-4DD9-4C6E-981C-EAAE9987BA41}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_Rcp50_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_Rcp50_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{534ADDF3-4CBB-4501-8E65-5ACD3F704B1D}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_Rcp50_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_Rcp50_40KHZ/TestMode_Rcp50_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_40KHZ_AUTO/TestMode_Rcp50_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{72D7F7CD-6E66-4C50-B303-686C73FEDE53}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_Rcp50_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_Rcp50_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_Rcp50_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_Rcp50_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{32ABE758-DDE4-4157-80DC-98FE9FFD1A24}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_Rcp50_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_RcpBelow50_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{F545753A-FD7A-4374-AABD-9483CECAB65A}</Property>
				<Property Name="App_INI_GUID" Type="Str">{BB3570B6-4BA5-431A-9184-922288563DEF}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{0FF6F740-9DAA-415F-9969-B82E6CA16ADD}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_RcpBelow50_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../builds/NI_AB_PROJECTNAME/TestMode_RcpBelow50_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{5B4E09A9-0417-40B0-AAF8-23EE2712EC11}</Property>
				<Property Name="Bld_version.build" Type="Int">1</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Application.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../builds/NI_AB_PROJECTNAME/TestMode_RcpBelow50_40KHZ/Application.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../builds/NI_AB_PROJECTNAME/TestMode_RcpBelow50_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Source[0].itemID" Type="Str">{72D7F7CD-6E66-4C50-B303-686C73FEDE53}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_RcpBelow50_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_RcpBelow50_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_RcpBelow50_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_RcpBelow50_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{22AC9C87-C33A-4D0F-B740-E333A77BA816}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Application.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
		</Item>
	</Item>
</Project>
