// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodoc/kodoc.h>

#include <gtest/gtest.h>

#include "test_helper.hpp"
#include "test_basic_api.hpp"
#include "test_decoder_symbol_status_api.hpp"

TEST(test_full_vector_codes, basic_api)
{
    if (kodoc_has_codec(kodoc_full_vector) == false)
        return;

    uint32_t max_symbols = rand_symbols();
    uint32_t max_symbol_size = rand_symbol_size();

    test_basic_api(kodoc_full_vector, max_symbols, max_symbol_size);
}

TEST(test_full_vector_codes, decoder_symbol_status_api)
{
    if (kodoc_has_codec(kodoc_full_vector) == false)
        return;

    test_decoder_symbol_status_api(kodoc_full_vector);
}
