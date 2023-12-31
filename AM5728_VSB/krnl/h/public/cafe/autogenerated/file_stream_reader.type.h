/* Copyright (c) 2014-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* This file has been autogenerated by cafe-type2h(1). */

#ifndef CAFE_TYPE2H_FILE_STREAM_READER_TYPE_autogenerated_h
#define CAFE_TYPE2H_FILE_STREAM_READER_TYPE_autogenerated_h

#include <cafe/autogenerated/cafe.type.h>

#ifdef __cplusplus
extern "C" {
#endif

#define file_stream_reader_client_data_VT_DEF "S[file_stream_reader.client_data,v[value,L]]"
#define file_stream_reader_client_data_VT_REF "t[file_stream_reader.client_data]"

#define file_stream_reader_end_of_file_VT_DEF "S[file_stream_reader.end_of_file]"
#define file_stream_reader_end_of_file_VT_REF "t[file_stream_reader.end_of_file]"

#define file_stream_reader_range_end_VT_DEF "S[file_stream_reader.range_end]"
#define file_stream_reader_range_end_VT_REF "t[file_stream_reader.range_end]"

#define file_stream_reader_range_start_VT_DEF "S[file_stream_reader.range_start]"
#define file_stream_reader_range_start_VT_REF "t[file_stream_reader.range_start]"

#define file_stream_reader_sequence_VT_DEF "S[file_stream_reader.sequence,v[number,Q]]"
#define file_stream_reader_sequence_VT_REF "t[file_stream_reader.sequence]"

#define file_stream_reader_start_VT_DEF "S[file_stream_reader.start]"
#define file_stream_reader_start_VT_REF "t[file_stream_reader.start]"

#define file_stream_reader_start_dispatch_VT_DEF "S[file_stream_reader.start_dispatch]"
#define file_stream_reader_start_dispatch_VT_REF "t[file_stream_reader.start_dispatch]"

#define file_stream_reader_workarea_VT_DEF "S[file_stream_reader.workarea,v[filename,s,n[cafe.setting]]v[eof_poll,L,n[cafe.setting]]v[range_start,Q,n[cafe.setting]]v[range_end,Q,n[cafe.setting]]v[start_type,s,n[cafe.setting]]v[range_start_type,s,n[cafe.setting]]v[range_end_type,s,n[cafe.setting]]v[rewind,?,n[cafe.setting],1]v[start_dispatch_type,s,n[cafe.setting]]v[client_data,L,n[cafe.setting]]v[end_of_file_type,s,n[cafe.setting]],n[cafe.output_inst_thread]]"
#define file_stream_reader_workarea_VT_REF "t[file_stream_reader.workarea]"

#define FILE_STREAM_READER_TYPE_ALL_VT_DEFS \
	file_stream_reader_client_data_VT_DEF \
	file_stream_reader_end_of_file_VT_DEF \
	file_stream_reader_range_end_VT_DEF \
	file_stream_reader_range_start_VT_DEF \
	file_stream_reader_sequence_VT_DEF \
	file_stream_reader_start_VT_DEF \
	file_stream_reader_start_dispatch_VT_DEF \
	file_stream_reader_workarea_VT_DEF \

struct file_stream_reader_client_data {
	unsigned long value;
};

struct file_stream_reader_sequence {
	unsigned long long number;
};

struct file_stream_reader_workarea {
	char *filename;
	unsigned long eof_poll;
	unsigned long long range_start;
	unsigned long long range_end;
	char *start_type;
	char *range_start_type;
	char *range_end_type;
	bool rewind;
	char *start_dispatch_type;
	unsigned long client_data;
	char *end_of_file_type;
};

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
