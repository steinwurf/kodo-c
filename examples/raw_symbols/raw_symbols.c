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

/// @example raw_symbols.c
///
/// Example showing how to process original symbols that were transmitted
/// without kodo headers. These symbols are inserted manually to the decoder
/// before the coded packets are processed.

int main()
{
    // Seed random number generator to produce different results every time
    srand(time(NULL));

    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint32_t symbols = 10;
    uint32_t symbol_size = 100;

    // Here we select the finite field to use common choices are
    // kodo_binary, kodo_binary8, kodo_binary16
    int32_t finite_field = kodo_binary;

    kodo_factory_t encoder_factory = kodo_new_encoder_factory(
        kodo_sparse_full_vector, finite_field, symbols, symbol_size);

    kodo_factory_t decoder_factory = kodo_new_decoder_factory(
        kodo_full_vector, finite_field, symbols, symbol_size);

    // If we wanted to build an encoder of decoder with a smaller number of
    // symbols or a different symbol size, then this can be adjusted using the
    // following functions:
    // kodo_factory_set_symbols(...) and kodo_factory_set_symbol_size(...)
    // We can however not exceed the maximum values which were used when
    // building the factory.

    kodo_coder_t encoder = kodo_factory_new_encoder(encoder_factory);
    kodo_coder_t decoder = kodo_factory_new_decoder(decoder_factory);

    // In this example, we do not use systematic coding. The original symbols
    // will be transferred to the decoder without using kodo.
    //
    // We explicitly turn off the systematic mode on the encoder:
    if (kodo_has_set_systematic_off(encoder))
    {
        kodo_set_systematic_off(encoder);
    }
    printf("Systematic encoding disabled\n");

    // Set the coding vector density on the encoder
    kodo_set_density(encoder, 0.2);
    printf("Coding vector density: %3.2f\n", kodo_density(encoder));

    uint32_t bytes_used;
    uint32_t payload_size = kodo_payload_size(encoder);
    uint8_t* payload = (uint8_t*) malloc(payload_size);

    // Allocate symbols in non-contiguous buffers
    uint8_t** input_symbols = (uint8_t**) malloc(symbols * sizeof(uint8_t*));
    uint8_t** output_symbols = (uint8_t**) malloc(symbols * sizeof(uint8_t*));

    uint32_t i, j;
    // Create the original symbols and store them in the encoder
    for (i = 0; i < symbols; ++i)
    {
        // Create the individual symbols for the encoder
        input_symbols[i] = (uint8_t*) malloc(symbol_size);

        // Randomize input data
        for (j = 0; j < symbol_size; ++j)
            input_symbols[i][j] = rand() % 256;

        // Store the symbol pointer in the encoder
        kodo_set_const_symbol(encoder, i, input_symbols[i], symbol_size);
    }

    // Transfer the original symbols to the decoder with some losses
    uint32_t lost_symbols = 0;
    for (i = 0; i < symbols; ++i)
    {
        // Create the output symbol buffers for the decoder
        output_symbols[i] = (uint8_t*) malloc(symbol_size);

        // Specify the output buffers used for decoding
        kodo_set_mutable_symbol(decoder, i, output_symbols[i], symbol_size);

        // Simulate a channel with a 50% loss rate
        if (rand() % 2)
        {
            lost_symbols++;
            printf("Symbol %d lost on channel\n\n", i);
            continue;
        }

        // If the symbol was not lost, then insert that symbol into the decoder
        // using the raw symbol data (no additional headers are needed)
        // This will copy the data from input_symbols[i] to output_symbols[i]
        kodo_read_uncoded_symbol(decoder, input_symbols[i], i);
    }

    printf("Number of lost symbols: %d\n", lost_symbols);

    // Now we generate coded packets with the encoder in order to recover the
    // lost packets on the decoder side
    while (!kodo_is_complete(decoder))
    {
        // The encoder will use a certain amount of bytes of the payload
        // buffer. It will never use more than payload_size, but it might
        // use less.
        bytes_used = kodo_write_payload(encoder, payload);
        printf("Payload generated by encoder, rank = %d, bytes used = %d\n",
               kodo_rank(encoder), bytes_used);

        // Pass the coded packet to the decoder
        kodo_read_payload(decoder, payload);
        printf("Payload processed by decoder, current rank = %d\n\n",
               kodo_rank(decoder));
    }

    // Compare the input and output symbols one-by-one
    uint32_t failures = 0;
    for (i = 0; i < symbols; ++i)
    {
        if (memcmp(input_symbols[i], output_symbols[i], symbol_size) != 0)
        {
            printf("Error: Symbol %d was not decoded correctly\n", i);
            failures++;
        }

        free(input_symbols[i]);
        free(output_symbols[i]);
    }

    if (failures == 0)
    {
        printf("Data decoded correctly\n");
    }


    free(input_symbols);
    free(output_symbols);
    free(payload);

    kodo_delete_encoder(encoder);
    kodo_delete_decoder(decoder);

    kodo_delete_encoder_factory(encoder_factory);
    kodo_delete_decoder_factory(decoder_factory);

    return 0;
}
