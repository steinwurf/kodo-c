// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodoc/kodoc.h>

#include <cstdlib>

#include <gtest/gtest.h>

#include "test_helper.hpp"

static uint32_t encoder_trace_called = 0;
static uint32_t decoder_trace_called = 0;

static void encoder_trace_callback(const char* zone, const char* data,
    void* context)
{
    (void) context;
    EXPECT_TRUE(zone != 0);
    EXPECT_TRUE(data != 0);

    encoder_trace_called++;
}

static void decoder_trace_callback(const char* zone, const char* data,
    void* context)
{
    (void) context;
    EXPECT_TRUE(zone != 0);
    EXPECT_TRUE(data != 0);

    decoder_trace_called++;
}

void test_sliding_window(uint32_t max_symbols, uint32_t max_symbol_size,
                         int32_t code_type, int32_t finite_field)
{
    kodo_factory_t encoder_factory =
        kodo_new_encoder_factory(code_type, finite_field,
                                 max_symbols, max_symbol_size);

    kodo_factory_t decoder_factory =
        kodo_new_decoder_factory(code_type, finite_field,
                                 max_symbols, max_symbol_size);

    kodo_coder_t encoder = kodo_factory_new_encoder(encoder_factory);
    kodo_coder_t decoder = kodo_factory_new_decoder(decoder_factory);

    EXPECT_EQ(max_symbols,kodo_factory_max_symbols(encoder_factory));
    EXPECT_EQ(max_symbol_size,kodo_factory_max_symbol_size(encoder_factory));
    EXPECT_EQ(max_symbols, kodo_symbols(encoder));
    EXPECT_EQ(max_symbol_size,kodo_symbol_size(encoder));

    EXPECT_EQ(max_symbols, kodo_factory_max_symbols(decoder_factory));
    EXPECT_EQ(max_symbol_size, kodo_factory_max_symbol_size(decoder_factory));
    EXPECT_EQ(max_symbols, kodo_symbols(decoder));
    EXPECT_EQ(max_symbol_size, kodo_symbol_size(decoder));

    EXPECT_EQ(max_symbols * max_symbol_size, kodo_block_size(encoder));
    EXPECT_EQ(max_symbols * max_symbol_size, kodo_block_size(decoder));

    EXPECT_TRUE(kodo_factory_max_payload_size(encoder_factory) >=
                kodo_payload_size(encoder));

    EXPECT_TRUE(kodo_factory_max_payload_size(decoder_factory) >=
                kodo_payload_size(decoder));

    EXPECT_EQ(kodo_factory_max_payload_size(encoder_factory),
              kodo_factory_max_payload_size(decoder_factory));

    uint32_t feedback_size = 0;

    EXPECT_EQ(kodo_feedback_size(encoder),
              kodo_feedback_size(decoder));

    feedback_size = kodo_feedback_size(encoder);
    EXPECT_TRUE(feedback_size > 0);

    // Allocate some storage for a "payload" the payload is what we would
    // eventually send over a network
    uint32_t payload_size = kodo_payload_size(encoder);
    uint8_t* payload = (uint8_t*) malloc(payload_size);
    uint8_t* feedback = (uint8_t*) malloc(feedback_size);

    // Allocate some data to encode. In this case we make a buffer
    // with the same size as the encoder's block size (the max.
    // amount a single encoder can encode)
    uint32_t block_size = kodo_block_size(encoder);
    uint8_t* data_in = (uint8_t*) malloc(block_size);
    uint8_t* data_out = (uint8_t*) malloc(block_size);

    // Just for fun - fill the data with random data
    for(uint32_t i = 0; i < block_size; ++i)
        data_in[i] = rand() % 256;

    // Install a custom trace function for the encoder and decoder
    kodo_set_trace_callback(encoder, encoder_trace_callback, NULL);
    kodo_set_trace_callback(decoder, decoder_trace_callback, NULL);


    // Assign the data buffer to the encoder so that we may start
    // to produce encoded symbols from it
    kodo_set_const_symbols(encoder, data_in, block_size);

    kodo_set_mutable_symbols(decoder, data_out, block_size);

    EXPECT_TRUE(kodo_is_complete(decoder) == 0);

    while (!kodo_is_complete(decoder))
    {
        // Encode the packet into the payload buffer
        uint32_t payload_used = kodo_write_payload(encoder, payload);
        EXPECT_TRUE(payload_used <= kodo_payload_size(encoder));

        // Pass that packet to the decoder
        kodo_read_payload(decoder, payload);
        EXPECT_TRUE(kodo_is_partial_complete(decoder) != 0);

        kodo_write_feedback(decoder, feedback);
        kodo_read_feedback(encoder, feedback);
    }
    EXPECT_TRUE(kodo_is_complete(decoder) != 0);

    // Check if we properly decoded the data
    EXPECT_EQ(memcmp(data_in, data_out, block_size), 0);

    // Check that the trace functions were called at least once
    EXPECT_GT(encoder_trace_called, 0U);
    EXPECT_GT(decoder_trace_called, 0U);

    free(data_in);
    free(data_out);
    free(payload);
    free(feedback);

    kodo_delete_encoder(encoder);
    kodo_delete_decoder(decoder);

    kodo_delete_encoder_factory(encoder_factory);
    kodo_delete_decoder_factory(decoder_factory);
}

TEST(test_sliding_window_codes, invoke_api)
{
    uint32_t max_symbols = rand_symbols();
    uint32_t max_symbol_size = rand_symbol_size();

    test_sliding_window(max_symbols, max_symbol_size,
                        kodo_sliding_window, kodo_binary);

    test_sliding_window(max_symbols, max_symbol_size,
                        kodo_sliding_window, kodo_binary4);

    test_sliding_window(max_symbols, max_symbol_size,
                        kodo_sliding_window, kodo_binary8);
}
