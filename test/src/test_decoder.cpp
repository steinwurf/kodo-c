// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodoc/kodoc.h>

#include <cstdint>

#include <gtest/gtest.h>

#include "test_helper.hpp"
#include "test_coder.hpp"

static void test_decoder(uint32_t symbols, uint32_t symbol_size,
                         int32_t codec, int32_t finite_field)
{
    kodoc_factory_t decoder_factory = kodoc_new_decoder_factory(
        codec, finite_field, symbols, symbol_size);

    kodoc_coder_t decoder = kodoc_factory_build_coder(decoder_factory);

    // Coder methods
    test_coder(decoder, symbols, symbol_size, codec);

    // Decoder methods
    // Some codecs do not provide write_payload, i.e. recoding
    if (codec == kodoc_seed || codec == kodoc_sparse_seed ||
        codec == kodoc_fulcrum || codec == kodoc_reed_solomon)
    {
        EXPECT_TRUE(kodoc_has_write_payload(decoder) == 0);
    }
    else
    {
        EXPECT_TRUE(kodoc_has_write_payload(decoder) != 0);
    }

    EXPECT_EQ(0U, kodoc_symbols_uncoded(decoder));
    EXPECT_EQ(0U, kodoc_symbols_partially_decoded(decoder));

    if (codec == kodoc_on_the_fly ||
        codec == kodoc_sliding_window)
    {
        EXPECT_TRUE(kodoc_has_partial_decoding_interface(decoder) != 0);
    }
    else if (codec == kodoc_full_vector)
    {
        EXPECT_TRUE(kodoc_has_partial_decoding_interface(decoder) == 0);
    }

    kodoc_delete_coder(decoder);
    kodoc_delete_factory(decoder_factory);
}

TEST(test_decoder, invoke_api)
{
    uint32_t symbols = rand_symbols();
    uint32_t symbol_size = rand_symbol_size();

    test_combinations(test_decoder, symbols, symbol_size);
}
