// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodoc/kodoc.h>

#include <gtest/gtest.h>

#include "test_helper.hpp"
#include "test_basic_api.hpp"

TEST(TestFullVectorCodes, basic_api)
{
    uint32_t max_symbols = rand_symbols();
    uint32_t max_symbol_size = rand_symbol_size();

    bool shallow_storage = false;

    test_basic_api(kodo_full_vector, max_symbols, max_symbol_size,
        shallow_storage);
}

TEST(TestFullVectorCodes, sparse_api)
{
    // Make sure that the decoding can complete with a lower density
    uint32_t max_symbols = rand_symbols() + 10;
    uint32_t max_symbol_size = rand_symbol_size();

    bool shallow_storage = false;

    test_basic_api(kodo_sparse_full_vector, kodo_full_vector, max_symbols,
        max_symbol_size, shallow_storage);
}

TEST(TestFullVectorCodes, shallow_api)
{
    uint32_t max_symbols = rand_symbols();
    uint32_t max_symbol_size = rand_symbol_size();

    bool shallow_storage = true;

    test_basic_api(kodo_full_vector, max_symbols, max_symbol_size,
        shallow_storage);
}

TEST(TestFullVectorCodes, sparse_shallow_api)
{
    // Make sure that the decoding can complete with a lower density
    uint32_t max_symbols = rand_symbols() + 10;
    uint32_t max_symbol_size = rand_symbol_size();

    bool shallow_storage = true;

    test_basic_api(kodo_sparse_full_vector, kodo_full_vector, max_symbols,
        max_symbol_size, shallow_storage);
}
