#pragma once
#pragma comment(lib, "../libdeflate/libdeflatestatic.lib")
#include "libdeflate/libdeflate.h"

struct z_stream_s
{
	const void* next_in;
	uint32_t avail_in;
	uint32_t total_in;
	void* next_out;
	uint32_t avail_out;
	uint32_t total_out;
	const char* msg;
	struct internal_state* state;
};

int hk_inflateInit(z_stream_s* Stream, const char* Version, int Mode)
{
	// Force inflateEnd to error out and skip frees
	Stream->state = nullptr;

	return 0;
}

int hk_inflate(z_stream_s* Stream, int Flush)
{
	size_t outBytes = 0;
	libdeflate_decompressor* decompressor = libdeflate_alloc_decompressor();

	libdeflate_result result = libdeflate_zlib_decompress(decompressor, Stream->next_in, Stream->avail_in, Stream->next_out, Stream->avail_out, &outBytes);
	libdeflate_free_decompressor(decompressor);

	if (result == LIBDEFLATE_SUCCESS)
	{

		Stream->total_in = Stream->avail_in;
		Stream->total_out = (uint32_t)outBytes;

		return 1;
	}

	if (result == LIBDEFLATE_INSUFFICIENT_SPACE)
		return -5;

	return -2;
}