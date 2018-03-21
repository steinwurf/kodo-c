// Copyright Steinwurf ApS 2015.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodoc/kodoc.h>

#include <cstdint>

#include <gtest/gtest.h>

static void test_coder(kodoc_coder_t coder, uint32_t symbols,
                       uint32_t symbol_size, int32_t codec)
{
    EXPECT_EQ(symbols, kodoc_symbols(coder));
    EXPECT_EQ(symbol_size, kodoc_symbol_size(coder));
    EXPECT_EQ(symbols * symbol_size, kodoc_block_size(coder));
    EXPECT_GT(kodoc_payload_size(coder), symbol_size);
    EXPECT_EQ(0U, kodoc_rank(coder));

    if (codec == kodoc_sliding_window)
    {
        EXPECT_TRUE(kodoc_has_feedback_size(coder) != 0);
        EXPECT_GT(kodoc_feedback_size(coder), 0U);
    }
    else
    {
        EXPECT_TRUE(kodoc_has_feedback_size(coder) == 0);
    }

    EXPECT_TRUE(kodoc_has_trace_interface(coder) != 0);
    kodoc_set_trace_stdout(coder);
    kodoc_set_trace_off(coder);
    kodoc_set_zone_prefix(coder, "prefix");
}
