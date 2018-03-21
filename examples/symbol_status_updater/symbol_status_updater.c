// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <kodoc/kodoc.h>

/// @example symbol_status_updater.c
///
/// Simple example showing the result of enabling the symbol status updater.
int main()
{
    // Seed the random number generator to produce different data every time
    srand(time(NULL));

    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint32_t max_symbols = 50;
    uint32_t max_symbol_size = 160;

    // Here we select the codec we wish to use
    int32_t codec = kodoc_full_vector;

    // Here we select the finite field to use.
    // To show the effect of the symbol status updater we need to use a lower
    // sized field - the lower the better.
    int32_t finite_field = kodoc_binary;

    // First, we create an encoder & decoder factory.
    // The factories are used to build actual encoders/decoders
    kodoc_factory_t encoder_factory = kodoc_new_encoder_factory(
        codec, finite_field, max_symbols, max_symbol_size);

    kodoc_factory_t decoder_factory = kodoc_new_decoder_factory(
        codec, finite_field, max_symbols, max_symbol_size);

    // If we wanted to build an encoder or decoder with a smaller number of
    // symbols or a different symbol size, then this can be adjusted using the
    // following functions:
    // kodoc_factory_set_symbols(...) and kodoc_factory_set_symbol_size(...)
    // We can however not exceed the maximum values that were used when
    // creating the factory.

    kodoc_coder_t encoder = kodoc_factory_build_coder(encoder_factory);
    // create two decoders, one which has the status updater turned on, and one
    // which has it off.
    kodoc_coder_t decoder1 = kodoc_factory_build_coder(decoder_factory);
    kodoc_coder_t decoder2 = kodoc_factory_build_coder(decoder_factory);

    kodoc_set_status_updater_on(decoder2);

    printf("decoder 1 status updater: %d\n",
           kodoc_is_status_updater_enabled(decoder1));
    printf("decoder 2 status updater: %d\n",
           kodoc_is_status_updater_enabled(decoder2));

    uint32_t payload_size = kodoc_payload_size(encoder);
    uint8_t* payload1 = (uint8_t*) malloc(payload_size);
    uint8_t* payload2 = (uint8_t*) malloc(payload_size);

    uint32_t block_size = kodoc_block_size(encoder);
    uint8_t* data_in = (uint8_t*) malloc(block_size);
    uint8_t* data_out1 = (uint8_t*) malloc(block_size);
    uint8_t* data_out2 = (uint8_t*) malloc(block_size);

    uint32_t i = 0;
    for (; i < block_size; ++i)
        data_in[i] = rand() % 256;

    kodoc_set_const_symbols(encoder, data_in, block_size);
    kodoc_set_mutable_symbols(decoder1, data_out1, block_size);
    kodoc_set_mutable_symbols(decoder2, data_out2, block_size);

    // Skip the systematic phase as the effect of the symbol status decoder is
    // only visible when reading coded packets.
    kodoc_set_systematic_off(encoder);

    while (!kodoc_is_complete(decoder1))
    {
        // The encoder will use a certain amount of bytes of the payload
        // buffer. It will never use more than payload_size, but it might
        // use less.
        kodoc_write_payload(encoder, payload1);
        memcpy(payload2, payload1, payload_size);

        // Pass the generated packet to the decoder
        kodoc_read_payload(decoder1, payload1);
        kodoc_read_payload(decoder2, payload2);
        printf("decoder1: %d\n", kodoc_symbols_uncoded(decoder1));
        printf("decoder2: %d\n", kodoc_symbols_uncoded(decoder2));
        printf("-----------------\n");
    }

    if (memcmp(data_in, data_out1, block_size) == 0 &&
        memcmp(data_in, data_out2, block_size) == 0)
    {
        printf("Data decoded correctly\n");
    }
    else
    {
        printf("Unexpected failure to decode, please file a bug report :)\n");
    }

    free(data_in);
    free(data_out1);
    free(data_out2);
    free(payload1);
    free(payload2);

    kodoc_delete_coder(encoder);
    kodoc_delete_coder(decoder1);
    kodoc_delete_coder(decoder2);

    kodoc_delete_factory(encoder_factory);
    kodoc_delete_factory(decoder_factory);

    return 0;
}
