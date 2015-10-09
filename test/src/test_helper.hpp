// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <cstdlib>
#include <vector>
#include <functional>

#include <gtest/gtest.h>

#include <kodoc/kodoc.h>

inline uint32_t rand_nonzero(uint32_t max_value = 256)
{
    assert(max_value > 0);
    return (rand() % max_value) + 1;
}

inline uint32_t rand_symbols(uint32_t max_symbols = 64)
{
    return rand_nonzero(max_symbols);
}

inline uint32_t rand_symbol_size(uint32_t max_symbol_size = 1600)
{
    /// Currently the biggest field we support is 2^32, so we always make
    /// sure that the symbol size is a multiple of 4 bytes.
    uint32_t granularity = 4;
    uint32_t elements = max_symbol_size / granularity;

    return rand_nonzero(elements) * granularity;
}

using test_function = std::function<
    void (uint32_t, uint32_t, int32_t, int32_t)>;

inline void test_combinations(
    test_function coder_test,
    uint32_t max_symbols,
    uint32_t max_symbol_size)
{
    SCOPED_TRACE(testing::Message() << "symbols = " << max_symbols);
    SCOPED_TRACE(testing::Message() << "symbol_size = " << max_symbol_size);

    std::vector<int32_t> code_types =
    {
        kodo_full_vector,
        kodo_on_the_fly//,
//         kodo_sliding_window,
//         kodo_seed,
//         kodo_sparse_seed,
//         kodo_perpetual
    };

    std::vector<int32_t> fields =
    {
        kodo_binary,
        kodo_binary4,
        kodo_binary8
    };

    for (auto& code : code_types)
    {
        for (auto& field : fields)
        {
            if (coder_test)
            {
                coder_test(max_symbols, max_symbol_size, code, field);
            }
        }
    }
}
