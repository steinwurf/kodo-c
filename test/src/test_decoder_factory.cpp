// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodoc/kodoc.h>

#include <cstdint>

#include <gtest/gtest.h>

#include "test_helper.hpp"

static void test_decoder_factory(uint32_t max_symbols, uint32_t max_symbol_size,
                                 int32_t code_type, int32_t finite_field)
{
    kodo_factory_t decoder_factory =
        kodo_new_decoder_factory(code_type, finite_field,
                                 max_symbols, max_symbol_size);

    // Test the max_* properties
    EXPECT_EQ(max_symbols, kodo_factory_max_symbols(decoder_factory));
    EXPECT_EQ(max_symbol_size, kodo_factory_max_symbol_size(decoder_factory));
    EXPECT_EQ(max_symbol_size * max_symbols,
              kodo_factory_max_block_size(decoder_factory));
    EXPECT_GT(kodo_factory_max_payload_size(decoder_factory), max_symbol_size);

    // Build an decoder with the default settings
    kodo_coder_t decoder = kodo_factory_new_decoder(decoder_factory);

    EXPECT_EQ(max_symbols, kodo_symbols(decoder));
    EXPECT_EQ(max_symbol_size, kodo_symbol_size(decoder));

    // Lower the number of symbols and the symbol_size
    uint32_t new_symbols = max_symbols / 2;
    kodo_factory_set_symbols(decoder_factory, new_symbols);

    uint32_t new_symbol_size = max_symbol_size - 4;
    kodo_factory_set_symbol_size(decoder_factory, new_symbol_size);

    // Test that the max_* properties are not changed
    EXPECT_EQ(max_symbols, kodo_factory_max_symbols(decoder_factory));
    EXPECT_EQ(max_symbol_size, kodo_factory_max_symbol_size(decoder_factory));
    EXPECT_EQ(max_symbol_size * max_symbols,
              kodo_factory_max_block_size(decoder_factory));
    EXPECT_GT(kodo_factory_max_payload_size(decoder_factory), max_symbol_size);

    // Build an decoder with the changed settings
    kodo_coder_t decoder2 = kodo_factory_new_decoder(decoder_factory);

    EXPECT_EQ(new_symbols, kodo_symbols(decoder2));
    EXPECT_EQ(new_symbol_size, kodo_symbol_size(decoder2));

    kodo_delete_decoder(decoder);
    kodo_delete_decoder(decoder2);
    kodo_delete_decoder_factory(decoder_factory);
}

TEST(test_decoder_factory, invoke_api)
{
    // Make sure that we can lower these values in the test
    uint32_t max_symbols = rand_symbols() + 1;
    uint32_t max_symbol_size = rand_symbol_size() + 4;

    test_combinations(test_decoder_factory, max_symbols, max_symbol_size);
}
