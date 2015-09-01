// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include "dummy.h"

#include <kodoc/kodoc.h>

uint8_t run_kodo()
{
    // Seed random number generator to produce different results every time
    srand(time(NULL));

    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint32_t max_symbols = 42;
    uint32_t max_symbol_size = 160;

    // Select the coding to use
    int32_t code_type = kodo_full_vector;

    // Select the finite field
    int32_t finite_field = kodo_binary;

    // Create the factories
    kodo_factory_t encoder_factory =
        kodo_new_encoder_factory(code_type, finite_field,
                                 max_symbols, max_symbol_size,
                                 kodo_trace_enabled,
                                 kodo_deep_storage);

    kodo_factory_t decoder_factory =
        kodo_new_decoder_factory(code_type, finite_field,
                                 max_symbols, max_symbol_size,
                                 kodo_trace_enabled,
                                 kodo_deep_storage);

    // Create the coders
    kodo_coder_t encoder = kodo_factory_new_encoder(encoder_factory);
    kodo_coder_t decoder = kodo_factory_new_decoder(decoder_factory);

    // Generate the data
    uint32_t payload_size = kodo_payload_size(encoder);
    uint8_t* payload = (uint8_t*) malloc(payload_size);

    uint32_t block_size = kodo_block_size(encoder);
    uint8_t* data_in = (uint8_t*) malloc(block_size);

    uint32_t i = 0;
    for(; i < block_size; ++i)
        data_in[i] = rand() % 256;

    // Set the date to encode
    kodo_set_const_symbols(encoder, data_in, block_size);

    // Run the decoding
    while (!kodo_is_complete(decoder))
    {
        kodo_encode(encoder, payload);
        kodo_decode(decoder, payload);
    }

    // Copy out the data
    uint8_t* data_out = (uint8_t*) malloc(block_size);
    kodo_copy_symbols(decoder, data_out, block_size);

    // Check if the decoding was successful
    uint8_t success = 0;
    if (memcmp(data_in, data_out, block_size) == 0)
        success = 1;

    // Clean up
    free(data_in);
    free(data_out);
    free(payload);

    kodo_delete_encoder(encoder);
    kodo_delete_decoder(decoder);

    kodo_delete_encoder_factory(encoder_factory);
    kodo_delete_decoder_factory(decoder_factory);

    return success;
}
