// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_bindings.cpp Unit tests for the Kodo C bindings

#include <gtest/gtest.h>

#include <ckodo/ckodo.h>

void test_on_the_fly(uint32_t max_symbols, uint32_t max_symbol_size,
                     size_t algorithm, size_t finite_field)
{
    kodo_factory_t* encoder_factory =
        kodo_new_encoder_factory(algorithm, finite_field,
                                 max_symbols, max_symbol_size);

    kodo_factory_t* decoder_factory =
        kodo_new_decoder_factory(algorithm, finite_field,
                                 max_symbols, max_symbol_size);

    kodo_coder_t* encoder = kodo_factory_new_encoder(encoder_factory);
    kodo_coder_t* decoder = kodo_factory_new_decoder(decoder_factory);

    uint32_t symbol_size = kodo_symbol_size(encoder);
    uint32_t payload_size = kodo_payload_size(encoder);
    uint8_t* payload = (uint8_t*) malloc(payload_size);

    uint32_t block_size = kodo_block_size(encoder);
    uint8_t* data_in = (uint8_t*) malloc(block_size);
    uint8_t* data_out = (uint8_t*) malloc(block_size);

    uint32_t i = 0;
    for (; i < block_size; ++i)
        data_in[i] = rand() % 256;

    while (!kodo_is_complete(decoder))
    {
        EXPECT_TRUE(kodo_rank(encoder) >= kodo_rank(decoder));

        // Randomly choose to add a new symbol (with 50% probability)
        // if the encoder rank is less than the maximum number of symbols
        if ((rand() % 2) && kodo_rank(encoder) < kodo_symbols(encoder))
        {
            // The rank of an encoder indicates how many symbols have been added,
            // i.e. how many symbols are available for encoding
            uint32_t rank = kodo_rank(encoder);

            // Calculate the offset to the next symbol to insert
            uint8_t* symbol = data_in + (rank * symbol_size);
            kodo_set_symbol(encoder, rank, symbol, symbol_size);
        }
        // Generate an encoded packet
        kodo_encode(encoder, payload);

        // Simulate that 50% of the packets are lost
        if (rand() % 2) continue;

        // Packet got through - pass that packet to the decoder
        kodo_decode(decoder, payload);

        // Check the decoder whether it is partially complete
        // For on-the-fly decoding the decoder has to support the partial
        // decoding tracker.

        if (kodo_has_partial_decoding_tracker(decoder) &&
            kodo_is_partial_complete(decoder))
        {
            uint32_t i = 0;
            for (; i < kodo_symbols(decoder); ++i)
            {
                // Go through all symbols that are already decoded
                if (kodo_is_symbol_decoded(decoder, i))
                {
                    uint8_t* original = data_in + i * symbol_size;
                    uint8_t* target = data_out + i * symbol_size;

                    // Copy the decoded symbol and verify it against the
                    // original data
                    kodo_copy_symbol(decoder, i, target, symbol_size);
                    EXPECT_EQ(memcmp(original, target, symbol_size), 0);
                }
            }
        }
    }

    kodo_copy_symbols(decoder, data_out, block_size);

    EXPECT_EQ(memcmp(data_in, data_out, block_size), 0);

    free(data_in);
    free(data_out);
    free(payload);

    kodo_delete_encoder(encoder);
    kodo_delete_decoder(decoder);

    kodo_delete_encoder_factory(encoder_factory);
    kodo_delete_decoder_factory(decoder_factory);
}


TEST(TestOnTheFlyRlncCodes, invoke_api_32)
{
    uint32_t symbols = 32;
    test_on_the_fly(symbols, 160, kodo_on_the_fly, kodo_binary);
    test_on_the_fly(symbols, 160, kodo_on_the_fly, kodo_binary8);
    test_on_the_fly(symbols, 160, kodo_on_the_fly, kodo_binary16);
}

TEST(TestOnTheFlyRlncCodes, invoke_api_128)
{
    uint32_t symbols = 128;
    test_on_the_fly(symbols, 160, kodo_on_the_fly, kodo_binary);
    test_on_the_fly(symbols, 160, kodo_on_the_fly, kodo_binary8);
    test_on_the_fly(symbols, 160, kodo_on_the_fly, kodo_binary16);
}
