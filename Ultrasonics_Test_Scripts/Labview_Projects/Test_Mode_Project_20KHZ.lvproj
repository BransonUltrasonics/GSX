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
		<Item Name="Default_TestMode_20KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_20KHZ_AUTO/Default_TestMode_20KHZ/Default_TestMode_20KHZ.vi"/>
		<Item Name="TestMode_Above50_20KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_20KHZ_AUTO/TestMode_Above50_20KHZ/TestMode_Above50_20KHZ.vi"/>
		<Item Name="TestMode_Above100_20KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_20KHZ_AUTO/TestMode_Above100_20KHZ/TestMode_Above100_20KHZ.vi"/>
		<Item Name="TestMode_Above500_20KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_20KHZ_AUTO/TestMode_Above500_20KHZ/TestMode_Above500_20KHZ.vi"/>
		<Item Name="TestMode_Boundary_Maxoneless_20KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_20KHZ_AUTO/TestMode_Boundary_Maxoneless_20KHZ/TestMode_Boundary_Maxoneless_20KHZ.vi"/>
		<Item Name="TestMode_Boundary_Maxonemore_20KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_20KHZ_AUTO/TestMode_Boundary_Maxonemore_20KHZ/TestMode_Boundary_Maxonemore_20KHZ.vi"/>
		<Item Name="TestMode_Boundary_Minoneless_20KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_20KHZ_AUTO/TestMode_Boundary_Minoneless_20KHZ/TestMode_Boundary_Minoneless_20KHZ.vi"/>
		<Item Name="TestMode_Boundary_Minonemore_20KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_20KHZ_AUTO/TestMode_Boundary_Minonemore_20KHZ/TestMode_Boundary_Minonemore_20KHZ.vi"/>
		<Item Name="TestMode_MaxAmpLpC1_20KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_20KHZ_AUTO/TestMode_MaxAmpLpC1_20KHZ/TestMode_MaxAmpLpC1_20KHZ.vi"/>
		<Item Name="TestMode_MaxAmpLpC2_20KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_20KHZ_AUTO/TestMode_MaxAmpLpC2_20KHZ/TestMode_MaxAmpLpC2_20KHZ.vi"/>
		<Item Name="TestMode_MaxPhLp_20KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_20KHZ_AUTO/TestMode_MaxPhLp_20KHZ/TestMode_MaxPhLp_20KHZ.vi"/>
		<Item Name="TestMode_MaxPhLpCF_20KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_20KHZ_AUTO/TestMode_MaxPhLpCF_20KHZ/TestMode_MaxPhLpCF_20KHZ.vi"/>
		<Item Name="TestMode_MaxRecipe_20KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_20KHZ_AUTO/TestMode_MaxRecipe_20KHZ/TestMode_MaxRecipe_20KHZ.vi"/>
		<Item Name="TestMode_MinAmpLpC1_20KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_20KHZ_AUTO/TestMode_MinAmpLpC1_20KHZ/TestMode_MinAmpLpC1_20KHZ.vi"/>
		<Item Name="TestMode_MinAmpLpC2_20KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_20KHZ_AUTO/TestMode_MinAmpLpC2_20KHZ/TestMode_MinAmpLpC2_20KHZ.vi"/>
		<Item Name="TestMode_MinPhLp_20KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_20KHZ_AUTO/TestMode_MinPhLp_20KHZ/TestMode_MinPhLp_20KHZ.vi"/>
		<Item Name="TestMode_MinPhLpCF_20KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_20KHZ_AUTO/TestMode_MinPhLpCF_20KHZ/TestMode_MinPhLpCF_20KHZ.vi"/>
		<Item Name="TestMode_MinRecipe_20KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_20KHZ_AUTO/TestMode_MinRecipe_20KHZ/TestMode_MinRecipe_20KHZ.vi"/>
		<Item Name="TestMode_Rcp50_20KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_20KHZ_AUTO/TestMode_Rcp50_20KHZ/TestMode_Rcp50_20KHZ.vi"/>
		<Item Name="TestMode_RcpBelow50_20KHZ.vi" Type="VI" URL="../../BF_INT_TestMode_20KHZ_AUTO/TestMode_RcpBelow50_20KHZ/TestMode_RcpBelow50_20KHZ.vi"/>
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
			<Item Name="Default_TestMode_20KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{8688F9E3-405A-43FC-B872-E414F56544CA}</Property>
				<Property Name="App_INI_GUID" Type="Str">{71B006F9-11CC-4DA8-BB4D-7C7C21FE7DA0}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{D30560DD-E7BC-46A2-A405-3592987C87E9}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Default_TestMode_20KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/NI_AB_PROJECTNAME</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{469C33C7-DCBE-4077-A475-93C5A2D33092}</Property>
				<Property Name="Bld_version.build" Type="Int">3</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Default_TestMode_20KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/NI_AB_PROJECTNAME/Default_TestMode_20KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/NI_AB_PROJECTNAME/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{4A7C9C58-7B56-4646-8983-0679BEC3A94C}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/Default_TestMode_20KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Default_TestMode_20KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Default_TestMode_20KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">Default_TestMode_20KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{3E835F50-D9A5-43C7-B2E3-0FAA56774E62}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Default_TestMode_20KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_Above50_20KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{03985330-9450-47E9-A2AD-0236C96833CC}</Property>
				<Property Name="App_INI_GUID" Type="Str">{A2FD36E5-4988-4EA1-A255-FD9C6DDBBC11}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{E2644A5A-79C0-4B8B-B553-67130DBEEF0A}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_Above50_20KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_Above50_20KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{A09B1946-AC30-4A9F-8377-34DDBE26C1CF}</Property>
				<Property Name="Bld_version.build" Type="Int">4</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_Above50_20KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_Above50_20KHZ/TestMode_Above50_20KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_Above50_20KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{78AE97D9-F453-492C-A54E-198FC388CB5B}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_Above50_20KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_Above50_20KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_Above50_20KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_Above50_20KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{98E23488-FC4E-4E02-B2CD-F4F379B2135C}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_Above50_20KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_Above100_20KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{3F1C6059-FE55-4CF8-A178-1D2EAE14E50D}</Property>
				<Property Name="App_INI_GUID" Type="Str">{A57F24A7-066C-413E-8335-DD1CB6D5E3D2}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{538AF7FA-10FD-42E0-8358-96190D39D8C6}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_Above100_20KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_Above100_20KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{7F0044E6-1BA6-48C6-A84E-9ADD821416B3}</Property>
				<Property Name="Bld_version.build" Type="Int">3</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_Above100_20KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_Above100_20KHZ/TestMode_Above100_20KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_Above100_20KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{78AE97D9-F453-492C-A54E-198FC388CB5B}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_Above100_20KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_Above100_20KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_Above100_20KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_Above100_20KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{44BA235C-B69F-4AD1-B2BD-A7ED07AC4BC9}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_Above100_20KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_Above500_20KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{865D5211-F0D9-4B8C-9766-63FB08535864}</Property>
				<Property Name="App_INI_GUID" Type="Str">{3C0BE3F7-B95B-46E6-8C02-D6722F3FAF43}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{B46873BF-63A8-4E2E-86D8-1C44F4B4BCCA}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_Above500_20KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_Above500_20KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{2B2B0124-1E44-4DA2-B46E-52151BAF85AB}</Property>
				<Property Name="Bld_version.build" Type="Int">3</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_Above500_20KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_Above500_20KHZ/TestMode_Above500_20KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_Above500_20KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{78AE97D9-F453-492C-A54E-198FC388CB5B}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_Above500_20KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_Above500_20KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_Above500_20KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_Above500_20KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{9E7879D8-969C-4BDF-98B0-7854196D5E26}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_Above500_20KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_Boundary_Maxoneless_20KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{5353A70D-FFDA-4F8F-A32E-DF70A2CCA070}</Property>
				<Property Name="App_INI_GUID" Type="Str">{99C46400-09DD-4F18-B986-6380C4C1CA95}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{BEFADF80-DADB-406C-9735-B6AD42B881A2}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_Boundary_Maxoneless_20KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_Boundary_Maxoneless_20KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{308613C5-91D1-4A3A-9A0B-238E8C830983}</Property>
				<Property Name="Bld_version.build" Type="Int">3</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_Boundary_Maxoneless_20KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_Boundary_Maxoneless_20KHZ/TestMode_Boundary_Maxoneless_20KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_Boundary_Maxoneless_20KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{78AE97D9-F453-492C-A54E-198FC388CB5B}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_Boundary_Maxoneless_20KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_Boundary_Maxoneless_20KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_Boundary_Maxoneless_20KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_Boundary_Maxoneless_20KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{2E9DB5DB-3CA0-4290-BE7A-A013BDD59682}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_Boundary_Maxoneless_20KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_Boundary_Maxonemore_20KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{7742E971-3715-415C-885C-651C1DF4F4ED}</Property>
				<Property Name="App_INI_GUID" Type="Str">{DA34CA22-9226-4532-A42D-80CCD5ED8557}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{A2F669E8-D014-4095-9A16-5FC89C31254E}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_Boundary_Maxonemore_20KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_Boundary_Maxonemore_20KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{A1FE292B-2145-468C-9B4E-AC487172F491}</Property>
				<Property Name="Bld_version.build" Type="Int">3</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_Boundary_Maxonemore_20KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_Boundary_Maxonemore_20KHZ/TestMode_Boundary_Maxonemore_20KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_Boundary_Maxonemore_20KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{78AE97D9-F453-492C-A54E-198FC388CB5B}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_Boundary_Maxonemore_20KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_Boundary_Maxonemore_20KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_Boundary_Maxonemore_20KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_Boundary_Maxonemore_20KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{2099B8B0-9F4F-405D-853B-68E84499EEFF}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_Boundary_Maxonemore_20KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_Boundary_Minoneless_20KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{649554F5-013B-456D-9CD9-CFE3CD9AE253}</Property>
				<Property Name="App_INI_GUID" Type="Str">{37869B94-CD34-402C-A2D0-623EC38B92CD}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{7CD73770-FAD3-476B-91A5-5EB3ED4F3219}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_Boundary_Minoneless_20KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_Boundary_Minoneless_20KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{CCA90C6E-543F-49D7-9917-0929FA6CA819}</Property>
				<Property Name="Bld_version.build" Type="Int">3</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_Boundary_Minoneless_20KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_Boundary_Minoneless_20KHZ/TestMode_Boundary_Minoneless_20KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_Boundary_Minoneless_20KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{78AE97D9-F453-492C-A54E-198FC388CB5B}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_Boundary_Minoneless_20KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_Boundary_Minoneless_20KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_Boundary_Minoneless_20KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_Boundary_Minoneless_20KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{284E71DF-D5F0-49D9-9CE3-2C9C20CA5DB5}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_Boundary_Minoneless_20KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_Boundary_Minonemore_20KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{599EE355-CA64-44B5-91DA-90E4DC0D8AD2}</Property>
				<Property Name="App_INI_GUID" Type="Str">{434CA123-35E9-476C-BBE0-0738C54CC401}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{4C0FBDF2-965F-465D-A731-744D2734F626}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_Boundary_Minonemore_20KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_Boundary_Minonemore_20KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{C23C3934-FF0E-4ADB-8984-9213CE800D4A}</Property>
				<Property Name="Bld_version.build" Type="Int">3</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_Boundary_Minonemore_20KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_Boundary_Minonemore_20KHZ/TestMode_Boundary_Minonemore_20KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_Boundary_Minonemore_20KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{78AE97D9-F453-492C-A54E-198FC388CB5B}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_Boundary_Minonemore_20KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_Boundary_Minonemore_20KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_Boundary_Minonemore_20KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_Boundary_Minonemore_20KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{99D4B1E6-237E-4B9F-BEA9-660DD5F64AF7}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_Boundary_Minonemore_20KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MaxAmpLpC1_20KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{48C227FA-60BC-4A06-BE77-D54FEBB9D69B}</Property>
				<Property Name="App_INI_GUID" Type="Str">{49D56700-324E-4CF6-AEA4-A76C67186B42}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{C538568E-5B8C-441C-B68D-149C85ECFE20}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MaxAmpLpC1_20KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MaxAmpLpC1_20KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{F20F3004-2288-42FE-867A-AE38A1467E8E}</Property>
				<Property Name="Bld_version.build" Type="Int">3</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MaxAmpLpC1_20KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MaxAmpLpC1_20KHZ/TestMode_MaxAmpLpC1_20KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MaxAmpLpC1_20KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{78AE97D9-F453-492C-A54E-198FC388CB5B}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MaxAmpLpC1_20KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MaxAmpLpC1_20KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MaxAmpLpC1_20KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MaxAmpLpC1_20KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{6E1E8A71-5984-4E92-B7B7-F153494D0607}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MaxAmpLpC1_20KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MaxAmpLpC2_20KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{4A51BE32-570B-47C2-B6B2-0AFE7BD82F3F}</Property>
				<Property Name="App_INI_GUID" Type="Str">{3E68A7CA-0961-43C1-A93A-9FA383C21637}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{38F80E37-B991-4F8A-B52D-CFAE488FEA2E}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MaxAmpLpC2_20KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MaxAmpLpC2_20KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{ED585376-77A4-4FB5-9E2A-1422289BF8B6}</Property>
				<Property Name="Bld_version.build" Type="Int">3</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MaxAmpLpC2_20KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MaxAmpLpC2_20KHZ/TestMode_MaxAmpLpC2_20KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MaxAmpLpC2_20KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{78AE97D9-F453-492C-A54E-198FC388CB5B}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MaxAmpLpC2_20KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MaxAmpLpC2_20KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MaxAmpLpC2_20KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MaxAmpLpC2_20KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{93B9FC1A-68F8-43DD-9A8F-67C9A2E0823F}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MaxAmpLpC2_20KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MaxPhLp_20KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{91BE7EB2-5791-4594-BE06-F43254696184}</Property>
				<Property Name="App_INI_GUID" Type="Str">{E63E6D4A-2741-4F1A-9D7F-84EF00174607}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{890EE62B-CD87-4438-8DF2-FC3029D6279D}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MaxPhLp_20KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MaxPhLp_20KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{B8A306A0-4BD6-4419-A19E-8E3AD2A40901}</Property>
				<Property Name="Bld_version.build" Type="Int">3</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MaxPhLp_20KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MaxPhLp_20KHZ/TestMode_MaxPhLp_20KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MaxPhLp_20KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{78AE97D9-F453-492C-A54E-198FC388CB5B}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MaxPhLp_20KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MaxPhLp_20KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MaxPhLp_20KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MaxPhLp_20KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{79FE9D5D-B087-4119-9D0E-6121EE26E8DB}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MaxPhLp_20KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MaxPhLpCF_20KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{718AEC40-8594-46F5-98E2-FDBFA4987923}</Property>
				<Property Name="App_INI_GUID" Type="Str">{AB8A2C83-77C7-4FE8-8BC9-4C014B5C672A}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{C97EA37A-CF3A-4EFD-BBB7-666E529E9709}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MaxPhLpCF_20KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MaxPhLpCF_20KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{1CF4D099-AB4F-45AC-81B3-FFA836884107}</Property>
				<Property Name="Bld_version.build" Type="Int">3</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MaxPhLpCF_20KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MaxPhLpCF_20KHZ/TestMode_MaxPhLpCF_20KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MaxPhLpCF_20KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{78AE97D9-F453-492C-A54E-198FC388CB5B}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MaxPhLpCF_20KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MaxPhLpCF_20KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MaxPhLpCF_20KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MaxPhLpCF_20KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{F0289612-64CC-4F17-B745-8357D054DD2A}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MaxPhLpCF_20KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MaxRecipe_20KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{B8412D04-D5AD-4607-B3FC-7584E34F5E5F}</Property>
				<Property Name="App_INI_GUID" Type="Str">{93886A59-74EA-45F5-8BD9-730311F7E84B}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{52412EA6-E02E-4852-BECE-1426B36F5970}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MaxRecipe_20KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MaxRecipe_20KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{F54A8C7F-D5F0-4E46-B86D-A4A28C7A1C73}</Property>
				<Property Name="Bld_version.build" Type="Int">3</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MaxRecipe_20KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MaxRecipe_20KHZ/TestMode_MaxRecipe_20KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MaxRecipe_20KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{78AE97D9-F453-492C-A54E-198FC388CB5B}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MaxRecipe_20KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MaxRecipe_20KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MaxRecipe_20KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MaxRecipe_20KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{BB9DD9AE-FA29-416C-BDE4-ED5520DC6FF2}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MaxRecipe_20KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MinAmpLpC1_20KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{10A648FC-BDA8-4EEE-8B83-3388A34480EF}</Property>
				<Property Name="App_INI_GUID" Type="Str">{CC762444-212F-4013-8C6B-16B355D1C92E}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{20A6F8BB-1EFA-4A5B-A0AA-42FA9BC3BDFD}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MinAmpLpC1_20KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MinAmpLpC1_20KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{BC180968-5710-416F-8A81-AECC1FB60BB7}</Property>
				<Property Name="Bld_version.build" Type="Int">3</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MinAmpLpC1_20KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MinAmpLpC1_20KHZ/TestMode_MinAmpLpC1_20KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MinAmpLpC1_20KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{78AE97D9-F453-492C-A54E-198FC388CB5B}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MinAmpLpC1_20KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MinAmpLpC1_20KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MinAmpLpC1_20KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MinAmpLpC1_20KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{F7069163-9D13-4750-80F7-6EE5D3B94207}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MinAmpLpC1_20KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MinAmpLpC2_20KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{E839EDE6-6E46-4786-9F20-478E0D741349}</Property>
				<Property Name="App_INI_GUID" Type="Str">{8D64DE83-BC41-407E-A514-CBAA953EFC55}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{25A65C13-738F-4E7C-A8B4-A8587CB1288D}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MinAmpLpC2_20KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MinAmpLpC2_20KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{C2416465-6008-4C9B-A71D-B9C74C7183A0}</Property>
				<Property Name="Bld_version.build" Type="Int">3</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MinAmpLpC2_20KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MinAmpLpC2_20KHZ/TestMode_MinAmpLpC2_20KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MinAmpLpC2_20KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{78AE97D9-F453-492C-A54E-198FC388CB5B}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MinAmpLpC2_20KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MinAmpLpC2_20KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MinAmpLpC2_20KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MinAmpLpC2_20KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{75ADFCEA-DBD0-4C86-892B-59C9A562204C}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MinAmpLpC2_20KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MinPhLp_20KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{42F1219C-043D-4F07-A85D-89369A4F71D3}</Property>
				<Property Name="App_INI_GUID" Type="Str">{D8AEAD6D-EAB9-40DB-83E0-F90B1A43D37F}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{25977C60-8AE9-4771-8719-1EB2AB01AF14}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MinPhLp_20KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MinPhLp_20KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{CC2DDE1F-86BD-4CD1-BBC0-EBCFE11CA0B7}</Property>
				<Property Name="Bld_version.build" Type="Int">3</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MinPhLp_20KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MinPhLp_20KHZ/TestMode_MinPhLp_20KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MinPhLp_20KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{78AE97D9-F453-492C-A54E-198FC388CB5B}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MinPhLp_20KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MinPhLp_20KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MinPhLp_20KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MinPhLp_20KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{0A1BF065-4283-497E-B67D-47D8B9A70FA6}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MinPhLp_20KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MinPhLpCF_20KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{D0818293-CFC2-45A9-B9A3-946011C26C98}</Property>
				<Property Name="App_INI_GUID" Type="Str">{EE8B03FE-E18C-43A1-9B4B-23E37223A4DE}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{C19BA9C5-B7DE-4FD3-83BE-D5B27AFA4C46}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MinPhLpCF_20KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MinPhLpCF_20KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{ACF34224-E9CF-49EB-9E57-AF481FC8C7F1}</Property>
				<Property Name="Bld_version.build" Type="Int">3</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MinPhLpCF_20KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MinPhLpCF_20KHZ/TestMode_MinPhLpCF_20KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MinPhLpCF_20KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{78AE97D9-F453-492C-A54E-198FC388CB5B}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MinPhLpCF_20KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MinPhLpCF_20KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MinPhLpCF_20KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MinPhLpCF_20KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{68A966D3-AF1F-4B96-A9AB-B5BBC35A07A9}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MinPhLpCF_20KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_MinRecipe_20KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{20F7419F-20A6-4AA8-8497-D05A712F0902}</Property>
				<Property Name="App_INI_GUID" Type="Str">{C73E2050-1BDB-4A60-AB47-26891F54C050}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{EB39089D-4BBF-4D8E-ADB8-4205E3970146}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_MinRecipe_20KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MinRecipe_20KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{8D3733B0-13B8-457D-AE30-9B0BBA17FAA3}</Property>
				<Property Name="Bld_version.build" Type="Int">3</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_MinRecipe_20KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MinRecipe_20KHZ/TestMode_MinRecipe_20KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_MinRecipe_20KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{78AE97D9-F453-492C-A54E-198FC388CB5B}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_MinRecipe_20KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_MinRecipe_20KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_MinRecipe_20KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_MinRecipe_20KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{1CD6E143-9158-4B4B-9C0C-B67DC71610A8}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_MinRecipe_20KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_Rcp50_20KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{7DB9A386-2CC1-468D-B9B4-FAA7A15CEF11}</Property>
				<Property Name="App_INI_GUID" Type="Str">{EDCD76B9-1CE1-4DA8-AA79-BE1C72364FE0}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{4A17E7DF-E1BD-44A7-A895-97FA4139C256}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_Rcp50_20KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_Rcp50_20KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{E5BB3C6E-A0B4-48A4-8198-59889B681D3F}</Property>
				<Property Name="Bld_version.build" Type="Int">3</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_Rcp50_20KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_Rcp50_20KHZ/TestMode_Rcp50_20KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_Rcp50_20KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{78AE97D9-F453-492C-A54E-198FC388CB5B}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_Rcp50_20KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_Rcp50_20KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_Rcp50_20KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_Rcp50_20KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{2F248718-5800-40D8-9A67-427F9CE66EF0}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_Rcp50_20KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_RcpBelow50_20KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{BE96874E-94EE-4F72-A535-04B2CC5FC190}</Property>
				<Property Name="App_INI_GUID" Type="Str">{5DF9DE2C-9800-4B39-9B13-666A6FF888FB}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{99229CBA-B212-4C7D-A8F2-EDD9C54E616E}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_RcpBelow50_20KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_RcpBelow50_20KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{44053D24-C034-4C38-A0DC-043DE2EDF292}</Property>
				<Property Name="Bld_version.build" Type="Int">3</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestMode_RcpBelow50_20KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_RcpBelow50_20KHZ/TestMode_RcpBelow50_20KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_TestMode_20KHZ_AUTO/TestMode_RcpBelow50_20KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{78AE97D9-F453-492C-A54E-198FC388CB5B}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TestMode_RcpBelow50_20KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_RcpBelow50_20KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_RcpBelow50_20KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_RcpBelow50_20KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{9461213F-72AE-43CE-9431-86626B670859}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestMode_RcpBelow50_20KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
		</Item>
	</Item>
</Project>
