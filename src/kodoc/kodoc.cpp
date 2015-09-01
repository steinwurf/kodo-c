// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include "kodoc.h"

#include <cstring>
#include <cstdint>
#include <cassert>

#include "kodo_decoder.hpp"
#include "kodo_encoder.hpp"
#include "kodo_factory.hpp"
#include "kodo_sparse_encoder.hpp"
#include "kodo_perpetual_encoder.hpp"
#include <kodo/api/api.hpp>

//------------------------------------------------------------------
// FACTORY API
//------------------------------------------------------------------

void kodo_delete_encoder_factory(kodo_factory_t factory)
{
    using interface = kodo::api::final_interface::config_interface;
    auto api = dynamic_cast<interface*>(factory);
    assert(api);
    api->reset();
}

void kodo_delete_decoder_factory(kodo_factory_t factory)
{
    using interface = kodo::api::final_interface::config_interface;
    auto api = dynamic_cast<interface*>(factory);
    assert(api);
    api->reset();
}

uint32_t kodo_factory_max_symbols(kodo_factory_t factory)
{
    using interface = kodo::api::storage_interface::config_interface;
    auto api = dynamic_cast<interface*>(factory);
    assert(api);
    return api->max_symbols();
}

uint32_t kodo_factory_max_symbol_size(kodo_factory_t factory)
{
    using interface = kodo::api::storage_interface::config_interface;
    auto api = dynamic_cast<interface*>(factory);
    assert(api);
    return api->max_symbol_size();
}

uint32_t kodo_factory_max_block_size(kodo_factory_t factory)
{
    using interface = kodo::api::storage_interface::config_interface;
    auto api = dynamic_cast<interface*>(factory);
    assert(api);
    return api->max_block_size();
}

uint32_t kodo_factory_max_payload_size(kodo_factory_t factory)
{
    using interface = kodo::api::payload_size_interface::config_interface;
    auto api = dynamic_cast<interface*>(factory);
    assert(api);
    return api->max_payload_size();
}

void kodo_factory_set_symbols(kodo_factory_t factory, uint32_t symbols)
{
    using interface = kodo::api::storage_interface::config_interface;
    auto api = dynamic_cast<interface*>(factory);
    assert(api);
    api->set_symbols(symbols);
}

void kodo_factory_set_symbol_size(kodo_factory_t factory, uint32_t symbol_size)
{
    using interface = kodo::api::storage_interface::config_interface;
    auto api = dynamic_cast<interface*>(factory);
    assert(api);
    api->set_symbol_size(symbol_size);
}

kodo_coder_t kodo_factory_new_encoder(kodo_factory_t factory)
{
    assert(factory);
    void* encoder = factory->build();
    return (kodo_coder_t) encoder;
}

kodo_coder_t kodo_factory_new_decoder(kodo_factory_t factory)
{
    assert(factory);
    void* decoder = factory->build();
    return (kodo_coder_t) decoder;
}

void kodo_delete_encoder(kodo_coder_t encoder)
{
    assert(encoder);
    kodo_encoder* the_encoder = (kodo_encoder*) encoder;
    delete the_encoder;
}

void kodo_delete_decoder(kodo_coder_t decoder)
{
    assert(decoder);
    kodo_decoder* the_decoder = (kodo_decoder*) decoder;
    delete the_decoder;
}

//------------------------------------------------------------------
// PAYLOAD API
//------------------------------------------------------------------

uint32_t kodo_payload_size(kodo_coder_t coder)
{
    assert(coder);
    return coder->payload_size();
}

void kodo_read_payload(kodo_coder_t decoder, uint8_t* payload)
{
    assert(decoder);
    kodo_decoder* the_decoder = (kodo_decoder*) decoder;
    the_decoder->read_payload(payload);
}

uint32_t kodo_write_payload(kodo_coder_t coder, uint8_t* payload)
{
    assert(coder);
    return coder->write_payload(payload);
}

uint8_t kodo_has_write_payload(kodo_coder_t coder)
{
    assert(coder);
    return (uint8_t)coder->has_write_payload();
}

//------------------------------------------------------------------
// SYMBOL STORAGE API
//------------------------------------------------------------------

uint32_t kodo_block_size(kodo_coder_t coder)
{
    assert(coder);
    return coder->block_size();
}

void kodo_set_symbols(kodo_coder_t coder, uint8_t* data, uint32_t size)
{
    assert(coder);
    coder->set_symbols(data, size);
}

void kodo_set_symbol(kodo_coder_t coder, uint32_t index,
                     uint8_t* data, uint32_t size)
{
    assert(coder);
    coder->set_symbol(index, data, size);
}

void kodo_copy_from_symbols(kodo_coder_t decoder, uint8_t* data, uint32_t size)
{
    assert(decoder);
    kodo_decoder* the_decoder = (kodo_decoder*) decoder;
    the_decoder->copy_from_symbols(data, size);
}

void kodo_copy_from_symbol(kodo_coder_t decoder, uint32_t index,
                           uint8_t* data, uint32_t size)
{
    assert(decoder);
    kodo_decoder* the_decoder = (kodo_decoder*) decoder;
    the_decoder->copy_from_symbol(index, data, size);
}

uint32_t kodo_symbol_size(kodo_coder_t coder)
{
    assert(coder);
    return coder->symbol_size();
}

uint32_t kodo_symbols(kodo_coder_t coder)
{
    assert(coder);
    return coder->symbols();
}

//------------------------------------------------------------------
// CODEC API
//------------------------------------------------------------------

uint8_t kodo_is_complete(kodo_coder_t decoder)
{
    assert(decoder);
    kodo_decoder* the_decoder = (kodo_decoder*) decoder;
    return (uint8_t)the_decoder->is_complete();
}

uint8_t kodo_is_partial_complete(kodo_coder_t decoder)
{
    assert(decoder);
    kodo_decoder* the_decoder = (kodo_decoder*) decoder;
    return (uint8_t)the_decoder->is_partial_complete();
}

uint8_t kodo_has_feedback_size(kodo_coder_t coder)
{
    assert(coder);
    return (uint8_t)coder->has_feedback_size();
}

uint8_t kodo_feedback_size(kodo_coder_t coder)
{
    assert(coder);
    return (uint8_t)coder->feedback_size();
}

void kodo_read_feedback(kodo_coder_t encoder, uint8_t* feedback)
{
    assert(encoder);
    kodo_encoder* the_encoder = (kodo_encoder*) encoder;
    the_encoder->read_feedback(feedback);
}

uint32_t kodo_write_feedback(kodo_coder_t decoder, uint8_t* feedback)
{
    assert(decoder);
    kodo_decoder* the_decoder = (kodo_decoder*) decoder;
    return the_decoder->write_feedback(feedback);
}

uint32_t kodo_rank(kodo_coder_t coder)
{
    assert(coder);
    return coder->rank();
}

uint8_t kodo_is_symbol_pivot(kodo_coder_t coder, uint32_t index)
{
    assert(coder);
    return coder->is_symbol_pivot(index);
}

uint8_t kodo_is_symbol_uncoded(kodo_coder_t decoder, uint32_t index)
{
    assert(decoder);
    kodo_decoder* the_decoder = (kodo_decoder*) decoder;
    return the_decoder->is_symbol_uncoded(index);
}

uint32_t kodo_symbols_uncoded(kodo_coder_t decoder)
{
    assert(decoder);
    kodo_decoder* the_decoder = (kodo_decoder*) decoder;
    return the_decoder->symbols_uncoded();
}

uint32_t kodo_symbols_seen(kodo_coder_t decoder)
{
    assert(decoder);
    kodo_decoder* the_decoder = (kodo_decoder*) decoder;
    return the_decoder->symbols_seen();
}

void kodo_read_symbol(kodo_coder_t decoder, uint8_t* symbol_data,
                      uint8_t* coefficients)
{
    assert(decoder);
    kodo_decoder* the_decoder = (kodo_decoder*) decoder;
    return the_decoder->read_symbol(symbol_data, coefficients);
}

void kodo_read_uncoded_symbol(kodo_coder_t decoder, uint8_t* symbol_data,
                              uint32_t index)
{
    assert(decoder);
    kodo_decoder* the_decoder = (kodo_decoder*) decoder;
    return the_decoder->read_uncoded_symbol(symbol_data, index);
}

uint32_t kodo_write_symbol(kodo_coder_t encoder, uint8_t* symbol_data,
                           uint8_t* coefficients)
{
    assert(encoder);
    kodo_encoder* the_encoder = (kodo_encoder*) encoder;
    return the_encoder->write_symbol(symbol_data, coefficients);
}

uint32_t kodo_write_uncoded_symbol(kodo_coder_t encoder, uint8_t* symbol_data,
                                   uint32_t index)
{
    assert(encoder);
    kodo_encoder* the_encoder = (kodo_encoder*) encoder;
    return the_encoder->write_uncoded_symbol(symbol_data, index);
}

//------------------------------------------------------------------
// SPARSE ENCODER API
//------------------------------------------------------------------

double kodo_density(kodo_coder_t encoder)
{
    assert(encoder);
    kodo_sparse_encoder* the_encoder = (kodo_sparse_encoder*) encoder;
    return the_encoder->density();
}

void kodo_set_density(kodo_coder_t encoder, double density)
{
    assert(encoder);
    kodo_sparse_encoder* the_encoder = (kodo_sparse_encoder*) encoder;
    the_encoder->set_density(density);
}

//------------------------------------------------------------------
// PERPETUAL ENCODER API
//------------------------------------------------------------------

uint8_t kodo_pseudo_systematic(kodo_coder_t encoder)
{
    assert(encoder);
    kodo_perpetual_encoder* the_encoder = (kodo_perpetual_encoder*) encoder;
    return the_encoder->pseudo_systematic();
}

void kodo_set_pseudo_systematic(kodo_coder_t encoder, uint8_t pseudo_systematic)
{
    assert(encoder);
    kodo_perpetual_encoder* the_encoder = (kodo_perpetual_encoder*) encoder;
    assert(the_encoder);
    the_encoder->set_pseudo_systematic(pseudo_systematic);
}

uint8_t kodo_pre_charging(kodo_coder_t encoder)
{
    assert(encoder);
    kodo_perpetual_encoder* the_encoder = (kodo_perpetual_encoder*) encoder;
    return the_encoder->pre_charging();
}

void kodo_set_pre_charging(kodo_coder_t encoder, uint8_t pre_charging)
{
    assert(encoder);
    kodo_perpetual_encoder* the_encoder = (kodo_perpetual_encoder*) encoder;
    the_encoder->set_pre_charging(pre_charging);
}

uint32_t kodo_width(kodo_coder_t encoder)
{
    assert(encoder);
    kodo_perpetual_encoder* the_encoder = (kodo_perpetual_encoder*) encoder;
    return the_encoder->width();
}

void kodo_set_width(kodo_coder_t encoder, uint32_t width)
{
    assert(encoder);
    kodo_perpetual_encoder* the_encoder = (kodo_perpetual_encoder*) encoder;
    the_encoder->set_width(width);
}

double kodo_width_ratio(kodo_coder_t encoder)
{
    assert(encoder);
    kodo_perpetual_encoder* the_encoder = (kodo_perpetual_encoder*) encoder;
    return the_encoder->width_ratio();
}

void kodo_set_width_ratio(kodo_coder_t encoder, double width_ratio)
{
    assert(encoder);
    kodo_perpetual_encoder* the_encoder = (kodo_perpetual_encoder*) encoder;
    the_encoder->set_width_ratio(width_ratio);
}

//------------------------------------------------------------------
// GENERIC API
//------------------------------------------------------------------

uint8_t kodo_has_partial_decoding_tracker(kodo_coder_t decoder)
{
    assert(decoder);
    kodo_decoder* the_decoder = (kodo_decoder*) decoder;
    return (uint8_t)the_decoder->has_partial_decoding_tracker();
}

uint8_t kodo_has_set_systematic_off(kodo_coder_t encoder)
{
    assert(encoder);
    kodo_encoder* the_encoder = (kodo_encoder*) encoder;
    return (uint8_t)the_encoder->has_set_systematic_off();
}

uint8_t kodo_is_systematic_on(kodo_coder_t encoder)
{
    assert(encoder);
    kodo_encoder* the_encoder = (kodo_encoder*) encoder;
    return (uint8_t)the_encoder->is_systematic_on();
}

void kodo_set_systematic_on(kodo_coder_t encoder)
{
    assert(encoder);
    kodo_encoder* the_encoder = (kodo_encoder*) encoder;
    the_encoder->set_systematic_on();
}

void kodo_set_systematic_off(kodo_coder_t encoder)
{
    assert(encoder);
    kodo_encoder* the_encoder = (kodo_encoder*) encoder;
    the_encoder->set_systematic_off();
}

//------------------------------------------------------------------
// TRACE API
//------------------------------------------------------------------

uint8_t kodo_has_set_trace_callback(kodo_coder_t coder)
{
    assert(coder);
    return (uint8_t)coder->has_set_trace_callback();
}

uint8_t kodo_has_set_trace_stdout(kodo_coder_t coder)
{
    assert(coder);
    return (uint8_t)coder->has_set_trace_stdout();
}

uint8_t kodo_has_set_trace_off(kodo_coder_t coder)
{
    assert(coder);
    return (uint8_t)coder->has_set_trace_off();
}

void kodo_set_trace_callback(kodo_coder_t coder, kodo_trace_callback_t callback, void* context)
{
    assert(coder);
    coder->set_trace_callback(callback, context);
}

void kodo_set_trace_stdout(kodo_coder_t coder)
{
    assert(coder);
    coder->set_trace_stdout();
}

void kodo_set_trace_off(kodo_coder_t coder)
{
    assert(coder);
    coder->set_trace_off();
}
