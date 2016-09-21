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

/// Struct containing the results for a single measurement run
struct results
{
    bool success;
    double setup_time;
    double encoding_time;
    double decoding_time;
    double encoding_rate;
    double decoding_rate;
};

// Runs a timed encoding and decoding benchmark.
//
// @param finite_field The finite field used in the benchmark
// @param symbols The number of original symbols in the block
// @param symbol_size The size of a symbol in bytes
// @return The results struct containing all metrics
results run_coding_test(int32_t finite_field, uint32_t symbols,
                        uint32_t symbol_size)
{
    results run;

    // Set the random seed to randomize encoded data
    srand((uint32_t)time(NULL));

    // Here we select the codec we wish to use
    int32_t codec = kodoc_full_vector;

    bc::high_resolution_clock::time_point start, stop;

    // First, we measure the combined setup time for the encoder and decoder
    start = bc::high_resolution_clock::now();

    kodoc_factory_t encoder_factory =
        kodoc_new_encoder_factory(codec, finite_field, symbols, symbol_size);

    kodoc_factory_t decoder_factory =
        kodoc_new_decoder_factory(codec, finite_field, symbols, symbol_size);

    kodoc_coder_t encoder = kodoc_factory_build_coder(encoder_factory);
    kodoc_coder_t decoder = kodoc_factory_build_coder(decoder_factory);

    // Stop the setup timer
    stop = bc::high_resolution_clock::now();

    run.setup_time =
        (double)(bc::duration_cast<bc::microseconds>(stop - start).count());

    // We measure pure coding, so we always turn off the systematic mode
    if (kodoc_has_systematic_interface(encoder))
    {
        kodoc_set_systematic_off(encoder);
    }

    uint32_t block_size = kodoc_block_size(encoder);
    uint8_t* data_in = (uint8_t*) malloc(block_size);
    uint8_t* data_out = (uint8_t*) malloc(block_size);

    // Allocate payloads in a contiguous buffer
    uint32_t payload_size = kodoc_payload_size(encoder);
    // Generate an ample number of coded symbols (considering kodoc_binary)
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
    for (i = 0; i < block_size; ++i)
        data_in[i] = rand() % 256;

    // Set the input and output symbol buffers
    kodoc_set_const_symbols(encoder, data_in, block_size);
    kodoc_set_mutable_symbols(decoder, data_out, block_size);

    // Start the encoding timer
    start = bc::high_resolution_clock::now();
    // Generate coded symbols with the encoder
    for (i = 0; i < payload_count; ++i)
    {
        kodoc_write_payload(encoder, payloads[i]);
    }
    // Stop the encoding timer
    stop = bc::high_resolution_clock::now();

    run.encoding_time =
        (double)(bc::duration_cast<bc::microseconds>(stop - start).count());

    // Calculate the encoding rate in megabytes / seconds
    uint32_t encoded_bytes = payload_count * symbol_size;
    run.encoding_rate = encoded_bytes / run.encoding_time;

    // Start the decoding timer
    start = bc::high_resolution_clock::now();
    // Feed the coded symbols to the decoder
    for (i = 0; !kodoc_is_complete(decoder) && i < payload_count; ++i)
    {
        kodoc_read_payload(decoder, payloads[i]);
    }
    // Stop the decoding timer
    stop = bc::high_resolution_clock::now();

    run.decoding_time =
        (double)(bc::duration_cast<bc::microseconds>(stop - start).count());

    // Calculate the decoding rate in megabytes / seconds
    uint32_t decoded_bytes = symbols * symbol_size;
    run.decoding_rate = decoded_bytes / run.decoding_time;

    if (memcmp(data_in, data_out, block_size) == 0)
    {
        run.success = true;
    }
    else
    {
        run.success = false;
    }

    free(data_in);
    free(data_out);
    free(payload_buffer);
    free(payloads);

    kodoc_delete_coder(encoder);
    kodoc_delete_coder(decoder);

    kodoc_delete_factory(encoder_factory);
    kodoc_delete_factory(decoder_factory);

    return run;
}

// The main function should not be defined on Windows Phone
#if !defined(PLATFORM_WINDOWS_PHONE)
int main(int argc, const char* argv[])
{
    if (argc != 4 && argc != 5)
    {
        printf("Usage: %s [binary|binary4|binary8] "
               "symbols symbol_size {runs=10}\n", argv[0]);
        return 1;
    }

    // Here we select the finite field to use
    int32_t field;
    if (strcmp(argv[1], "binary") == 0)
    {
        field = kodoc_binary;
    }
    else if (strcmp(argv[1], "binary4") == 0)
    {
        field = kodoc_binary4;
    }
    else if (strcmp(argv[1], "binary8") == 0)
    {
        field = kodoc_binary8;
    }
    else
    {
        printf("Invalid finite field: %s\n", argv[1]);
        return 1;
    }

    uint32_t runs = 10;
    if (argc == 5)
    {
        runs = atoi(argv[4]);
    }

    uint32_t symbols = atoi(argv[2]);
    uint32_t symbol_size = atoi(argv[3]);

    bool decoding_success = true;
    double setup_time = 0.0;
    double encoding_time = 0.0;
    double decoding_time = 0.0;
    double encoding_rate = 0.0;
    double decoding_rate = 0.0;

    printf("Runs: %d / Symbols: %d / Symbol_size: %d\n", runs, symbols,
           symbol_size);

    for (uint32_t i = 0; i < runs; ++i)
    {
        results run = run_coding_test(field, symbols, symbol_size);

        decoding_success &= run.success;

        setup_time += run.setup_time;
        encoding_time += run.encoding_time;
        decoding_time += run.decoding_time;
        encoding_rate += run.encoding_rate;
        decoding_rate += run.decoding_rate;
    }

    printf("Average setup time: %5.4f microsec\n", setup_time / runs);
    printf("Average encoding time: %5.4f microsec\n", encoding_time / runs);
    printf("Average decoding time: %5.4f microsec\n", decoding_time / runs);
    printf("Average encoding rate: %0.2f MB/s\n", encoding_rate / runs);
    printf("Average decoding rate: %0.2f MB/s\n", decoding_rate / runs);

    if (decoding_success)
    {
        printf("All data decoded correctly.\n");
    }
    else
    {
        printf("Decoding failed.\n");
    }

    return 0;
}
#endif
