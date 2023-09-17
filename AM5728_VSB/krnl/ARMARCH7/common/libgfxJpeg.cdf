Symbol jpeg_CreateCompress {
}
Symbol memset {
}
Symbol jinit_memory_mgr {
}
Symbol jpeg_destroy_compress {
}
Symbol jpeg_destroy {
}
Symbol jpeg_abort_compress {
}
Symbol jpeg_abort {
}
Symbol jpeg_suppress_tables {
}
Symbol jpeg_finish_compress {
}
Symbol jpeg_write_marker {
}
Symbol jpeg_write_m_header {
}
Symbol jpeg_write_m_byte {
}
Symbol jpeg_write_tables {
}
Symbol jinit_marker_writer {
}
Symbol jpeg_start_compress {
}
Symbol jinit_compress_master {
}
Symbol jpeg_write_scanlines {
}
Symbol jpeg_write_raw_data {
}
Symbol jzero_far {
}
Symbol __udiv32 {
}
Symbol jinit_c_coef_controller {
}
Symbol jround_up {
}
Symbol jinit_color_converter {
}
Symbol __div32 {
}
Symbol jinit_forward_dct {
}
Symbol jpeg_fdct_islow {
}
Symbol jpeg_fdct_ifast {
}
Symbol jpeg_fdct_float {
}
Symbol jpeg_make_c_derived_tbl {
}
Symbol jpeg_natural_order {
}
Symbol jpeg_gen_optimal_table {
}
Symbol memcpy {
}
Symbol jpeg_alloc_huff_table {
}
Symbol jinit_huff_encoder {
}
Symbol jinit_c_master_control {
}
Symbol jinit_downsampler {
}
Symbol jinit_c_prep_controller {
}
Symbol jinit_phuff_encoder {
}
Symbol jinit_c_main_controller {
}
Symbol jdiv_round_up {
}
Symbol jpeg_alloc_quant_table {
}
Symbol jpeg_add_quant_table {
}
Symbol jpeg_set_linear_quality {
}
Symbol jpeg_quality_scaling {
}
Symbol jpeg_set_quality {
}
Symbol jpeg_set_colorspace {
}
Symbol jpeg_default_colorspace {
}
Symbol jpeg_set_defaults {
}
Symbol jpeg_simple_progression {
}
Symbol jcopy_sample_rows {
}
Symbol jpeg_write_coefficients {
}
Symbol jpeg_copy_critical_parameters {
}
Symbol jpeg_CreateDecompress {
}
Symbol jinit_marker_reader {
}
Symbol jinit_input_controller {
}
Symbol jpeg_destroy_decompress {
}
Symbol jpeg_abort_decompress {
}
Symbol jpeg_consume_input {
}
Symbol jpeg_read_header {
}
Symbol jpeg_input_complete {
}
Symbol jpeg_has_multiple_scans {
}
Symbol jpeg_finish_decompress {
}
Symbol jpeg_start_decompress {
}
Symbol jinit_master_decompress {
}
Symbol jpeg_read_scanlines {
}
Symbol jpeg_read_raw_data {
}
Symbol jpeg_start_output {
}
Symbol jpeg_finish_output {
}
Symbol fwrite {
}
Symbol fflush {
}
Symbol jpeg_stdio_dest {
}
Symbol fread {
}
Symbol jpeg_stdio_src {
}
Symbol jpeg_resync_to_restart {
}
Symbol jcopy_block_row {
}
Symbol jinit_d_coef_controller {
}
Symbol jinit_color_deconverter {
}
Symbol jpeg_idct_1x1 {
}
Symbol jpeg_idct_2x2 {
}
Symbol jpeg_idct_4x4 {
}
Symbol jpeg_idct_islow {
}
Symbol jpeg_idct_ifast {
}
Symbol jpeg_idct_float {
}
Symbol jinit_inverse_dct {
}
Symbol jpeg_make_d_derived_tbl {
}
Symbol jpeg_fill_bit_buffer {
}
Symbol jpeg_huff_decode {
}
Symbol jinit_huff_decoder {
}
Symbol jinit_d_main_controller {
}
Symbol jpeg_save_markers {
}
Symbol jpeg_set_marker_processor {
}
Symbol jpeg_calc_output_dimensions {
}
Symbol jinit_1pass_quantizer {
}
Symbol jinit_2pass_quantizer {
}
Symbol jinit_merged_upsampler {
}
Symbol jinit_upsampler {
}
Symbol jinit_d_post_controller {
}
Symbol jinit_phuff_decoder {
}
Symbol jpeg_new_colormap {
}
Symbol jpeg_read_coefficients {
}
Symbol exit {
}
Symbol __stderr {
}
Symbol fprintf {
}
Symbol sprintf {
}
Symbol jpeg_std_error {
}
Symbol jpeg_std_message_table {
}
Symbol jpeg_get_small {
}
Symbol malloc {
}
Symbol jpeg_free_small {
}
Symbol free {
}
Symbol jpeg_get_large {
}
Symbol jpeg_free_large {
}
Symbol jpeg_mem_available {
}
Symbol fseek {
}
Symbol fclose {
}
Symbol jpeg_open_backing_store {
}
Symbol tmpfile {
}
Symbol strncpy {
}
Symbol jpeg_mem_init {
}
Symbol jpeg_mem_term {
}
ObjectFile libgfxJpeg.a::jcapimin.o {
	NAME Object file jcapimin.o from archive libgfxJpeg.a
	EXPORTS jpeg_CreateCompress jpeg_destroy_compress jpeg_abort_compress jpeg_suppress_tables jpeg_finish_compress jpeg_write_marker jpeg_write_m_header jpeg_write_m_byte jpeg_write_tables
	IMPORTS memset jinit_memory_mgr jpeg_destroy jpeg_abort jinit_marker_writer
	DECLARES 
	USES 
}
Module jcapimin.o {
	OBJECT += libgfxJpeg.a::jcapimin.o
}
ObjectFile libgfxJpeg.a::jcapistd.o {
	NAME Object file jcapistd.o from archive libgfxJpeg.a
	EXPORTS jpeg_start_compress jpeg_write_scanlines jpeg_write_raw_data
	IMPORTS jpeg_suppress_tables jinit_compress_master
	DECLARES 
	USES 
}
Module jcapistd.o {
	OBJECT += libgfxJpeg.a::jcapistd.o
}
ObjectFile libgfxJpeg.a::jccoefct.o {
	NAME Object file jccoefct.o from archive libgfxJpeg.a
	EXPORTS jinit_c_coef_controller
	IMPORTS jzero_far __udiv32 jround_up
	DECLARES 
	USES 
}
Module jccoefct.o {
	OBJECT += libgfxJpeg.a::jccoefct.o
}
ObjectFile libgfxJpeg.a::jccolor.o {
	NAME Object file jccolor.o from archive libgfxJpeg.a
	EXPORTS jinit_color_converter
	IMPORTS 
	DECLARES 
	USES 
}
Module jccolor.o {
	OBJECT += libgfxJpeg.a::jccolor.o
}
ObjectFile libgfxJpeg.a::jcdctmgr.o {
	NAME Object file jcdctmgr.o from archive libgfxJpeg.a
	EXPORTS jinit_forward_dct
	IMPORTS __div32 jpeg_fdct_islow jpeg_fdct_ifast jpeg_fdct_float
	DECLARES 
	USES 
}
Module jcdctmgr.o {
	OBJECT += libgfxJpeg.a::jcdctmgr.o
}
ObjectFile libgfxJpeg.a::jchuff.o {
	NAME Object file jchuff.o from archive libgfxJpeg.a
	EXPORTS jpeg_make_c_derived_tbl jpeg_gen_optimal_table jinit_huff_encoder
	IMPORTS memset jpeg_natural_order memcpy jpeg_alloc_huff_table
	DECLARES 
	USES 
}
Module jchuff.o {
	OBJECT += libgfxJpeg.a::jchuff.o
}
ObjectFile libgfxJpeg.a::jcinit.o {
	NAME Object file jcinit.o from archive libgfxJpeg.a
	EXPORTS jinit_compress_master
	IMPORTS jinit_c_master_control jinit_color_converter jinit_downsampler jinit_c_prep_controller jinit_forward_dct jinit_phuff_encoder jinit_huff_encoder jinit_c_coef_controller jinit_c_main_controller jinit_marker_writer
	DECLARES 
	USES 
}
Module jcinit.o {
	OBJECT += libgfxJpeg.a::jcinit.o
}
ObjectFile libgfxJpeg.a::jcmainct.o {
	NAME Object file jcmainct.o from archive libgfxJpeg.a
	EXPORTS jinit_c_main_controller
	IMPORTS 
	DECLARES 
	USES 
}
Module jcmainct.o {
	OBJECT += libgfxJpeg.a::jcmainct.o
}
ObjectFile libgfxJpeg.a::jcmarker.o {
	NAME Object file jcmarker.o from archive libgfxJpeg.a
	EXPORTS jinit_marker_writer
	IMPORTS jpeg_natural_order
	DECLARES 
	USES 
}
Module jcmarker.o {
	OBJECT += libgfxJpeg.a::jcmarker.o
}
ObjectFile libgfxJpeg.a::jcmaster.o {
	NAME Object file jcmaster.o from archive libgfxJpeg.a
	EXPORTS jinit_c_master_control
	IMPORTS jdiv_round_up __udiv32
	DECLARES 
	USES 
}
Module jcmaster.o {
	OBJECT += libgfxJpeg.a::jcmaster.o
}
ObjectFile libgfxJpeg.a::jcomapi.o {
	NAME Object file jcomapi.o from archive libgfxJpeg.a
	EXPORTS jpeg_abort jpeg_destroy jpeg_alloc_quant_table jpeg_alloc_huff_table
	IMPORTS 
	DECLARES 
	USES 
}
Module jcomapi.o {
	OBJECT += libgfxJpeg.a::jcomapi.o
}
ObjectFile libgfxJpeg.a::jcparam.o {
	NAME Object file jcparam.o from archive libgfxJpeg.a
	EXPORTS jpeg_add_quant_table jpeg_set_linear_quality jpeg_quality_scaling jpeg_set_quality jpeg_set_colorspace jpeg_default_colorspace jpeg_set_defaults jpeg_simple_progression
	IMPORTS jpeg_alloc_quant_table __div32 jpeg_alloc_huff_table memcpy
	DECLARES 
	USES 
}
Module jcparam.o {
	OBJECT += libgfxJpeg.a::jcparam.o
}
ObjectFile libgfxJpeg.a::jcphuff.o {
	NAME Object file jcphuff.o from archive libgfxJpeg.a
	EXPORTS jinit_phuff_encoder
	IMPORTS memset jpeg_make_c_derived_tbl jpeg_natural_order jpeg_alloc_huff_table jpeg_gen_optimal_table
	DECLARES 
	USES 
}
Module jcphuff.o {
	OBJECT += libgfxJpeg.a::jcphuff.o
}
ObjectFile libgfxJpeg.a::jcprepct.o {
	NAME Object file jcprepct.o from archive libgfxJpeg.a
	EXPORTS jinit_c_prep_controller
	IMPORTS jcopy_sample_rows __div32 memcpy
	DECLARES 
	USES 
}
Module jcprepct.o {
	OBJECT += libgfxJpeg.a::jcprepct.o
}
ObjectFile libgfxJpeg.a::jcsample.o {
	NAME Object file jcsample.o from archive libgfxJpeg.a
	EXPORTS jinit_downsampler
	IMPORTS __div32 jcopy_sample_rows
	DECLARES 
	USES 
}
Module jcsample.o {
	OBJECT += libgfxJpeg.a::jcsample.o
}
ObjectFile libgfxJpeg.a::jctrans.o {
	NAME Object file jctrans.o from archive libgfxJpeg.a
	EXPORTS jpeg_write_coefficients jpeg_copy_critical_parameters
	IMPORTS jzero_far jinit_c_master_control jinit_phuff_encoder jinit_huff_encoder jinit_marker_writer jpeg_suppress_tables jpeg_set_defaults jpeg_set_colorspace jpeg_alloc_quant_table memcpy
	DECLARES 
	USES 
}
Module jctrans.o {
	OBJECT += libgfxJpeg.a::jctrans.o
}
ObjectFile libgfxJpeg.a::jdapimin.o {
	NAME Object file jdapimin.o from archive libgfxJpeg.a
	EXPORTS jpeg_CreateDecompress jpeg_destroy_decompress jpeg_abort_decompress jpeg_consume_input jpeg_read_header jpeg_input_complete jpeg_has_multiple_scans jpeg_finish_decompress
	IMPORTS memset jinit_memory_mgr jinit_marker_reader jinit_input_controller jpeg_destroy jpeg_abort
	DECLARES 
	USES 
}
Module jdapimin.o {
	OBJECT += libgfxJpeg.a::jdapimin.o
}
ObjectFile libgfxJpeg.a::jdapistd.o {
	NAME Object file jdapistd.o from archive libgfxJpeg.a
	EXPORTS jpeg_start_decompress jpeg_read_scanlines jpeg_read_raw_data jpeg_start_output jpeg_finish_output
	IMPORTS jinit_master_decompress
	DECLARES 
	USES 
}
Module jdapistd.o {
	OBJECT += libgfxJpeg.a::jdapistd.o
}
ObjectFile libgfxJpeg.a::jdatadst.o {
	NAME Object file jdatadst.o from archive libgfxJpeg.a
	EXPORTS jpeg_stdio_dest
	IMPORTS fwrite fflush
	DECLARES 
	USES 
}
Module jdatadst.o {
	OBJECT += libgfxJpeg.a::jdatadst.o
}
ObjectFile libgfxJpeg.a::jdatasrc.o {
	NAME Object file jdatasrc.o from archive libgfxJpeg.a
	EXPORTS jpeg_stdio_src
	IMPORTS fread jpeg_resync_to_restart
	DECLARES 
	USES 
}
Module jdatasrc.o {
	OBJECT += libgfxJpeg.a::jdatasrc.o
}
ObjectFile libgfxJpeg.a::jdcoefct.o {
	NAME Object file jdcoefct.o from archive libgfxJpeg.a
	EXPORTS jinit_d_coef_controller
	IMPORTS jzero_far __udiv32 jcopy_block_row __div32 jround_up
	DECLARES 
	USES 
}
Module jdcoefct.o {
	OBJECT += libgfxJpeg.a::jdcoefct.o
}
ObjectFile libgfxJpeg.a::jdcolor.o {
	NAME Object file jdcolor.o from archive libgfxJpeg.a
	EXPORTS jinit_color_deconverter
	IMPORTS jcopy_sample_rows
	DECLARES 
	USES 
}
Module jdcolor.o {
	OBJECT += libgfxJpeg.a::jdcolor.o
}
ObjectFile libgfxJpeg.a::jddctmgr.o {
	NAME Object file jddctmgr.o from archive libgfxJpeg.a
	EXPORTS jinit_inverse_dct
	IMPORTS jpeg_idct_1x1 jpeg_idct_2x2 jpeg_idct_4x4 jpeg_idct_islow jpeg_idct_ifast jpeg_idct_float memset
	DECLARES 
	USES 
}
Module jddctmgr.o {
	OBJECT += libgfxJpeg.a::jddctmgr.o
}
ObjectFile libgfxJpeg.a::jdhuff.o {
	NAME Object file jdhuff.o from archive libgfxJpeg.a
	EXPORTS jpeg_make_d_derived_tbl jpeg_fill_bit_buffer jpeg_huff_decode jinit_huff_decoder
	IMPORTS memset __div32 jpeg_natural_order
	DECLARES 
	USES 
}
Module jdhuff.o {
	OBJECT += libgfxJpeg.a::jdhuff.o
}
ObjectFile libgfxJpeg.a::jdinput.o {
	NAME Object file jdinput.o from archive libgfxJpeg.a
	EXPORTS jinit_input_controller
	IMPORTS jdiv_round_up __udiv32 memcpy
	DECLARES 
	USES 
}
Module jdinput.o {
	OBJECT += libgfxJpeg.a::jdinput.o
}
ObjectFile libgfxJpeg.a::jdmainct.o {
	NAME Object file jdmainct.o from archive libgfxJpeg.a
	EXPORTS jinit_d_main_controller
	IMPORTS __div32 __udiv32
	DECLARES 
	USES 
}
Module jdmainct.o {
	OBJECT += libgfxJpeg.a::jdmainct.o
}
ObjectFile libgfxJpeg.a::jdmarker.o {
	NAME Object file jdmarker.o from archive libgfxJpeg.a
	EXPORTS jpeg_resync_to_restart jinit_marker_reader jpeg_save_markers jpeg_set_marker_processor
	IMPORTS jpeg_alloc_huff_table memcpy jpeg_alloc_quant_table jpeg_natural_order
	DECLARES 
	USES 
}
Module jdmarker.o {
	OBJECT += libgfxJpeg.a::jdmarker.o
}
ObjectFile libgfxJpeg.a::jdmaster.o {
	NAME Object file jdmaster.o from archive libgfxJpeg.a
	EXPORTS jpeg_calc_output_dimensions jpeg_new_colormap jinit_master_decompress
	IMPORTS jdiv_round_up memset memcpy jinit_1pass_quantizer jinit_2pass_quantizer jinit_merged_upsampler jinit_color_deconverter jinit_upsampler jinit_d_post_controller jinit_inverse_dct jinit_phuff_decoder jinit_huff_decoder jinit_d_coef_controller jinit_d_main_controller
	DECLARES 
	USES 
}
Module jdmaster.o {
	OBJECT += libgfxJpeg.a::jdmaster.o
}
ObjectFile libgfxJpeg.a::jdmerge.o {
	NAME Object file jdmerge.o from archive libgfxJpeg.a
	EXPORTS jinit_merged_upsampler
	IMPORTS jcopy_sample_rows
	DECLARES 
	USES 
}
Module jdmerge.o {
	OBJECT += libgfxJpeg.a::jdmerge.o
}
ObjectFile libgfxJpeg.a::jdphuff.o {
	NAME Object file jdphuff.o from archive libgfxJpeg.a
	EXPORTS jinit_phuff_decoder
	IMPORTS jpeg_make_d_derived_tbl __div32 jpeg_fill_bit_buffer jpeg_huff_decode jpeg_natural_order
	DECLARES 
	USES 
}
Module jdphuff.o {
	OBJECT += libgfxJpeg.a::jdphuff.o
}
ObjectFile libgfxJpeg.a::jdpostct.o {
	NAME Object file jdpostct.o from archive libgfxJpeg.a
	EXPORTS jinit_d_post_controller
	IMPORTS jround_up
	DECLARES 
	USES 
}
Module jdpostct.o {
	OBJECT += libgfxJpeg.a::jdpostct.o
}
ObjectFile libgfxJpeg.a::jdsample.o {
	NAME Object file jdsample.o from archive libgfxJpeg.a
	EXPORTS jinit_upsampler
	IMPORTS jcopy_sample_rows __div32 jround_up
	DECLARES 
	USES 
}
Module jdsample.o {
	OBJECT += libgfxJpeg.a::jdsample.o
}
ObjectFile libgfxJpeg.a::jdtrans.o {
	NAME Object file jdtrans.o from archive libgfxJpeg.a
	EXPORTS jpeg_read_coefficients
	IMPORTS jinit_phuff_decoder jinit_huff_decoder jinit_d_coef_controller
	DECLARES 
	USES 
}
Module jdtrans.o {
	OBJECT += libgfxJpeg.a::jdtrans.o
}
ObjectFile libgfxJpeg.a::jerror.o {
	NAME Object file jerror.o from archive libgfxJpeg.a
	EXPORTS jpeg_std_error jpeg_std_message_table
	IMPORTS jpeg_destroy exit __stderr fprintf sprintf
	DECLARES 
	USES 
}
Module jerror.o {
	OBJECT += libgfxJpeg.a::jerror.o
}
ObjectFile libgfxJpeg.a::jfdctflt.o {
	NAME Object file jfdctflt.o from archive libgfxJpeg.a
	EXPORTS jpeg_fdct_float
	IMPORTS 
	DECLARES 
	USES 
}
Module jfdctflt.o {
	OBJECT += libgfxJpeg.a::jfdctflt.o
}
ObjectFile libgfxJpeg.a::jfdctfst.o {
	NAME Object file jfdctfst.o from archive libgfxJpeg.a
	EXPORTS jpeg_fdct_ifast
	IMPORTS 
	DECLARES 
	USES 
}
Module jfdctfst.o {
	OBJECT += libgfxJpeg.a::jfdctfst.o
}
ObjectFile libgfxJpeg.a::jfdctint.o {
	NAME Object file jfdctint.o from archive libgfxJpeg.a
	EXPORTS jpeg_fdct_islow
	IMPORTS 
	DECLARES 
	USES 
}
Module jfdctint.o {
	OBJECT += libgfxJpeg.a::jfdctint.o
}
ObjectFile libgfxJpeg.a::jidctflt.o {
	NAME Object file jidctflt.o from archive libgfxJpeg.a
	EXPORTS jpeg_idct_float
	IMPORTS 
	DECLARES 
	USES 
}
Module jidctflt.o {
	OBJECT += libgfxJpeg.a::jidctflt.o
}
ObjectFile libgfxJpeg.a::jidctfst.o {
	NAME Object file jidctfst.o from archive libgfxJpeg.a
	EXPORTS jpeg_idct_ifast
	IMPORTS 
	DECLARES 
	USES 
}
Module jidctfst.o {
	OBJECT += libgfxJpeg.a::jidctfst.o
}
ObjectFile libgfxJpeg.a::jidctint.o {
	NAME Object file jidctint.o from archive libgfxJpeg.a
	EXPORTS jpeg_idct_islow
	IMPORTS 
	DECLARES 
	USES 
}
Module jidctint.o {
	OBJECT += libgfxJpeg.a::jidctint.o
}
ObjectFile libgfxJpeg.a::jidctred.o {
	NAME Object file jidctred.o from archive libgfxJpeg.a
	EXPORTS jpeg_idct_4x4 jpeg_idct_2x2 jpeg_idct_1x1
	IMPORTS 
	DECLARES 
	USES 
}
Module jidctred.o {
	OBJECT += libgfxJpeg.a::jidctred.o
}
ObjectFile libgfxJpeg.a::jmemansi.o {
	NAME Object file jmemansi.o from archive libgfxJpeg.a
	EXPORTS jpeg_get_small jpeg_free_small jpeg_get_large jpeg_free_large jpeg_mem_available jpeg_open_backing_store jpeg_mem_init jpeg_mem_term
	IMPORTS malloc free fseek fread fwrite fclose tmpfile strncpy
	DECLARES 
	USES 
}
Module jmemansi.o {
	OBJECT += libgfxJpeg.a::jmemansi.o
}
ObjectFile libgfxJpeg.a::jmemmgr.o {
	NAME Object file jmemmgr.o from archive libgfxJpeg.a
	EXPORTS jinit_memory_mgr
	IMPORTS jpeg_get_small jpeg_get_large __udiv32 jpeg_mem_available __div32 jpeg_open_backing_store jzero_far jpeg_free_large jpeg_free_small jpeg_mem_term jpeg_mem_init
	DECLARES 
	USES 
}
Module jmemmgr.o {
	OBJECT += libgfxJpeg.a::jmemmgr.o
}
ObjectFile libgfxJpeg.a::jquant1.o {
	NAME Object file jquant1.o from archive libgfxJpeg.a
	EXPORTS jinit_1pass_quantizer
	IMPORTS __div32 jzero_far
	DECLARES 
	USES 
}
Module jquant1.o {
	OBJECT += libgfxJpeg.a::jquant1.o
}
ObjectFile libgfxJpeg.a::jquant2.o {
	NAME Object file jquant2.o from archive libgfxJpeg.a
	EXPORTS jinit_2pass_quantizer
	IMPORTS __div32 jzero_far
	DECLARES 
	USES 
}
Module jquant2.o {
	OBJECT += libgfxJpeg.a::jquant2.o
}
ObjectFile libgfxJpeg.a::jutils.o {
	NAME Object file jutils.o from archive libgfxJpeg.a
	EXPORTS jdiv_round_up jround_up jcopy_sample_rows jcopy_block_row jzero_far jpeg_natural_order
	IMPORTS __div32 memcpy memset
	DECLARES 
	USES 
}
Module jutils.o {
	OBJECT += libgfxJpeg.a::jutils.o
}
Library libgfxJpeg.a {
	MODULES libgfxJpeg.a::jcapimin.o libgfxJpeg.a::jcapistd.o libgfxJpeg.a::jccoefct.o libgfxJpeg.a::jccolor.o libgfxJpeg.a::jcdctmgr.o libgfxJpeg.a::jchuff.o libgfxJpeg.a::jcinit.o libgfxJpeg.a::jcmainct.o libgfxJpeg.a::jcmarker.o libgfxJpeg.a::jcmaster.o libgfxJpeg.a::jcomapi.o libgfxJpeg.a::jcparam.o libgfxJpeg.a::jcphuff.o libgfxJpeg.a::jcprepct.o libgfxJpeg.a::jcsample.o libgfxJpeg.a::jctrans.o libgfxJpeg.a::jdapimin.o libgfxJpeg.a::jdapistd.o libgfxJpeg.a::jdatadst.o libgfxJpeg.a::jdatasrc.o libgfxJpeg.a::jdcoefct.o libgfxJpeg.a::jdcolor.o libgfxJpeg.a::jddctmgr.o libgfxJpeg.a::jdhuff.o libgfxJpeg.a::jdinput.o libgfxJpeg.a::jdmainct.o libgfxJpeg.a::jdmarker.o libgfxJpeg.a::jdmaster.o libgfxJpeg.a::jdmerge.o libgfxJpeg.a::jdphuff.o libgfxJpeg.a::jdpostct.o libgfxJpeg.a::jdsample.o libgfxJpeg.a::jdtrans.o libgfxJpeg.a::jerror.o libgfxJpeg.a::jfdctflt.o libgfxJpeg.a::jfdctfst.o libgfxJpeg.a::jfdctint.o libgfxJpeg.a::jidctflt.o libgfxJpeg.a::jidctfst.o libgfxJpeg.a::jidctint.o libgfxJpeg.a::jidctred.o libgfxJpeg.a::jmemansi.o libgfxJpeg.a::jmemmgr.o libgfxJpeg.a::jquant1.o libgfxJpeg.a::jquant2.o libgfxJpeg.a::jutils.o
}
