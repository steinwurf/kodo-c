// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodoc/kodoc.h>

#include <gtest/gtest.h>

#include "test_helper.hpp"
#include "test_basic_api.hpp"
#include "test_decoder_symbol_status_api.hpp"

TEST(test_sliding_window_codes, basic_api)
{
    if (kodoc_has_codec(kodoc_sliding_window) == false)
        return;

    uint32_t max_symbols = rand_symbols();
    uint32_t max_symbol_size = rand_symbol_size();

    test_basic_api(kodoc_sliding_window, max_symbols, max_symbol_size);
}

TEST(test_sliding_window_codes, decoder_symbol_status_api)
{
    if (kodoc_has_codec(kodoc_sliding_window) == false)
        return;

    test_decoder_symbol_status_api(kodoc_sliding_window);
}

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
                         int32_t finite_field)
{
    kodoc_factory_t encoder_factory = kodoc_new_encoder_factory(
        kodoc_sliding_window, finite_field, max_symbols, max_symbol_size);

    kodoc_factory_t decoder_factory = kodoc_new_decoder_factory(
        kodoc_sliding_window, finite_field, max_symbols, max_symbol_size);

    kodoc_coder_t encoder = kodoc_factory_build_coder(encoder_factory);
    kodoc_coder_t decoder = kodoc_factory_build_coder(decoder_factory);

    EXPECT_EQ(max_symbols,kodoc_factory_max_symbols(encoder_factory));
    EXPECT_EQ(max_symbol_size,kodoc_factory_max_symbol_size(encoder_factory));
    EXPECT_EQ(max_symbols, kodoc_symbols(encoder));
    EXPECT_EQ(max_symbol_size,kodoc_symbol_size(encoder));

    EXPECT_EQ(max_symbols, kodoc_factory_max_symbols(decoder_factory));
    EXPECT_EQ(max_symbol_size, kodoc_factory_max_symbol_size(decoder_factory));
    EXPECT_EQ(max_symbols, kodoc_symbols(decoder));
    EXPECT_EQ(max_symbol_size, kodoc_symbol_size(decoder));

    EXPECT_EQ(max_symbols * max_symbol_size, kodoc_block_size(encoder));
    EXPECT_EQ(max_symbols * max_symbol_size, kodoc_block_size(decoder));

    EXPECT_TRUE(kodoc_factory_max_payload_size(encoder_factory) >=
                kodoc_payload_size(encoder));

    EXPECT_TRUE(kodoc_factory_max_payload_size(decoder_factory) >=
                kodoc_payload_size(decoder));

    EXPECT_EQ(kodoc_factory_max_payload_size(encoder_factory),
              kodoc_factory_max_payload_size(decoder_factory));

    uint32_t feedback_size = 0;

    EXPECT_EQ(kodoc_feedback_size(encoder), kodoc_feedback_size(decoder));

    feedback_size = kodoc_feedback_size(encoder);
    EXPECT_TRUE(feedback_size > 0);

    // Allocate some storage for a "payload" the payload is what we would
    // eventually send over a network
    uint32_t payload_size = kodoc_payload_size(encoder);
    uint8_t* payload = (uint8_t*) malloc(payload_size);
    uint8_t* feedback = (uint8_t*) malloc(feedback_size);

    // Allocate some data to encode. In this case we make a buffer
    // with the same size as the encoder's block size (the max.
    // amount a single encoder can encode)
    uint32_t block_size = kodoc_block_size(encoder);
    uint8_t* data_in = (uint8_t*) malloc(block_size);
    uint8_t* data_out = (uint8_t*) malloc(block_size);

    // Just for fun - fill the data with random data
    for (uint32_t i = 0; i < block_size; ++i)
        data_in[i] = rand() % 256;

    // Install a custom trace function for the encoder and decoder
    kodoc_set_trace_callback(encoder, encoder_trace_callback, NULL);
    kodoc_set_trace_callback(decoder, decoder_trace_callback, NULL);

    // Assign the data buffer to the encoder so that we may start
    // to produce encoded symbols from it
    kodoc_set_const_symbols(encoder, data_in, block_size);

    kodoc_set_mutable_symbols(decoder, data_out, block_size);

    EXPECT_TRUE(kodoc_is_complete(decoder) == 0);

    while (!kodoc_is_complete(decoder))
    {
        // Encode the packet into the payload buffer
        uint32_t payload_used = kodoc_write_payload(encoder, payload);
        EXPECT_TRUE(payload_used <= kodoc_payload_size(encoder));

        // Pass that packet to the decoder
        kodoc_read_payload(decoder, payload);
        // All payloads must be innovative due to the perfect feedback
        EXPECT_TRUE(kodoc_is_partially_complete(decoder) != 0);

        kodoc_write_feedback(decoder, feedback);
        kodoc_read_feedback(encoder, feedback);
    }
    EXPECT_TRUE(kodoc_is_complete(decoder) != 0);

    // Check if we properly decoded the data
    EXPECT_EQ(memcmp(data_in, data_out, block_size), 0);

    // Check that the trace functions were called at least once
    EXPECT_GT(encoder_trace_called, 0U);
    EXPECT_GT(decoder_trace_called, 0U);

    free(data_in);
    free(data_out);
    free(payload);
    free(feedback);

    kodoc_delete_coder(encoder);
    kodoc_delete_coder(decoder);

    kodoc_delete_factory(encoder_factory);
    kodoc_delete_factory(decoder_factory);
}

TEST(test_sliding_window_codes, invoke_api)
{
    if (kodoc_has_codec(kodoc_sliding_window) == false)
        return;

    uint32_t max_symbols = rand_symbols();
    uint32_t max_symbol_size = rand_symbol_size();

    test_sliding_window(max_symbols, max_symbol_size, kodoc_binary);

    test_sliding_window(max_symbols, max_symbol_size, kodoc_binary4);

    test_sliding_window(max_symbols, max_symbol_size, kodoc_binary8);
}
