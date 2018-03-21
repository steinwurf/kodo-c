// Copyright Steinwurf ApS 2015.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodoc/kodoc.h>

#include <gtest/gtest.h>

#include "test_helper.hpp"
#include "test_basic_api.hpp"

TEST(test_reed_solomon_codes, basic_api)
{
    if (kodoc_has_codec(kodoc_reed_solomon) == false)
        return;

    uint32_t max_symbols = rand_symbols();
    uint32_t max_symbol_size = rand_symbol_size();

    run_test_basic_api(kodoc_reed_solomon, kodoc_reed_solomon, kodoc_binary8,
                       max_symbols, max_symbol_size);
}
