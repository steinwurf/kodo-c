// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <cstdlib>

#include <gtest/gtest.h>

#include <kodoc/kodoc.h>

inline void run_test_basic_api(int32_t encoder_type, int32_t decoder_type,
                               int32_t finite_field, uint32_t symbols,
                               uint32_t symbol_size)
{
    kodoc_factory_t encoder_factory = kodoc_new_encoder_factory(
        encoder_type, finite_field, symbols, symbol_size);

    kodoc_factory_t decoder_factory = kodoc_new_decoder_factory(
        decoder_type, finite_field, symbols, symbol_size);

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

    if (encoder_type == kodoc_sparse_full_vector ||
        encoder_type == kodoc_sparse_seed)
    {
        // Set the coding vector density on the encoder
        kodoc_set_density(encoder, 0.2);
        EXPECT_EQ(0.2, kodoc_density(encoder));
    }

    uint32_t payload_size = kodoc_payload_size(encoder);
    uint8_t* payload = (uint8_t*) malloc(payload_size);

    uint8_t** input_symbols = NULL;
    uint8_t** output_symbols = NULL;

    // Allocate symbols in non-contiguous buffers
    input_symbols = (uint8_t**) malloc(symbols * sizeof(uint8_t*));
    output_symbols = (uint8_t**) malloc(symbols * sizeof(uint8_t*));

    for (uint32_t i = 0; i < symbols; ++i)
    {
        // Create the individual symbols for the encoder
        input_symbols[i] = (uint8_t*) malloc(symbol_size);

        // Randomize input data
        for (uint32_t j = 0; j < symbol_size; ++j)
            input_symbols[i][j] = rand() % 256;

        // Store the symbol pointer in the encoder
        kodoc_set_const_symbol(encoder, i, input_symbols[i], symbol_size);

        // Create the output symbol buffers for the decoder
        output_symbols[i] = (uint8_t*) malloc(symbol_size);

        // Specify the output buffers used for decoding
        kodoc_set_mutable_symbol(decoder, i, output_symbols[i], symbol_size);
    }

    if (kodoc_has_symbol_decoding_status_updater_interface(decoder))
    {
        EXPECT_FALSE(kodoc_is_status_updater_enabled(decoder));
        kodoc_set_status_updater_on(decoder);
        EXPECT_TRUE(kodoc_is_status_updater_enabled(decoder));
        kodoc_set_status_updater_off(decoder);
        EXPECT_FALSE(kodoc_is_status_updater_enabled(decoder));
    }
    else
    {
        EXPECT_TRUE(
            decoder_type == kodoc_fulcrum ||
            decoder_type == kodoc_reed_solomon
        );
    }

    EXPECT_TRUE(kodoc_is_complete(decoder) == 0);

    while (!kodoc_is_complete(decoder))
    {
        kodoc_write_payload(encoder, payload);
        kodoc_read_payload(decoder, payload);
    }

    EXPECT_TRUE(kodoc_is_complete(decoder) != 0);
    EXPECT_EQ(symbols, kodoc_rank(decoder));
    EXPECT_EQ(symbols, kodoc_symbols_uncoded(decoder));
    EXPECT_EQ(0U, kodoc_symbols_partially_decoded(decoder));
    EXPECT_EQ(0U, kodoc_symbols_missing(decoder));

    assert(input_symbols);
    assert(output_symbols);
    // Compare the input and output symbols one-by-one
    for (uint32_t i = 0; i < symbols; ++i)
    {
        EXPECT_EQ(memcmp(input_symbols[i], output_symbols[i], symbol_size), 0);

        free(input_symbols[i]);
        free(output_symbols[i]);
    }

    free(input_symbols);
    free(output_symbols);

    free(payload);

    kodoc_delete_coder(encoder);
    kodoc_delete_coder(decoder);

    kodoc_delete_factory(encoder_factory);
    kodoc_delete_factory(decoder_factory);
}

inline void test_basic_api(int32_t encoder_type, int32_t decoder_type,
                           uint32_t symbols, uint32_t symbol_size)
{
    SCOPED_TRACE(testing::Message() << "symbols = " << symbols);
    SCOPED_TRACE(testing::Message() << "symbol_size = " << symbol_size);

    {
        SCOPED_TRACE(testing::Message() << "field = binary");
        run_test_basic_api(encoder_type, decoder_type, kodoc_binary, symbols,
                           symbol_size);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary4");
        run_test_basic_api(encoder_type, decoder_type, kodoc_binary4, symbols,
                           symbol_size);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary8");
        run_test_basic_api(encoder_type, decoder_type, kodoc_binary8, symbols,
                           symbol_size);
    }
}

inline void test_basic_api(int32_t coder_type, uint32_t symbols,
                           uint32_t symbol_size)
{
    test_basic_api(coder_type, coder_type, symbols, symbol_size);
}
