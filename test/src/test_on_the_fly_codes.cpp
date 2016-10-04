// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodoc/kodoc.h>

#include <gtest/gtest.h>

#include "test_helper.hpp"
#include "test_basic_api.hpp"

TEST(test_on_the_fly_codes, basic_api)
{
    if (kodoc_has_codec(kodoc_on_the_fly) == false)
        return;

    uint32_t max_symbols = rand_symbols();
    uint32_t max_symbol_size = rand_symbol_size();

    test_basic_api(kodoc_on_the_fly, max_symbols, max_symbol_size);
}

void test_on_the_fly(uint32_t max_symbols, uint32_t max_symbol_size,
    int32_t finite_field)
{
    kodoc_factory_t encoder_factory = kodoc_new_encoder_factory(
        kodoc_on_the_fly, finite_field, max_symbols, max_symbol_size);

    kodoc_factory_t decoder_factory = kodoc_new_decoder_factory(
        kodoc_on_the_fly, finite_field, max_symbols, max_symbol_size);

    kodoc_coder_t encoder = kodoc_factory_build_coder(encoder_factory);
    kodoc_coder_t decoder = kodoc_factory_build_coder(decoder_factory);

    uint32_t symbol_size = kodoc_symbol_size(encoder);
    uint32_t payload_size = kodoc_payload_size(encoder);
    uint8_t* payload = (uint8_t*) malloc(payload_size);

    uint32_t block_size = kodoc_block_size(encoder);
    uint8_t* data_in = (uint8_t*) malloc(block_size);
    uint8_t* data_out = (uint8_t*) malloc(block_size);

    for (uint32_t i = 0; i < block_size; ++i)
        data_in[i] = rand() % 256;

    kodoc_set_mutable_symbols(decoder, data_out, block_size);

    EXPECT_TRUE(kodoc_is_complete(decoder) == 0);

    while (!kodoc_is_complete(decoder))
    {
        EXPECT_GE(kodoc_rank(encoder), kodoc_rank(decoder));

        // The rank of an encoder indicates how many symbols have been added,
        // i.e. how many symbols are available for encoding
        uint32_t encoder_rank = kodoc_rank(encoder);

        // Randomly choose to add a new symbol (with 50% probability)
        // if the encoder rank is less than the maximum number of symbols
        if ((rand() % 2) && encoder_rank < kodoc_symbols(encoder))
        {
            // Calculate the offset to the next symbol to insert
            uint8_t* symbol = data_in + (encoder_rank * symbol_size);
            kodoc_set_const_symbol(encoder, encoder_rank, symbol, symbol_size);
        }
        // Generate an encoded packet
        kodoc_write_payload(encoder, payload);

        // Simulate that 50% of the packets are lost
        if (rand() % 2) continue;

        // Packet got through - pass that packet to the decoder
        kodoc_read_payload(decoder, payload);

        // Check the decoder rank and symbol counters
        EXPECT_GE(kodoc_rank(encoder), kodoc_rank(decoder));
        EXPECT_GE(kodoc_rank(decoder), kodoc_symbols_uncoded(decoder));
        EXPECT_GE(kodoc_rank(decoder), kodoc_symbols_partially_decoded(decoder));
        EXPECT_EQ(kodoc_symbols(decoder) - kodoc_rank(decoder),
                  kodoc_symbols_missing(decoder));

        // Check the decoder whether it is partially complete
        // The decoder has to support the partial decoding tracker
        if (kodoc_has_partial_decoding_interface(decoder) &&
            kodoc_is_partially_complete(decoder))
        {
            for (uint32_t i = 0; i < kodoc_symbols(decoder); ++i)
            {
                // Go through all symbols that are already decoded
                if (kodoc_is_symbol_uncoded(decoder, i))
                {
                    // All uncoded symbols must have a pivot
                    EXPECT_TRUE(kodoc_is_symbol_pivot(decoder, i) != 0);
                    // The uncoded symbols cannot be missing
                    EXPECT_TRUE(kodoc_is_symbol_missing(decoder, i) == 0);
                    // The uncoded symbols cannot be partially decoded
                    EXPECT_TRUE(
                        kodoc_is_symbol_partially_decoded(decoder, i) == 0);

                    uint8_t* original = data_in + i * symbol_size;
                    uint8_t* target = data_out + i * symbol_size;

                    // verify the decoded symbol against the original data
                    EXPECT_EQ(memcmp(original, target, symbol_size), 0);
                }
            }
        }
    }

    EXPECT_EQ(memcmp(data_in, data_out, block_size), 0);

    free(data_in);
    free(data_out);
    free(payload);

    kodoc_delete_coder(encoder);
    kodoc_delete_coder(decoder);

    kodoc_delete_factory(encoder_factory);
    kodoc_delete_factory(decoder_factory);
}

TEST(test_on_the_fly_codes, invoke_api)
{
    if (kodoc_has_codec(kodoc_on_the_fly) == false)
        return;

    uint32_t max_symbols = rand_symbols();
    uint32_t max_symbol_size = rand_symbol_size();

    test_on_the_fly(max_symbols, max_symbol_size, kodoc_binary);

    test_on_the_fly(max_symbols, max_symbol_size, kodoc_binary4);

    test_on_the_fly(max_symbols, max_symbol_size, kodoc_binary8);
}
