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
		<Item Name="Default_TestMode_30KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_30KHZ_AUTO/Default_TestMode_30KHZ/Default_TestMode_30KHZ.vi"/>
		<Item Name="TestMode_Above50_30KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_30KHZ_AUTO/TestMode_Above50_30KHZ/TestMode_Above50_30KHZ.vi"/>
		<Item Name="TestMode_Above100_30KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_30KHZ_AUTO/TestMode_Above100_30KHZ/TestMode_Above100_30KHZ.vi"/>
		<Item Name="TestMode_Above500_30KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_30KHZ_AUTO/TestMode_Above500_30KHZ/TestMode_Above500_30KHZ.vi"/>
		<Item Name="TestMode_Boundary_Maxoneless_30KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_30KHZ_AUTO/TestMode_Boundary_Maxoneless_30KHZ/TestMode_Boundary_Maxoneless_30KHZ.vi"/>
		<Item Name="TestMode_Boundary_Maxonemore_30KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_30KHZ_AUTO/TestMode_Boundary_Maxonemore_30KHZ/TestMode_Boundary_Maxonemore_30KHZ.vi"/>
		<Item Name="TestMode_Boundary_Minoneless_30KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_30KHZ_AUTO/TestMode_Boundary_Minoneless_30KHZ/TestMode_Boundary_Minoneless_30KHZ.vi"/>
		<Item Name="TestMode_Boundary_Minonemore_30KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_30KHZ_AUTO/TestMode_Boundary_Minonemore_30KHZ/TestMode_Boundary_Minonemore_30KHZ.vi"/>
		<Item Name="TestMode_MaxAmpLpC1_30KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_30KHZ_AUTO/TestMode_MaxAmpLpC1_30KHZ/TestMode_MaxAmpLpC1_30KHZ.vi"/>
		<Item Name="TestMode_MaxAmpLpC2_30KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_30KHZ_AUTO/TestMode_MaxAmpLpC2_30KHZ/TestMode_MaxAmpLpC2_30KHZ.vi"/>
		<Item Name="TestMode_MaxPhLp_30KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_30KHZ_AUTO/TestMode_MaxPhLp_30KHZ/TestMode_MaxPhLp_30KHZ.vi"/>
		<Item Name="TestMode_MaxPhLpCF_30KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_30KHZ_AUTO/TestMode_MaxPhLpCF_30KHZ/TestMode_MaxPhLpCF_30KHZ.vi"/>
		<Item Name="TestMode_MaxRecipe_30KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_30KHZ_AUTO/TestMode_MaxRecipe_30KHZ/TestMode_MaxRecipe_30KHZ.vi"/>
		<Item Name="TestMode_MinAmpLpC1_30KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_30KHZ_AUTO/TestMode_MinAmpLpC1_30KHZ/TestMode_MinAmpLpC1_30KHZ.vi"/>
		<Item Name="TestMode_MinAmpLpC2_30KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_30KHZ_AUTO/TestMode_MinAmpLpC2_30KHZ/TestMode_MinAmpLpC2_30KHZ.vi"/>
		<Item Name="TestMode_MinPhLp_30KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_30KHZ_AUTO/TestMode_MinPhLp_30KHZ/TestMode_MinPhLp_30KHZ.vi"/>
		<Item Name="TestMode_MinPhLpCF_30KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_30KHZ_AUTO/TestMode_MinPhLpCF_30KHZ/TestMode_MinPhLpCF_30KHZ.vi"/>
		<Item Name="TestMode_MinRecipe_30KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_30KHZ_AUTO/TestMode_MinRecipe_30KHZ/TestMode_MinRecipe_30KHZ.vi"/>
		<Item Name="TestMode_Rcp50_30KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_30KHZ_AUTO/TestMode_Rcp50_30KHZ/TestMode_Rcp50_30KHZ.vi"/>
		<Item Name="TestMode_RcpBelow50_30KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_30KHZ_AUTO/TestMode_RcpBelow50_30KHZ/TestMode_RcpBelow50_30KHZ.vi"/>
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
			<Item Name="TestMode_Operation_SUB_VI_default.vi" Type="VI" URL="../../Generic_Common/Test_Mode/TestMode_Operation_SUB_VI_default.vi"/>
			<Item Name="TestMode_RangeValidation.vi" Type="VI" URL="../../Generic_Common/SEEK/TestMode_RangeValidation.vi"/>
			<Item Name="Write_Html_Report_Sub_VI.vi" Type="VI" URL="../../Generic_Common/Write_Html_Report_Sub_VI.vi"/>
		</Item>
		<Item Name="Build Specifications" Type="Build">
			<Item Name="Default_TestMode_30KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{E801AEDD-7F00-4D53-BE8D-378930915D8D}</Property>
				<Property Name="App_INI_GUID" Type="Str">{16612BC7-5A92-4EEC-93A0-81C09E3E091C}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{BEE71164-5E39-492D-91EA-1799F1761738}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Default_TestMode_30KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/Default_TestMode_30KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{44B6A251-F72C-4429-A363-612CF341BF84}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Default_TestMode_30KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/Default_TestMode_30KHZ/Default_TestMode_30KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/Default_TestMode_30KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{C3570199-D61A-4DBF-AB6C-966303B12822}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/Default_TestMode_30KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Default_TestMode_30KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Default_TestMode_30KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">Default_TestMode_30KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{952BB2CE-CF50-4A82-B050-49E1C9278F59}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Default_TestMode_30KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_Above50_30KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{44C25CBC-9D2C-45ED-922D-5D3459CBCD51}</Property>
				<Property Name="App_INI_GUID" Type="Str">{D66EE963-8160-49E8-8B08-D26A4A8D1799}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{62856CE9-6B68-442D-9DD6-C87EB1568368}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_Above50_30KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_Above50_30KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{34E13461-EB96-465D-A1AC-DB9DEC7C0404}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_Above50_30KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_Above50_30KHZ/TestMode_Above50_30KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_Above50_30KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{C3570199-D61A-4DBF-AB6C-966303B12822}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_Above50_30KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_Above50_30KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_Above50_30KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_Above50_30KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{F51A94DD-7256-4760-9EA4-E6E121DFD69B}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_Above50_30KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_Above100_30KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{048F8E6F-D078-4FB6-ABE3-49EB4DE881EA}</Property>
				<Property Name="App_INI_GUID" Type="Str">{2AFC7AE2-F2DC-4943-84AE-9558678BEDA3}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{328EDB8C-9777-4A6F-AAAC-2C3DBD60C766}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_Above100_30KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_Above100_30KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{25C496A4-5408-4770-9D24-F2966D0F6043}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_Above100_30KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_Above100_30KHZ/TestMode_Above100_30KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_Above100_30KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{C3570199-D61A-4DBF-AB6C-966303B12822}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_Above100_30KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_Above100_30KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_Above100_30KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_Above100_30KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{98F26C27-1A60-4863-935E-B173D58DDB95}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_Above100_30KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_Above500_30KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{8DC43C2A-6AC1-4A74-A433-2BC9C2A793C5}</Property>
				<Property Name="App_INI_GUID" Type="Str">{A0E88CF0-23F6-4A4C-BFDB-056476F40F93}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{7477AD4B-18B7-4B5C-8808-6ACEA7647A4A}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_Above500_30KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_Above500_30KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{CBB75F8C-3249-45D8-AE1D-F60585E4DCCC}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_Above500_30KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_Above500_30KHZ/TestMode_Above500_30KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_Above500_30KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{C3570199-D61A-4DBF-AB6C-966303B12822}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_Above500_30KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_Above500_30KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_Above500_30KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_Above500_30KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{1BD1705F-FF77-408D-B6A9-0899A85515D5}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_Above500_30KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_Boundary_Maxoneless_30KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{DFCC56A9-836C-4010-9FE5-49147D166420}</Property>
				<Property Name="App_INI_GUID" Type="Str">{6F8AE463-5224-44AB-8652-E5F7A96F79F7}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{D2484C28-8E08-45D9-94C0-FC7B9801568C}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_Boundary_Maxoneless_30KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_Boundary_Maxoneless_30KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{560B9478-2532-473C-AB88-7012A15D0665}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_Boundary_Maxoneless_30KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_Boundary_Maxoneless_30KHZ/TestMode_Boundary_Maxoneless_30KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_Boundary_Maxoneless_30KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{C3570199-D61A-4DBF-AB6C-966303B12822}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_Boundary_Maxoneless_30KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_Boundary_Maxoneless_30KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_Boundary_Maxoneless_30KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_Boundary_Maxoneless_30KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{65BF5C18-8980-4BA0-B36B-59E856911EA0}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_Boundary_Maxoneless_30KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_Boundary_Maxonemore_30KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{7A34DCFD-7F8E-4C19-A844-2311EA02F1C4}</Property>
				<Property Name="App_INI_GUID" Type="Str">{96A3D4FC-31D5-48C2-A519-F1ED1642A39B}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{0571D652-E0A2-4390-815F-018C368D696F}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_Boundary_Maxonemore_30KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_Boundary_Maxonemore_30KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{3DD199CC-4628-454E-B8F0-28E7EBF1B9CD}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_Boundary_Maxonemore_30KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_Boundary_Maxonemore_30KHZ/TestMode_Boundary_Maxonemore_30KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_Boundary_Maxonemore_30KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{C3570199-D61A-4DBF-AB6C-966303B12822}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_Boundary_Maxonemore_30KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_Boundary_Maxonemore_30KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_Boundary_Maxonemore_30KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_Boundary_Maxonemore_30KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{1E5FB822-1EA2-4A6C-BA91-ABFC313D682D}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_Boundary_Maxonemore_30KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_Boundary_Minoneless_30KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{35FAFA98-3375-4501-8BEB-43299FC97EDD}</Property>
				<Property Name="App_INI_GUID" Type="Str">{ACA2F591-3E73-4A51-9E23-9463033BC4A4}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{8D4FE287-8DD4-489A-A24A-7B4BD2F96BF7}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_Boundary_Minoneless_30KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_Boundary_Minoneless_30KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{8AE1A803-27EE-4BE7-AD1F-A3A76DF42E26}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_Boundary_Minoneless_30KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_Boundary_Minoneless_30KHZ/TestMode_Boundary_Minoneless_30KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_Boundary_Minoneless_30KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{C3570199-D61A-4DBF-AB6C-966303B12822}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_Boundary_Minoneless_30KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_Boundary_Minoneless_30KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_Boundary_Minoneless_30KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_Boundary_Minoneless_30KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{C85A9914-4BAE-489A-8FC6-FDE1B131CDAD}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_Boundary_Minoneless_30KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_Boundary_Minonemore_30KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{37435CB4-7E20-4DE3-98EB-C61B1A9048EB}</Property>
				<Property Name="App_INI_GUID" Type="Str">{044779EE-07C7-4C63-A8F3-6494152C6D3F}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{622F5798-6A49-43B3-9159-DE4B0BBB73FD}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_Boundary_Minonemore_30KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_Boundary_Minonemore_30KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{7FD85FFD-443D-477D-B49F-22F06A9B41DB}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_Boundary_Minonemore_30KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_Boundary_Minonemore_30KHZ/TestMode_Boundary_Minonemore_30KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_Boundary_Minonemore_30KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{C3570199-D61A-4DBF-AB6C-966303B12822}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_Boundary_Minonemore_30KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_Boundary_Minonemore_30KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_Boundary_Minonemore_30KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_Boundary_Minonemore_30KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{353BE6D6-8500-459E-98E7-0C8D94099A09}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_Boundary_Minonemore_30KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MaxAmpLpC1_30KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{41968BD5-2262-4B95-9A13-286F228EC77C}</Property>
				<Property Name="App_INI_GUID" Type="Str">{E2C85600-54A8-4094-99E6-3F006460F417}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{DADB6F74-863F-4623-B8ED-E77FC2CE968B}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MaxAmpLpC1_30KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MaxAmpLpC1_30KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{C75521D1-1185-4B18-9183-E669078CFCF5}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MaxAmpLpC1_30KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MaxAmpLpC1_30KHZ/TestMode_MaxAmpLpC1_30KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MaxAmpLpC1_30KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{C3570199-D61A-4DBF-AB6C-966303B12822}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MaxAmpLpC1_30KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MaxAmpLpC1_30KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MaxAmpLpC1_30KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MaxAmpLpC1_30KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{0A2ED205-6197-439D-BB10-328B9D772E97}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MaxAmpLpC1_30KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MaxAmpLpC2_30KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{C9092F07-0EEA-4AF0-BBFA-99DA21283F01}</Property>
				<Property Name="App_INI_GUID" Type="Str">{362EEACF-953D-4874-A729-075E1BCFA834}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{873DE73A-19C1-477B-8310-DBEC33680E95}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MaxAmpLpC2_30KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MaxAmpLpC2_30KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{49BB8DD5-D99C-4B93-8A05-13CAC43EF7D7}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MaxAmpLpC2_30KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MaxAmpLpC2_30KHZ/TestMode_MaxAmpLpC2_30KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MaxAmpLpC2_30KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{C3570199-D61A-4DBF-AB6C-966303B12822}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MaxAmpLpC2_30KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MaxAmpLpC2_30KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MaxAmpLpC2_30KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MaxAmpLpC2_30KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{89283DEE-62F1-4D56-BF8C-CC7718408E5E}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MaxAmpLpC2_30KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MaxPhLp_30KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{F41FA9A2-7390-4954-BFF6-3060A820DBC1}</Property>
				<Property Name="App_INI_GUID" Type="Str">{BB78D5C5-5805-4789-A6E5-EBCA0C7A61BC}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{C67A6C3B-DA8C-46EE-A645-4C8BB0370765}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MaxPhLp_30KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MaxPhLp_30KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{6B183394-1E4D-478A-A810-60D8C90744D2}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MaxPhLp_30KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MaxPhLp_30KHZ/TestMode_MaxPhLp_30KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MaxPhLp_30KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{C3570199-D61A-4DBF-AB6C-966303B12822}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MaxPhLp_30KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MaxPhLp_30KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MaxPhLp_30KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MaxPhLp_30KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{1AF9668A-9EDA-4511-BDBB-CB474E53DF3B}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MaxPhLp_30KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MaxPhLpCF_30KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{EB842D14-906B-41EF-B2C3-5A6FB1930037}</Property>
				<Property Name="App_INI_GUID" Type="Str">{47CA8CCF-8C06-4419-983E-DE5E872E761C}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{2825C506-0FFD-419D-845F-7B5C494418E3}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MaxPhLpCF_30KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MaxPhLpCF_30KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{97E69A4D-0867-4FCC-AF05-A5C9E9E243F0}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MaxPhLpCF_30KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MaxPhLpCF_30KHZ/TestMode_MaxPhLpCF_30KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MaxPhLpCF_30KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{C3570199-D61A-4DBF-AB6C-966303B12822}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MaxPhLpCF_30KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MaxPhLpCF_30KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MaxPhLpCF_30KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MaxPhLpCF_30KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{F7376AB5-DB89-4259-9D07-D7685862EAE4}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MaxPhLpCF_30KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MaxRecipe_30KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{BD4447DA-790C-4DA0-9D60-8C95A1E1F377}</Property>
				<Property Name="App_INI_GUID" Type="Str">{87C17918-F1B6-43BE-AA15-8A3CDB4AFDC8}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{0D720CB0-C432-4CD1-BC45-ED8A14138466}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MaxRecipe_30KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MaxRecipe_30KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{8D67BBAE-5FDD-4B3A-8DA0-24739B63D2B3}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MaxRecipe_30KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MaxRecipe_30KHZ/TestMode_MaxRecipe_30KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MaxRecipe_30KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{C3570199-D61A-4DBF-AB6C-966303B12822}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MaxRecipe_30KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MaxRecipe_30KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MaxRecipe_30KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MaxRecipe_30KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{0E14DE65-9EB1-466B-8FAF-B0240E52243E}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MaxRecipe_30KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MinAmpLpC1_30KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{95A12E0F-E9B8-44F3-BFF1-2033F68EF9B2}</Property>
				<Property Name="App_INI_GUID" Type="Str">{CD9BECED-A6C0-49B3-A354-5DF891786496}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{360011CB-D648-46A0-887C-2BC3EF4C87AF}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MinAmpLpC1_30KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MinAmpLpC1_30KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{140467A6-8B33-47EF-AB94-A0642C0A5F31}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MinAmpLpC1_30KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MinAmpLpC1_30KHZ/TestMode_MinAmpLpC1_30KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MinAmpLpC1_30KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{C3570199-D61A-4DBF-AB6C-966303B12822}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MinAmpLpC1_30KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MinAmpLpC1_30KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MinAmpLpC1_30KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MinAmpLpC1_30KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{746189A7-74FC-4CAE-B4CF-156DC8372D67}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MinAmpLpC1_30KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MinAmpLpC2_30KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{6861A97B-C86B-4EAC-B5FD-4FB3B89249DF}</Property>
				<Property Name="App_INI_GUID" Type="Str">{F2013DA8-4953-49AA-89D8-799BEA5B7B55}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{D294846D-ABC2-47A8-B59D-CEA44F8EF9C3}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MinAmpLpC2_30KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MinAmpLpC2_30KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{DC3297AF-72A9-4D67-BBDA-7B9363247F64}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MinAmpLpC2_30KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MinAmpLpC2_30KHZ/TestMode_MinAmpLpC2_30KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MinAmpLpC2_30KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{C3570199-D61A-4DBF-AB6C-966303B12822}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MinAmpLpC2_30KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MinAmpLpC2_30KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MinAmpLpC2_30KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MinAmpLpC2_30KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{6FD6434E-E799-46AF-9852-05BEFBF07D73}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MinAmpLpC2_30KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MinPhLp_30KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{7DA4EB99-DA11-4D3C-8AF5-8F8AFBDCCDD7}</Property>
				<Property Name="App_INI_GUID" Type="Str">{D7A4460E-E194-4F96-A7DD-7BC6B9A59CF1}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{2BEA98EB-5BF8-4E83-9CDE-E64E4194C91F}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MinPhLp_30KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MinPhLp_30KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{25737F5F-CA45-4548-8C0F-BC7FE9E0562D}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MinPhLp_30KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MinPhLp_30KHZ/TestMode_MinPhLp_30KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MinPhLp_30KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{C3570199-D61A-4DBF-AB6C-966303B12822}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MinPhLp_30KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MinPhLp_30KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MinPhLp_30KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MinPhLp_30KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{91C55756-E6C6-49E6-97E3-B3FA5412BB28}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MinPhLp_30KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MinPhLpCF_30KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{16F17F01-94CC-4446-98C7-97D6B34947B4}</Property>
				<Property Name="App_INI_GUID" Type="Str">{AD17BD73-6D91-4FC2-BD11-BD90405A173B}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{85650019-A205-4FA3-88C6-2BD7AC5D3B66}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MinPhLpCF_30KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MinPhLpCF_30KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{A1917158-0950-432B-8C30-65D699F99790}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MinPhLpCF_30KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MinPhLpCF_30KHZ/TestMode_MinPhLpCF_30KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MinPhLpCF_30KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{C3570199-D61A-4DBF-AB6C-966303B12822}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MinPhLpCF_30KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MinPhLpCF_30KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MinPhLpCF_30KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MinPhLpCF_30KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{088A9B3B-8BC1-49CB-8BE7-6B3ECF402205}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MinPhLpCF_30KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MinRecipe_30KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{61D23E04-DC30-409F-B924-231B3968F304}</Property>
				<Property Name="App_INI_GUID" Type="Str">{91BC8CD4-356F-487C-B976-41A48112C854}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{90F3D647-8CA4-4FCC-AE5F-6ED59E3C7BCB}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MinRecipe_30KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MinRecipe_30KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{02D7D20C-8E7A-4CB5-BCAD-3381EB22DDEC}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MinRecipe_30KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MinRecipe_30KHZ/TestMode_MinRecipe_30KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_MinRecipe_30KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{C3570199-D61A-4DBF-AB6C-966303B12822}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MinRecipe_30KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MinRecipe_30KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MinRecipe_30KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MinRecipe_30KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{1A5FFD51-AC31-41AF-996C-E30BEEDF05CB}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MinRecipe_30KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_Rcp50_30KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{18BC4470-8F53-451A-9927-163CF946DA90}</Property>
				<Property Name="App_INI_GUID" Type="Str">{0303C3A4-380A-401B-856E-6C21B2F57F83}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{E5A8F83E-C28D-4F81-9508-6CA594253481}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_Rcp50_30KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_Rcp50_30KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{E8DC5F4A-1CB8-4377-9720-F89A0FB4C185}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_Rcp50_30KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_Rcp50_30KHZ/TestMode_Rcp50_30KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_Rcp50_30KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{C3570199-D61A-4DBF-AB6C-966303B12822}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_Rcp50_30KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_Rcp50_30KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_Rcp50_30KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_Rcp50_30KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{01E83A17-6723-4598-AFED-4E32D5361B37}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_Rcp50_30KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_RcpBelow50_30KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{A2AA3263-FE93-4834-8661-4690568D8229}</Property>
				<Property Name="App_INI_GUID" Type="Str">{2AFEA312-6DA2-4317-B40E-37E60FAC13A5}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{976E1464-F73F-4E02-BE00-84CC4ECBB03E}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_RcpBelow50_30KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_RcpBelow50_30KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{29610A12-75B3-44F6-92CD-A56911FD27AE}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_RcpBelow50_30KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_RcpBelow50_30KHZ/TestMode_RcpBelow50_30KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_30KHZ_AUTO/TestMode_RcpBelow50_30KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{C3570199-D61A-4DBF-AB6C-966303B12822}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_RcpBelow50_30KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_RcpBelow50_30KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_RcpBelow50_30KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_RcpBelow50_30KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{42013B1B-348A-4B18-B293-7E395C88CBD4}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_RcpBelow50_30KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
		</Item>
	</Item>
</Project>
