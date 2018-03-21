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

/// @example switch_systematic_on_off.c
///
/// This example shows how to enable or disable systematic coding for
/// coding stacks that support it.
/// Systematic coding is used to reduce the amount of work done by an
/// encoder and a decoder. This is achieved by initially sending all
/// symbols which has not previously been sent uncoded. Kodo allows this
/// feature to be optionally turn of or off.

int main()
{
    // Seed random number generator to produce different results every time
    srand(time(NULL));

    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint8_t max_symbols = 10;
    uint8_t max_symbol_size = 100;

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

    kodoc_set_const_symbols(encoder, data_in, block_size);

    uint8_t* data_out = (uint8_t*) malloc(block_size);
    kodoc_set_mutable_symbols(decoder, data_out, block_size);

    printf("Starting encoding / decoding\n");
    while (!kodoc_is_complete(decoder))
    {
        // If the chosen codec stack supports systematic coding
        if (kodoc_has_systematic_interface(encoder))
        {
            // With 50% probability toggle systematic
            if ((rand() % 2) == 0)
            {
                if (kodoc_is_systematic_on(encoder))
                {
                    printf("Turning systematic OFF\n");
                    kodoc_set_systematic_off(encoder);
                }
                else
                {
                    printf("Turn systematic ON\n");
                    kodoc_set_systematic_on(encoder);
                }
            }
        }

        // Encode a packet into the payload buffer
        kodoc_write_payload(encoder, payload);

        if ((rand() % 2) == 0)
        {
            printf("Drop packet\n");
            continue;
        }

        // Pass that packet to the decoder
        kodoc_read_payload(decoder, payload);

        printf("Rank of decoder %d\n", kodoc_rank(decoder));

        // Symbols that were received in the systematic phase correspond
        // to the original source symbols and are therefore marked as
        // decoded
        printf("Symbols decoded %d\n", kodoc_symbols_uncoded(decoder));
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
    free(payload);

    kodoc_delete_coder(encoder);
    kodoc_delete_coder(decoder);

    kodoc_delete_factory(encoder_factory);
    kodoc_delete_factory(decoder_factory);

    return 0;
}
