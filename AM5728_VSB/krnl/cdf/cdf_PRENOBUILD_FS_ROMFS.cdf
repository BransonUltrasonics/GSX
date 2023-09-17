

/* this file is automatically generated */


Layer Layer::FS_ROMFS {
}




/*********************************
 Component INCLUDE_ROMFS_DRV 
 original object sources located at 
  "$(WIND_BASE)/pkgs/storage/fs-1.0.0.2/romfs-1.1.1.1/cdf/20comp_romfs.cdf" 46 63  
**********************************/
Component	INCLUDE_ROMFS_DRV {
		LAYER += Layer::FS_ROMFS
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/storage/fs-1.0.0.2/romfs-1.1.1.1/cdf/20comp_romfs.cdf" 46 63 }
}


/*********************************
 Component INCLUDE_ROMFS 
 original object sources located at 
  "$(WIND_BASE)/pkgs/storage/fs-1.0.0.2/romfs-1.1.1.1/cdf/20comp_romfs.cdf" 65 73  
**********************************/
Component	INCLUDE_ROMFS {
		LAYER += Layer::FS_ROMFS
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/storage/fs-1.0.0.2/romfs-1.1.1.1/cdf/20comp_romfs.cdf" 65 73 }
}


/*********************************
 Folder FOLDER_FILESYSTEM 
 original object sources located at 
  "$(WIND_BASE)/pkgs/storage/fs-1.0.0.2/romfs-1.1.1.1/cdf/20comp_romfs.cdf" 27 33  
**********************************/
Folder	FOLDER_FILESYSTEM {
		LAYER += Layer::FS_ROMFS
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/storage/fs-1.0.0.2/romfs-1.1.1.1/cdf/20comp_romfs.cdf" 27 33 }
}


/*********************************
 Folder FOLDER_ROMFS 
 original object sources located at 
  "$(WIND_BASE)/pkgs/storage/fs-1.0.0.2/romfs-1.1.1.1/cdf/20comp_romfs.cdf" 35 44  
**********************************/
Folder	FOLDER_ROMFS {
		LAYER += Layer::FS_ROMFS
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/storage/fs-1.0.0.2/romfs-1.1.1.1/cdf/20comp_romfs.cdf" 35 44 }
}


/*********************************
 Layer Layer::FS_ROMFS 
 original object sources located at 
  "C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf/cdf_PRENOBUILD_FS_ROMFS.tmp" 13 15  
**********************************/
Layer	Layer::FS_ROMFS {
		LAYER += Layer::FS_ROMFS
		LAYER_SOURCE		{ "C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf/cdf_PRENOBUILD_FS_ROMFS.tmp" 13 15 }
}


/*********************************
 Parameter ROMFS_MAX_OPEN_FILES 
 original object sources located at 
  "$(WIND_BASE)/pkgs/storage/fs-1.0.0.2/romfs-1.1.1.1/cdf/20comp_romfs.cdf" 82 91  
**********************************/
Parameter	ROMFS_MAX_OPEN_FILES {
		LAYER += Layer::FS_ROMFS
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/storage/fs-1.0.0.2/romfs-1.1.1.1/cdf/20comp_romfs.cdf" 82 91 }
}


/*********************************
 Parameter ROMFS_DEV_NAME 
 original object sources located at 
  "$(WIND_BASE)/pkgs/storage/fs-1.0.0.2/romfs-1.1.1.1/cdf/20comp_romfs.cdf" 75 80  
**********************************/
Parameter	ROMFS_DEV_NAME {
		LAYER += Layer::FS_ROMFS
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/storage/fs-1.0.0.2/romfs-1.1.1.1/cdf/20comp_romfs.cdf" 75 80 }
}
