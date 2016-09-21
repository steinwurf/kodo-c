// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodoc/kodoc.h>

#include <cstdlib>

#include <gtest/gtest.h>

#include "test_helper.hpp"

void test_read_write_symbol(uint32_t symbols, uint32_t symbol_size,
                            int32_t codec, int32_t finite_field)
{
    kodoc_factory_t encoder_factory =
        kodoc_new_encoder_factory(codec, finite_field, symbols, symbol_size);

    kodoc_factory_t decoder_factory =
        kodoc_new_decoder_factory(codec, finite_field, symbols, symbol_size);

    kodoc_coder_t encoder = kodoc_factory_build_coder(encoder_factory);
    kodoc_coder_t decoder = kodoc_factory_build_coder(decoder_factory);

    EXPECT_EQ(symbols, kodoc_factory_max_symbols(encoder_factory));
    EXPECT_EQ(symbol_size, kodoc_factory_max_symbol_size(encoder_factory));
    EXPECT_EQ(symbols, kodoc_symbols(encoder));
    EXPECT_EQ(symbol_size, kodoc_symbol_size(encoder));

    EXPECT_EQ(symbols, kodoc_factory_max_symbols(decoder_factory));
    EXPECT_EQ(symbol_size, kodoc_factory_max_symbol_size(decoder_factory));
    EXPECT_EQ(symbols, kodoc_symbols(decoder));
    EXPECT_EQ(symbol_size, kodoc_symbol_size(decoder));

    EXPECT_EQ(symbols * symbol_size, kodoc_block_size(encoder));
    EXPECT_EQ(symbols * symbol_size, kodoc_block_size(decoder));

    EXPECT_TRUE(kodoc_factory_max_payload_size(encoder_factory) >=
                kodoc_payload_size(encoder));

    EXPECT_TRUE(kodoc_factory_max_payload_size(decoder_factory) >=
                kodoc_payload_size(decoder));

    EXPECT_EQ(kodoc_factory_max_payload_size(encoder_factory),
              kodoc_factory_max_payload_size(decoder_factory));

    uint8_t* coded_symbol = (uint8_t*) malloc(symbol_size);

    // Allocate symbols in non-contiguous buffers
    uint8_t** input_symbols = (uint8_t**) malloc(symbols * sizeof(uint8_t*));
    uint8_t** output_symbols = (uint8_t**) malloc(symbols * sizeof(uint8_t*));

    // To illustrate decoding, random data has been filled into the
    // matrices below. It is crucial that the equation below is correct
    // if the purpose is to test if the decoder decodes correctly as this
    // example evaluates in the end of the example.
    //
    // For additional information, please see the article
    //
    //   Christina Fragouli, Jean-Yves Le Boudec, and Jörg Widmer.
    //   Network Coding: An Instant Primer.
    //   SIGCOMM Comput. Commun. Rev., 36(1):63-68, January 2006.
    //
    // from which the notation in the example is based on.
    //
    //
    //
    // original_symbols (M):    Symbols we expect to obtain from decoding
    //                          encoded_symbols using the symbol_coefficients.
    // symbol_coefficients (G): Coefficients used to encode/decode between
    //                          original_symbols and encoded_symbols.
    // encoded_symbols (X):     Symbols that has been encoded from
    //                          original_symbols using the symbol_coefficients.
    //
    //
    //                          X = G M
    //
    //                        X^j = sum_{i=1}^{n} g_i^j M^i
    //
    //                |   X^1   |   | g^1_1 g^1_2 g^1_3 | |   M^1   |
    //                |   X^2   | = | g^2_1 g^2_2 g^2_3 | |   M^2   |
    //                |   X^3   |   | g^3_1 g^3_2 g^3_3 | |   M^3   |
    //
    //       | encoded symbol 1 |   | encoding vect 1 | | original symbol 1 |
    //       | encoded symbol 2 | = | encoding vect 2 | | original symbol 2 |
    //       | encoded symbol 3 |   | encoding vect 3 | | original symbol 3 |
    //
    //        | 0 1 0 1 1 1 0 0 |   | 0 1 0 | | 1 1 1 0 1 1 0 1 |
    //        | 1 0 1 1 0 0 0 1 | = | 1 1 0 | | 0 1 0 1 1 1 0 0 |
    //        | 0 1 1 0 1 0 1 1 |   | 1 0 1 | | 1 0 0 0 0 1 1 0 |
    //
    // From the above matrix, the first encoded symbol is just the second
    // original symbol M_2. The second encoded symbol is M_1 bitwise xor M_2,
    // and the third encoded symbol is M_1 bitwise xor M_3.

    // The computer reads the bits in the opposite direction of how the
    // elements are written matematically in the matrices above.
    // Therefore, it may be easier to find the hex values, which we input into
    // the variables below, if the matrices above are rewritten with the bits
    // in the direction which they are stored in memory. This is shown in the
    // matrices below:
    //
    //        | 0 0 1 1 1 0 1 0 |   | 0 1 0 | | 1 0 1 1 0 1 1 1 |
    //        | 1 0 0 0 1 1 0 1 | = | 0 1 1 | | 0 0 1 1 1 0 1 0 |
    //        | 1 1 0 1 0 1 1 0 |   | 1 0 1 | | 0 1 1 0 0 0 0 1 |

    uint8_t original_symbols[] = { 0xb7, 0x3a, 0x61 };
    uint8_t symbol_coefficients[] = { 0x02, 0x03, 0x05 };
    uint8_t encoded_symbols[] = { 0x3a, 0x8d, 0xd6 };

    uint32_t i, j;
    // Initialize symbol storage of the encoder and decoder
    for (i = 0; i < symbols; ++i)
    {
        // Create the individual symbols for the encoder
        input_symbols[i] = (uint8_t*) malloc(symbol_size);

        // Use the predefined input data
        for (j = 0; j < symbol_size; ++j)
            input_symbols[i][j] = original_symbols[i];

        // Store the symbol pointer in the encoder
        kodoc_set_const_symbol(encoder, i, input_symbols[i], symbol_size);

        // Create the output symbol buffers for the decoder
        output_symbols[i] = (uint8_t*) malloc(symbol_size);

        // Specify the output buffers used for decoding
        kodoc_set_mutable_symbol(decoder, i, output_symbols[i], symbol_size);
    }

    EXPECT_TRUE(kodoc_is_complete(decoder) == 0);

    // Generate coded symbols and feed them to the decoder
    for (i = 0; i < symbols; ++i)
    {
        kodoc_write_symbol(encoder, coded_symbol, &symbol_coefficients[i]);

        // We test that kodoc_write_symbol writes the pre-computed symbol
        // to the coded_symbol buffer
        EXPECT_EQ(memcmp(&encoded_symbols[i], coded_symbol, symbol_size), 0);

        // Pass the i'th symbol and coefficients to decoder
        kodoc_read_symbol(decoder, coded_symbol, &symbol_coefficients[i]);

        EXPECT_EQ(i + 1, kodoc_rank(decoder));
    }

    EXPECT_TRUE(kodoc_is_complete(decoder) != 0);

    // Compare the input and output symbols one-by-one
    for (i = 0; i < symbols; ++i)
    {
        EXPECT_EQ(memcmp(input_symbols[i], output_symbols[i], symbol_size), 0);

        free(input_symbols[i]);
        free(output_symbols[i]);
    }

    free(input_symbols);
    free(output_symbols);
    free(coded_symbol);

    kodoc_delete_coder(encoder);
    kodoc_delete_coder(decoder);

    kodoc_delete_factory(encoder_factory);
    kodoc_delete_factory(decoder_factory);
}

TEST(test_read_write_symbol, coded_symbols)
{
    if (kodoc_has_codec(kodoc_full_vector) == false)
        return;

    test_read_write_symbol(3, 1, kodoc_full_vector, kodoc_binary);
}
