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
		<Item Name="BF_INT_WELDING_40KHZ_AUTO" Type="Folder" URL="../../BF_INT_WELDING_40KHZ_AUTO">
			<Property Name="NI.DISK" Type="Bool">true</Property>
		</Item>
		<Item Name="WeldAutomation" Type="Folder" URL="../../Generic_Common/WeldAutomation">
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
			<Item Name="Create_Error_String_Sub_VI.vi" Type="VI" URL="../../Generic_Common/Create_Error_String_Sub_VI.vi"/>
			<Item Name="Global_Read_INI_Sub_VI.vi" Type="VI" URL="../../Generic_Common/Global_Read_INI_Sub_VI.vi"/>
			<Item Name="kernel32.dll" Type="Document" URL="kernel32.dll">
				<Property Name="NI.PreserveRelativePath" Type="Bool">true</Property>
			</Item>
			<Item Name="Log_Msg_Sub_VI.vi" Type="VI" URL="../../Generic_Common/Log_Msg_Sub_VI.vi"/>
			<Item Name="Write_Html_Report_Sub_VI.vi" Type="VI" URL="../../Generic_Common/Write_Html_Report_Sub_VI.vi"/>
		</Item>
		<Item Name="Build Specifications" Type="Build">
			<Item Name="collapse_dist_mid_collapse_dist_welding_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{3AD6C160-F39A-4434-89DD-BFA9C9CB211B}</Property>
				<Property Name="App_INI_GUID" Type="Str">{C462FC88-C7F9-4732-8476-CB6E34BBE849}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{3950768D-E9B2-4FD9-810B-A4AC16A6A63E}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">collapse_dist_mid_collapse_dist_welding_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/collapse_dist_mid_collapse_dist_welding_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{1ED10D1B-A5B1-41AD-9AC8-B6C2112F7DC2}</Property>
				<Property Name="Bld_version.build" Type="Int">1</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">collapse_dist_mid_collapse_dist_welding_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/collapse_dist_mid_collapse_dist_welding_40KHZ/collapse_dist_mid_collapse_dist_welding_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/collapse_dist_mid_collapse_dist_welding_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{46947173-40D0-49DB-9186-07821F059459}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/collapse_dist_mid_collapse_dist_welding_40KHZ/collapse_dist_mid_collapse_dist_welding_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">collapse_dist_mid_collapse_dist_welding_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">collapse_dist_mid_collapse_dist_welding_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">collapse_dist_mid_collapse_dist_welding_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{23292A9D-04C9-4C78-97AD-3B89C5A5148B}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">collapse_dist_mid_collapse_dist_welding_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="collapse_dist_mid_hold_force_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{B192FDE0-6F9B-4A64-83F8-F3536CA51487}</Property>
				<Property Name="App_INI_GUID" Type="Str">{EC861F78-34DB-4883-A0B9-8868858FDE58}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{144A5665-E0DF-4452-88ED-2CAE783AFA08}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">collapse_dist_mid_hold_force_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/collapse_dist_mid_hold_force_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{36F15DD2-4BB3-492F-AE8F-00F1A5E4CF98}</Property>
				<Property Name="Bld_version.build" Type="Int">1</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">collapse_dist_mid_hold_force_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/collapse_dist_mid_hold_force_40KHZ/collapse_dist_mid_hold_force_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/collapse_dist_mid_hold_force_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{EB51E9AD-B165-492B-ACDB-32CE6F190851}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/collapse_dist_mid_hold_force_40KHZ/collapse_dist_mid_hold_force_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">collapse_dist_mid_hold_force_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">collapse_dist_mid_hold_force_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">collapse_dist_mid_hold_force_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{AAE0FF09-4292-4797-BCD5-410C39CA204C}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">collapse_dist_mid_hold_force_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="collapse_dist_mid_trigger_force_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{E1680F1E-2D32-4CF8-904E-722092A39A02}</Property>
				<Property Name="App_INI_GUID" Type="Str">{65059DEE-8DEB-4C1E-BF51-8B8D42540555}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{B04386E4-F095-466C-865C-ACEA95C854B6}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">collapse_dist_mid_trigger_force_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/collapse_dist_mid_trigger_force_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{8AC17047-ACC5-4EAC-A689-1D77885C3F09}</Property>
				<Property Name="Bld_version.build" Type="Int">1</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">collapse_dist_mid_trigger_force_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/collapse_dist_mid_trigger_force_40KHZ/collapse_dist_mid_trigger_force_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/collapse_dist_mid_trigger_force_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{46947173-40D0-49DB-9186-07821F059459}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/collapse_dist_mid_trigger_force_40KHZ/collapse_dist_mid_trigger_force_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">collapse_dist_mid_trigger_force_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">collapse_dist_mid_trigger_force_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">collapse_dist_mid_trigger_force_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{F88FE020-C4DF-41FA-BC82-C0199FCF737B}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">collapse_dist_mid_trigger_force_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="collapse_dist_mid_weld_force_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{82D0836C-C7A0-454F-B838-798E414F25F5}</Property>
				<Property Name="App_INI_GUID" Type="Str">{B5F030E1-12F2-44EB-9D31-5F9A3E058417}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{3F3F527E-A114-4EE6-9084-CC3C6481AAC6}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">collapse_dist_mid_weld_force_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/collapse_dist_mid_weld_force_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{51F35CD4-432B-4389-A6C5-E8C5591DCBB6}</Property>
				<Property Name="Bld_version.build" Type="Int">1</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">collapse_dist_mid_weld_force_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/collapse_dist_mid_weld_force_40KHZ/collapse_dist_mid_weld_force_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/collapse_dist_mid_weld_force_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{46947173-40D0-49DB-9186-07821F059459}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/collapse_dist_mid_weld_force_40KHZ/collapse_dist_mid_weld_force_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">collapse_dist_mid_weld_force_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">collapse_dist_mid_weld_force_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">collapse_dist_mid_weld_force_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{0BA12168-506C-490E-8C23-CFE35E4B79E8}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">collapse_dist_mid_weld_force_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="collapse_dist_min_collapse_dist_welding_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{5655209C-DB40-4B04-B4C9-FA2A16435E63}</Property>
				<Property Name="App_INI_GUID" Type="Str">{EB334EDA-8546-4CB3-B1A4-97D938C44A58}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{DB97427A-BA42-4DF7-83B3-F215EAFED323}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">collapse_dist_min_collapse_dist_welding_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/collapse_dist_min_collapse_dist_welding_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{EBDA3DBA-65CB-43FC-813F-B63D53733C1D}</Property>
				<Property Name="Bld_version.build" Type="Int">1</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">collapse_dist_min_collapse_dist_welding_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/collapse_dist_min_collapse_dist_welding_40KHZ/collapse_dist_min_collapse_dist_welding_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/collapse_dist_min_collapse_dist_welding_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{46947173-40D0-49DB-9186-07821F059459}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/collapse_dist_min_collapse_dist_welding_40KHZ/collapse_dist_min_collapse_dist_welding_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">collapse_dist_min_collapse_dist_welding_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">collapse_dist_min_collapse_dist_welding_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">collapse_dist_min_collapse_dist_welding_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{544B16B7-39C5-46EE-8F19-5E52415F250E}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">collapse_dist_min_collapse_dist_welding_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="collapse_dist_min_hold_force_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{4E751894-2B41-4AF9-81E1-0C3860C65009}</Property>
				<Property Name="App_INI_GUID" Type="Str">{98671A35-F7AB-41FE-9236-8A2D15C05473}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{0C497F67-9A01-4D69-BCAF-29E7C57EA9C0}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">collapse_dist_min_hold_force_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/collapse_dist_min_hold_force_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{7B0E65EA-EE7E-48A8-BE11-498992A13D44}</Property>
				<Property Name="Bld_version.build" Type="Int">1</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">collapse_dist_min_hold_force_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/collapse_dist_min_hold_force_40KHZ/collapse_dist_min_hold_force_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/collapse_dist_min_hold_force_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{46947173-40D0-49DB-9186-07821F059459}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/collapse_dist_min_hold_force_40KHZ/collapse_dist_min_hold_force_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">collapse_dist_min_hold_force_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">collapse_dist_min_hold_force_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">collapse_dist_min_hold_force_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{526702A9-F5AC-4E08-8E48-D9710DDD1B7B}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">collapse_dist_min_hold_force_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="collapse_dist_min_trigger_force_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{269DB88F-4389-428E-AFA7-791BF55FC5EE}</Property>
				<Property Name="App_INI_GUID" Type="Str">{2FEAA8B3-68C7-4F94-9ADF-DACA7C5B1749}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{84AC2ABE-9431-4223-82BA-181C64661362}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">collapse_dist_min_trigger_force_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/collapse_dist_min_trigger_force_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{015DE778-053C-49E1-8D25-3497068FEE4C}</Property>
				<Property Name="Bld_version.build" Type="Int">1</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">collapse_dist_min_trigger_force_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/collapse_dist_min_trigger_force_40KHZ/collapse_dist_min_trigger_force_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/collapse_dist_min_trigger_force_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{46947173-40D0-49DB-9186-07821F059459}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/collapse_dist_min_trigger_force_40KHZ/collapse_dist_min_trigger_force_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">collapse_dist_min_trigger_force_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">collapse_dist_min_trigger_force_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">collapse_dist_min_trigger_force_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{2BC2E7DE-6F97-451A-9308-40A34C42CAC2}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">collapse_dist_min_trigger_force_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="collapse_dist_min_weld_force_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{93D81B0E-F438-40AC-9A22-95AA3B043EAE}</Property>
				<Property Name="App_INI_GUID" Type="Str">{4399F739-F165-465D-980E-9947CA7C1593}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{ED3D9754-0EAC-4038-9B37-7495C4215ABD}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">collapse_dist_min_weld_force_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/collapse_dist_min_weld_force_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{0168E39A-6851-47FD-8044-2EEC7472525A}</Property>
				<Property Name="Bld_version.build" Type="Int">1</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">collapse_dist_min_weld_force_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/collapse_dist_min_weld_force_40KHZ/collapse_dist_min_weld_force_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/collapse_dist_min_weld_force_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{46947173-40D0-49DB-9186-07821F059459}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/collapse_dist_min_weld_force_40KHZ/collapse_dist_min_weld_force_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">collapse_dist_min_weld_force_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">collapse_dist_min_weld_force_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">collapse_dist_min_weld_force_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{51CC9621-CCE1-49A1-999F-C00821E0D5B3}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">collapse_dist_min_weld_force_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Energymode_mid_energy_welding_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{2B8930F1-1CEE-485D-99A9-00FF938806E9}</Property>
				<Property Name="App_INI_GUID" Type="Str">{0117BBEE-C8E0-4BA4-B6EB-050A6705AB5A}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{99CC8EE0-28ED-464C-A130-6EF85F2FD28A}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Energymode_mid_energy_welding_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Energymode_mid_energy_welding_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{CF4BFBAB-4EE8-492B-94A9-22E71ECEFE84}</Property>
				<Property Name="Bld_version.build" Type="Int">9</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Energymode_mid_energy_welding_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Energymode_mid_energy_welding_40KHZ/Energymode_mid_energy_welding_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Energymode_mid_energy_welding_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{5959C081-986A-4826-968D-0E393C5E20E8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/Energymode_mid_energy_welding_40KHZ/Energymode_mid_energy_welding_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Energymode_mid_energy_welding_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Energymode_mid_energy_welding_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Energymode_mid_energy_welding_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{5C61EA33-F007-4E0E-8CAF-78ED9D766E0A}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Energymode_mid_energy_welding_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Energymode_mid_hold_force_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{93C0D8D5-BA53-4C2C-8C97-D7A949D8DB96}</Property>
				<Property Name="App_INI_GUID" Type="Str">{A808BD59-55BE-4C64-9E64-05B941B7E006}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{FE24E141-E28E-4206-9620-D8D7FD72C64D}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Energymode_mid_hold_force_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Energymode_mid_hold_force_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{3CB34CB7-2609-47D0-9CE1-A6D2B9D38B66}</Property>
				<Property Name="Bld_version.build" Type="Int">9</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Energymode_mid_hold_force_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Energymode_mid_hold_force_40KHZ/Energymode_mid_hold_force_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Energymode_mid_hold_force_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{5959C081-986A-4826-968D-0E393C5E20E8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/Energymode_mid_hold_force_40KHZ/Energymode_mid_hold_force_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Energymode_mid_hold_force_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Energymode_mid_hold_force_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Energymode_mid_hold_force_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{6A974641-1488-41AF-B7AE-544AA7A98555}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Energymode_mid_hold_force_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Energymode_mid_trigger_force_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{5DA4A3F3-4C38-405F-9311-9210CC11E67D}</Property>
				<Property Name="App_INI_GUID" Type="Str">{BA066A21-8A94-41BD-9453-859B673FB32B}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{74CF4A24-1585-4C4F-8E20-772A3DE3EF47}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Energymode_mid_trigger_force_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Energymode_mid_trigger_force_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{165C4586-5B27-4074-91AD-AB71F1346E3B}</Property>
				<Property Name="Bld_version.build" Type="Int">9</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Energymode_mid_trigger_force_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Energymode_mid_trigger_force_40KHZ/Energymode_mid_trigger_force_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Energymode_mid_trigger_force_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{5959C081-986A-4826-968D-0E393C5E20E8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/Energymode_mid_trigger_force_40KHZ/Energymode_mid_trigger_force_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Energymode_mid_trigger_force_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Energymode_mid_trigger_force_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Energymode_mid_trigger_force_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{DBB81853-4B8F-4D4D-B56B-7E35E84DB84C}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Energymode_mid_trigger_force_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Energymode_mid_weld_force_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{BEDD83CA-7E18-4B80-B165-94D67EAA5AD5}</Property>
				<Property Name="App_INI_GUID" Type="Str">{46C4F24F-3DEF-4E02-8A60-3546E6F992AC}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{371F8AF4-C712-4EB3-8654-302AE04BA82A}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Energymode_mid_weld_force_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Energymode_mid_weld_force_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{EB7B3B31-C97F-495E-AB0B-F2FF52E9578E}</Property>
				<Property Name="Bld_version.build" Type="Int">9</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Energymode_mid_weld_force_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Energymode_mid_weld_force_40KHZ/Energymode_mid_weld_force_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Energymode_mid_weld_force_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{5959C081-986A-4826-968D-0E393C5E20E8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/Energymode_mid_weld_force_40KHZ/Energymode_mid_weld_force_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Energymode_mid_weld_force_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Energymode_mid_weld_force_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Energymode_mid_weld_force_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{7BC08D3C-7416-4DA1-9A58-A8C350C3BC34}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Energymode_mid_weld_force_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Energymode_min_energy_welding_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{C31BDB53-3B25-4CB9-8FF2-16F2E3CDEED1}</Property>
				<Property Name="App_INI_GUID" Type="Str">{39B03B53-9090-40A4-8F7A-519E264C0CCC}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{8CB5B55F-9D30-492C-B3E0-DABE0BE44FCF}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Energymode_min_energy_welding_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Energymode_min_energy_welding_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{32749495-6E4C-40AA-8329-D966638F4F0D}</Property>
				<Property Name="Bld_version.build" Type="Int">8</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Energymode_min_energy_welding_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Energymode_min_energy_welding_40KHZ/Energymode_min_energy_welding_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Energymode_min_energy_welding_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{5959C081-986A-4826-968D-0E393C5E20E8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/Energymode_min_energy_welding_40KHZ/Energymode_min_energy_welding_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Energymode_min_energy_welding_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Energymode_min_energy_welding_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Energymode_min_energy_welding_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{6351CCC4-C249-4509-9B08-41AF74872079}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Energymode_min_energy_welding_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Energymode_min_hold_force_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{E24AE226-ABC7-4C85-A701-EF2A703F50A9}</Property>
				<Property Name="App_INI_GUID" Type="Str">{96B54793-CB82-4E62-9E0F-89360A1A296A}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{961782C2-EECC-4ADA-A3EE-F9E54EC52AF7}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Energymode_min_hold_force_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Energymode_min_hold_force_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{B8249862-EFB9-401E-A4A0-0DA146785DC1}</Property>
				<Property Name="Bld_version.build" Type="Int">8</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Energymode_min_hold_force_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Energymode_min_hold_force_40KHZ/Energymode_min_hold_force_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Energymode_min_hold_force_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{5959C081-986A-4826-968D-0E393C5E20E8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/Energymode_min_hold_force_40KHZ/Energymode_min_hold_force_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Energymode_min_hold_force_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Energymode_min_hold_force_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Energymode_min_hold_force_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{A6DC3E20-818B-4A89-9411-631ECE82BF2E}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Energymode_min_hold_force_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Energymode_min_trigger_force_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{02AAA352-23B0-47F2-8330-BBA5EAF67FAC}</Property>
				<Property Name="App_INI_GUID" Type="Str">{200995C6-BA0B-43C4-88CE-D0CF81207B0E}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{6F4A5EB8-94A5-445E-A04B-08E4D393DCE4}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Energymode_min_trigger_force_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Energymode_min_trigger_force_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{C59F2993-FA4D-4D97-98E4-F32DF3E7C300}</Property>
				<Property Name="Bld_version.build" Type="Int">8</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Energymode_min_trigger_force_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Energymode_min_trigger_force_40KHZ/Energymode_min_trigger_force_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Energymode_min_trigger_force_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{5959C081-986A-4826-968D-0E393C5E20E8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/Energymode_min_trigger_force_40KHZ/Energymode_min_trigger_force_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Energymode_min_trigger_force_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Energymode_min_trigger_force_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Energymode_min_trigger_force_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{FE6C82A8-EB9C-4BBD-BFFA-ADF6D5EA3A78}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Energymode_min_trigger_force_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Energymode_min_weld_force_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{02EF45E1-B39F-440A-AE0F-8352CFCB35B4}</Property>
				<Property Name="App_INI_GUID" Type="Str">{9A32D1C3-6698-4C04-B57E-F1F188845DAA}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{F9857F68-A98D-40C3-8617-19F4A69F38FA}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Energymode_min_weld_force_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Energymode_min_weld_force_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{977ED6DD-52CB-4103-988B-0FB1E2B84AEF}</Property>
				<Property Name="Bld_version.build" Type="Int">8</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Energymode_min_weld_force_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Energymode_min_weld_force_40KHZ/Energymode_min_weld_force_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Energymode_min_weld_force_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{5959C081-986A-4826-968D-0E393C5E20E8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/Energymode_min_weld_force_40KHZ/Energymode_min_weld_force_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Energymode_min_weld_force_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Energymode_min_weld_force_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Energymode_min_weld_force_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{9ED0CC92-BA43-48FA-8D9E-6C978E0D470D}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Energymode_min_weld_force_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Peakpowermode_mid_hold_force_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{D73BBCD5-5DDD-4E57-ACE1-B1C7323B322B}</Property>
				<Property Name="App_INI_GUID" Type="Str">{16FC3FCB-816E-4D3C-B101-8B7F7442EC8D}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{C63C85EB-753C-475F-BC4A-C727FEDCC9E0}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Peakpowermode_mid_hold_force_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_mid_hold_force_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{A1750005-2084-49B6-B7AB-30C856EE85C0}</Property>
				<Property Name="Bld_version.build" Type="Int">8</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Peakpowermode_mid_hold_force_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_mid_hold_force_40KHZ/Peakpowermode_mid_hold_force_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_mid_hold_force_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{5959C081-986A-4826-968D-0E393C5E20E8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_mid_hold_force_40KHZ/Peakpowermode_mid_hold_force_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Peakpowermode_mid_hold_force_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Peakpowermode_mid_hold_force_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Peakpowermode_mid_hold_force_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{E3FA9552-9942-41B1-9CC0-9336821BD9BD}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Peakpowermode_mid_hold_force_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Peakpowermode_mid_peakpower_welding_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{745D49F2-435C-46F8-880C-BD9FF836187E}</Property>
				<Property Name="App_INI_GUID" Type="Str">{4BDDBF06-BDED-45CB-B21C-9FA54930978C}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{6C9C121E-1814-4B20-8486-2EE2894173E1}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Peakpowermode_mid_peakpower_welding_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_mid_peakpower_welding_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{EB50A60B-87BF-4787-A9D9-4267635D85E3}</Property>
				<Property Name="Bld_version.build" Type="Int">8</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Peakpowermode_mid_peakpower_welding_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_mid_peakpower_welding_40KHZ/Peakpowermode_mid_peakpower_welding_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_mid_peakpower_welding_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{5959C081-986A-4826-968D-0E393C5E20E8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_mid_peakpower_welding_40KHZ/Peakpowermode_mid_peakpower_welding_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Peakpowermode_mid_peakpower_welding_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Peakpowermode_mid_peakpower_welding_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Peakpowermode_mid_peakpower_welding_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{DADD6428-69B8-4E96-8DEA-C2B6392EF8D9}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Peakpowermode_mid_peakpower_welding_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Peakpowermode_mid_trigger_force_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{0C0C6ED8-F7FE-4AFA-9F3C-1036A14B3964}</Property>
				<Property Name="App_INI_GUID" Type="Str">{6F5CCEBE-B1CE-4E02-BB69-AE37361AC34E}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{1A899444-89F3-4761-91EF-7D36250A1E57}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Peakpowermode_mid_trigger_force_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_mid_trigger_force_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{6A8593B9-FA83-43B8-8289-A6371AF4F65D}</Property>
				<Property Name="Bld_version.build" Type="Int">8</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Peakpowermode_mid_trigger_force_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_mid_trigger_force_40KHZ/Peakpowermode_mid_trigger_force_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_mid_trigger_force_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{5959C081-986A-4826-968D-0E393C5E20E8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_mid_trigger_force_40KHZ/Peakpowermode_mid_trigger_force_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Peakpowermode_mid_trigger_force_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Peakpowermode_mid_trigger_force_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Peakpowermode_mid_trigger_force_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{0CEF92B6-00D6-4BB6-8D8A-0A668BE6418D}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Peakpowermode_mid_trigger_force_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Peakpowermode_mid_weld_force_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{E4895ACF-6C56-436F-97FB-06968B8721BA}</Property>
				<Property Name="App_INI_GUID" Type="Str">{E2F1B3F1-172B-4970-8068-372F9B8C9038}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{060283B6-AAB8-4A1E-A699-73A1DAD4FEE0}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Peakpowermode_mid_weld_force_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_mid_weld_force_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{50E70EF4-0460-4F8E-9912-7407DD142E0B}</Property>
				<Property Name="Bld_version.build" Type="Int">8</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Peakpowermode_mid_weld_force_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_mid_weld_force_40KHZ/Peakpowermode_mid_weld_force_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_mid_weld_force_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{5959C081-986A-4826-968D-0E393C5E20E8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_mid_weld_force_40KHZ/Peakpowermode_mid_weld_force_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Peakpowermode_mid_weld_force_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Peakpowermode_mid_weld_force_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Peakpowermode_mid_weld_force_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{EBF38E88-03E0-4A75-9FA7-923C33DDC962}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Peakpowermode_mid_weld_force_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Peakpowermode_min_hold_force_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{0D004652-1028-42B6-9EA3-037BEF39A81A}</Property>
				<Property Name="App_INI_GUID" Type="Str">{C0B55B8A-68F0-4969-A2A8-9C7D28E69785}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{9DAFC830-91D9-40D8-A376-D3665CA8666B}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Peakpowermode_min_hold_force_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_min_hold_force_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{5B323B7C-4537-4ED4-A604-BA57FC4D8719}</Property>
				<Property Name="Bld_version.build" Type="Int">8</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Peakpowermode_min_hold_force_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_min_hold_force_40KHZ/Peakpowermode_min_hold_force_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_min_hold_force_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{5959C081-986A-4826-968D-0E393C5E20E8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_min_hold_force_40KHZ/Peakpowermode_min_hold_force_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Peakpowermode_min_hold_force_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Peakpowermode_min_hold_force_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Peakpowermode_min_hold_force_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{671FCDCC-D8A7-4C7D-93A1-58ED498944D3}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Peakpowermode_min_hold_force_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Peakpowermode_min_peakpower_welding_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{7AF07F17-7E55-40ED-BC9D-DE63447C1CB0}</Property>
				<Property Name="App_INI_GUID" Type="Str">{4B9FB958-5590-4A50-A3B6-882FE5C9A324}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{CC13E262-87E2-4129-89B4-47F0928EC26E}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Peakpowermode_min_peakpower_welding_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_min_peakpower_welding_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{A0305B8B-FC50-4E56-9358-05E7662071DF}</Property>
				<Property Name="Bld_version.build" Type="Int">8</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Peakpowermode_min_peakpower_welding_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_min_peakpower_welding_40KHZ/Peakpowermode_min_peakpower_welding_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_min_peakpower_welding_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{5959C081-986A-4826-968D-0E393C5E20E8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_min_peakpower_welding_40KHZ/Peakpowermode_min_peakpower_welding_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Peakpowermode_min_peakpower_welding_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Peakpowermode_min_peakpower_welding_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Peakpowermode_min_peakpower_welding_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{DAD6C6DC-5358-4D79-8B31-CC558A703E47}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Peakpowermode_min_peakpower_welding_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Peakpowermode_min_trigger_force_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{9386A40F-D5B4-493D-B58E-996D26F53742}</Property>
				<Property Name="App_INI_GUID" Type="Str">{36D42220-13C7-4FEE-99CB-EE0947F05D9F}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{00784021-3F7D-42CA-99F3-3548F9367237}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Peakpowermode_min_trigger_force_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_min_trigger_force_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{24DA5E2A-5C88-4AC8-AAC9-D02E08B03DBF}</Property>
				<Property Name="Bld_version.build" Type="Int">8</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Peakpowermode_min_trigger_force_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_min_trigger_force_40KHZ/Peakpowermode_min_trigger_force_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_min_trigger_force_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{5959C081-986A-4826-968D-0E393C5E20E8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_min_trigger_force_40KHZ/Peakpowermode_min_trigger_force_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Peakpowermode_min_trigger_force_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Peakpowermode_min_trigger_force_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Peakpowermode_min_trigger_force_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{AAC5F402-7BAC-4266-97C4-9CA35694AACB}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Peakpowermode_min_trigger_force_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Peakpowermode_min_weld_force_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{B37A71BA-2678-4896-A652-F90D6C0CF920}</Property>
				<Property Name="App_INI_GUID" Type="Str">{8BF85E41-694F-4734-A7DF-668385511DC9}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{179CACE5-4023-4065-B935-FBF84F8D2644}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Peakpowermode_min_weld_force_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_min_weld_force_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{0533D9D7-7B44-4C12-AF9F-FE2F28B68B72}</Property>
				<Property Name="Bld_version.build" Type="Int">8</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Peakpowermode_min_weld_force_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_min_weld_force_40KHZ/Peakpowermode_min_weld_force_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_min_weld_force_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{5959C081-986A-4826-968D-0E393C5E20E8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/Peakpowermode_min_weld_force_40KHZ/Peakpowermode_min_weld_force_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Peakpowermode_min_weld_force_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Peakpowermode_min_weld_force_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Peakpowermode_min_weld_force_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{8EC1E6C2-B59C-4C5D-9510-A5E73879CAD6}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Peakpowermode_min_weld_force_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Timemode_default_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{982CC77E-3EBA-48BD-8B4F-9977798C4575}</Property>
				<Property Name="App_INI_GUID" Type="Str">{2428395F-6DD4-4DAC-9FBE-25636DFD3826}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{FB3E4C97-7D27-4159-937B-520CD0705F1F}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Timemode_default_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_default_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{F7EB2260-C54A-4BB4-A8F8-E9337EC25ECE}</Property>
				<Property Name="Bld_version.build" Type="Int">34</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Timemode_default_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_default_40KHZ/Timemode_default_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_default_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{5959C081-986A-4826-968D-0E393C5E20E8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/Timemode_default_40KHZ/Timemode_default_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Timemode_default_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Timemode_default_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Timemode_default_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{0016C842-26E5-413A-915F-184162AB0B83}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Timemode_default_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Timemode_equal_weldtime_holdtime_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{81722CFE-C49C-424F-9E6A-0FA9F8BDDE1B}</Property>
				<Property Name="App_INI_GUID" Type="Str">{D7A79D27-34C8-40AE-AC74-1D0AD309E3CC}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{3C843C52-0E29-4EAC-9A71-BE1F0FA55795}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Timemode_equal_weldtime_holdtime_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_equal_weldtime_holdtime_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{BFC7B913-EEAB-4FEE-A98F-384106DEDCD4}</Property>
				<Property Name="Bld_version.build" Type="Int">8</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Timemode_equal_weldtime_holdtime_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_equal_weldtime_holdtime_40KHZ/Timemode_equal_weldtime_holdtime_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_equal_weldtime_holdtime_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{5959C081-986A-4826-968D-0E393C5E20E8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/Timemode_equal_weldtime_holdtime_40KHZ/Timemode_equal_weldtime_holdtime_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Timemode_equal_weldtime_holdtime_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Timemode_equal_weldtime_holdtime_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Timemode_equal_weldtime_holdtime_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{3C08F128-C52B-4E29-A11E-2A13DE71B513}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Timemode_equal_weldtime_holdtime_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Timemode_max_time_welding_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{0CE82E0E-A236-4224-9B76-460316439EEC}</Property>
				<Property Name="App_INI_GUID" Type="Str">{2127CA29-F5BD-4930-BBB7-894136759F23}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{06658ED4-CF42-40EE-9846-166C579475AC}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Timemode_max_time_welding_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_max_time_welding_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{F860C523-BD75-4887-9467-BF14E60FD46C}</Property>
				<Property Name="Bld_version.build" Type="Int">8</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Timemode_max_time_welding_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_max_time_welding_40KHZ/Timemode_max_time_welding_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_max_time_welding_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{5959C081-986A-4826-968D-0E393C5E20E8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/Timemode_max_time_welding_40KHZ/Timemode_max_time_welding_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Timemode_max_time_welding_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Timemode_max_time_welding_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Timemode_max_time_welding_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{7A550170-15E6-41FF-B23A-5CAA962E9870}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Timemode_max_time_welding_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Timemode_mid_hold_force_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{D4AF69C3-078B-4F37-A515-D1E396BB5F51}</Property>
				<Property Name="App_INI_GUID" Type="Str">{491F6599-5821-4569-ACD0-D6A267E9E019}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{FF63DF9F-7061-4BE3-8BC4-42A2EA9EC5C7}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Timemode_mid_hold_force_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_mid_hold_force_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{F634B923-88DB-4F4D-8C21-187DE4B5C68C}</Property>
				<Property Name="Bld_version.build" Type="Int">8</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Timemode_mid_hold_force_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_mid_hold_force_40KHZ/Timemode_mid_hold_force_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_mid_hold_force_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{5959C081-986A-4826-968D-0E393C5E20E8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/Timemode_mid_hold_force_40KHZ/Timemode_mid_hold_force_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Timemode_mid_hold_force_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Timemode_mid_hold_force_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Timemode_mid_hold_force_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{5B5229AA-DE22-4D34-AAC7-7DE5E867DB8E}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Timemode_mid_hold_force_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Timemode_mid_time_welding_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{AC2E456C-0C52-4DF5-B05D-A26BABA7B820}</Property>
				<Property Name="App_INI_GUID" Type="Str">{41DA9D08-9172-4B96-8362-3F6F29BA1F41}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{37049972-4AE2-4B2C-87B0-413B475924C6}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Timemode_mid_time_welding_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_mid_time_welding_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{0A25BB03-435C-4B18-9DB7-64AA5DB6B740}</Property>
				<Property Name="Bld_version.build" Type="Int">8</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Timemode_mid_time_welding_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_mid_time_welding_40KHZ/Timemode_mid_time_welding_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_mid_time_welding_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{5959C081-986A-4826-968D-0E393C5E20E8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/Timemode_mid_time_welding_40KHZ/Timemode_mid_time_welding_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Timemode_mid_time_welding_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Timemode_mid_time_welding_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Timemode_mid_time_welding_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{40174F89-1E63-4631-A28D-BA43E99D26B8}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Timemode_mid_time_welding_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Timemode_mid_trigger_force_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{7331B343-A407-4E37-A8A8-4D7F2770F7B1}</Property>
				<Property Name="App_INI_GUID" Type="Str">{F7C25C14-1ACB-40B3-81A4-50A7D8EAC176}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{F13FEF94-9B11-44BF-BFFE-3A41F6E85749}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Timemode_mid_trigger_force_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_mid_trigger_force_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{DDF1E4E2-DCE3-460E-A8E9-B8C7FEDF4A08}</Property>
				<Property Name="Bld_version.build" Type="Int">8</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Timemode_mid_trigger_force_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_mid_trigger_force_40KHZ/Timemode_mid_trigger_force_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_mid_trigger_force_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{5959C081-986A-4826-968D-0E393C5E20E8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/Timemode_mid_trigger_force_40KHZ/Timemode_mid_trigger_force_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Timemode_mid_trigger_force_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Timemode_mid_trigger_force_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Timemode_mid_trigger_force_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{665D2F93-2F97-4041-835D-A47A3A5B5F24}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Timemode_mid_trigger_force_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Timemode_mid_weld_force_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{8558896F-FFB2-499C-B426-3CDEE7523776}</Property>
				<Property Name="App_INI_GUID" Type="Str">{ADE9CC6A-730A-4E87-80B1-DE2CCB3174AB}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{DE558D1C-9522-49E5-8B45-C7F354990920}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Timemode_mid_weld_force_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_mid_weld_force_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{34F05051-5FFF-4692-8789-FA31CBF86331}</Property>
				<Property Name="Bld_version.build" Type="Int">8</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Timemode_mid_weld_force_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_mid_weld_force_40KHZ/Timemode_mid_weld_force_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_mid_weld_force_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{5959C081-986A-4826-968D-0E393C5E20E8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/Timemode_mid_weld_force_40KHZ/Timemode_mid_weld_force_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Timemode_mid_weld_force_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Timemode_mid_weld_force_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Timemode_mid_weld_force_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{A519B6BE-4F65-4AD0-99A8-C2F4B6BD64A0}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Timemode_mid_weld_force_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Timemode_min_hold_force_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{3202D422-CFC9-4A9C-B05E-F95B4C846A9B}</Property>
				<Property Name="App_INI_GUID" Type="Str">{F0CC123C-80C3-4D21-930A-CA1EC537228E}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{0B41FF67-5EF4-447F-8CBD-FC25E9005044}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Timemode_min_hold_force_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_min_hold_force_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{FB4FAB82-83F2-45B7-8B1E-E2268C7FA024}</Property>
				<Property Name="Bld_version.build" Type="Int">8</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Timemode_min_hold_force_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_min_hold_force_40KHZ/Timemode_min_hold_force_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_min_hold_force_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{5959C081-986A-4826-968D-0E393C5E20E8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/Timemode_min_hold_force_40KHZ/Timemode_min_hold_force_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Timemode_min_hold_force_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Timemode_min_hold_force_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Timemode_min_hold_force_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{6C21BB85-650C-42C8-9E97-07E330FA268E}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Timemode_min_hold_force_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Timemode_min_time_welding_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{8F2278B3-312C-4A45-8E90-380D7F8BD965}</Property>
				<Property Name="App_INI_GUID" Type="Str">{6819E093-EEC0-4746-9606-A75AE035FD0A}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{0C281B70-14F9-4A42-81CB-9E4691F0C850}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Timemode_min_time_welding_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_min_time_welding_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{ED48159E-C7D9-4AEA-AA39-9EDC20A771D1}</Property>
				<Property Name="Bld_version.build" Type="Int">8</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Timemode_min_time_welding_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_min_time_welding_40KHZ/Timemode_min_time_welding_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_min_time_welding_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{5959C081-986A-4826-968D-0E393C5E20E8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/Timemode_min_time_welding_40KHZ/Timemode_min_time_welding_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Timemode_min_time_welding_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Timemode_min_time_welding_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Timemode_min_time_welding_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{B60DAD0C-C9F5-4515-AC20-E266478ADFF0}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Timemode_min_time_welding_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Timemode_min_trigger_force_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{90972DBB-8058-45A4-A5CD-DFD052888110}</Property>
				<Property Name="App_INI_GUID" Type="Str">{F9DC30A0-0776-4B15-8A36-1954A0B2292D}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{F02038CE-ED17-4462-8A79-6F447F12D8FB}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Timemode_min_trigger_force_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_min_trigger_force_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{D4475ADB-30B8-4713-A207-3254D007F8A9}</Property>
				<Property Name="Bld_version.build" Type="Int">8</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Timemode_min_trigger_force_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_min_trigger_force_40KHZ/Timemode_min_trigger_force_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_min_trigger_force_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{5959C081-986A-4826-968D-0E393C5E20E8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/Timemode_min_trigger_force_40KHZ/Timemode_min_trigger_force_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Timemode_min_trigger_force_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Timemode_min_trigger_force_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Timemode_min_trigger_force_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{7B1F95E5-81B9-4383-BD36-2E4C925972C7}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Timemode_min_trigger_force_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
			<Item Name="Timemode_min_weld_force_40KHZ" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{F7C591F8-93FE-4953-9903-A9B00EA3454E}</Property>
				<Property Name="App_INI_GUID" Type="Str">{F0F0275A-2D1A-4086-A7A5-95BEA582F4BB}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{FD5B740E-CD0B-4288-BF27-1614073F5136}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Timemode_min_weld_force_40KHZ</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_min_weld_force_40KHZ</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{CD13C219-F8FD-477E-9C15-AA1B4A204092}</Property>
				<Property Name="Bld_version.build" Type="Int">8</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Timemode_min_weld_force_40KHZ.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_min_weld_force_40KHZ/Timemode_min_weld_force_40KHZ.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../BF_INT_WELDING_40KHZ_AUTO/Timemode_min_weld_force_40KHZ/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{5959C081-986A-4826-968D-0E393C5E20E8}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/BF_INT_WELDING_40KHZ_AUTO/Timemode_min_weld_force_40KHZ/Timemode_min_weld_force_40KHZ.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Timemode_min_weld_force_40KHZ</Property>
				<Property Name="TgtF_internalName" Type="Str">Timemode_min_weld_force_40KHZ</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2019 </Property>
				<Property Name="TgtF_productName" Type="Str">Timemode_min_weld_force_40KHZ</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{0BC6FB26-A0C5-463A-B0C9-8B097C6DB672}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Timemode_min_weld_force_40KHZ.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
		</Item>
	</Item>
</Project>
