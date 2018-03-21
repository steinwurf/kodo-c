// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <cstdlib>

#include <gtest/gtest.h>

#include <kodoc/kodoc.h>

inline void test_decoder_symbol_status_api(int32_t decoder_type)
{
    uint32_t symbols = 4;
    uint32_t symbol_size = 40;
    kodoc_factory_t decoder_factory = kodoc_new_decoder_factory(
        decoder_type, kodoc_binary8, symbols, symbol_size);

    kodoc_coder_t decoder = kodoc_factory_build_coder(decoder_factory);

    std::vector<uint8_t> data_out(kodoc_block_size(decoder), '\0');
    kodoc_set_mutable_symbols(decoder, data_out.data(), data_out.size());

    std::vector<uint8_t> symbol(kodoc_symbol_size(decoder));
    std::vector<uint8_t> coefficients(kodoc_coefficient_vector_size(decoder));

    coefficients = {1, 0, 0, 0};
    kodoc_read_symbol(decoder, symbol.data(), coefficients.data());

    EXPECT_EQ(0U, kodoc_symbols_uncoded(decoder));

    kodoc_update_symbol_status(decoder);

    EXPECT_EQ(1U, kodoc_symbols_uncoded(decoder));

    kodoc_set_status_updater_on(decoder);

    coefficients = {0, 1, 0, 0};
    kodoc_read_symbol(decoder, symbol.data(), coefficients.data());

    EXPECT_EQ(2U, kodoc_symbols_uncoded(decoder));

    kodoc_set_status_updater_off(decoder);

    kodoc_delete_coder(decoder);
    kodoc_delete_factory(decoder_factory);
}
