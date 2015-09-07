// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <vector>

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

/// stuct containing all the data accociated with a seperate run.
struct run
{
    kodo_factory_t encoder_factory;
    kodo_factory_t decoder_factory;
    kodo_coder_t encoder;
    kodo_coder_t decoder;

    double setup_time;
    double encoding_time;
    double decoding_time;
    double encoding_rate;
    double decoding_rate;

    uint8_t* data_in;
    uint8_t* data_out;
    uint8_t* payload_buffer;
    uint8_t** payloads;
};

// Runs a timed encoding and decoding benchmark.
//
// @param symbols The number of original symbols in the block
// @param symbol_size The size of a symbol in bytes
// @param encoding_rate The measured encoding rate in megabytes/seconds
// @param decoding_rate The measured decoding rate in megabytes/seconds
// @param runs The number of runs to perform
// @return True if the original data was correctly decoded
bool run_coding_test(int32_t finite_field, uint32_t symbols,
                     uint32_t symbol_size, double& encoding_rate,
                     double& decoding_rate, uint32_t number_of_runs = 4)
{
    std::vector<run> runs(number_of_runs);

    // Set the random seed to randomize encoded data
    srand((uint32_t)time(NULL));

    // Here we select the coding code_type we wish to use
    int32_t code_type = kodo_full_vector;
    bc::high_resolution_clock::time_point start, stop;

    printf("Setup: ");

    for (auto& run : runs)
    {
        // First, we measure the combined setup time for the encoder and decoder
        start = bc::high_resolution_clock::now();

        run.encoder_factory =
            kodo_new_encoder_factory(code_type, finite_field,
                                             symbols, symbol_size);

        run.decoder_factory =
            kodo_new_decoder_factory(code_type, finite_field,
                                             symbols, symbol_size);

        run.encoder = kodo_factory_new_encoder(run.encoder_factory);
        run.decoder = kodo_factory_new_decoder(run.decoder_factory);

        // Stop the setup timer
        stop = bc::high_resolution_clock::now();

        run.setup_time =
            (double)(bc::duration_cast<bc::microseconds>(stop - start).count());
    }
    printf("Done.\n");

    // Generate an ample number of coded symbols (considering kodo_binary)
    uint32_t payload_count = 2 * symbols;

    printf("Encoding: ");
    for (auto& run : runs)
    {
        uint32_t block_size = kodo_block_size(run.encoder);
        run.data_in = (uint8_t*) malloc(block_size);

        // Allocate payloads in a contiguous buffer
        uint32_t payload_size = kodo_payload_size(runs[0].encoder);

        run.payload_buffer = (uint8_t*) malloc(payload_size * payload_count);
        run.payloads = (uint8_t**) malloc(payload_count * sizeof(uint8_t*));

        // Set the payload pointers for easy access
        for (uint32_t i = 0; i < payload_count; ++i)
        {
            run.payloads[i] = &run.payload_buffer[i * payload_size];
        }

        // Randomize input data
        for (uint32_t i = 0; i < block_size; ++i)
        {
            run.data_in[i] = rand() % 256;
        }

        // We measure pure coding, so we always turn off the systematic mode
        if (kodo_has_set_systematic_off(run.encoder))
        {
            kodo_set_systematic_off(run.encoder);
        }

        // Set the input symbol buffers
        kodo_set_const_symbols(run.encoder, run.data_in, block_size);

        // Start the encoding timer
        start = bc::high_resolution_clock::now();
        // Generate coded symbols with the encoder
        for (uint32_t i = 0; i < payload_count; ++i)
        {
            kodo_write_payload(run.encoder, run.payloads[i]);
        }
        // Stop the encoding timer
        stop = bc::high_resolution_clock::now();

        run.encoding_time =
            (double)(bc::duration_cast<bc::microseconds>(stop - start).count());

        // Calculate the encoding rate in megabytes / seconds
        uint32_t encoded_bytes = payload_count * symbol_size;
        run.encoding_rate = encoded_bytes / run.encoding_time;
    }
    printf("Done.\n");

    printf("Decoding: ");
    bool success = true;
    for (auto& run : runs)
    {
        uint32_t block_size = kodo_block_size(run.decoder);
        run.data_out = (uint8_t*) malloc(block_size);

        // Set the output symbol buffers
        kodo_set_mutable_symbols(run.decoder, run.data_out, block_size);

        // Start the decoding timer
        start = bc::high_resolution_clock::now();
        // Feed the coded symbols to the decoder
        for (uint32_t i = 0;
            !kodo_is_complete(run.decoder) && i < payload_count; ++i)
        {
            kodo_read_payload(run.decoder, run.payloads[i]);
        }
        // Stop the decoding timer
        stop = bc::high_resolution_clock::now();

        run.decoding_time =
            (double)(bc::duration_cast<bc::microseconds>(stop - start).count());

        // Calculate the decoding rate in megabytes / seconds
        uint32_t decoded_bytes = symbols * symbol_size;
        run.decoding_rate = decoded_bytes / run.decoding_time;

        if (memcmp(run.data_in, run.data_out, block_size) == 0)
        {
            success &= true;
        }
        else
        {
            success &= false;
        }
    }
    printf("Done.\n");

    printf("Cleanup: ");
    for (uint32_t i = 0; i < number_of_runs; ++i)
    {
        auto& run = runs[i];
        free(run.data_out);
        free(run.data_in);
        free(run.payload_buffer);
        free(run.payloads);
        kodo_delete_encoder(run.encoder);
        kodo_delete_encoder_factory(run.encoder_factory);
        kodo_delete_decoder(run.decoder);
        kodo_delete_decoder_factory(run.decoder_factory);
    }

    double total_setup_time = 0.0;
    double total_encoding_time = 0.0;
    double total_decoding_time = 0.0;
    double total_encoding_rate = 0.0;
    double total_decoding_rate = 0.0;

    for (uint32_t i = 0; i < number_of_runs; ++i)
    {
        auto& run = runs[i];
        total_setup_time += run.setup_time;
        total_encoding_time += run.encoding_time;
        total_decoding_time += run.decoding_time;
        total_encoding_rate += run.encoding_rate;
        total_decoding_rate += run.decoding_rate;
    }
    printf("Done.\n");

    double setup_time = total_setup_time / number_of_runs;
    double encoding_time = total_encoding_time / number_of_runs;
    double decoding_time = total_decoding_time / number_of_runs;
    encoding_rate = total_encoding_rate / number_of_runs;
    decoding_rate = total_decoding_rate / number_of_runs;

    printf("Average setup time: %5.4f microsec\n", setup_time);
    printf("Average encoding time: %5.4f microsec\n", encoding_time);
    printf("Average decoding time: %5.4f microsec\n", decoding_time);
    return success;
}

// The main function should not be defined on Windows Phone
#if !defined(PLATFORM_WINDOWS_PHONE)
int main(int argc, const char* argv[])
{
    if (argc != 4 && argc != 5)
    {
        printf("Usage: %s [binary|binary4|binary8|binary16] "
               "symbols symbol_size {runs=10}\n", argv[0]);
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

    uint32_t runs = 10;
    if (argc == 5)
    {
        runs = atoi(argv[4]);
    }

    double encoding_rate = 0.0;
    double decoding_rate = 0.0;

    printf("Runs: %d\n", runs);

    bool decoding_success =
        run_coding_test(field, symbols, symbol_size,
                        encoding_rate, decoding_rate, runs);

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
