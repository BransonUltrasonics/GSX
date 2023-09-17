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
		<Item Name="BF_INT_HornScan_40KHZ_AUTO" Type="Folder" URL="../../BF_INT_HornScan_40KHZ_AUTO">
			<Property Name="NI.DISK" Type="Bool">true</Property>
		</Item>
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
				<Item Name="Empty Picture" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Empty Picture"/>
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
			<Item Name="Create_Error_String_Sub_VI.vi" Type="VI" URL="../../Generic_Common/Create_Error_String_Sub_VI.vi"/>
			<Item Name="Global_Read_INI_Sub_VI.vi" Type="VI" URL="../../Generic_Common/Global_Read_INI_Sub_VI.vi"/>
			<Item Name="HORN_SCAN_Common_Socket_Cmd_Sub_VI.vi" Type="VI" URL="../../Generic_Common/HornScan/HORN_SCAN_Common_Socket_Cmd_Sub_VI.vi"/>
			<Item Name="HornReceipe_Local_Read_INI_Sub_VI.vi" Type="VI" URL="../../Generic_Common/HornScan/HornReceipe_Local_Read_INI_Sub_VI.vi"/>
			<Item Name="HornScan_Recipe_Sub_Vi.vi" Type="VI" URL="../../Generic_Common/HornScan/HornScan_Recipe_Sub_Vi.vi"/>
			<Item Name="HornScan_Recipe_with_HornScan_SUB_VI.vi" Type="VI" URL="../../Generic_Common/HornScan/HornScan_Recipe_with_HornScan_SUB_VI.vi"/>
			<Item Name="HornScan_SUB_VI.vi" Type="VI" URL="../../Generic_Common/HornScan/HornScan_SUB_VI.vi"/>
			<Item Name="HornScanAbort_Recipe_with_HornScan_SUB_VI.vi" Type="VI" URL="../../Generic_Common/HornScan/HornScanAbort_Recipe_with_HornScan_SUB_VI.vi"/>
			<Item Name="kernel32.dll" Type="Document" URL="kernel32.dll">
				<Property Name="NI.PreserveRelativePath" Type="Bool">true</Property>
			</Item>
			<Item Name="Local_Read_INI_Sub_VI.vi" Type="VI" URL="../../Generic_Common/Local_Read_INI_Sub_VI.vi"/>
			<Item Name="Log_Msg_Sub_VI.vi" Type="VI" URL="../../Generic_Common/Log_Msg_Sub_VI.vi"/>
			<Item Name="Query_ResonantFrequency_Sub_VI.vi" Type="VI" URL="../../Generic_Common/HornScan/Query_ResonantFrequency_Sub_VI.vi"/>
			<Item Name="Write_Html_Report_Sub_VI.vi" Type="VI" URL="../../Generic_Common/Write_Html_Report_Sub_VI.vi"/>
		</Item>
		<Item Name="Build Specifications" Type="Build">
			<Item Name="Default_HornScan_40KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{9239C7AE-5C03-4409-9CED-ED1B7DBAA480}</Property>
				<Property Name="App_INI_GUID" Type="Str">{F192C493-E533-4BA7-A6FD-B3E4353DC0A5}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{F53A218C-493A-41F6-9663-583703E79588}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Default_HornScan_40KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/Default_HornScan_40KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{81F14277-D6C6-40C0-9079-DE660E229D0D}</Property>
				<Property Name="Bld_version.build" Type="Int">9</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Default_HornScan_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/Default_HornScan_40KHZ_Test/Default_HornScan_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/Default_HornScan_40KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{6C69D59E-0D2A-4823-BFBB-18DD417CB9C6}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_HornScan_40KHZ_AUTO/Default_HornScan_40KHZ_Test/Default_HornScan_40KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Default_HornScan_40KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">Default_HornScan_40KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">Default_HornScan_40KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{0E400FD5-84E0-49A4-B733-E30DE0C9BD00}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Default_HornScan_40KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="HornScan_Above50_40KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{EA8A7E1B-05A2-423E-A31D-B611DE593680}</Property>
				<Property Name="App_INI_GUID" Type="Str">{E0006A0B-5240-410A-AF87-A840D6260DD8}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{E16D4A7B-F796-466D-BE40-A932FE0179E0}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">HornScan_Above50_40KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_Above50_40KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{B6D58135-0F55-42F3-A770-EFE2740CC065}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">HornScan_Above50_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_Above50_40KHZ_Test/HornScan_Above50_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_Above50_40KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{6C69D59E-0D2A-4823-BFBB-18DD417CB9C6}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_HornScan_40KHZ_AUTO/HornScan_Above50_40KHZ_Test/HornScan_Above50_40KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">HornScan_Above50_40KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">HornScan_Above50_40KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">HornScan_Above50_40KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{50C3BDDE-BF54-4EDB-94A8-BAFD1A3C5F77}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">HornScan_Above50_40KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="HornScan_AboveMax_40KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{377E3496-60B5-424B-9107-291F0A8A4B0F}</Property>
				<Property Name="App_INI_GUID" Type="Str">{CD50D51D-D5DD-460B-BBD2-69C923C772C2}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{F176EDFF-04E8-4B3F-8444-B4533B8CDB5E}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">HornScan_AboveMax_40KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_AboveMax_40KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{505F2A14-3056-4183-816F-D59A5D080B53}</Property>
				<Property Name="Bld_version.build" Type="Int">5</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">HornScan_AboveMax_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_AboveMax_40KHZ_Test/HornScan_AboveMax_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_AboveMax_40KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{137EBD65-6069-40E7-BEE3-EC590B9370D1}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_HornScan_40KHZ_AUTO/HornScan_AboveMax_40KHZ_Test/HornScan_AboveMax_40KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">HornScan_AboveMax_40KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">HornScan_AboveMax_40KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">HornScan_AboveMax_40KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{49B1B489-CCA9-4604-BCE6-8BD1191C1F9F}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">HornScan_AboveMax_40KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="HornScan_AboveMin_40KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{570C90F4-5ACD-4436-8B52-C42C697F9FE0}</Property>
				<Property Name="App_INI_GUID" Type="Str">{ADAFCCAC-E5D0-4F10-BDDD-D8831F5D4AAE}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{6E0DF912-BDB7-4620-AD92-51E127648DDC}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">HornScan_AboveMin_40KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_AboveMin_40KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{CC8A7BFB-848A-40E9-AEBC-E81CE0E01F49}</Property>
				<Property Name="Bld_version.build" Type="Int">4</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">HornScan_AboveMin_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_AboveMin_40KHZ_Test/HornScan_AboveMin_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_AboveMin_40KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{137EBD65-6069-40E7-BEE3-EC590B9370D1}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_HornScan_40KHZ_AUTO/HornScan_AboveMin_40KHZ_Test/HornScan_AboveMin_40KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">HornScan_AboveMin_40KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">HornScan_AboveMin_40KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">HornScan_AboveMin_40KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{75BBBEDA-0D01-4CC3-B6E5-27D2E68F530E}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">HornScan_AboveMin_40KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="HornScan_Below10_40KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{7610D429-6E94-4BEC-A3AA-CFBE7FA95012}</Property>
				<Property Name="App_INI_GUID" Type="Str">{94FAFD3A-3F82-4E41-96C0-1891A8DEE0B0}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{6629C712-5218-4FE4-A7D4-7DAF28A3AEDD}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">HornScan_Below10_40KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_Below10_40KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{25D60455-0A56-4A65-8B3D-BA28A797C7EA}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">HornScan_Below10_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_Below10_40KHZ_Test/HornScan_Below10_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_Below10_40KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{6C69D59E-0D2A-4823-BFBB-18DD417CB9C6}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_HornScan_40KHZ_AUTO/HornScan_Below10_40KHZ_Test/HornScan_Below10_40KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">HornScan_Below10_40KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">HornScan_Below10_40KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">HornScan_Below10_40KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{04A347A8-51CF-44CD-8D01-86A289DA32FC}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">HornScan_Below10_40KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="HornScan_Below50_40KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{FCFDB144-BBB9-458F-94B8-F78DE4F24351}</Property>
				<Property Name="App_INI_GUID" Type="Str">{A83E0A85-2B0B-45A8-A16A-2132CAEE1C11}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{12F21FA3-D76E-4BD3-98B3-DF31B469FE17}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">HornScan_Below50_40KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_Below50_40KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{24E935FF-56D0-4FA6-A7EF-DCB29C3DCC42}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">HornScan_Below50_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_Below50_40KHZ_Test/HornScan_Below50_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_Below50_40KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{6C69D59E-0D2A-4823-BFBB-18DD417CB9C6}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_HornScan_40KHZ_AUTO/HornScan_Below50_40KHZ_Test/HornScan_Below50_40KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">HornScan_Below50_40KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">HornScan_Below50_40KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">HornScan_Below50_40KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{9745CF3E-DDC7-42D3-B464-4AF3A3C8B41C}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">HornScan_Below50_40KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="HornScan_BelowMax_40KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{EAEE625A-4150-4595-A579-1887006192F4}</Property>
				<Property Name="App_INI_GUID" Type="Str">{1B919DA3-D0AB-4F8E-9A35-15293F62E12F}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{CECB6803-01A9-448B-A452-E49EE2DF4E63}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">HornScan_BelowMax_40KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_BelowMax_40KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{9277CFCB-5672-4298-9AE3-E2FD553283CE}</Property>
				<Property Name="Bld_version.build" Type="Int">4</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">HornScan_BelowMax_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_BelowMax_40KHZ_Test/HornScan_BelowMax_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_BelowMax_40KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{137EBD65-6069-40E7-BEE3-EC590B9370D1}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_HornScan_40KHZ_AUTO/HornScan_BelowMax_40KHZ_Test/HornScan_BelowMax_40KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">HornScan_BelowMax_40KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">HornScan_BelowMax_40KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">HornScan_BelowMax_40KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{13E2DB4C-007C-4FC6-B5BA-5DF35A97D9CD}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">HornScan_BelowMax_40KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="HornScan_BelowMin_40KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{D396022D-3E7D-4A4C-B8E7-F0A8205D1565}</Property>
				<Property Name="App_INI_GUID" Type="Str">{3EB337D3-7C90-4E56-A602-3689D7FF3C2F}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{DC6FA10C-00BB-4564-9D89-1737A3219181}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">HornScan_BelowMin_40KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_BelowMin_40KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{E16D2967-9CF3-431C-A3DC-3DF75CC239C0}</Property>
				<Property Name="Bld_version.build" Type="Int">4</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">HornScan_BelowMin_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_BelowMin_40KHZ_Test/HornScan_BelowMin_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_BelowMin_40KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{137EBD65-6069-40E7-BEE3-EC590B9370D1}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_HornScan_40KHZ_AUTO/HornScan_BelowMin_40KHZ_Test/HornScan_BelowMin_40KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">HornScan_BelowMin_40KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">HornScan_BelowMin_40KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">HornScan_BelowMin_40KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{CDE27A37-1650-429C-91D4-252605BC8719}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">HornScan_BelowMin_40KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="HornScan_MaxFreq_40KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{AC61FEA4-F45B-4E67-BB9C-3E019018B91D}</Property>
				<Property Name="App_INI_GUID" Type="Str">{24457DB4-691B-4EE6-A32B-5A5506C770AC}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{C5F85F40-5BC7-43BB-B208-F050CCEA69B3}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">HornScan_MaxFreq_40KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_MaxFreq_40KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{2F9D732D-33C5-46CA-B647-18967D9E0C2A}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">HornScan_MaxFreq_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_MaxFreq_40KHZ_Test/HornScan_MaxFreq_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_MaxFreq_40KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{6C69D59E-0D2A-4823-BFBB-18DD417CB9C6}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_HornScan_40KHZ_AUTO/HornScan_MaxFreq_40KHZ_Test/HornScan_MaxFreq_40KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">HornScan_MaxFreq_40KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">HornScan_MaxFreq_40KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">HornScan_MaxFreq_40KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{91D5AE17-1ADD-427D-94E7-C7426854C815}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">HornScan_MaxFreq_40KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="HornScan_MaxMA_40KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{BF35928A-395A-4D3D-AD44-5DE7B10D698B}</Property>
				<Property Name="App_INI_GUID" Type="Str">{907698A4-340C-41A2-8CBF-785E136400DA}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{A19B797C-EE59-4E96-98CD-B6CA3865F531}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">HornScan_MaxMA_40KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_MaxMA_40KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{C8CE916C-3987-4FA2-8C73-A02FB1B37FB1}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">HornScan_MaxMA_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_MaxMA_40KHZ_Test/HornScan_MaxMA_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_MaxMA_40KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{6C69D59E-0D2A-4823-BFBB-18DD417CB9C6}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_HornScan_40KHZ_AUTO/HornScan_MaxMA_40KHZ_Test/HornScan_MaxMA_40KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">HornScan_MaxMA_40KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">HornScan_MaxMA_40KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">HornScan_MaxMA_40KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{B2FAB324-D03F-4FA3-9335-080FB719823C}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">HornScan_MaxMA_40KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="HornScan_MaxMC_40KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{B6C4CF91-22FB-4E5E-B63F-A9456BBD2845}</Property>
				<Property Name="App_INI_GUID" Type="Str">{5F0DADCE-11DD-44E5-A44D-05843600953D}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{1B04BAED-4C94-442F-B4CD-18B07BBF1AD6}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">HornScan_MaxMC_40KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_MaxMC_40KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{B914620D-01C7-4C6C-8D3B-B74C7FE142C3}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">HornScan_MaxMC_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_MaxMC_40KHZ_Test/HornScan_MaxMC_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_MaxMC_40KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{6C69D59E-0D2A-4823-BFBB-18DD417CB9C6}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_HornScan_40KHZ_AUTO/HornScan_MaxMC_40KHZ_Test/HornScan_MaxMC_40KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">HornScan_MaxMC_40KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">HornScan_MaxMC_40KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">HornScan_MaxMC_40KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{A72B531B-8B88-42CE-AC54-5E76C6E0227B}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">HornScan_MaxMC_40KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="HornScan_MaxRecipe_40KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{AF59F105-5383-492B-AE2B-D2AA1BEB7297}</Property>
				<Property Name="App_INI_GUID" Type="Str">{844999C4-888C-44D8-9BBA-82C9BB1C0A80}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{49BBABB5-258E-45CB-94DE-49DA6C3C2CB1}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">HornScan_MaxRecipe_40KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_MaxRecipe_40KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{32CFF68B-A8F5-4334-B494-0EC6D2A6E2E9}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">HornScan_MaxRecipe_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_MaxRecipe_40KHZ_Test/HornScan_MaxRecipe_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_MaxRecipe_40KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{6C69D59E-0D2A-4823-BFBB-18DD417CB9C6}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_HornScan_40KHZ_AUTO/HornScan_MaxRecipe_40KHZ_Test/HornScan_MaxRecipe_40KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">HornScan_MaxRecipe_40KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">HornScan_MaxRecipe_40KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">HornScan_MaxRecipe_40KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{E393C127-4692-49CC-9312-FA67D81097C5}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">HornScan_MaxRecipe_40KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="HornScan_MaxTD_40KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{E698DF46-DB9A-4844-B127-0833128BD159}</Property>
				<Property Name="App_INI_GUID" Type="Str">{8A6913AA-7CEC-4202-ABDC-CC26AAD08330}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{BEEA6C2E-45E3-4D11-B183-BE1321418391}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">HornScan_MaxTD_40KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_MaxTD_40KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{5A4C0712-2B6C-40C9-8F99-A377794CFDB5}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">HornScan_MaxTD_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_MaxTD_40KHZ_Test/HornScan_MaxTD_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_MaxTD_40KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{6C69D59E-0D2A-4823-BFBB-18DD417CB9C6}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_HornScan_40KHZ_AUTO/HornScan_MaxTD_40KHZ_Test/HornScan_MaxTD_40KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">HornScan_MaxTD_40KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">HornScan_MaxTD_40KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">HornScan_MaxTD_40KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{202EA16B-3CB3-4B50-8322-0C266FFA597B}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">HornScan_MaxTD_40KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="HornScan_MinFreq_TD_40KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{D65FC5D3-981E-426C-B220-7FCCDEF8C999}</Property>
				<Property Name="App_INI_GUID" Type="Str">{1ECCD686-34EB-411E-BA96-1C94B3C82056}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{B054ECFE-75EF-46AA-A6F6-6A580404D5C6}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">HornScan_MinFreq_TD_40KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_MinFreq_TD_40KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{835FCC2C-65BE-4E24-86CE-20E51628C577}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">HornScan_MinFreq_TD_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_MinFreq_TD_40KHZ_Test/HornScan_MinFreq_TD_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_MinFreq_TD_40KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{6C69D59E-0D2A-4823-BFBB-18DD417CB9C6}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_HornScan_40KHZ_AUTO/HornScan_MinFreq_TD_40KHZ_Test/HornScan_MinFreq_TD_40KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">HornScan_MinFreq_TD_40KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">HornScan_MinFreq_TD_40KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">HornScan_MinFreq_TD_40KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{663B06CB-ED13-4E3A-8659-694489A73E75}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">HornScan_MinFreq_TD_40KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="HornScan_MinMA_40KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{B9A3E6EE-251A-46A0-A088-A594455978D4}</Property>
				<Property Name="App_INI_GUID" Type="Str">{BEBDA8A5-03DD-426E-AC62-F2974D27259A}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{E2EABFFC-B2B0-4847-8F90-8D7BE1A5F428}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">HornScan_MinMA_40KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_MinMA_40KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{BDB746A9-6756-4256-BDE8-963EABEED784}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">HornScan_MinMA_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_MinMA_40KHZ_Test/HornScan_MinMA_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_MinMA_40KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{6C69D59E-0D2A-4823-BFBB-18DD417CB9C6}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_HornScan_40KHZ_AUTO/HornScan_MinMA_40KHZ_Test/HornScan_MinMA_40KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">HornScan_MinMA_40KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">HornScan_MinMA_40KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">HornScan_MinMA_40KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{03B73391-8C2B-4C84-89AA-9405495888E0}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">HornScan_MinMA_40KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="HornScan_MinMC_40KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{45ECBC17-B76E-4274-A296-96B03F9C1EAB}</Property>
				<Property Name="App_INI_GUID" Type="Str">{9C0DACD0-3135-4410-BAE9-656186134C6C}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{E5A320FC-505F-40A3-A781-5C0E2B65862E}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">HornScan_MinMC_40KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_MinMC_40KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{F84F0E8E-FF7B-4703-9B40-68A877A827E1}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">HornScan_MinMC_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_MinMC_40KHZ_Test/HornScan_MinMC_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_MinMC_40KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{6C69D59E-0D2A-4823-BFBB-18DD417CB9C6}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_HornScan_40KHZ_AUTO/HornScan_MinMC_40KHZ_Test/HornScan_MinMC_40KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">HornScan_MinMC_40KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">HornScan_MinMC_40KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">HornScan_MinMC_40KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{495EBE48-E4BD-4A50-B3F1-C1C71588A445}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">HornScan_MinMC_40KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="HornScan_MinRecipe_40KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{737E0B25-1E02-4290-BA00-BAF62907EABD}</Property>
				<Property Name="App_INI_GUID" Type="Str">{05EA2CC8-4898-4683-918B-4EA9D8903B69}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{3A73D2FD-B900-47BD-95F0-C4B3E154C3E9}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">HornScan_MinRecipe_40KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_MinRecipe_40KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{4A336A3E-561B-4998-9593-C9C3BE5D2A4D}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">HornScan_MinRecipe_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_MinRecipe_40KHZ_Test/HornScan_MinRecipe_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_MinRecipe_40KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{6C69D59E-0D2A-4823-BFBB-18DD417CB9C6}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_HornScan_40KHZ_AUTO/HornScan_MinRecipe_40KHZ_Test/HornScan_MinRecipe_40KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">HornScan_MinRecipe_40KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">HornScan_MinRecipe_40KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">HornScan_MinRecipe_40KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{34819018-A15E-4917-B38E-5B505B929188}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">HornScan_MinRecipe_40KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="HornScan_Recipe50_40KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{7C7C163B-0FDF-4BCA-9236-2A21388EC0B1}</Property>
				<Property Name="App_INI_GUID" Type="Str">{F7D95745-6A5B-4802-89B3-10BAB485BBA0}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{6B05033D-8E5C-4728-87F6-F0107A971BEC}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">HornScan_Recipe50_40KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_Recipe50_40KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{BFF43263-6357-4053-B568-63C69FCA6E77}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">HornScan_Recipe50_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_Recipe50_40KHZ_Test/HornScan_Recipe50_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScan_Recipe50_40KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{6C69D59E-0D2A-4823-BFBB-18DD417CB9C6}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_HornScan_40KHZ_AUTO/HornScan_Recipe50_40KHZ_Test/HornScan_Recipe50_40KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">HornScan_Recipe50_40KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">HornScan_Recipe50_40KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">HornScan_Recipe50_40KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{777DDDE0-5B84-4B53-94C0-3E8CA8527A2B}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">HornScan_Recipe50_40KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="HornScanAbort_Above50_40KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{7E639A1A-9947-4BA7-9141-79321316E34D}</Property>
				<Property Name="App_INI_GUID" Type="Str">{3D3BE492-4D71-45CB-A078-5E45DF891B4B}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{736AADDA-9E92-475E-BAB5-AE2578F35A54}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">HornScanAbort_Above50_40KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScanAbort_Above50_40KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{DF3014B6-6831-4AD3-9D32-4829A4EBA1FD}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">HornScanAbort_Above50_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScanAbort_Above50_40KHZ_Test/HornScanAbort_Above50_40KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_HornScan_40KHZ_AUTO/HornScanAbort_Above50_40KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{4358A747-0687-4CB1-ADCC-BD04B21881A0}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_HornScan_40KHZ_AUTO/HornScanAbort_Above50_40KHZ_Test/HornScanAbort_Above50_40KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">HornScanAbort_Above50_40KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">HornScanAbort_Above50_40KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">HornScanAbort_Above50_40KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{1C484BC4-2021-460C-9F89-1CF58EF71044}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">HornScanAbort_Above50_40KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
		</Item>
	</Item>
</Project>
