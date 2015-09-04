// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
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

    int32_t code_type = kodo_full_vector;
    int32_t finite_field = kodo_binary8;

    // In the following we will make an encoder/decoder factory.
    // The factories are used to build actual encoders/decoder
    kodo_factory_t encoder_factory =
        kodo_new_encoder_factory(code_type, finite_field,
                                 max_symbols, max_symbol_size);

    kodo_factory_t decoder_factory =
        kodo_new_decoder_factory(code_type, finite_field,
                                 max_symbols, max_symbol_size);

    // If we wanted to build an encoder or decoder with a smaller number of
    // symbols or a different symbol size, then this can be adjusted using the
    // following functions:
    //      kodo_factory_set_symbols(...)
    //      kodo_factory_set_symbol_size(...)
    // We cannot exceed the maximum values which was used when building
    // the factory.
    kodo_coder_t encoder = kodo_factory_new_encoder(encoder_factory);
    kodo_coder_t decoder = kodo_factory_new_decoder(decoder_factory);

    // Allocate some storage for a "payload" the payload is what we would
    // eventually send over a network
    uint32_t payload_size = kodo_payload_size(encoder);
    uint8_t* payload = (uint8_t*) malloc(payload_size);

    // Allocate some data to encode. In this case we make a buffer
    // with the same size as the encoder's block size (the max.
    // amount a single encoder can encode)
    uint32_t block_size = kodo_block_size(encoder);
    uint8_t* data_in = (uint8_t*) malloc(block_size);

    // Just for fun - fill the data with random data
    uint32_t i = 0;
    for (; i < block_size; ++i)
    {
        data_in[i] = rand() % 256;
    }

    kodo_set_const_symbols(encoder, data_in, block_size);

    uint8_t* data_out = (uint8_t*) malloc(block_size);
    kodo_set_mutable_symbols(decoder, data_out, block_size);

    printf("Starting encoding / decoding\n");
    while (!kodo_is_complete(decoder))
    {
        // If the chosen codec stack supports systematic coding
        if (kodo_has_set_systematic_off(encoder))
        {
            // With 50% probability toggle systematic
            if ((rand() % 2) == 0)
            {
                if (kodo_is_systematic_on(encoder))
                {
                    printf("Turning systematic OFF\n");
                    kodo_set_systematic_off(encoder);
                }
                else
                {
                    printf("Turn systematic ON\n");
                    kodo_set_systematic_on(encoder);
                }
            }
        }

        // Encode a packet into the payload buffer
        kodo_write_payload(encoder, payload);

        if ((rand() % 2) == 0)
        {
            printf("Drop packet\n");
            continue;
        }

        // Pass that packet to the decoder
        kodo_read_payload(decoder, payload);

        printf("Rank of decoder %d\n", kodo_rank(decoder));

        // Symbols that were received in the systematic phase correspond
        // to the original source symbols and are therefore marked as
        // decoded
        printf("Symbols decoded %d\n", kodo_symbols_uncoded(decoder));
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

    kodo_delete_encoder(encoder);
    kodo_delete_decoder(decoder);

    kodo_delete_encoder_factory(encoder_factory);
    kodo_delete_decoder_factory(decoder_factory);

    return 0;
}
