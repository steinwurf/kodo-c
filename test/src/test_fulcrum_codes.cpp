// Copyright Steinwurf ApS 2015.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodoc/kodoc.h>

#include <gtest/gtest.h>

#include "test_helper.hpp"
#include "test_basic_api.hpp"

TEST(test_fulcrum_codes, basic_api)
{
    if (kodoc_has_codec(kodoc_fulcrum) == false)
        return;

    uint32_t max_symbols = rand_symbols();
    uint32_t max_symbol_size = rand_symbol_size();

    test_basic_api(kodoc_fulcrum, max_symbols, max_symbol_size);
}

static void test_fulcrum_api(kodoc_factory_t fulcrum_factory)
{
    uint32_t max_expansion = kodoc_factory_max_expansion(fulcrum_factory);

    EXPECT_EQ(10U, max_expansion);

    uint32_t max_symbols = kodoc_factory_max_symbols(fulcrum_factory);

    EXPECT_EQ(max_expansion + max_symbols,
              kodoc_factory_max_inner_symbols(fulcrum_factory));

    kodoc_factory_set_expansion(fulcrum_factory, max_expansion);

    kodoc_coder_t coder = kodoc_factory_build_coder(fulcrum_factory);
    EXPECT_TRUE(coder != 0);
    EXPECT_EQ(max_expansion, kodoc_expansion(coder));
    EXPECT_EQ(max_symbols + max_expansion, kodoc_inner_symbols(coder));
    kodoc_delete_coder(coder);

    uint32_t other_expansion = max_expansion / 2;
    kodoc_factory_set_expansion(fulcrum_factory, other_expansion);

    kodoc_coder_t other_coder = kodoc_factory_build_coder(fulcrum_factory);
    EXPECT_TRUE(other_coder != 0);
    EXPECT_EQ(other_expansion, kodoc_expansion(other_coder));
    EXPECT_EQ(other_expansion + max_symbols, kodoc_inner_symbols(other_coder));
    kodoc_delete_coder(other_coder);
}

TEST(test_fulcrum_codes, fulcrum_encoder_api)
{
    if (kodoc_has_codec(kodoc_fulcrum) == false)
        return;

    uint32_t max_symbols = rand_symbols();
    uint32_t max_symbol_size = rand_symbol_size();

    kodoc_factory_t encoder_factory = kodoc_new_encoder_factory(
        kodoc_fulcrum, kodoc_binary8, max_symbols, max_symbol_size);
    test_fulcrum_api(encoder_factory);

    kodoc_coder_t encoder = kodoc_factory_build_coder(encoder_factory);

    uint32_t symbols = kodoc_symbols(encoder);
    uint32_t expansion = kodoc_expansion(encoder);
    EXPECT_EQ(symbols + expansion, kodoc_nested_symbols(encoder));
    EXPECT_EQ(kodoc_symbol_size(encoder), kodoc_nested_symbol_size(encoder));

    kodoc_delete_coder(encoder);
    kodoc_delete_factory(encoder_factory);
}

TEST(test_fulcrum_codes, fulcrum_decoder_api)
{
    if (kodoc_has_codec(kodoc_fulcrum) == false)
        return;

    uint32_t max_symbols = rand_symbols();
    uint32_t max_symbol_size = rand_symbol_size();

    kodoc_factory_t decoder_factory = kodoc_new_decoder_factory(
        kodoc_fulcrum, kodoc_binary8, max_symbols, max_symbol_size);
    test_fulcrum_api(decoder_factory);
    kodoc_delete_factory(decoder_factory);
}
