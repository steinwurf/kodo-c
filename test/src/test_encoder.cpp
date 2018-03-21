// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodoc/kodoc.h>

#include <cstdint>

#include <gtest/gtest.h>

#include "test_helper.hpp"
#include "test_coder.hpp"

static void test_encoder(uint32_t symbols, uint32_t symbol_size,
                         int32_t codec, int32_t finite_field)
{
    kodoc_factory_t encoder_factory = kodoc_new_encoder_factory(
        codec, finite_field, symbols, symbol_size);

    kodoc_coder_t encoder = kodoc_factory_build_coder(encoder_factory);

    // Coder methods
    test_coder(encoder, symbols, symbol_size, codec);

    // Encoder methods
    EXPECT_TRUE(kodoc_has_write_payload(encoder) != 0);

    if (kodoc_has_systematic_interface(encoder) != 0)
    {
        EXPECT_TRUE(kodoc_is_systematic_on(encoder) != 0);
        kodoc_set_systematic_off(encoder);
        EXPECT_TRUE(kodoc_is_systematic_on(encoder) == 0);
        kodoc_set_systematic_on(encoder);
        EXPECT_TRUE(kodoc_is_systematic_on(encoder) != 0);
    }

    kodoc_delete_coder(encoder);
    kodoc_delete_factory(encoder_factory);
}

TEST(test_encoder, invoke_api)
{
    uint32_t symbols = rand_symbols();
    uint32_t symbol_size = rand_symbol_size();

    test_combinations(test_encoder, symbols, symbol_size);
}
