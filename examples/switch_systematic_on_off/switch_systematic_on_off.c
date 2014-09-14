// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <ckodo/ckodo.h>


int main()
{
    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint8_t max_symbols = 16;
    uint8_t max_symbol_size = 160;

    size_t algorithm = kodo_full_rlnc;
    size_t finite_field = kodo_binary8;

    uint8_t trace_enabled = 0;

    // In the following we will make an encoder/decoder factory.
    // The factories are used to build actual encoders/decoder
    kodo_factory_t* encoder_factory =
        kodo_new_encoder_factory(algorithm, finite_field,
                                   max_symbols, max_symbol_size,
                                   trace_enabled);

    kodo_factory_t* decoder_factory =
        kodo_new_decoder_factory(algorithm, finite_field,
                                 max_symbols, max_symbol_size,
                                 trace_enabled);

    // If we wanted to build an encoder or decoder with a smaller number of
    // symbols or a different symbol size, then this can be adjusted using the
    // following functions:
    //      kodo_factory_set_symbols(...)
    //      kodo_factory_set_symbol_size(...)
    // We cannot exceed the maximum values which was used when building
    // the factory.
    kodo_coder_t* encoder = kodo_factory_new_encoder(encoder_factory);
    kodo_coder_t* decoder = kodo_factory_new_decoder(decoder_factory);

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

    kodo_set_symbols(encoder, data_in, block_size);


    printf("Starting encoding / decoding\n");
    while (!kodo_is_complete(decoder))
    {
        // If the chosen codec stack supports systematic coding
        if (kodo_has_systematic_encoder(encoder))
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
        kodo_encode(encoder, payload);

        if ((rand() % 2) == 0)
        {
            printf("Drop packet\n");
            continue;
        }

        // Pass that packet to the decoder
        kodo_decode(decoder, payload);

        printf("Rank of decoder %d\n", kodo_rank(decoder));


        // Symbols that were received in the systematic phase correspond
        // to the original source symbols and are therefore marked as
        // decoded
        printf("Symbols decoded %d\n", kodo_symbols_uncoded(decoder));
    }

    uint8_t* data_out = (uint8_t*) malloc(kodo_block_size(decoder));
    kodo_copy_symbols(decoder, data_out, kodo_block_size(decoder));

    if (memcmp(data_in, data_out, block_size) == 0)
    {
        printf("Data decoded correctly\n");
    }
    else
    {
        printf("Unexpected failure to decode, please file a bug report :)\n");
    }


    //Clean up
    free(data_in);
    free(payload);

    kodo_delete_encoder(encoder);
    kodo_delete_decoder(decoder);

    kodo_delete_encoder_factory(encoder_factory);
    kodo_delete_decoder_factory(decoder_factory);

    return 0;
}