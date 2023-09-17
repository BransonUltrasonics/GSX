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
		<Item Name="BF_INT_Seek_30KHZ_AUTO" Type="Folder" URL="../../BF_INT_Seek_30KHZ_AUTO">
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
			<Item Name="kernel32.dll" Type="Document" URL="kernel32.dll">
				<Property Name="NI.PreserveRelativePath" Type="Bool">true</Property>
			</Item>
			<Item Name="Local_Read_INI_Sub_VI.vi" Type="VI" URL="../../Generic_Common/Local_Read_INI_Sub_VI.vi"/>
			<Item Name="Log_Msg_Sub_VI.vi" Type="VI" URL="../../Generic_Common/Log_Msg_Sub_VI.vi"/>
			<Item Name="SEEK_Common_Socket_Cmd_Sub_VI.vi" Type="VI" URL="../../Generic_Common/SEEK/SEEK_Common_Socket_Cmd_Sub_VI.vi"/>
			<Item Name="Seek_Operation_SUB_VI.vi" Type="VI" URL="../../Generic_Common/SEEK/Seek_Operation_SUB_VI.vi"/>
			<Item Name="SEEK_Recipe_Range_Validation.vi" Type="VI" URL="../../Generic_Common/SEEK/SEEK_Recipe_Range_Validation.vi"/>
			<Item Name="SeekReceipe_Local_Read_INI_Sub_VI.vi" Type="VI" URL="../../Generic_Common/SEEK/SeekReceipe_Local_Read_INI_Sub_VI.vi"/>
			<Item Name="Sending_Seek_Recipe_Sub_VI.vi" Type="VI" URL="../../Generic_Common/SEEK/Sending_Seek_Recipe_Sub_VI.vi"/>
			<Item Name="Sending_Seek_Recipe_With_Seek_Cmd_SUB_VI.vi" Type="VI" URL="../../Generic_Common/SEEK/Sending_Seek_Recipe_With_Seek_Cmd_SUB_VI.vi"/>
			<Item Name="Write_Html_Report_Sub_VI.vi" Type="VI" URL="../../Generic_Common/Write_Html_Report_Sub_VI.vi"/>
		</Item>
		<Item Name="Build Specifications" Type="Build">
			<Item Name="Default_Seek_30KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{BCFDE96A-D34C-4B7E-B60E-1617F038D913}</Property>
				<Property Name="App_INI_GUID" Type="Str">{8415CEBE-43E2-4B63-9667-9A63060C2455}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{FE39BAF2-7DF6-46A8-AAD1-FBA0430AE7A4}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Default_Seek_30KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Default_Seek_30KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{D6D47259-4DF7-400A-ADD7-F1E04BBDEB6C}</Property>
				<Property Name="Bld_version.build" Type="Int">10</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Default_Seek_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Default_Seek_30KHZ_Test/Default_Seek_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Default_Seek_30KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{3DE74979-0E52-4CCF-8B72-CF1AA4305563}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_Seek_30KHZ_AUTO/Default_Seek_30KHZ_Test/Default_Seek_30KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Default_Seek_30KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">Default_Seek_30KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">Default_Seek_30KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{84F3F81E-B5C2-4613-80C3-F979B57AF00E}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Default_Seek_30KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Seek_Above50_30KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{E3D380DA-4EB2-46EC-8124-82D50583915A}</Property>
				<Property Name="App_INI_GUID" Type="Str">{7C837D54-0442-4422-8F01-C0D2CFDDFCF1}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{0FE074E5-3BF2-4B8E-BFD4-0C944A2E997A}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Seek_Above50_30KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_Above50_30KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{EBB5E54A-62A2-4617-961E-B2855E84639F}</Property>
				<Property Name="Bld_version.build" Type="Int">11</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Seek_Above50_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_Above50_30KHZ_Test/Seek_Above50_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_Above50_30KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{51FEDD4E-366C-41E1-A0B6-AD7E7C4D2D80}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_Seek_30KHZ_AUTO/Seek_Above50_30KHZ_Test/Seek_Above50_30KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Seek_Above50_30KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">Seek_Above50_30KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">Seek_Above50_30KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{C49B6264-B057-4D33-84AA-17B6446E7CD4}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Seek_Above50_30KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Seek_Above100_30KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{9B9F6F3C-325E-47F7-8988-7FE75E7931FF}</Property>
				<Property Name="App_INI_GUID" Type="Str">{CCCE75AB-BACB-4CAE-87D8-8F530C6C1D63}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{CF2A34D9-4E61-49AE-BF56-9A422CE32566}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Seek_Above100_30KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_Above100_30KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{67E0E8B9-1673-4E87-AEE0-45B607B83FAA}</Property>
				<Property Name="Bld_version.build" Type="Int">10</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Seek_Above100_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_Above100_30KHZ_Test/Seek_Above100_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_Above100_30KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{51FEDD4E-366C-41E1-A0B6-AD7E7C4D2D80}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_Seek_30KHZ_AUTO/Seek_Above100_30KHZ_Test/Seek_Above100_30KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Seek_Above100_30KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">Seek_Above100_30KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">Seek_Above100_30KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{75613A79-4D7D-4858-AE3D-300E9AE4B0F3}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Seek_Above100_30KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Seek_Above500_30KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{B76A92E0-7B24-4560-85CA-4CD44AA2BC59}</Property>
				<Property Name="App_INI_GUID" Type="Str">{3901555A-7333-4DC9-B632-5E46F50E4892}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{687A653C-7BEC-4A05-B6DE-8E6A9451FDFC}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Seek_Above500_30KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_Above500_30KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{1687FC27-3178-4F90-8540-91929248ACF8}</Property>
				<Property Name="Bld_version.build" Type="Int">11</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Seek_Above500_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_Above500_30KHZ_Test/Seek_Above500_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_Above500_30KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{3DE74979-0E52-4CCF-8B72-CF1AA4305563}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_Seek_30KHZ_AUTO/Seek_Above500_30KHZ_Test/Seek_Above500_30KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Seek_Above500_30KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">Seek_Above500_30KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">Seek_Above500_30KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{0C6A21FE-F88B-429A-BD60-051E18CDA221}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Seek_Above500_30KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Seek_AboveMax_30KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{86C987AF-822C-4FD3-89A7-02CCE8844F2E}</Property>
				<Property Name="App_INI_GUID" Type="Str">{1D04635F-EDFA-4DF8-B545-CACE815E4B79}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{D5C5F451-8E16-4B76-B0DF-DE58C2E37338}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Seek_AboveMax_30KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_AboveMax_30KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{E1BB2C41-94BC-4420-9B1F-E2CB8F3E6672}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Seek_AboveMax_30KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_AboveMax_30KHZ/Seek_AboveMax_30KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_AboveMax_30KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{18DA79CF-77D3-467C-A5F7-E9DD0C1F0C6B}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_Seek_30KHZ_AUTO/Seek_AboveMax_30KHZ/Seek_AboveMax_30KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Seek_AboveMax_30KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Seek_AboveMax_30KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Seek_AboveMax_30KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{79485746-FCAC-44B6-822D-5EAFE3715852}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Seek_AboveMax_30KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Seek_AboveMin_30KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{1876BB37-5B3C-4BF2-B6A2-B8B9D54CD419}</Property>
				<Property Name="App_INI_GUID" Type="Str">{D067250C-8766-4C63-892A-0A799D051D7F}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{F0462E62-34CB-4AAE-AB29-BAF606660A56}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Seek_AboveMin_30KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_AboveMin_30KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{2938D4F6-8B9E-497D-951A-3C2A967F7414}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Seek_AboveMin_30KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_AboveMin_30KHZ/Seek_AboveMin_30KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_AboveMin_30KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{D4907649-70E6-4CBA-A86D-1B827BA22A9C}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_Seek_30KHZ_AUTO/Seek_AboveMin_30KHZ/Seek_AboveMin_30KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Seek_AboveMin_30KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Seek_AboveMin_30KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">Seek_AboveMin_30KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{5EB1AA00-44B0-4BE6-9C48-737A9729A517}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Seek_AboveMin_30KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Seek_BelowMax_30KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{319E9F7F-6C32-4CD6-8713-B59F5B13CC39}</Property>
				<Property Name="App_INI_GUID" Type="Str">{543B69C0-CBEF-42A7-835E-2C7A080F2A20}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{66AF51FD-3225-4C5F-9F9F-7195CBF9A8A7}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Seek_BelowMax_30KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_BelowMax_30KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{9193C979-0070-44DE-91D6-F34C16E712F0}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Seek_BelowMax_30KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_BelowMax_30KHZ/Seek_BelowMax_30KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_BelowMax_30KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{D4907649-70E6-4CBA-A86D-1B827BA22A9C}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_Seek_30KHZ_AUTO/Seek_BelowMax_30KHZ/Seek_BelowMax_30KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Seek_BelowMax_30KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Seek_BelowMax_30KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">Seek_BelowMax_30KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{A6FD0B66-8902-40C8-AC09-01F73E559885}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Seek_BelowMax_30KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Seek_BelowMin_30KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{583107C3-CDF3-4F46-AC07-205F8D93DC0F}</Property>
				<Property Name="App_INI_GUID" Type="Str">{43809DA7-CF56-4D04-95BA-13365C19B00C}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{721B7128-C440-4968-991D-8E1C1675EF85}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Seek_BelowMin_30KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_BelowMin_30KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{E2A6443C-4F21-4097-AD2E-06EC70FB55B4}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Seek_BelowMin_30KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_BelowMin_30KHZ/Seek_BelowMin_30KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_BelowMin_30KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{D4907649-70E6-4CBA-A86D-1B827BA22A9C}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_Seek_30KHZ_AUTO/Seek_BelowMin_30KHZ/Seek_BelowMin_30KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Seek_BelowMin_30KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Seek_BelowMin_30KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">Seek_BelowMin_30KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{1F40DAB7-7BE3-4FD6-A83F-D61476F818A6}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Seek_BelowMin_30KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Seek_MaxAmpLpC1_30KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{40C1AA37-9B3D-44C0-95BB-7D9AB7301513}</Property>
				<Property Name="App_INI_GUID" Type="Str">{F453F726-1EE0-497B-AF99-9D1377495C8F}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{0C1F6C49-C2F6-4F11-8C31-7C2C7B7A339F}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Seek_MaxAmpLpC1_30KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MaxAmpLpC1_30KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{09CBFD4C-60B2-409D-AEB1-8192CA4A9A9E}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Seek_MaxAmpLpC1_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MaxAmpLpC1_30KHZ_Test/Seek_MaxAmpLpC1_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MaxAmpLpC1_30KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{51FEDD4E-366C-41E1-A0B6-AD7E7C4D2D80}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_Seek_30KHZ_AUTO/Seek_MaxAmpLpC1_30KHZ_Test/Seek_MaxAmpLpC1_30KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Seek_MaxAmpLpC1_30KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">Seek_MaxAmpLpC1_30KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">Seek_MaxAmpLpC1_30KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{F0F8D991-2511-4D23-8CD8-5A8CDFD8A4CA}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Seek_MaxAmpLpC1_30KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Seek_MaxAmpLpC2_30KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{A9B99D7A-C88F-4954-A6ED-1B26FB9AFD69}</Property>
				<Property Name="App_INI_GUID" Type="Str">{57A68C1A-D5D8-47B0-8953-86F0A34657C1}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{44D50FDF-D6EA-45B3-87C3-648DF2731FDA}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Seek_MaxAmpLpC2_30KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MaxAmpLpC2_30KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{F3DBD070-7CC8-4523-9DC1-FDBAE7A8772A}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Seek_MaxAmpLpC2_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MaxAmpLpC2_30KHZ_Test/Seek_MaxAmpLpC2_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MaxAmpLpC2_30KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{51FEDD4E-366C-41E1-A0B6-AD7E7C4D2D80}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_Seek_30KHZ_AUTO/Seek_MaxAmpLpC2_30KHZ_Test/Seek_MaxAmpLpC2_30KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Seek_MaxAmpLpC2_30KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">Seek_MaxAmpLpC2_30KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">Seek_MaxAmpLpC2_30KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{A9938DBA-5C62-40DB-9D70-0F979F40F6DA}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Seek_MaxAmpLpC2_30KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Seek_MaxFreqOffset_30KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{D40223B0-14E5-456D-89A6-D02183D351EC}</Property>
				<Property Name="App_INI_GUID" Type="Str">{134F6BFB-36A4-4BF7-B75F-07242C6B65EA}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{097E69FF-7B8B-4196-BBCC-B5A4710B5817}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Seek_MaxFreqOffset_30KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MaxFreqOffset_30KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{DCFF3328-2EED-4483-B766-0F2FE494B11D}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Seek_MaxFreqOffset_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MaxFreqOffset_30KHZ_Test/Seek_MaxFreqOffset_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MaxFreqOffset_30KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{51FEDD4E-366C-41E1-A0B6-AD7E7C4D2D80}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_Seek_30KHZ_AUTO/Seek_MaxFreqOffset_30KHZ_Test/Seek_MaxFreqOffset_30KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Seek_MaxFreqOffset_30KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">Seek_MaxFreqOffset_30KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">Seek_MaxFreqOffset_30KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{56639B0A-C6A0-486B-9C59-2EFDD66CCF43}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Seek_MaxFreqOffset_30KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Seek_MaxPhLp_30KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{D93C856D-ABC4-4902-991A-B2C05FACA302}</Property>
				<Property Name="App_INI_GUID" Type="Str">{88F3F9E0-B5EE-4CC7-A55A-5FFB2C110126}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{E4947AE9-554A-4F50-8939-E627577473F8}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Seek_MaxPhLp_30KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MaxPhLp_30KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{81108FB7-CD98-46BE-94E4-869BCD90B907}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Seek_MaxPhLp_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MaxPhLp_30KHZ_Test/Seek_MaxPhLp_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MaxPhLp_30KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{51FEDD4E-366C-41E1-A0B6-AD7E7C4D2D80}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_Seek_30KHZ_AUTO/Seek_MaxPhLp_30KHZ_Test/Seek_MaxPhLp_30KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Seek_MaxPhLp_30KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">Seek_MaxPhLp_30KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">Seek_MaxPhLp_30KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{85451AD3-465F-410C-AD3A-ED92766FC9BD}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Seek_MaxPhLp_30KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Seek_MaxPhLpCF_30KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{24FACD3C-F734-4EC7-A5A0-BF87449B3787}</Property>
				<Property Name="App_INI_GUID" Type="Str">{33E6F7F2-ACB2-470C-AD2B-C4B8AE589D1A}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{2BA6AED8-B4D4-4C05-87A4-77DAA0BA6C57}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Seek_MaxPhLpCF_30KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MaxPhLpCF_30KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{A1A42990-320D-4C33-8B7D-D7A8684CCA52}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Seek_MaxPhLpCF_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MaxPhLpCF_30KHZ_Test/Seek_MaxPhLpCF_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MaxPhLpCF_30KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{51FEDD4E-366C-41E1-A0B6-AD7E7C4D2D80}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_Seek_30KHZ_AUTO/Seek_MaxPhLpCF_30KHZ_Test/Seek_MaxPhLpCF_30KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Seek_MaxPhLpCF_30KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">Seek_MaxPhLpCF_30KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">Seek_MaxPhLpCF_30KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{617EFBD6-231F-4966-89E9-2594A8549E0F}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Seek_MaxPhLpCF_30KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Seek_MaxRecipe_30KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{CFFA047A-C30A-41A3-ADE8-95E73C443607}</Property>
				<Property Name="App_INI_GUID" Type="Str">{3CE96299-ADB3-4307-BDB9-F0F932A70B61}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{3BE8544F-0137-40A9-A3C1-FBDB6E265C09}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Seek_MaxRecipe_30KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MaxRecipe_30KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{0F9EF9A5-43E0-4ABF-88B7-574259D725C1}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Seek_MaxRecipe_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MaxRecipe_30KHZ_Test/Seek_MaxRecipe_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MaxRecipe_30KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{51FEDD4E-366C-41E1-A0B6-AD7E7C4D2D80}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_Seek_30KHZ_AUTO/Seek_MaxRecipe_30KHZ_Test/Seek_MaxRecipe_30KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Seek_MaxRecipe_30KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">Seek_MaxRecipe_30KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">Seek_MaxRecipe_30KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{EF9ED5D3-6FFD-4178-85ED-08BF64F7BEEE}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Seek_MaxRecipe_30KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Seek_MaxRmpTime_30KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{134513B5-9D75-4907-830D-FE54D65D4E76}</Property>
				<Property Name="App_INI_GUID" Type="Str">{A45D589D-DC02-409A-B00A-E42B9694EF57}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{92D61D40-98EC-4456-B0D8-A1DCD9D3E6F0}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Seek_MaxRmpTime_30KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MaxRmpTime_30KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{AB298959-C159-41C5-B614-EDC24B4BB164}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Seek_MaxRmpTime_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MaxRmpTime_30KHZ_Test/Seek_MaxRmpTime_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MaxRmpTime_30KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{51FEDD4E-366C-41E1-A0B6-AD7E7C4D2D80}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_Seek_30KHZ_AUTO/Seek_MaxRmpTime_30KHZ_Test/Seek_MaxRmpTime_30KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Seek_MaxRmpTime_30KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">Seek_MaxRmpTime_30KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">Seek_MaxRmpTime_30KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{91486772-EC1C-465C-9BE1-B40F5C7752D6}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Seek_MaxRmpTime_30KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Seek_Min_30KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{BA615D51-4F43-4930-B771-F25C0C032CA3}</Property>
				<Property Name="App_INI_GUID" Type="Str">{FAF2F382-1429-4DC0-8182-5D54A74FF560}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{EB9F2481-588E-4DF9-A2B5-49D06A5EBA40}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Seek_Min_30KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_Min_30KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{4849D72D-F098-4EE5-BC7E-A514C6B5C801}</Property>
				<Property Name="Bld_version.build" Type="Int">2</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Seek_Min_30KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_Min_30KHZ/Seek_Min_30KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_Min_30KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{D4907649-70E6-4CBA-A86D-1B827BA22A9C}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_Seek_30KHZ_AUTO/Seek_Min_30KHZ/Seek_Min_30KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Seek_Min_30KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Seek_Min_30KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">Seek_Min_30KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{F4E6211E-1A6B-4ECA-9311-4A62EEDE64EB}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Seek_Min_30KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Seek_MinAmpLpC1_30KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{18A9FB3D-A0D6-4409-A13C-4A433CAD4419}</Property>
				<Property Name="App_INI_GUID" Type="Str">{9EF16C57-22F7-4A15-88F3-CA063D9C5C50}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{133D2CA9-D53C-4F9B-A548-37DB7C8CC023}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Seek_MinAmpLpC1_30KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MinAmpLpC1_30KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{8486BEE0-7AA6-4F84-9F57-2581C2E9732B}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Seek_MinAmpLpC1_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MinAmpLpC1_30KHZ_Test/Seek_MinAmpLpC1_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MinAmpLpC1_30KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{51FEDD4E-366C-41E1-A0B6-AD7E7C4D2D80}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_Seek_30KHZ_AUTO/Seek_MinAmpLpC1_30KHZ_Test/Seek_MinAmpLpC1_30KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Seek_MinAmpLpC1_30KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">Seek_MinAmpLpC1_30KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">Seek_MinAmpLpC1_30KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{9AAD5EE1-07F3-410B-A25F-39B3AEC62F07}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Seek_MinAmpLpC1_30KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Seek_MinAmpLpC2_30KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{F6E7B1E4-2588-49DB-93C3-3965426454EF}</Property>
				<Property Name="App_INI_GUID" Type="Str">{E988D3F8-3D30-46DD-B13C-DD3B6C47041D}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{C076964D-AFF8-432A-9D96-4CC646B4CE3C}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Seek_MinAmpLpC2_30KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MinAmpLpC2_30KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{4342C84F-74E7-49D1-9A5B-7409E54688BA}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Seek_MinAmpLpC2_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MinAmpLpC2_30KHZ_Test/Seek_MinAmpLpC2_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MinAmpLpC2_30KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{51FEDD4E-366C-41E1-A0B6-AD7E7C4D2D80}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_Seek_30KHZ_AUTO/Seek_MinAmpLpC2_30KHZ_Test/Seek_MinAmpLpC2_30KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Seek_MinAmpLpC2_30KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">Seek_MinAmpLpC2_30KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">Seek_MinAmpLpC2_30KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{240784EC-9A74-4CF4-BC4D-55342E77C49A}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Seek_MinAmpLpC2_30KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Seek_MinFreqOffset_30KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{F87194E2-C5A8-4FBB-935D-EF35646128F8}</Property>
				<Property Name="App_INI_GUID" Type="Str">{08A7EE74-C430-4992-A307-28BE73C738B4}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{56B99ABF-F890-4C39-96A7-A40F6F3FF292}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Seek_MinFreqOffset_30KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MinFreqOffset_30KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{4FB71B25-930B-4DC7-ACA1-18ADF5386ADF}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Seek_MinFreqOffset_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MinFreqOffset_30KHZ_Test/Seek_MinFreqOffset_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MinFreqOffset_30KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{51FEDD4E-366C-41E1-A0B6-AD7E7C4D2D80}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_Seek_30KHZ_AUTO/Seek_MinFreqOffset_30KHZ_Test/Seek_MinFreqOffset_30KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Seek_MinFreqOffset_30KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">Seek_MinFreqOffset_30KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">Seek_MinFreqOffset_30KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{A2A9BCB1-763B-4BA0-85F7-AA0DE64E35EC}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Seek_MinFreqOffset_30KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Seek_MinPhLp_30KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{845A2F4E-4364-4CE5-BCDA-23FC8D706CC4}</Property>
				<Property Name="App_INI_GUID" Type="Str">{A3569FA4-67DB-4573-99B3-67FB17D11A14}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{593143DC-9877-4A22-97BE-BC6B47B40EC5}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Seek_MinPhLp_30KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MinPhLp_30KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{BEBD7083-E10D-4593-BC66-95610D8AE320}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Seek_MinPhLp_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MinPhLp_30KHZ_Test/Seek_MinPhLp_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MinPhLp_30KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{51FEDD4E-366C-41E1-A0B6-AD7E7C4D2D80}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_Seek_30KHZ_AUTO/Seek_MinPhLp_30KHZ_Test/Seek_MinPhLp_30KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Seek_MinPhLp_30KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">Seek_MinPhLp_30KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">Seek_MinPhLp_30KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{BDEA3C07-E6B5-46BA-A5CA-CA35BAA92E12}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Seek_MinPhLp_30KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Seek_MinPhLpCF_30KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{FD3E573D-42A6-4221-9B65-75E197AC676F}</Property>
				<Property Name="App_INI_GUID" Type="Str">{D2BAE48C-24AC-4CED-9278-AE2DB87AA1FC}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{07B40232-C29F-4D23-A331-824252815A43}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Seek_MinPhLpCF_30KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MinPhLpCF_30KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{04D370D3-43DE-41DC-8462-679AB93CB40D}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Seek_MinPhLpCF_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MinPhLpCF_30KHZ_Test/Seek_MinPhLpCF_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MinPhLpCF_30KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{51FEDD4E-366C-41E1-A0B6-AD7E7C4D2D80}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_Seek_30KHZ_AUTO/Seek_MinPhLpCF_30KHZ_Test/Seek_MinPhLpCF_30KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Seek_MinPhLpCF_30KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">Seek_MinPhLpCF_30KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">Seek_MinPhLpCF_30KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{451DA529-0AE8-4D8B-8131-202D34820891}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Seek_MinPhLpCF_30KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Seek_MinRecipe_30KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{31C4ED22-50F6-486E-B951-1508304E689F}</Property>
				<Property Name="App_INI_GUID" Type="Str">{538FE8E4-15E6-4E75-B733-D76D80712284}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{2A0154D4-7A9A-4A4A-8227-0905757EABAF}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Seek_MinRecipe_30KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MinRecipe_30KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{0124099B-6C55-4157-AC65-3F12FE91CB4B}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Seek_MinRecipe_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MinRecipe_30KHZ_Test/Seek_MinRecipe_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MinRecipe_30KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{51FEDD4E-366C-41E1-A0B6-AD7E7C4D2D80}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_Seek_30KHZ_AUTO/Seek_MinRecipe_30KHZ_Test/Seek_MinRecipe_30KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Seek_MinRecipe_30KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">Seek_MinRecipe_30KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">Seek_MinRecipe_30KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{F95EB94A-D6D6-4B93-A92A-99CE9C914B4A}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Seek_MinRecipe_30KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Seek_MinRmpTime_30KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{58D5B743-D726-4C88-AC57-1C121D48424D}</Property>
				<Property Name="App_INI_GUID" Type="Str">{8D5C2DA1-D40E-428C-8C11-55B4D7B28C4E}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{04B942AB-3516-4D19-AEAB-9980CC28D4EB}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Seek_MinRmpTime_30KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MinRmpTime_30KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{06046CE5-C01B-469D-94D9-E79CEDA58863}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Seek_MinRmpTime_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MinRmpTime_30KHZ_Test/Seek_MinRmpTime_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MinRmpTime_30KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{51FEDD4E-366C-41E1-A0B6-AD7E7C4D2D80}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_Seek_30KHZ_AUTO/Seek_MinRmpTime_30KHZ_Test/Seek_MinRmpTime_30KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Seek_MinRmpTime_30KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">Seek_MinRmpTime_30KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">Seek_MinRmpTime_30KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{DE5CDC06-5DE1-458C-96C1-08A8EF131357}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Seek_MinRmpTime_30KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Seek_MinSeekTime_30KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{5EDC65DE-F0D5-4A64-978D-A1AE414DBBC0}</Property>
				<Property Name="App_INI_GUID" Type="Str">{FC1A7E7E-9F33-4461-A449-627885CF9C69}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{C1EC0D2E-B411-4574-842C-0A529451FFD2}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Seek_MinSeekTime_30KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MinSeekTime_30KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{8DF64C3E-14DC-469B-BB31-14A870871FD7}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Seek_MinSeekTime_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MinSeekTime_30KHZ_Test/Seek_MinSeekTime_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_MinSeekTime_30KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{51FEDD4E-366C-41E1-A0B6-AD7E7C4D2D80}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_Seek_30KHZ_AUTO/Seek_MinSeekTime_30KHZ_Test/Seek_MinSeekTime_30KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Seek_MinSeekTime_30KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">Seek_MinSeekTime_30KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">Seek_MinSeekTime_30KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{7B3B38B0-E270-4039-87BF-4D21B17FDF49}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Seek_MinSeekTime_30KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Seek_Rcp50_30KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{E4262614-864D-4064-93BB-33C465C61E22}</Property>
				<Property Name="App_INI_GUID" Type="Str">{4037206F-8710-4C14-B1B4-460643486AB0}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{34A874D6-8437-4314-840A-580AF5F1E5CC}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Seek_Rcp50_30KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_Rcp50_30KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{845CBF10-C4D4-4E66-B6D1-D688A1BD1762}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Seek_Rcp50_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_Rcp50_30KHZ_Test/Seek_Rcp50_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_Rcp50_30KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{51FEDD4E-366C-41E1-A0B6-AD7E7C4D2D80}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_Seek_30KHZ_AUTO/Seek_Rcp50_30KHZ_Test/Seek_Rcp50_30KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Seek_Rcp50_30KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">Seek_Rcp50_30KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">Seek_Rcp50_30KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{CE0DB042-DBF9-4C4A-BF47-AA7A3AD20DA7}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Seek_Rcp50_30KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Seek_RcpBelow50_30KHZ_Test" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{258E4E4F-68C3-4AA0-BB8F-4FABE18F87B6}</Property>
				<Property Name="App_INI_GUID" Type="Str">{2801B419-7317-4A32-9530-E4FC9F84CF6F}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{78EA879A-387D-4BF2-8E31-D3F1979DF3E8}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Seek_RcpBelow50_30KHZ_Test</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_RcpBelow50_30KHZ_Test</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{29FB83D3-AE8A-409B-BCAA-09E40C87373A}</Property>
				<Property Name="Bld_version.build" Type="Int">7</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Seek_RcpBelow50_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_RcpBelow50_30KHZ_Test/Seek_RcpBelow50_30KHZ_Test.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_Seek_30KHZ_AUTO/Seek_RcpBelow50_30KHZ_Test/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{51FEDD4E-366C-41E1-A0B6-AD7E7C4D2D80}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_Seek_30KHZ_AUTO/Seek_RcpBelow50_30KHZ_Test/Seek_RcpBelow50_30KHZ_Test.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Seek_RcpBelow50_30KHZ_Test</Property>
				<Property Name="TgtF_internalName" Type="Str">Seek_RcpBelow50_30KHZ_Test</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">Seek_RcpBelow50_30KHZ_Test</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{DE2976A9-D73A-4149-AB8A-7900F67E45DD}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Seek_RcpBelow50_30KHZ_Test.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="TestMode_Above50_20KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{75C80547-FD11-46A1-9A8E-EF15F8E533FD}</Property>
				<Property Name="App_INI_GUID" Type="Str">{9DA791CF-75E4-41FE-A3AF-75468285988F}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{C27B5D5E-C1F3-4A98-BD44-94038E2361EE}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestMode_Above50_20KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../builds/NI_AB_PROJECTNAME/TestMode_Above50_20KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{68418F97-10D8-480D-9703-67F3B5439F38}</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Application.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../builds/NI_AB_PROJECTNAME/TestMode_Above50_20KHZ/Application.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../builds/NI_AB_PROJECTNAME/TestMode_Above50_20KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Source[0].itemID" Type="Str">{18DA79CF-77D3-467C-A5F7-E9DD0C1F0C6B}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/Dependencies/Items in Memory/TestMode_Above50_20KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">TestMode_Above50_20KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">TestMode_Above50_20KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">TestMode_Above50_20KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{D41D47DF-9182-46CC-8BEB-2B46509F39C6}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Application.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
		</Item>
	</Item>
</Project>
