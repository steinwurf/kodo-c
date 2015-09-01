// Copyright Steinwurf ApS 2015.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodoc/kodoc.h>

#include <gtest/gtest.h>

#include "test_helper.hpp"
#include "test_basic_api.hpp"

TEST(test_perpetual_codes, basic_api)
{
    uint32_t max_symbols = rand_symbols();
    uint32_t max_symbol_size = rand_symbol_size();

    test_basic_api(kodo_perpetual, max_symbols, max_symbol_size,
        kodo_deep_storage);
}

TEST(test_perpetual_codes, perpetual_api)
{
    uint32_t max_symbols = rand_symbols();
    uint32_t max_symbol_size = rand_symbol_size();

    kodo_factory_t encoder_factory = kodo_new_encoder_factory(kodo_perpetual,
        kodo_binary8, max_symbols, max_symbol_size, kodo_trace_enabled,
        kodo_deep_storage);

    kodo_coder_t encoder = kodo_factory_new_encoder(encoder_factory);
    EXPECT_TRUE(encoder);

    EXPECT_FALSE(kodo_pseudo_systematic(encoder));
    uint8_t pseudo_systematic = true;
    kodo_set_pseudo_systematic(encoder, pseudo_systematic);
    EXPECT_TRUE(kodo_pseudo_systematic(encoder));

    EXPECT_FALSE(kodo_pre_charging(encoder));
    uint8_t pre_charging = true;
    kodo_set_pre_charging(encoder, pre_charging);
    EXPECT_TRUE(kodo_pre_charging(encoder));

    // width must be < symbols
    uint32_t width = max_symbols - 1;
    kodo_set_width(encoder, width);
    EXPECT_EQ(width, kodo_width(encoder));

    double width_ratio = 1.0;
    kodo_set_width_ratio(encoder, width_ratio);
    EXPECT_EQ(width_ratio, kodo_width_ratio(encoder));

    kodo_delete_encoder(encoder);
    kodo_delete_encoder_factory(encoder_factory);
}
