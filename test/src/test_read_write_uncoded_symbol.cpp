// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodoc/kodoc.h>

#include <cstdlib>

#include <gtest/gtest.h>

#include "test_helper.hpp"

void test_read_write_uncoded(uint32_t symbols, uint32_t symbol_size,
                             int32_t code_type, int32_t finite_field)
{
    kodo_factory_t encoder_factory =
        kodo_new_encoder_factory(code_type, finite_field, symbols, symbol_size);

    kodo_factory_t decoder_factory =
        kodo_new_decoder_factory(code_type, finite_field, symbols, symbol_size);

    kodo_coder_t encoder = kodo_factory_new_encoder(encoder_factory);
    kodo_coder_t decoder = kodo_factory_new_decoder(decoder_factory);

    EXPECT_EQ(symbols, kodo_factory_max_symbols(encoder_factory));
    EXPECT_EQ(symbol_size, kodo_factory_max_symbol_size(encoder_factory));
    EXPECT_EQ(symbols, kodo_symbols(encoder));
    EXPECT_EQ(symbol_size, kodo_symbol_size(encoder));

    EXPECT_EQ(symbols, kodo_factory_max_symbols(decoder_factory));
    EXPECT_EQ(symbol_size, kodo_factory_max_symbol_size(decoder_factory));
    EXPECT_EQ(symbols, kodo_symbols(decoder));
    EXPECT_EQ(symbol_size, kodo_symbol_size(decoder));

    EXPECT_EQ(symbols * symbol_size, kodo_block_size(encoder));
    EXPECT_EQ(symbols * symbol_size, kodo_block_size(decoder));

    EXPECT_TRUE(kodo_factory_max_payload_size(encoder_factory) >=
                kodo_payload_size(encoder));

    EXPECT_TRUE(kodo_factory_max_payload_size(decoder_factory) >=
                kodo_payload_size(decoder));

    EXPECT_EQ(kodo_factory_max_payload_size(encoder_factory),
              kodo_factory_max_payload_size(decoder_factory));

    uint32_t payload_size = kodo_payload_size(encoder);
    uint8_t* payload = (uint8_t*) malloc(payload_size);
    uint8_t* test_symbol = (uint8_t*) malloc(symbol_size);

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
            continue;
        }

        // We test that kodo_write_uncoded_symbol writes the original symbol
        // to the test buffer
        kodo_write_uncoded_symbol(encoder, test_symbol, i);
        // Compare input_symbols[i] with test_symbol
        EXPECT_EQ(memcmp(input_symbols[i], test_symbol, symbol_size), 0);

        // If the symbol was not lost, then insert that symbol into the decoder
        // using the raw symbol data (no additional headers are needed)
        // This will copy the data from input_symbols[i] to output_symbols[i]
        kodo_read_uncoded_symbol(decoder, input_symbols[i], i);

        EXPECT_EQ(i + 1 - lost_symbols, kodo_rank(decoder));
    }

    // Now we generate coded packets with the encoder in order to recover the
    // lost packets on the decoder side
    while (!kodo_is_complete(decoder))
    {
        kodo_write_payload(encoder, payload);
        kodo_read_payload(decoder, payload);
    }

    EXPECT_TRUE(kodo_is_complete(decoder) != 0);

    // Compare the input and output symbols one-by-one
    for (i = 0; i < symbols; ++i)
    {
        EXPECT_EQ(memcmp(input_symbols[i], output_symbols[i], symbol_size), 0);

        free(input_symbols[i]);
        free(output_symbols[i]);
    }

    free(input_symbols);
    free(output_symbols);
    free(payload);
    free(test_symbol);

    kodo_delete_encoder(encoder);
    kodo_delete_decoder(decoder);

    kodo_delete_encoder_factory(encoder_factory);
    kodo_delete_decoder_factory(decoder_factory);
}

TEST(test_read_write_uncoded_symbol, uncoded_symbols)
{
    uint32_t symbols = rand_symbols();
    uint32_t symbol_size = rand_symbol_size();

    test_read_write_uncoded(symbols, symbol_size,
                            kodo_full_vector, kodo_binary);

    test_read_write_uncoded(symbols, symbol_size,
                            kodo_full_vector, kodo_binary8);
}
