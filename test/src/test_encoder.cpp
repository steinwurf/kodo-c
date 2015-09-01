// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodoc/kodoc.h>

#include <cstdint>

#include <gtest/gtest.h>

#include "test_helper.hpp"
#include "test_coder.hpp"

static void test_encoder(uint32_t symbols, uint32_t symbol_size,
                         int32_t code_type, int32_t finite_field,
                         int32_t trace)
{
    kodo_factory_t encoder_factory = kodo_new_encoder_factory(
        code_type, finite_field, symbols, symbol_size, trace,
        kodo_deep_storage);

    kodo_coder_t encoder = kodo_factory_new_encoder(encoder_factory);

    // Coder methods
    test_coder(encoder, symbols, symbol_size, code_type, trace);

    // Encoder methods
    EXPECT_TRUE(kodo_has_write_payload(encoder) != 0);

    if (kodo_has_set_systematic_off(encoder) != 0)
    {
        EXPECT_TRUE(kodo_is_systematic_on(encoder) != 0);
        kodo_set_systematic_off(encoder);
        EXPECT_TRUE(kodo_is_systematic_on(encoder) == 0);
        kodo_set_systematic_on(encoder);
        EXPECT_TRUE(kodo_is_systematic_on(encoder) != 0);
    }

    kodo_delete_encoder(encoder);
    kodo_delete_encoder_factory(encoder_factory);
}

TEST(test_encoder, invoke_api)
{
    uint32_t symbols = rand_symbols();
    uint32_t symbol_size = rand_symbol_size();

    test_combinations(
        test_encoder,
        symbols, symbol_size, kodo_trace_disabled);

    test_combinations(
        test_encoder,
        symbols, symbol_size, kodo_trace_enabled);
}
