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

/// @example sliding_window.c
///
/// This example shows how to use the sliding window encoder and decoder
/// stacks. The sliding window is special in that it does not require
/// that all symbols are available at the encoder before encoding can
/// start. In addition, it uses feedback between the decoder and encoder
/// such that symbols that have already been received at the decoder
/// are not included in the encoding again (saving computations).

void trace_callback(const char* zone, const char* data, void* context)
{
    (void) context;
    if (strcmp(zone, "decoder_state") == 0 ||
        strcmp(zone, "symbol_coefficients_before_read_symbol") == 0 ||
        strcmp(zone, "symbol_index_before_read_uncoded_symbol") == 0)
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
    uint8_t max_symbols = 6;
    uint8_t max_symbol_size = 100;

    int32_t codec = kodoc_sliding_window;
    int32_t finite_field = kodoc_binary8;

    // In the following we will make an encoder/decoder factory.
    // The factories are used to build actual encoders/decoders
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

    uint8_t* data_out = (uint8_t*) malloc(block_size);
    kodoc_set_mutable_symbols(decoder, data_out, block_size);

    uint8_t feedback_size = (uint8_t) kodoc_feedback_size(encoder);
    uint8_t* feedback = (uint8_t*) malloc(feedback_size);

    uint32_t i = 0;

    //Just for fun - fill data_in with random data
    for (; i < block_size; ++i)
    {
        data_in[i] = rand() % 256;
    }

    // Install a custom trace function for the decoder
    kodoc_set_trace_callback(decoder, trace_callback, NULL);

    while (!kodoc_is_complete(decoder))
    {
        // Insert a new symbol until the encoder is full
        if (kodoc_rank(encoder) < max_symbols)
        {
            uint32_t rank = kodoc_rank(encoder);
            uint8_t* symbol = data_in + (rank * max_symbol_size);
            kodoc_set_const_symbol(encoder, rank, symbol, max_symbol_size);
            printf("Symbol %d added to the encoder\n", rank);
        }

        // Only send packets if the encoder has more data than the decoder
        if (kodoc_rank(encoder) == kodoc_rank(decoder))
        {
            continue;
        }

        // Write an encoded packet into the payload buffer
        kodoc_write_payload(encoder, payload);
        printf("Encoded packet generated\n");

        // Here we simulate that we are losing 50% of the packets
        if (rand() % 2)
        {
            printf("Packet dropped on channel\n\n");
            continue;
        }

        printf("Decoder received packet\n");

        // Packet got through - pass that packet to the decoder
        kodoc_read_payload(decoder, payload);

        printf("Encoder rank = %d\n", kodoc_rank(encoder));
        printf("Decoder rank = %d\n", kodoc_rank(decoder));

        printf("Decoder uncoded = %d\n", kodoc_symbols_uncoded(decoder));
        printf("Decoder partially decoded = %d\n",
               kodoc_symbols_partially_decoded(decoder));

        // Transmit the feedback
        kodoc_write_feedback(decoder, feedback);

        // Note that the feedback packets can also be lost in a real network,
        // but here we deliver all of them for the sake of simplicity
        printf("Received feedback from decoder\n\n");
        kodoc_read_feedback(encoder, feedback);
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
    free(feedback);

    kodoc_delete_coder(encoder);
    kodoc_delete_coder(decoder);

    kodoc_delete_factory(encoder_factory);
    kodoc_delete_factory(decoder_factory);

    return 0;
}
