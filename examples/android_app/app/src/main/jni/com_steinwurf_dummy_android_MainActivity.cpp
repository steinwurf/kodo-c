// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include "com_steinwurf_dummy_android_MainActivity.h"

JNIEXPORT
jboolean JNICALL Java_com_steinwurf_dummy_1android_MainActivity_runKodo(
    JNIEnv* env, jobject thiz)
{
    // Seed random number generator to produce different results every time
    srand(time(NULL));

    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint32_t max_symbols = 42;
    uint32_t max_symbol_size = 160;

    // Select the coding to use
    int32_t code_type = kodoc_full_vector;

    // Select the finite field
    int32_t finite_field = kodoc_binary;

    // Create the factories
    kodoc_factory_t encoder_factory =
        kodoc_new_encoder_factory(code_type, finite_field,
                                  max_symbols, max_symbol_size);

    kodoc_factory_t decoder_factory =
        kodoc_new_decoder_factory(code_type, finite_field,
                                  max_symbols, max_symbol_size);

    // Create the coders
    kodoc_coder_t encoder = kodoc_factory_build_coder(encoder_factory);
    kodoc_coder_t decoder = kodoc_factory_build_coder(decoder_factory);

    // Generate the data
    uint32_t payload_size = kodoc_payload_size(encoder);
    uint8_t* payload = (uint8_t*) malloc(payload_size);

    uint32_t block_size = kodoc_block_size(encoder);
    uint8_t* data_in = (uint8_t*) malloc(block_size);

    uint32_t i = 0;
    for (; i < block_size; ++i)
        data_in[i] = rand() % 256;

    // Set the date to encode
    kodoc_set_const_symbols(encoder, data_in, block_size);

    uint8_t* data_out = (uint8_t*) malloc(block_size);
    kodoc_set_mutable_symbols(decoder, data_out, block_size);

    // Run the decoding
    while (!kodoc_is_complete(decoder))
    {
        kodoc_write_payload(encoder, payload);
        kodoc_read_payload(decoder, payload);
    }

    // Check if the decoding was successful
    bool success = false;
    if (memcmp(data_in, data_out, block_size) == 0)
        success = true;

    // Clean up
    free(data_in);
    free(data_out);
    free(payload);

    kodoc_delete_coder(encoder);
    kodoc_delete_coder(decoder);

    kodoc_delete_factory(encoder_factory);
    kodoc_delete_factory(decoder_factory);

    return success;
}