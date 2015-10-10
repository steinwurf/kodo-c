// Copyright Steinwurf ApS 2015.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodoc/kodoc.h>

#include <cstdint>

#include <gtest/gtest.h>

static void test_coder(kodo_coder_t coder, uint32_t symbols,
                       uint32_t symbol_size, int32_t code_type)
{
    EXPECT_EQ(symbols, kodo_symbols(coder));
    EXPECT_EQ(symbol_size, kodo_symbol_size(coder));
    EXPECT_EQ(symbols * symbol_size, kodo_block_size(coder));
    EXPECT_GT(kodo_payload_size(coder), symbol_size);
    EXPECT_EQ(0U, kodo_rank(coder));

    if (code_type == kodo_sliding_window)
    {
        EXPECT_TRUE(kodo_has_feedback_size(coder) != 0);
        EXPECT_GT(kodo_feedback_size(coder), 0U);
    }
    else
    {
        EXPECT_TRUE(kodo_has_feedback_size(coder) == 0);
    }

    EXPECT_TRUE(kodo_has_set_trace_callback(coder) != 0);
    EXPECT_TRUE(kodo_has_set_trace_stdout(coder) != 0);
    EXPECT_TRUE(kodo_has_set_trace_off(coder) != 0);
    kodo_set_trace_stdout(coder);
    kodo_set_trace_off(coder);
}
