// IIS Compression Scheme DLL export function definitions.  See <httpcompression.h>

#include "zstd.h"

// Create a new compression context, called at the start of each response to be compressed.
HRESULT WINAPI CreateCompression(OUT PVOID *context, IN ULONG reserved)
{
	*context = ZSTD_createCCtx();
	return *context ? S_OK : E_FAIL;
}

// Destroy compression context, called at the end of each compressed response.
VOID WINAPI DestroyCompression(IN PVOID context)
{
    ZSTD_freeCCtx((ZSTD_CCtx*)context);
}

// Compress data, called in a loop until full response is processed.
HRESULT WINAPI Compress(
    IN OUT PVOID           context,            // compression context
    IN CONST BYTE*         input_buffer,       // input buffer
    IN LONG                input_buffer_size,  // size of input buffer
    IN PBYTE               output_buffer,      // output buffer
    IN LONG                output_buffer_size, // size of output buffer
    OUT PLONG              input_used,         // amount of input buffer used
    OUT PLONG              output_used,        // amount of output buffer used
    IN INT                 compression_level   // compression level (1..22 = negative (-1..-22), 23-45 = positive (0..22))
)
{
    // handle negative compression levels
    // 0..22 negative range, 23..45 positive range
    // (lowest)    (middle)      (highest)
    // 1 2 3 ... 22 (0 23) 24 ... 43 44 45
    int comp_lev = compression_level > 22 ? compression_level - 23 : compression_level * -1;

    if (comp_lev < ZSTD_minCLevel() || comp_lev > ZSTD_maxCLevel())
        return E_INVALIDARG;

    ZSTD_CCtx* cctx = (ZSTD_CCtx*)context;

    ZSTD_CCtx_setParameter(cctx, ZSTD_c_compressionLevel, comp_lev);

    *input_used = 0;
    *output_used = 0;

    ZSTD_inBuffer input = { input_buffer, (size_t)input_buffer_size, (size_t)*input_used };  // is this right?
    ZSTD_outBuffer output = { output_buffer, (size_t)output_buffer_size, (size_t)*output_used};

    ZSTD_EndDirective mode = input_buffer_size ? ZSTD_e_end : ZSTD_e_continue;
    size_t bytes_left = ZSTD_compressStream2(cctx, &output, &input, mode);

    if (ZSTD_isError(bytes_left))
        return E_FAIL;

    *input_used = (LONG)input.pos;  // Update input_used with the consumed input
    *output_used = (LONG)output.pos;  // Update output_used with the produced output
	// S_OK to continue looping, S_FALSE on success
	return input_buffer_size || mode == ZSTD_e_continue && (bytes_left || input.pos < input_buffer_size) ? S_OK : S_FALSE;
}