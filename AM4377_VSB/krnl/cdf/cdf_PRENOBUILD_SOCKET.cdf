

/* this file is automatically generated */


Layer Layer::SOCKET {
}




/*********************************
 Component INCLUDE_SC_SOCKLIB 
 original object sources located at 
  "$(WIND_BASE)/pkgs/os/service/socket-1.0.3.2/cdf/01comp_sc_socklib_vx.cdf" 14 27  
**********************************/
Component	INCLUDE_SC_SOCKLIB {
		LAYER += Layer::SOCKET
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/os/service/socket-1.0.3.2/cdf/01comp_sc_socklib_vx.cdf" 14 27 }
}


/*********************************
 Component INCLUDE_SOCKLIB 
 original object sources located at 
  "$(WIND_BASE)/pkgs/os/service/socket-1.0.3.2/cdf/01comp_socklib_vx.cdf" 18 28  
**********************************/
Component	INCLUDE_SOCKLIB {
		LAYER += Layer::SOCKET
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/os/service/socket-1.0.3.2/cdf/01comp_socklib_vx.cdf" 18 28 }
}


/*********************************
 Folder FOLDER_SOCKET 
 original object sources located at 
  "$(WIND_BASE)/pkgs/os/service/socket-1.0.3.2/cdf/01folder_osservice_socket.cdf" 18 25  
**********************************/
Folder	FOLDER_SOCKET {
		LAYER += Layer::SOCKET
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/os/service/socket-1.0.3.2/cdf/01folder_osservice_socket.cdf" 18 25 }
}


/*********************************
 Layer Layer::SOCKET 
 original object sources located at 
  "C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_SOCKET.tmp" 15 17  
**********************************/
Layer	Layer::SOCKET {
		LAYER += Layer::SOCKET
		LAYER_SOURCE		{ "C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_SOCKET.tmp" 15 17 }
}
