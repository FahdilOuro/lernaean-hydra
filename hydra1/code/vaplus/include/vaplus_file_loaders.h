#ifndef vaplus_vaplus_file_loaders_h
#define vaplus_vaplus_file_loaders_h
#include "../config.h"
#include "../globals.h"
#include "ts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vaplus_index.h"
#include "calc_utils.h"

enum response vaplus_query_ascii_file(const char *ifilename, int q_num, const char delimiter, struct vaplus_index *index, float minimum_distance);
void vaplus_query_binary_file(const char *ifilename, int q_num, struct vaplus_index *index, float minimum_distance);
void vaplus_tlb_binary_file(const char *ifilename, int q_num, struct vaplus_index *index);


enum response vaplus_index_binary_file(const char *ifilename, file_position_type ts_num, struct vaplus_index *index);
enum response vaplus_index_ascii_file(const char *ifilename, file_position_type ts_num, const char delimiter, struct vaplus_index *index);
enum response reorder_query(ts_type * query_ts, ts_type * query_ts_reordered, int * query_order, int ts_length);

#endif
