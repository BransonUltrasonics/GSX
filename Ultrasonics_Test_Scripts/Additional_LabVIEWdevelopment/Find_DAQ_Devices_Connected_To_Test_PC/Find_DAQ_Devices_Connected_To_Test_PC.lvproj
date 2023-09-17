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
		<Item Name="Find_DAQ_Devices_Connected_To_Test_PC.vi" Type="VI" URL="../Find_DAQ_Devices_Connected_To_Test_PC.vi"/>
		<Item Name="Dependencies" Type="Dependencies"/>
		<Item Name="Build Specifications" Type="Build">
			<Item Name="Find_DAQ_Devices_Connected_To_Test_PC" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{5327900B-281F-4AA3-A597-9F666E2C9C67}</Property>
				<Property Name="App_INI_GUID" Type="Str">{2F240B9B-377B-4786-9183-A13D07ED5F13}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{04E3027B-E134-42C0-AE3F-BCDBDF192077}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Find_DAQ_Devices_Connected_To_Test_PC</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../exe</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToProject</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{805E6467-38FA-4781-8E88-79ACC34A9AF8}</Property>
				<Property Name="Bld_version.build" Type="Int">1</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Find_DAQ_Devices_Connected_To_Test_PC.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../exe/NI_AB_PROJECTNAME.exe</Property>
				<Property Name="Destination[0].path.type" Type="Str">relativeToProject</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../exe/data</Property>
				<Property Name="Destination[1].path.type" Type="Str">relativeToProject</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{74AC92B5-B799-4B00-BB34-7517F560E7B2}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/Find_DAQ_Devices_Connected_To_Test_PC.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Sasken Technologies Ltd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Find_DAQ_Devices_Connected_To_Test_PC</Property>
				<Property Name="TgtF_internalName" Type="Str">Find_DAQ_Devices_Connected_To_Test_PC</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2018 Sasken Technologies Ltd</Property>
				<Property Name="TgtF_productName" Type="Str">Find_DAQ_Devices_Connected_To_Test_PC</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{564DE4A5-D59B-4E3B-BA79-161F7D10CCEE}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Find_DAQ_Devices_Connected_To_Test_PC.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
		</Item>
	</Item>
</Project>
