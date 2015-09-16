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
    uint32_t max_symbols = rand_symbols();
    uint32_t max_symbol_size = rand_symbol_size();

    test_basic_api(kodo_fulcrum, max_symbols, max_symbol_size);
}

TEST(test_fulcrum_codes, fulcrum_encoder_api)
{
    uint32_t max_symbols = rand_symbols();
    uint32_t max_symbol_size = rand_symbol_size();

    kodo_factory_t encoder_factory = kodo_new_encoder_factory(kodo_fulcrum,
        kodo_binary8, max_symbols, max_symbol_size);

    kodo_coder_t encoder = kodo_factory_new_encoder(encoder_factory);
    EXPECT_TRUE(encoder);

    kodo_delete_encoder(encoder);
    kodo_delete_encoder_factory(encoder_factory);
}

TEST(test_fulcrum_codes, fulcrum_decoder_api)
{
    uint32_t max_symbols = rand_symbols();
    uint32_t max_symbol_size = rand_symbol_size();

    kodo_factory_t decoder_factory = kodo_new_decoder_factory(kodo_fulcrum,
        kodo_binary8, max_symbols, max_symbol_size);

    kodo_coder_t decoder = kodo_factory_new_decoder(decoder_factory);
    EXPECT_TRUE(decoder);

    kodo_delete_encoder(decoder);
    kodo_delete_encoder_factory(decoder_factory);
}
