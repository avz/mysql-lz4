#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <mysql.h>

#include "../deps/lz4-r127/lib/lz4.h"

my_bool LZ4_COMPRESS_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
	if(args->arg_count != 1) {
		strcpy(message, "This function takes exactly 1 argument");
		return 1;
	}

	args->arg_type[0] = STRING_RESULT;
	initid->maybe_null = 1;
	initid->max_length = LZ4_MAX_INPUT_SIZE;
	initid->ptr = NULL;

	return 0;
}

void LZ4_COMPRESS_deinit(UDF_INIT *initid) {
	if(initid->ptr) {
		free(initid->ptr);
		initid->ptr = NULL;
	}
}

char *LZ4_COMPRESS(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length,
	char *is_null, char *error)
{
	size_t maxLength;
	size_t realLength;
	void *buf;

	if(!args->args[0]) {
		*is_null = 1;
		return NULL;
	}

	if(args->lengths[0] >= INT_MAX) {
		*error = 1;
		return NULL;
	}

	maxLength = (size_t)LZ4_compressBound((int)args->lengths[0]);
	if(maxLength == 0) {
		*error = 1;
		return NULL;
	}

	if(initid->ptr) {
		buf = realloc(initid->ptr, maxLength);
	} else {
		buf = malloc(maxLength);
	}

	initid->ptr = buf;

	realLength = (size_t)LZ4_compress(args->args[0], buf, (int)args->lengths[0]);

	if(!realLength) {
		*error = 1;
		return NULL;
	}

	*length = realLength;

	return buf;
}

my_bool LZ4_DECOMPRESS_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
	if(args->arg_count != 1) {
		strcpy(message, "This function takes exactly 1 argument");
		return 1;
	}

	args->arg_type[0] = STRING_RESULT;
	initid->maybe_null = 1;
	initid->max_length = LZ4_MAX_INPUT_SIZE;
	initid->ptr = NULL;

	return 0;
}

void LZ4_DECOMPRESS_deinit(UDF_INIT *initid) {
	if(initid->ptr) {
		free(initid->ptr);
		initid->ptr = NULL;
	}
}

char *LZ4_DECOMPRESS(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length,
	char *is_null, char *error)
{
	size_t maxLength;
	int realLength;
	void *buf;

	if(!args->args[0]) {
		*is_null = 1;
		return NULL;
	}

	if(!args->lengths[0]) {
		*error = 1;
		return NULL;
	}

	if(args->lengths[0] >= INT_MAX) {
		*error = 1;
		return NULL;
	}

	maxLength = args->lengths[0] * 255; /* 255 - max compress ratio for LZ4 */

	if(maxLength > INT_MAX) {
		*error = 1;
		return NULL;
	}

	if(initid->ptr) {
		buf = realloc(initid->ptr, maxLength);
	} else {
		buf = malloc(maxLength);
	}

	initid->ptr = buf;

	realLength = LZ4_decompress_safe(args->args[0], buf, (int)args->lengths[0], (int)maxLength);

	if(realLength < 0) {
		*error = 1;
		return NULL;
	}

	*length = (unsigned long)realLength;

	return buf;
}
