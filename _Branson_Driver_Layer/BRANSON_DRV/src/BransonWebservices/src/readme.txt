
1. Webclidemo usage

Step 1) Enable WEBCLI layer and WEBCLIDEMO in VSB then build.

Step 2) Include INCLUDE_WEBCLI_WEBCLIDEMO in VIP,then the other required 
        components will be included automatically.
    
Step 3) Create romfs and reference to VIP project, then add 
        "/romfs/htdocs/server.pem" to romfs file system.

Step 4) Build and run vxWorks image, then you can test to access webpages on 
        web browser and test to access CLI on console.
   
2. Customize Webcli usage

Step 1) Rename the folder and file name to proper one,including folder name 
        "webclidemo" and file name "cdf/40webclidemo.cdf".

Step 2) Modify CDF file "cdf/40webclidemo.cdf" and file layer.vsbl.

Step 3) Add user code and modify src/Makefile. 

Step 4) Add or modify resource files in htdocs/ for web server.

Step 5) Modify config.webcli by MCE(or manual) and then using MCE(or mannual 
        run "../../tools/bin/x86-linux2/preMake.sh ./ all l" on linux enviroment
        or preMake.bat on windows) to generate following files.
        For web server:
        wmb_webclidemo.h and wmw_httpconf.h will be generated.
        wmb_webclidemo.c wmw_httpconf.c and wm_filesys.c will be generated on
        src/.
        For CLI:
        wmb_webclidemo.h wmc_webclidemo.h rcc_handler.h and wmc_options.h 
        will be generated.
        wmb_webclidemo.c rcc_handler.c and wmc_webclidemo.c will be generated.
        
Step 6) Enable WEBCLI layer in VSB, and then enable COMMON, MIBWAY, BACKPLANE,
        CURL, HTTP, CLI and WEBCLIDEMO.

Step 7) Include INCLUDE_WEBCLI_WEBCLIDEMO in VIP,then the other required 
        components will be included automatically.

Step 8) Build VSB and VIP,then run vxWorks image, then you can test to access 
        CLI and Webserver.

Note:
1)preMake.sh and preMake.bat locate in 
  webcli/tools/bin/x86-linux2(x86-win32).The generated files will be replaced 
  with new generated files. So it's better to backup the original files and merge 
  back to new generated files for some customerlized codes.
  Usage sample of preMake.sh: <./preMake.sh ../../../webclidemo/src all l>
  The first parameter should be the path of config.webcli.
  The second parameter should be 'web', 'cli' or 'all'.If the the endian is big, 
  the third parameter should be 'b',if the the endian is little, the third parameter 
  should be 'l'.
2)if renamed webclidemo to xxx, some file name and symbol name should be changed in 
  "src/Makefile" and "cdf/40xxx.cdf". webcli/common/cdf/40webcli.cdf also need to 
  be changed.
3)xxxCliInit.c xxxHttpInit.c and xxxCommonInit.c should be copied to user layer.
4)Please do not make changes on webclidemo layer. 
5)This webclidemo project is just adapt to little endian BSP.

For web server usage:
1) If use romfs file system to store html files or certification file, romfs 
   project should be created and reference to VIP project.
2) The default file upload path shoud be writable.
3) User should customize following files sampleHttpUserFuncs.c sampleJSUser.c 
   sampleJSUser.h sampleUserCgi.c sampleUserCgi.h sampleUserRpm.c and 
   sampleUserRpm.h.
4) The standalone embedded HTTP server is a special(nontypical) use case.If 
   enable WEBCLI_STANDALONE_WEBSERVER in vsb, http and common should be enabled. 
   following must be disabled mannually, backplane,mibway,curl,SMTP,live control,
   file upload,post RPM, WMB RPM WEBCLI_ENABLE_WMB_USERS and WEBCLI_JAVASCRIPT_DIGEST_ENABLED.
   The user layer should be configured correctly.
5) If enable HTTPS feature,OPENSSL layer in VSB and INCLUDE_IPSSL in VIP is 
   needed. 
6) If webserver is needed and CLI is not needed, please refer to webdemo layer.

For CLI usage:
1) User should customize following files usrCustomFunc.c and usrCustomFunc.h.
2) If enable SSH feature,SSH layer in VSB and INCLUDE_SSH in VIP is needed.
3) If CLI is needed and webserver is not needed, please refer to clidemo layer.
        
