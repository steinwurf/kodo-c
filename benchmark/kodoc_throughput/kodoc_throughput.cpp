// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <platform/config.hpp>

#include <kodoc/kodoc.h>

#include <chrono>
#include <boost/chrono.hpp>

// Use boost::chrono to get a high-precision clock on all platforms
// Note: std::chrono seems to have insufficient precision on Windows
namespace bc = boost::chrono;

/// @example kodoc_throughput.cpp
///
/// Simple example for measuring the coding throughput with the Kodo C API.


// Runs a timed encoding and decoding benchmark.
//
// @param symbols The number of original symbols in the block
// @param symbol_size The size of a symbol in bytes
// @param encoding_rate The measured encoding rate in megabytes/seconds
// @param decoding_rate The measured decoding rate in megabytes/seconds
// @return True if the original data was correctly decoded
bool run_coding_test(int32_t finite_field, uint32_t symbols,
                     uint32_t symbol_size, double& encoding_rate,
                     double& decoding_rate)
{
    // Set the random seed to randomize encoded data
    srand((uint32_t)time(NULL));

    // Here we select the coding code_type we wish to use
    int32_t code_type = kodo_full_vector;

    bc::high_resolution_clock::time_point start, stop;

    // First, we measure the combined setup time for the encoder and decoder
    start = bc::high_resolution_clock::now();

    kodo_factory_t encoder_factory =
        kodo_new_encoder_factory(code_type, finite_field, symbols, symbol_size);

    kodo_factory_t decoder_factory =
        kodo_new_decoder_factory(code_type, finite_field, symbols, symbol_size);

    kodo_coder_t encoder = kodo_factory_new_encoder(encoder_factory);
    kodo_coder_t decoder = kodo_factory_new_decoder(decoder_factory);

    // Stop the setup timer
    stop = bc::high_resolution_clock::now();

    double setup_time =
        (double)(bc::duration_cast<bc::microseconds>(stop - start).count());

    // We measure pure coding, so we always turn off the systematic mode
    if (kodo_has_set_systematic_off(encoder))
    {
        kodo_set_systematic_off(encoder);
    }

    uint32_t block_size = kodo_block_size(encoder);
    uint8_t* data_in = (uint8_t*) malloc(block_size);
    uint8_t* data_out = (uint8_t*) malloc(block_size);

    // Allocate payloads in a contiguous buffer
    uint32_t payload_size = kodo_payload_size(encoder);
    // Generate an ample number of coded symbols (considering kodo_binary)
    uint32_t payload_count = 2 * symbols;
    uint8_t* payload_buffer = (uint8_t*) malloc(payload_size * payload_count);
    uint8_t** payloads = (uint8_t**) malloc(payload_count * sizeof(uint8_t*));

    uint32_t i;
    // Set the payload pointers for easy access
    for (i = 0; i < payload_count; ++i)
    {
        payloads[i] = &payload_buffer[i * payload_size];
    }

    // Randomize input data
    for(i = 0; i < block_size; ++i)
        data_in[i] = rand() % 256;

    // Set the input and output symbol buffers
    kodo_set_const_symbols(encoder, data_in, block_size);
    kodo_set_mutable_symbols(decoder, data_out, block_size);

    // Start the encoding timer
    start = bc::high_resolution_clock::now();
    // Generate coded symbols with the encoder
    for (i = 0; i < payload_count; ++i)
    {
        kodo_write_payload(encoder, payloads[i]);
    }
    // Stop the encoding timer
    stop = bc::high_resolution_clock::now();

    double encoding_time =
        (double)(bc::duration_cast<bc::microseconds>(stop - start).count());

    // Calculate the encoding rate in megabytes / seconds
    uint32_t encoded_bytes = payload_count * symbol_size;
    encoding_rate = encoded_bytes / encoding_time;

    // Start the decoding timer
    start = bc::high_resolution_clock::now();
    // Feed the coded symbols to the decoder
    for (i = 0; !kodo_is_complete(decoder) && i < payload_count; ++i)
    {
        kodo_read_payload(decoder, payloads[i]);
    }
    // Stop the decoding timer
    stop = bc::high_resolution_clock::now();

    double decoding_time =
        (double)(bc::duration_cast<bc::microseconds>(stop - start).count());

    // Calculate the decoding rate in megabytes / seconds
    uint32_t decoded_bytes = symbols * symbol_size;
    decoding_rate = decoded_bytes / decoding_time;

    bool success;
    if (memcmp(data_in, data_out, block_size) == 0)
    {
        success = true;
    }
    else
    {
        success = false;
    }

    free(data_in);
    free(data_out);
    free(payload_buffer);
    free(payloads);

    kodo_delete_encoder(encoder);
    kodo_delete_decoder(decoder);

    kodo_delete_encoder_factory(encoder_factory);
    kodo_delete_decoder_factory(decoder_factory);

    printf("Setup time: %5.4f microsec\n", setup_time);
    printf("Encoding time: %5.4f microsec\n", encoding_time);
    printf("Decoding time: %5.4f microsec\n", decoding_time);

    return success;
}

// The main function should not be defined on Windows Phone
#if !defined(PLATFORM_WINDOWS_PHONE)
int main(int argc, const char* argv[])
{
    if (argc != 4)
    {
        printf(
            "Usage: %s [binary|binary4|binary8] "
            "symbols symbol_size\n", argv[0]);
        return 1;
    }

    // Here we select the finite field to use
    int32_t field;
    if (strcmp(argv[1], "binary") == 0)
    {
        field = kodo_binary;
    }
    else if (strcmp(argv[1], "binary4") == 0)
    {
        field = kodo_binary4;
    }
    else if (strcmp(argv[1], "binary8") == 0)
    {
        field = kodo_binary8;
    }
    else
    {
        printf("Invalid finite field: %s\n", argv[1]);
        return 1;
    }

    uint32_t symbols = atoi(argv[2]);
    uint32_t symbol_size = atoi(argv[3]);

    double encoding_rate = 0.0;
    double decoding_rate = 0.0;

    bool decoding_success =
        run_coding_test(field, symbols, symbol_size,
                        encoding_rate, decoding_rate);

    printf("Symbols: %d / Symbol_size: %d\n", symbols, symbol_size);
    printf("Encoding rate: %0.2f MB/s\n", encoding_rate);
    printf("Decoding rate: %0.2f MB/s\n", decoding_rate);

    if (decoding_success)
    {
        printf("Data decoded correctly\n");
    }
    else
    {
        printf("Decoding failed.\n");
    }

    return 0;
}
#endif
