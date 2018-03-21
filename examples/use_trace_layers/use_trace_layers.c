// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @example use_trace_layers.c
///
/// Simple example to show the trace functionality in Kodo.

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <kodoc/kodoc.h>

// Helper function to determine if a string ends with a certain suffix
int ends_with(const char* str, const char* suffix)
{
    if (!str || !suffix) return 0;
    size_t len_str = strlen(str);
    size_t len_suffix = strlen(suffix);
    if (len_suffix >  len_str) return 0;
    return strncmp(str + len_str - len_suffix, suffix, len_suffix) == 0;
}

// This callback function will be called when the decoder produces trace output
void trace_callback(const char* zone, const char* data, void* context)
{
    (void) context;
    // The zone string starts with our custom prefix, so it is easier to
    // check if the zone ends with a given suffix
    if (ends_with(zone, "decoder_state") ||
        ends_with(zone, "symbol_coefficients_before_read_symbol") ||
        ends_with(zone, "symbol_index_before_read_uncoded_symbol"))
    {
        printf("%s:\n", zone);
        printf("%s\n", data);
    }
}

int main()
{
    // Seed random number generator to produce different results every time
    srand(time(NULL));

    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint32_t max_symbols = 3;
    uint32_t max_symbol_size = 16;

    int32_t codec = kodoc_full_vector;
    int32_t finite_field = kodoc_binary8;

    // In the following we will make an encoder/decoder factory.
    // The factories are used to build actual encoders/decoder
    kodoc_factory_t encoder_factory =
        kodoc_new_encoder_factory(codec, finite_field,
                                  max_symbols, max_symbol_size);

    kodoc_factory_t decoder_factory =
        kodoc_new_decoder_factory(codec, finite_field,
                                  max_symbols, max_symbol_size);

    // If we wanted to build an encoder or decoder with a smaller number of
    // symbols or a different symbol size, then this can be adjusted using the
    // following functions:
    //      kodoc_factory_set_symbols(...)
    //      kodoc_factory_set_symbol_size(...)
    // We cannot exceed the maximum values which was used when building
    // the factory.
    kodoc_coder_t encoder = kodoc_factory_build_coder(encoder_factory);
    kodoc_coder_t decoder = kodoc_factory_build_coder(decoder_factory);

    // Allocate some storage for a "payload" the payload is what we would
    // eventually send over a network
    uint32_t payload_size = kodoc_payload_size(encoder);
    uint8_t* payload = (uint8_t*) malloc(payload_size);

    // Allocate some data to encode. In this case we make a buffer
    // with the same size as the encoder's block size (the max.
    // amount a single encoder can encode)
    uint32_t block_size = kodoc_block_size(encoder);
    uint8_t* data_in = (uint8_t*) malloc(block_size);

    // Just for fun - fill the data with random data
    uint32_t i = 0;
    for (; i < block_size; ++i)
    {
        data_in[i] = rand() % 256;
    }

    // Install the stdout trace function for the encoder (everything will
    // be printed to stdout without filtering)
    kodoc_set_trace_stdout(encoder);
    // Set a custom zone prefix for the encoder (this helps to differentiate
    // the trace output of the encoder and the decoder)
    kodoc_set_zone_prefix(encoder, "Encoder");

    // Install a custom trace function for the decoder (we can process and
    // filter the data in our trace callback)
    kodoc_set_trace_callback(decoder, trace_callback, NULL);
    // Set a custom zone prefix for the decoder
    kodoc_set_zone_prefix(decoder, "Decoder");

    kodoc_set_const_symbols(encoder, data_in, block_size);

    uint8_t* data_out = (uint8_t*) malloc(block_size);
    kodoc_set_mutable_symbols(decoder, data_out, block_size);

    while (!kodoc_is_complete(decoder))
    {
        kodoc_write_payload(encoder, payload);

        if ((rand() % 2) == 0)
        {
            continue;
        }

        kodoc_read_payload(decoder, payload);
    }

    if (memcmp(data_in, data_out, block_size) == 0)
    {
        printf("Data decoded correctly\n");
    }
    else
    {
        printf("Unexpected failure to decode, please file a bug report :)\n");
    }

    free(data_in);
    free(data_out);
    free(payload);

    kodoc_delete_coder(encoder);
    kodoc_delete_coder(decoder);

    kodoc_delete_factory(encoder_factory);
    kodoc_delete_factory(decoder_factory);

    return 0;
}
