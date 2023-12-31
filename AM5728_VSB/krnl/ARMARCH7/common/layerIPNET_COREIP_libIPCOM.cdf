 
/* this file is generated */

Layer Layer::IPNET_COREIP {

LIBRARIES += libIPCOM.a

OBJECTS libIPCOM.a::ipcom_access.o libIPCOM.a::ipcom_array.o libIPCOM.a::ipcom_atomic.o libIPCOM.a::ipcom_auth_login.o libIPCOM.a::ipcom_auth_chap.o libIPCOM.a::ipcom_bitarray.o libIPCOM.a::ipcom_buffer.o libIPCOM.a::ipcom_cache.o libIPCOM.a::ipcom_ccivalidate.o libIPCOM.a::ipcom_checksum.o libIPCOM.a::ipcom_cmd.o libIPCOM.a::ipcom_cmd_cpu.o libIPCOM.a::ipcom_cmd_echo.o libIPCOM.a::ipcom_cmd_errno.o libIPCOM.a::ipcom_cmd_fwctrl.o libIPCOM.a::ipcom_cmd_ipd.o libIPCOM.a::ipcom_cmd_mib2.o libIPCOM.a::ipcom_cmd_netsmp.o libIPCOM.a::ipcom_cmd_shellcmds.o libIPCOM.a::ipcom_cmd_smptest.o libIPCOM.a::ipcom_cmd_sockperf.o libIPCOM.a::ipcom_cmd_socktest.o libIPCOM.a::ipcom_cmd_svlanfwd.o libIPCOM.a::ipcom_cmd_syslog.o libIPCOM.a::ipcom_cmd_sysvar.o libIPCOM.a::ipcom_cmd_tracert.o libIPCOM.a::ipcom_cmd_ttcp.o libIPCOM.a::ipcom_cmd_version.o libIPCOM.a::ipcom_cmd_vxshell.o libIPCOM.a::ipcom_ctime.o libIPCOM.a::ipcom_ctype.o libIPCOM.a::ipcom_debug.o libIPCOM.a::ipcom_dmalloc.o libIPCOM.a::ipcom_dns.o libIPCOM.a::ipcom_drv_eth_filter.o libIPCOM.a::ipcom_drv_eth.o libIPCOM.a::ipcom_drv_mux.o libIPCOM.a::ipcom_drv_ppp.o libIPCOM.a::ipcom_err_string.o libIPCOM.a::ipcom_errno.o libIPCOM.a::ipcom_exit.o libIPCOM.a::ipcom_ffs.o libIPCOM.a::ipcom_file.o libIPCOM.a::ipcom_flow_fingerprint.o libIPCOM.a::ipcom_func.o libIPCOM.a::ipcom_generic_llpipe.o libIPCOM.a::ipcom_generic_pipe.o libIPCOM.a::ipcom_generic_slab.o libIPCOM.a::ipcom_getenv.o libIPCOM.a::ipcom_gethostby.o libIPCOM.a::ipcom_getinaddrprefixbyaddr.o libIPCOM.a::ipcom_getinfo.o libIPCOM.a::ipcom_getinfo2.o libIPCOM.a::ipcom_getline.o libIPCOM.a::ipcom_getopt.o libIPCOM.a::ipcom_getput.o libIPCOM.a::ipcom_getservby.o libIPCOM.a::ipcom_getsockaddrbyaddr.o libIPCOM.a::ipcom_getuid.o libIPCOM.a::ipcom_gmtime.o libIPCOM.a::ipcom_hash.o libIPCOM.a::ipcom_heap_sort.o libIPCOM.a::ipcom_hostname.o libIPCOM.a::ipcom_ifaddrs.o libIPCOM.a::ipcom_inet.o libIPCOM.a::ipcom_inet_ntop.o libIPCOM.a::ipcom_inet_pton.o libIPCOM.a::ipcom_inet6.o libIPCOM.a::ipcom_init.o libIPCOM.a::ipcom_ipd.o libIPCOM.a::ipcom_job_queue.o libIPCOM.a::ipcom_job_queue_pipe.o libIPCOM.a::ipcom_list.o libIPCOM.a::ipcom_malloc.o libIPCOM.a::ipcom_mask.o libIPCOM.a::ipcom_mcmd_ifconfig.o libIPCOM.a::ipcom_mcmd_route.o libIPCOM.a::ipcom_mktime.o libIPCOM.a::ipcom_mshell.o libIPCOM.a::ipcom_mutex.o libIPCOM.a::ipcom_netlink.o libIPCOM.a::ipcom_normpath.o libIPCOM.a::ipcom_observer.o libIPCOM.a::ipcom_once.o libIPCOM.a::ipcom_os.o libIPCOM.a::ipcom_perror.o libIPCOM.a::ipcom_pfile.o libIPCOM.a::ipcom_pipe.o libIPCOM.a::ipcom_pkt.o libIPCOM.a::ipcom_pkt_util.o libIPCOM.a::ipcom_pqueue.o libIPCOM.a::ipcom_printf.o libIPCOM.a::ipcom_prof.o libIPCOM.a::ipcom_pshell.o libIPCOM.a::ipcom_psock.o libIPCOM.a::ipcom_ramdisk.o libIPCOM.a::ipcom_random.o libIPCOM.a::ipcom_route.o libIPCOM.a::ipcom_rwsem.o libIPCOM.a::ipcom_sem.o libIPCOM.a::ipcom_set.o libIPCOM.a::ipcom_setvbuf.o libIPCOM.a::ipcom_shell.o libIPCOM.a::ipcom_shell_start.o libIPCOM.a::ipcom_shellalias.o libIPCOM.a::ipcom_shellcmd.o libIPCOM.a::ipcom_shellcmd_vxworks.o libIPCOM.a::ipcom_shellctx.o libIPCOM.a::ipcom_shellfile.o libIPCOM.a::ipcom_shellio.o libIPCOM.a::ipcom_signal.o libIPCOM.a::ipcom_snprintf.o libIPCOM.a::ipcom_sock2.o libIPCOM.a::ipcom_spinlock.o libIPCOM.a::ipcom_stdio.o libIPCOM.a::ipcom_stdio_syslog.o libIPCOM.a::ipcom_strcasecmp.o libIPCOM.a::ipcom_strdup.o libIPCOM.a::ipcom_strerror.o libIPCOM.a::ipcom_strftime.o libIPCOM.a::ipcom_string_mem.o libIPCOM.a::ipcom_strl.o libIPCOM.a::ipcom_strlib.o libIPCOM.a::ipcom_strsep.o libIPCOM.a::ipcom_strto.o libIPCOM.a::ipcom_strtok_r.o libIPCOM.a::ipcom_syslog_data.o libIPCOM.a::ipcom_syslogd.o libIPCOM.a::ipcom_sysvar.o libIPCOM.a::ipcom_sysvar_conf.o libIPCOM.a::ipcom_sysvar2.o libIPCOM.a::ipcom_telnet.o libIPCOM.a::ipcom_termios.o libIPCOM.a::ipcom_time.o libIPCOM.a::ipcom_times.o libIPCOM.a::ipcom_tmo.o libIPCOM.a::ipcom_tmo2.o libIPCOM.a::ipcom_uname.o libIPCOM.a::ipcom_usr_init.o libIPCOM.a::ipcom_vr.o libIPCOM.a::ipcom_vxworks.o libIPCOM.a::ipcom_waitif.o libIPCOM.a::ipcom_windnet_compat.o libIPCOM.a::ipcom_windview.o libIPCOM.a::ipcom_xtime.o libIPCOM.a::ipcom_xtoyz.o libIPCOM.a::zbufLib.o libIPCOM.a::zbufSockLib.o libIPCOM.a::ipcom_cfgfile.o

}


ObjectFile libIPCOM.a::ipcom_access.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_array.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_atomic.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_auth_login.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_auth_chap.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_bitarray.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_buffer.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_cache.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_ccivalidate.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_checksum.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_cmd.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_cmd_cpu.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_cmd_echo.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_cmd_errno.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_cmd_fwctrl.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_cmd_ipd.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_cmd_mib2.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_cmd_netsmp.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_cmd_shellcmds.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_cmd_smptest.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_cmd_sockperf.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_cmd_socktest.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_cmd_svlanfwd.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_cmd_syslog.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_cmd_sysvar.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_cmd_tracert.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_cmd_ttcp.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_cmd_version.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_cmd_vxshell.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_ctime.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_ctype.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_debug.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_dmalloc.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_dns.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_drv_eth_filter.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_drv_eth.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_drv_mux.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_drv_ppp.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_err_string.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_errno.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_exit.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_ffs.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_file.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_flow_fingerprint.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_func.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_generic_llpipe.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_generic_pipe.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_generic_slab.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_getenv.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_gethostby.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_getinaddrprefixbyaddr.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_getinfo.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_getinfo2.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_getline.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_getopt.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_getput.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_getservby.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_getsockaddrbyaddr.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_getuid.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_gmtime.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_hash.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_heap_sort.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_hostname.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_ifaddrs.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_inet.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_inet_ntop.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_inet_pton.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_inet6.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_init.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_ipd.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_job_queue.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_job_queue_pipe.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_list.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_malloc.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_mask.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_mcmd_ifconfig.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_mcmd_route.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_mktime.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_mshell.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_mutex.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_netlink.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_normpath.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_observer.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_once.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_os.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_perror.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_pfile.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_pipe.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_pkt.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_pkt_util.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_pqueue.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_printf.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_prof.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_pshell.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_psock.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_ramdisk.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_random.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_route.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_rwsem.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_sem.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_set.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_setvbuf.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_shell.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_shell_start.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_shellalias.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_shellcmd.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_shellcmd_vxworks.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_shellctx.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_shellfile.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_shellio.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_signal.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_snprintf.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_sock2.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_spinlock.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_stdio.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_stdio_syslog.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_strcasecmp.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_strdup.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_strerror.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_strftime.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_string_mem.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_strl.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_strlib.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_strsep.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_strto.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_strtok_r.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_syslog_data.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_syslogd.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_sysvar.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_sysvar_conf.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_sysvar2.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_telnet.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_termios.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_time.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_times.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_tmo.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_tmo2.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_uname.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_usr_init.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_vr.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_vxworks.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_waitif.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_windnet_compat.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_windview.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_xtime.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_xtoyz.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::zbufLib.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::zbufSockLib.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}
 
ObjectFile libIPCOM.a::ipcom_cfgfile.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objIPCOM/
}



