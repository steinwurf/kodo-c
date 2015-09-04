// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodoc/kodoc.h>

#include <cstdint>

#include <gtest/gtest.h>

#include "test_helper.hpp"

static void test_encoder_factory(uint32_t max_symbols, uint32_t max_symbol_size,
                                 int32_t code_type, int32_t finite_field)
{
    kodo_factory_t encoder_factory =
        kodo_new_encoder_factory(code_type, finite_field,
                                 max_symbols, max_symbol_size);

    // Test the max_* properties
    EXPECT_EQ(max_symbols, kodo_factory_max_symbols(encoder_factory));
    EXPECT_EQ(max_symbol_size, kodo_factory_max_symbol_size(encoder_factory));
    EXPECT_EQ(max_symbol_size * max_symbols,
              kodo_factory_max_block_size(encoder_factory));
    EXPECT_GT(kodo_factory_max_payload_size(encoder_factory), max_symbol_size);

    // Build an encoder with the default settings
    kodo_coder_t encoder = kodo_factory_new_encoder(encoder_factory);

    EXPECT_EQ(max_symbols, kodo_symbols(encoder));
    EXPECT_EQ(max_symbol_size, kodo_symbol_size(encoder));

    // Lower the number of symbols and the symbol_size
    uint32_t new_symbols = max_symbols / 2;
    kodo_factory_set_symbols(encoder_factory, new_symbols);

    uint32_t new_symbol_size = max_symbol_size - 4;
    kodo_factory_set_symbol_size(encoder_factory, new_symbol_size);

    // Test that the max_* properties are not changed
    EXPECT_EQ(max_symbols, kodo_factory_max_symbols(encoder_factory));
    EXPECT_EQ(max_symbol_size, kodo_factory_max_symbol_size(encoder_factory));
    EXPECT_EQ(max_symbol_size * max_symbols,
              kodo_factory_max_block_size(encoder_factory));
    EXPECT_GT(kodo_factory_max_payload_size(encoder_factory), max_symbol_size);

    // Build an encoder with the changed settings
    kodo_coder_t encoder2 = kodo_factory_new_encoder(encoder_factory);

    EXPECT_EQ(new_symbols, kodo_symbols(encoder2));
    EXPECT_EQ(new_symbol_size, kodo_symbol_size(encoder2));

    kodo_delete_encoder(encoder);
    kodo_delete_encoder(encoder2);
    kodo_delete_encoder_factory(encoder_factory);
}

TEST(test_encoder_factory, invoke_api)
{
    // Make sure that we can lower these values in the test
    uint32_t max_symbols = rand_symbols() + 1;
    uint32_t max_symbol_size = rand_symbol_size() + 4;

    test_combinations(test_encoder_factory, max_symbols, max_symbol_size);
}
