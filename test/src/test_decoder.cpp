// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodoc/kodoc.h>

#include <cstdint>

#include <gtest/gtest.h>

#include "test_helper.hpp"
#include "test_coder.hpp"

static void test_decoder(uint32_t symbols, uint32_t symbol_size,
                         int32_t code_type, int32_t finite_field,
                         int32_t trace)
{
    kodo_factory_t decoder_factory =
        kodo_new_decoder_factory(code_type, finite_field,
                                 symbols, symbol_size,
                                 trace,
                                 kodo_deep_storage);

    kodo_coder_t decoder = kodo_factory_new_decoder(decoder_factory);

    // Coder methods
    test_coder(decoder, symbols, symbol_size, code_type, trace);

    // Decoder methods
    // Seed-based codecs do not provide write_payload, i.e. recoding
    if (code_type == kodo_seed)
    {
        EXPECT_TRUE(kodo_has_write_payload(decoder) == 0);
    }
    else
    {
        EXPECT_TRUE(kodo_has_write_payload(decoder) != 0);
    }

    EXPECT_EQ(0U, kodo_symbols_uncoded(decoder));
    EXPECT_EQ(0U, kodo_symbols_seen(decoder));

    if (code_type == kodo_on_the_fly ||
        code_type == kodo_sliding_window)
    {
        EXPECT_TRUE(kodo_has_partial_decoding_tracker(decoder) != 0);
    }
    else if (code_type == kodo_full_vector)
    {
        EXPECT_TRUE(kodo_has_partial_decoding_tracker(decoder) == 0);
    }

    kodo_delete_decoder(decoder);
    kodo_delete_decoder_factory(decoder_factory);
}

TEST(test_decoder, invoke_api)
{
    uint32_t symbols = rand_symbols();
    uint32_t symbol_size = rand_symbol_size();

    test_combinations(
        test_decoder,
        symbols, symbol_size, kodo_trace_disabled);

    test_combinations(
        test_decoder,
        symbols, symbol_size, kodo_trace_enabled);
}
