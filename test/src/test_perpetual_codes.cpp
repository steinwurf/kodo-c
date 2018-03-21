// Copyright Steinwurf ApS 2015.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodoc/kodoc.h>

#include <gtest/gtest.h>

#include "test_helper.hpp"
#include "test_basic_api.hpp"
#include "test_decoder_symbol_status_api.hpp"

TEST(test_perpetual_codes, basic_api)
{
    if (kodoc_has_codec(kodoc_perpetual) == false)
        return;

    uint32_t max_symbols = rand_symbols();
    uint32_t max_symbol_size = rand_symbol_size();

    test_basic_api(kodoc_perpetual, max_symbols, max_symbol_size);
}

TEST(test_perpetual_codes, decoder_symbol_status_api)
{
    if (kodoc_has_codec(kodoc_perpetual) == false)
        return;

    test_decoder_symbol_status_api(kodoc_perpetual);
}

TEST(test_perpetual_codes, perpetual_api)
{
    if (kodoc_has_codec(kodoc_perpetual) == false)
        return;

    uint32_t max_symbols = rand_symbols();
    uint32_t max_symbol_size = rand_symbol_size();

    kodoc_factory_t encoder_factory = kodoc_new_encoder_factory(
        kodoc_perpetual, kodoc_binary8, max_symbols, max_symbol_size);

    kodoc_coder_t encoder = kodoc_factory_build_coder(encoder_factory);
    EXPECT_TRUE(encoder != 0);

    EXPECT_FALSE(kodoc_pseudo_systematic(encoder) != 0);
    uint8_t pseudo_systematic = true;
    kodoc_set_pseudo_systematic(encoder, pseudo_systematic);
    EXPECT_TRUE(kodoc_pseudo_systematic(encoder) != 0);

    EXPECT_FALSE(kodoc_pre_charging(encoder));
    uint8_t pre_charging = true;
    kodoc_set_pre_charging(encoder, pre_charging);
    EXPECT_TRUE(kodoc_pre_charging(encoder) != 0);

    // width must be < symbols
    uint32_t width = max_symbols - 1;
    kodoc_set_width(encoder, width);
    EXPECT_EQ(width, kodoc_width(encoder));

    double width_ratio = 1.0;
    kodoc_set_width_ratio(encoder, width_ratio);
    EXPECT_EQ(width_ratio, kodoc_width_ratio(encoder));

    kodoc_delete_coder(encoder);
    kodoc_delete_factory(encoder_factory);
}
