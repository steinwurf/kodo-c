// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_bindings.cpp Unit tests for the Kodo C bindings

#include <gtest/gtest.h>

#include <ckodo/ckodo.h>

void test_basic_api(uint32_t max_symbols, uint32_t max_symbol_size,
                    size_t algorithm, size_t finite_field)
{
    kodo_factory_t* encoder_factory =
        kodo_new_encoder_factory(algorithm, finite_field,
                                 max_symbols, max_symbol_size);

    kodo_factory_t* decoder_factory =
        kodo_new_decoder_factory(algorithm, finite_field,
                                 max_symbols, max_symbol_size);

    kodo_coder_t* encoder = kodo_factory_new_encoder(encoder_factory);
    kodo_coder_t* decoder = kodo_factory_new_decoder(decoder_factory);

    uint32_t payload_size = kodo_payload_size(encoder);
    uint8_t* payload = (uint8_t*) malloc(payload_size);

    uint32_t block_size = kodo_block_size(encoder);
    uint8_t* data_in = (uint8_t*) malloc(block_size);
    uint8_t* data_out = (uint8_t*) malloc(block_size);

    uint32_t i = 0;
    for(; i < block_size; ++i)
        data_in[i] = rand() % 256;

    kodo_set_symbols(encoder, data_in, block_size);

    while(!kodo_is_complete(decoder))
    {
        kodo_encode(encoder, payload);
        kodo_decode(decoder, payload);
    }

    kodo_copy_symbols(decoder, data_out, block_size);

    EXPECT_EQ(memcmp(data_in, data_out, block_size), 0);

    free(data_in);
    free(data_out);
    free(payload);

    kodo_delete_encoder(encoder);
    kodo_delete_decoder(decoder);

    kodo_delete_encoder_factory(encoder_factory);
    kodo_delete_decoder_factory(decoder_factory);
}


TEST(TestFullRlncCodes, invoke_api)
{
    test_basic_api(42, 160, kodo_full_rlnc, kodo_binary);
    test_basic_api(42, 160, kodo_full_rlnc, kodo_binary8);
    test_basic_api(42, 160, kodo_full_rlnc, kodo_binary16);
}


