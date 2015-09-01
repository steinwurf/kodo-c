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
    int32_t finite_field, uint32_t symbols, uint32_t symbol_size,
    int32_t storage_mode)
{
    kodo_factory_t encoder_factory =
        kodo_new_encoder_factory(encoder_type, finite_field,
                                         symbols, symbol_size,
                                         kodo_trace_disabled,
                                         storage_mode);

    kodo_factory_t decoder_factory =
        kodo_new_decoder_factory(decoder_type, finite_field,
                                         symbols, symbol_size,
                                         kodo_trace_disabled,
                                         storage_mode);

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

    if (encoder_type == kodo_sparse_full_vector)
    {
        // Set the coding vector density on the encoder
        kodo_set_density(encoder, 0.2);
        EXPECT_EQ(0.2, kodo_density(encoder));
    }

    uint32_t payload_size = kodo_payload_size(encoder);
    uint8_t* payload = (uint8_t*) malloc(payload_size);

    uint8_t** input_symbols = NULL;
    uint8_t** output_symbols = NULL;
    uint8_t* data_in = NULL;
    uint8_t* data_out = NULL;

    if (storage_mode == kodo_shallow_storage)
    {
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
            kodo_set_const_symbol(encoder, i, input_symbols[i], symbol_size);

            // Create the output symbol buffers for the decoder
            output_symbols[i] = (uint8_t*) malloc(symbol_size);

            // Specify the output buffers used for decoding
            kodo_set_mutable_symbol(decoder, i, output_symbols[i], symbol_size);
        }
    }
    else // if deep
    {
        uint32_t block_size = kodo_block_size(encoder);
        data_in = (uint8_t*) malloc(block_size);
        data_out = (uint8_t*) malloc(block_size);

        for(uint32_t i = 0; i < block_size; ++i)
            data_in[i] = rand() % 256;

        kodo_set_const_symbols(encoder, data_in, block_size);
    }

    EXPECT_TRUE(kodo_is_complete(decoder) == 0);

    while (!kodo_is_complete(decoder))
    {
        kodo_write_payload(encoder, payload);
        kodo_read_payload(decoder, payload);
    }

    EXPECT_TRUE(kodo_is_complete(decoder) != 0);

    if (storage_mode == kodo_shallow_storage)
    {
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
    }
    else // if deep
    {
        assert(data_in);
        assert(data_out);

        uint32_t block_size = kodo_block_size(encoder);
        kodo_copy_from_symbols(decoder, data_out, block_size);

        EXPECT_EQ(memcmp(data_in, data_out, block_size), 0);

        free(data_in);
        free(data_out);
    }

    free(payload);

    kodo_delete_encoder(encoder);
    kodo_delete_decoder(decoder);

    kodo_delete_encoder_factory(encoder_factory);
    kodo_delete_decoder_factory(decoder_factory);
}

inline void test_basic_api(int32_t encoder_type, int32_t decoder_type,
                           uint32_t symbols, uint32_t symbol_size,
                           int32_t storage_mode)
{
    SCOPED_TRACE(testing::Message() << "symbols = " << symbols);
    SCOPED_TRACE(testing::Message() << "symbol_size = " << symbol_size);
    SCOPED_TRACE(testing::Message() << "storage type = "
                                    << (storage_mode == kodo_shallow_storage ?
                                        "shallow" : "deep"));
    {
        SCOPED_TRACE(testing::Message() << "field = binary");
        run_test_basic_api(encoder_type, decoder_type, kodo_binary, symbols,
            symbol_size, storage_mode);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary4");
        run_test_basic_api(encoder_type, decoder_type, kodo_binary4, symbols,
            symbol_size, storage_mode);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary8");
        run_test_basic_api(encoder_type, decoder_type, kodo_binary8, symbols,
            symbol_size, storage_mode);
    }
}

inline void test_basic_api(int32_t coder_type, uint32_t symbols,
    uint32_t symbol_size, int32_t storage_mode)
{
    test_basic_api(coder_type, coder_type, symbols, symbol_size, storage_mode);
}
