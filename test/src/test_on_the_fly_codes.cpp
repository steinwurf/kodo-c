// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodoc/kodoc.h>

#include <cstdlib>

#include <gtest/gtest.h>

#include "test_helper.hpp"

void test_on_the_fly(uint32_t max_symbols, uint32_t max_symbol_size,
                     int32_t code_type, int32_t finite_field)
{
    kodo_factory_t encoder_factory =
        kodo_new_encoder_factory(code_type, finite_field,
                                 max_symbols, max_symbol_size);

    kodo_factory_t decoder_factory =
        kodo_new_decoder_factory(code_type, finite_field,
                                 max_symbols, max_symbol_size);

    kodo_coder_t encoder = kodo_factory_new_encoder(encoder_factory);
    kodo_coder_t decoder = kodo_factory_new_decoder(decoder_factory);

    uint32_t symbol_size = kodo_symbol_size(encoder);
    uint32_t payload_size = kodo_payload_size(encoder);
    uint8_t* payload = (uint8_t*) malloc(payload_size);

    uint32_t block_size = kodo_block_size(encoder);
    uint8_t* data_in = (uint8_t*) malloc(block_size);
    uint8_t* data_out = (uint8_t*) malloc(block_size);

    for (uint32_t i = 0; i < block_size; ++i)
        data_in[i] = rand() % 256;

    kodo_set_mutable_symbols(decoder, data_out, block_size);

    EXPECT_TRUE(kodo_is_complete(decoder) == 0);

    while (!kodo_is_complete(decoder))
    {
        EXPECT_GE(kodo_rank(encoder), kodo_rank(decoder));

        // The rank of an encoder indicates how many symbols have been added,
        // i.e. how many symbols are available for encoding
        uint32_t encoder_rank = kodo_rank(encoder);

        // Randomly choose to add a new symbol (with 50% probability)
        // if the encoder rank is less than the maximum number of symbols
        if ((rand() % 2) && encoder_rank < kodo_symbols(encoder))
        {
            // Calculate the offset to the next symbol to insert
            uint8_t* symbol = data_in + (encoder_rank * symbol_size);
            kodo_set_const_symbol(encoder, encoder_rank, symbol, symbol_size);
        }
        // Generate an encoded packet
        kodo_write_payload(encoder, payload);

        // Simulate that 50% of the packets are lost
        if (rand() % 2) continue;

        // Packet got through - pass that packet to the decoder
        kodo_read_payload(decoder, payload);

        // Check the decoder rank and symbol counters
        EXPECT_GE(kodo_rank(encoder), kodo_rank(decoder));
        EXPECT_GE(kodo_rank(decoder), kodo_symbols_uncoded(decoder));
        EXPECT_GE(kodo_rank(decoder), kodo_symbols_seen(decoder));

        // Check the decoder whether it is partially complete
        // For on-the-fly decoding the decoder has to support the partial
        // decoding tracker.

        if (kodo_has_partial_decoding_tracker(decoder) &&
            kodo_is_partial_complete(decoder))
        {
            for (uint32_t i = 0; i < kodo_symbols(decoder); ++i)
            {
                // Go through all symbols that are already decoded
                if (kodo_is_symbol_uncoded(decoder, i))
                {
                    // All uncoded symbols must have a pivot
                    EXPECT_TRUE(kodo_is_symbol_pivot(decoder, i) != 0);

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

    kodo_delete_encoder(encoder);
    kodo_delete_decoder(decoder);

    kodo_delete_encoder_factory(encoder_factory);
    kodo_delete_decoder_factory(decoder_factory);
}

TEST(test_on_the_fly_codes, invoke_api)
{
    uint32_t max_symbols = rand_symbols();
    uint32_t max_symbol_size = rand_symbol_size();

    test_on_the_fly(max_symbols, max_symbol_size,
                    kodo_on_the_fly, kodo_binary);

    test_on_the_fly(max_symbols, max_symbol_size,
                    kodo_on_the_fly, kodo_binary4);

    test_on_the_fly(max_symbols, max_symbol_size,
                    kodo_on_the_fly, kodo_binary8);
}
