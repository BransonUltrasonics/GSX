Symbol hpcGetFirstBitSet {
}
Symbol printf {
}
Symbol vxCpuIndexGet {
}
Symbol intCount {
}
Symbol hpcHwArmDriverInit {
}
Symbol hpcHwArmDriverFinit {
}
Symbol hpcHwDrvArmRtns {
}
Symbol armEventSettingsGet {
}
Symbol hpcDrvEvtDescArmCtxA15 {
}
Symbol hpcArmCtxA15NumEvents {
}
Symbol hpcHwDrvArmCtxA15 {
}
Symbol vxwArmPmuIsrEnable {
}
Symbol vxwArmPmuIsrDisable {
}
Symbol hpcCafeArmDrvCfgInit {
}
Symbol getArmPmuInfo {
}
Symbol enableArmPmu {
}
Symbol resetArmPmuAllCounters {
}
Symbol disableArmPmu {
}
Symbol configArmPmuCounterEvent {
}
Symbol getArmPmuCounterEvent {
}
Symbol enableArmPmuCounter {
}
Symbol disableArmPmuCounter {
}
Symbol getArmPmuCounterState {
}
Symbol setArmPmuEvtCount {
}
Symbol getArmPmuEvtCount {
}
Symbol enableArmPmuCounterInt {
}
Symbol getArmPmuCounterIntState {
}
Symbol disableArmPmuCounterInt {
}
Symbol getArmPmuCounterOverflow {
}
Symbol clearArmPmuCounterOverflow {
}
Symbol memset {
}
Symbol hpcCafeArmDrvCfgFinit {
}
Symbol hpcCafeArmDrvCfgGet {
}
Symbol intCpuLock {
}
Symbol intCpuUnlock {
}
Symbol armEventsPrivTest {
}
Symbol hpcDrvEvtDescArmCtxA8 {
}
Symbol hpcArmCtxA8NumEvents {
}
Symbol hpcHwDrvArmCtxA8 {
}
Symbol hpcDrvEvtDescArmCtxA9 {
}
Symbol hpcArmCtxA9NumEvents {
}
Symbol hpcHwDrvArmCtxA9 {
}
Symbol hpc_count_show {
}
Symbol __mul64 {
}
Symbol __udiv64 {
}
Symbol cafe_vx_timestamp64 {
}
Symbol taskIdSelf {
}
Symbol cafe_event_action_list_read_lock {
}
Symbol cafe_dispatch_error {
}
Symbol cafe_expr_eval_value {
}
Symbol cafe_cfg_vxworks_cur_int_num_get {
}
Symbol hpcGetRegSet {
}
Symbol ST_context_create {
}
Symbol ST_trace_error {
}
Symbol ST_perform_stack_trace {
}
Symbol ST_context_delete {
}
Symbol cafe_dispatch_event_unlocked {
}
Symbol cafe_event_action_list_read_unlock {
}
Symbol hpc_verbosity {
}
Symbol memcpy {
}
Symbol hpcIterateCpus {
}
Symbol cafe_ep_inst_state_started {
}
Symbol cafe_ep_inst_state_stopped {
}
Symbol memcmp {
}
Symbol strcmp {
}
Symbol cafe_param_value_get {
}
Symbol cafe_event_action_list_swap {
}
Symbol cafe_error_bbuf_write {
}
Symbol cafe_dispatch_status {
}
Symbol cafe_error_clear {
}
Symbol cafe_ep_inst_event_action_info_get {
}
Symbol cafe_event_action_list_alloc {
}
Symbol cafe_ep_inst_get_sink_event_actions {
}
Symbol cafe_event_action_list_release {
}
Symbol cafe_error_set {
}
Symbol cafe_error_cur {
}
Symbol hpcChkEvtPeriodTaskResume {
}
Symbol hpc_unregister_configuration {
}
Symbol cafe_ep_spec_unregister {
}
Symbol free {
}
Symbol hpc_register_configuration {
}
Symbol cafe_ep_spec_register {
}
Symbol cafe_vx_timestamp_freq64 {
}
Symbol hpc_vc_check_period {
}
Symbol hpcChkEvtPeriodTaskStart {
}
Symbol __urem64 {
}
Symbol cafe_strerror {
}
Symbol __stderr {
}
Symbol fprintf {
}
Symbol __cafe_lock {
}
Symbol __cafe_unlock {
}
Symbol hpc_hw_driver_register {
}
Symbol hpc_hw_driver_unregister {
}
Symbol hpc_init {
}
Symbol malloc {
}
Symbol hpc_finit {
}
Symbol hpcChkEvtPeriodTaskDelete {
}
Symbol hpc_force_shutdown {
}
Symbol hpc_driver_list_show {
}
Symbol hpc_driver_show {
}
Symbol hpc_driver_show_all {
}
Symbol hpc_inst_show {
}
Symbol hpc_vc_show {
}
Symbol puts {
}
Symbol hpc_test {
}
Symbol hpcExecuteOnSingleCpu {
}
Symbol taskSuspend {
}
Symbol taskDelay {
}
Symbol semGive {
}
Symbol semBCreate {
}
Symbol sysClkRateGet {
}
Symbol __div32 {
}
Symbol taskSpawn {
}
Symbol taskResume {
}
Symbol semTake {
}
Symbol __errno {
}
Symbol semDelete {
}
Symbol hpc_log_debug_log_msg {
}
Symbol logMsg {
}
Symbol hpc_log_debug {
}
Symbol hpc_log_debug_isr {
}
Symbol hpc_log_error_log_msg {
}
Symbol hpc_log_error {
}
ObjectFile libruntime_analysis_hpc.a::hpcHwDrvUtils.o {
	NAME Object file hpcHwDrvUtils.o from archive libruntime_analysis_hpc.a
	EXPORTS hpcGetFirstBitSet
	IMPORTS 
	DECLARES 
	USES 
}
Module hpcHwDrvUtils.o {
	OBJECT += libruntime_analysis_hpc.a::hpcHwDrvUtils.o
}
ObjectFile libruntime_analysis_hpc.a::armDrv.o {
	NAME Object file armDrv.o from archive libruntime_analysis_hpc.a
	EXPORTS hpcHwArmDriverInit hpcHwArmDriverFinit hpcHwDrvArmRtns
	IMPORTS printf vxCpuIndexGet intCount hpcGetFirstBitSet
	DECLARES 
	USES 
}
Module armDrv.o {
	OBJECT += libruntime_analysis_hpc.a::armDrv.o
}
ObjectFile libruntime_analysis_hpc.a::armDrvSet.o {
	NAME Object file armDrvSet.o from archive libruntime_analysis_hpc.a
	EXPORTS armEventSettingsGet
	IMPORTS 
	DECLARES 
	USES 
}
Module armDrvSet.o {
	OBJECT += libruntime_analysis_hpc.a::armDrvSet.o
}
ObjectFile libruntime_analysis_hpc.a::armCtxA15EvtCfg.o {
	NAME Object file armCtxA15EvtCfg.o from archive libruntime_analysis_hpc.a
	EXPORTS hpcDrvEvtDescArmCtxA15 hpcArmCtxA15NumEvents hpcHwDrvArmCtxA15
	IMPORTS 
	DECLARES 
	USES 
}
Module armCtxA15EvtCfg.o {
	OBJECT += libruntime_analysis_hpc.a::armCtxA15EvtCfg.o
}
ObjectFile libruntime_analysis_hpc.a::armDrvCfg.o {
	NAME Object file armDrvCfg.o from archive libruntime_analysis_hpc.a
	EXPORTS hpcCafeArmDrvCfgInit hpcCafeArmDrvCfgFinit hpcCafeArmDrvCfgGet armEventsPrivTest
	IMPORTS vxwArmPmuIsrEnable vxwArmPmuIsrDisable getArmPmuInfo enableArmPmu resetArmPmuAllCounters disableArmPmu configArmPmuCounterEvent getArmPmuCounterEvent enableArmPmuCounter disableArmPmuCounter getArmPmuCounterState setArmPmuEvtCount getArmPmuEvtCount enableArmPmuCounterInt getArmPmuCounterIntState disableArmPmuCounterInt getArmPmuCounterOverflow clearArmPmuCounterOverflow hpcHwDrvArmRtns armEventSettingsGet memset intCpuLock vxCpuIndexGet intCpuUnlock
	DECLARES 
	USES 
}
Module armDrvCfg.o {
	OBJECT += libruntime_analysis_hpc.a::armDrvCfg.o
}
ObjectFile libruntime_analysis_hpc.a::armCtxA8EvtCfg.o {
	NAME Object file armCtxA8EvtCfg.o from archive libruntime_analysis_hpc.a
	EXPORTS hpcDrvEvtDescArmCtxA8 hpcArmCtxA8NumEvents hpcHwDrvArmCtxA8
	IMPORTS 
	DECLARES 
	USES 
}
Module armCtxA8EvtCfg.o {
	OBJECT += libruntime_analysis_hpc.a::armCtxA8EvtCfg.o
}
ObjectFile libruntime_analysis_hpc.a::armCtxA9EvtCfg.o {
	NAME Object file armCtxA9EvtCfg.o from archive libruntime_analysis_hpc.a
	EXPORTS hpcDrvEvtDescArmCtxA9 hpcArmCtxA9NumEvents hpcHwDrvArmCtxA9
	IMPORTS 
	DECLARES 
	USES 
}
Module armCtxA9EvtCfg.o {
	OBJECT += libruntime_analysis_hpc.a::armCtxA9EvtCfg.o
}
ObjectFile libruntime_analysis_hpc.a::hpc_ep.o {
	NAME Object file hpc_ep.o from archive libruntime_analysis_hpc.a
	EXPORTS hpc_count_show hpc_unregister_configuration hpc_register_configuration hpc_vc_check_period hpc_hw_driver_register hpc_hw_driver_unregister hpc_init hpc_finit hpc_force_shutdown hpc_driver_list_show hpc_driver_show hpc_driver_show_all hpc_inst_show hpc_vc_show hpc_test
	IMPORTS printf __mul64 __udiv64 cafe_vx_timestamp64 taskIdSelf intCount cafe_event_action_list_read_lock cafe_dispatch_error cafe_expr_eval_value vxCpuIndexGet cafe_cfg_vxworks_cur_int_num_get memset hpcGetRegSet ST_context_create ST_trace_error ST_perform_stack_trace ST_context_delete cafe_dispatch_event_unlocked cafe_event_action_list_read_unlock hpc_verbosity memcpy hpcIterateCpus cafe_ep_inst_state_started cafe_ep_inst_state_stopped memcmp strcmp cafe_param_value_get cafe_event_action_list_swap cafe_error_bbuf_write cafe_dispatch_status cafe_error_clear cafe_ep_inst_event_action_info_get cafe_event_action_list_alloc cafe_ep_inst_get_sink_event_actions cafe_event_action_list_release cafe_error_set hpcChkEvtPeriodTaskResume cafe_ep_spec_unregister free cafe_ep_spec_register cafe_vx_timestamp_freq64 hpcChkEvtPeriodTaskStart __urem64 cafe_strerror __stderr fprintf __cafe_lock __cafe_unlock malloc hpcChkEvtPeriodTaskDelete puts
	DECLARES cafe_error_cur
	USES 
}
Module hpc_ep.o {
	OBJECT += libruntime_analysis_hpc.a::hpc_ep.o
}
ObjectFile libruntime_analysis_hpc.a::hpc_utils.o {
	NAME Object file hpc_utils.o from archive libruntime_analysis_hpc.a
	EXPORTS hpcExecuteOnSingleCpu hpcIterateCpus hpcChkEvtPeriodTaskStart hpcChkEvtPeriodTaskResume hpcChkEvtPeriodTaskDelete hpcGetRegSet hpc_log_debug_log_msg hpc_verbosity hpc_log_debug hpc_log_debug_isr hpc_log_error_log_msg hpc_log_error
	IMPORTS taskSuspend taskDelay semGive semBCreate sysClkRateGet __div32 taskSpawn taskResume semTake __errno semDelete logMsg
	DECLARES 
	USES 
}
Module hpc_utils.o {
	OBJECT += libruntime_analysis_hpc.a::hpc_utils.o
}
Library libruntime_analysis_hpc.a {
	MODULES libruntime_analysis_hpc.a::hpcHwDrvUtils.o libruntime_analysis_hpc.a::armDrv.o libruntime_analysis_hpc.a::armDrvSet.o libruntime_analysis_hpc.a::armCtxA15EvtCfg.o libruntime_analysis_hpc.a::armDrvCfg.o libruntime_analysis_hpc.a::armCtxA8EvtCfg.o libruntime_analysis_hpc.a::armCtxA9EvtCfg.o libruntime_analysis_hpc.a::hpc_ep.o libruntime_analysis_hpc.a::hpc_utils.o
}
