// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include "kodoc.h"

#include <cstring>
#include <cstdint>
#include <cassert>

#include <kodo/api/api.hpp>
#include <kodo/rlnc/api/perpetual_encoder_interface.hpp>
#include <kodo/rlnc/api/fulcrum_interface.hpp>

struct kodo_factory : kodo::api::final_interface::config_interface
{
    virtual ~kodo_factory() { }
};

struct kodo_coder : kodo::api::final_interface
{
    virtual ~kodo_coder() { }
};

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
    using interface = kodo::api::build_interface::config_interface;
    auto api = dynamic_cast<interface*>(factory);
    assert(api);
    auto coder = api->build_interface();
    return (kodo_coder_t) coder->keep_alive();
}

kodo_coder_t kodo_factory_new_decoder(kodo_factory_t factory)
{
    using interface = kodo::api::build_interface::config_interface;
    auto api = dynamic_cast<interface*>(factory);
    assert(api);
    auto coder = api->build_interface();
    return (kodo_coder_t) coder->keep_alive();
}

void kodo_delete_encoder(kodo_coder_t encoder)
{
    using interface = kodo::api::final_interface;
    auto api = dynamic_cast<interface*>(encoder);
    assert(api);
    api->reset();
}

void kodo_delete_decoder(kodo_coder_t decoder)
{
    using interface = kodo::api::final_interface;
    auto api = dynamic_cast<interface*>(decoder);
    assert(api);
    api->reset();
}

//------------------------------------------------------------------
// PAYLOAD API
//------------------------------------------------------------------

uint32_t kodo_payload_size(kodo_coder_t coder)
{
    using interface = kodo::api::payload_size_interface;
    auto api = dynamic_cast<interface*>(coder);
    assert(api);
    return api->payload_size();
}

void kodo_read_payload(kodo_coder_t decoder, uint8_t* payload)
{
    using interface = kodo::api::read_payload_interface;
    auto api = dynamic_cast<interface*>(decoder);
    assert(api);
    api->read_payload(payload);
}

uint32_t kodo_write_payload(kodo_coder_t coder, uint8_t* payload)
{
    using interface = kodo::api::write_payload_interface;
    auto api = dynamic_cast<interface*>(coder);
    assert(api);
    return api->write_payload(payload);
}

uint8_t kodo_has_write_payload(kodo_coder_t coder)
{
    using interface = kodo::api::final_interface;
    auto api = dynamic_cast<interface*>(coder);
    return kodo::api::has_interface<kodo::api::write_payload_interface>(api);
}

//------------------------------------------------------------------
// SYMBOL STORAGE API
//------------------------------------------------------------------

uint32_t kodo_block_size(kodo_coder_t coder)
{
    using interface = kodo::api::storage_interface;
    auto api = dynamic_cast<interface*>(coder);
    assert(api);
    return api->block_size();
}

void kodo_set_symbols(kodo_coder_t coder, uint8_t* data, uint32_t size)
{
    using interface = kodo::api::const_storage_interface;
    auto api = dynamic_cast<interface*>(coder);
    assert(api);
    api->set_const_symbols(sak::storage(data, size));
}

void kodo_set_symbol(kodo_coder_t coder, uint32_t index,
                     uint8_t* data, uint32_t size)
{
    using interface = kodo::api::const_storage_interface;
    auto api = dynamic_cast<interface*>(coder);
    assert(api);
    api->set_const_symbol(index, sak::storage(data, size));
}

void kodo_copy_from_symbols(kodo_coder_t decoder, uint8_t* data, uint32_t size)
{
    using interface = kodo::api::storage_interface;
    auto api = dynamic_cast<interface*>(decoder);
    assert(api);
    api->copy_from_symbols(sak::storage(data, size));
}

void kodo_copy_from_symbol(kodo_coder_t decoder, uint32_t index,
                           uint8_t* data, uint32_t size)
{
    using interface = kodo::api::storage_interface;
    auto api = dynamic_cast<interface*>(decoder);
    assert(api);
    api->copy_from_symbol(index, sak::storage(data, size));
}

uint32_t kodo_symbol_size(kodo_coder_t coder)
{
    using interface = kodo::api::storage_interface;
    auto api = dynamic_cast<interface*>(coder);
    assert(api);
    return api->symbol_size();
}

uint32_t kodo_symbols(kodo_coder_t coder)
{
    using interface = kodo::api::storage_interface;
    auto api = dynamic_cast<interface*>(coder);
    assert(api);
    return api->symbols();
}

//------------------------------------------------------------------
// CODEC API
//------------------------------------------------------------------

uint8_t kodo_is_complete(kodo_coder_t decoder)
{
    using interface = kodo::api::decoder_interface;
    auto api = dynamic_cast<interface*>(decoder);
    assert(api);
    return api->is_complete();
}

uint8_t kodo_is_partial_complete(kodo_coder_t decoder)
{
    using interface = kodo::api::partial_decoding_interface;
    auto api = dynamic_cast<interface*>(decoder);
    assert(api);
    return api->is_partial_complete();
}

uint8_t kodo_has_feedback_size(kodo_coder_t coder)
{
    using interface = kodo::api::final_interface;
    auto api = dynamic_cast<interface*>(coder);
    return kodo::api::has_interface<kodo::api::feedback_size_interface>(api);
}

uint8_t kodo_feedback_size(kodo_coder_t coder)
{
    using interface = kodo::api::feedback_size_interface;
    auto api = dynamic_cast<interface*>(coder);
    assert(api);
    return api->feedback_size();
}

void kodo_read_feedback(kodo_coder_t encoder, uint8_t* feedback)
{
    using interface = kodo::api::read_feedback_interface;
    auto api = dynamic_cast<interface*>(encoder);
    assert(api);
    api->read_feedback(feedback);
}

uint32_t kodo_write_feedback(kodo_coder_t decoder, uint8_t* feedback)
{
    using interface = kodo::api::write_feedback_interface;
    auto api = dynamic_cast<interface*>(decoder);
    assert(api);
    return api->write_feedback(feedback);
}

uint32_t kodo_rank(kodo_coder_t coder)
{
    using interface = kodo::api::decoder_interface;
    auto api = dynamic_cast<interface*>(coder);
    assert(api);
    return api->rank();
}

uint8_t kodo_is_symbol_pivot(kodo_coder_t coder, uint32_t index)
{
    using interface = kodo::api::decoder_interface;
    auto api = dynamic_cast<interface*>(coder);
    assert(api);
    return api->is_symbol_pivot(index);
}

uint8_t kodo_is_symbol_uncoded(kodo_coder_t decoder, uint32_t index)
{
    using interface = kodo::api::decoder_interface;
    auto api = dynamic_cast<interface*>(decoder);
    assert(api);
    return api->is_symbol_uncoded(index);
}

uint32_t kodo_symbols_uncoded(kodo_coder_t decoder)
{
    using interface = kodo::api::decoder_interface;
    auto api = dynamic_cast<interface*>(decoder);
    assert(api);
    return api->symbols_uncoded();
}

uint32_t kodo_symbols_seen(kodo_coder_t decoder)
{
    using interface = kodo::api::decoder_interface;
    auto api = dynamic_cast<interface*>(decoder);
    assert(api);
    return api->symbols_seen();
}

void kodo_read_symbol(kodo_coder_t decoder, uint8_t* symbol_data,
                      uint8_t* coefficients)
{
    using interface = kodo::api::decoder_interface;
    auto api = dynamic_cast<interface*>(decoder);
    assert(api);
    return api->read_symbol(symbol_data, coefficients);
}

void kodo_read_uncoded_symbol(kodo_coder_t decoder, uint8_t* symbol_data,
                              uint32_t index)
{
    using interface = kodo::api::decoder_interface;
    auto api = dynamic_cast<interface*>(decoder);
    assert(api);
    return api->read_uncoded_symbol(symbol_data, index);
}

uint32_t kodo_write_symbol(kodo_coder_t encoder, uint8_t* symbol_data,
                           uint8_t* coefficients)
{
    using interface = kodo::api::encoder_interface;
    auto api = dynamic_cast<interface*>(encoder);
    assert(api);
    return api->write_symbol(symbol_data, coefficients);
}

uint32_t kodo_write_uncoded_symbol(kodo_coder_t encoder, uint8_t* symbol_data,
                                   uint32_t index)
{
    using interface = kodo::api::encoder_interface;
    auto api = dynamic_cast<interface*>(encoder);
    assert(api);
    return api->write_uncoded_symbol(symbol_data, index);
}

//------------------------------------------------------------------
// SPARSE ENCODER API
//------------------------------------------------------------------

double kodo_density(kodo_coder_t encoder)
{
    using interface = kodo::api::sparse_encoder_interface;
    auto api = dynamic_cast<interface*>(encoder);
    assert(api);
    return api->density();
}

void kodo_set_density(kodo_coder_t encoder, double density)
{
    using interface = kodo::api::sparse_encoder_interface;
    auto api = dynamic_cast<interface*>(encoder);
    assert(api);
    api->set_density(density);
}

//------------------------------------------------------------------
// PERPETUAL ENCODER API
//------------------------------------------------------------------

uint8_t kodo_pseudo_systematic(kodo_coder_t encoder)
{
    using interface = kodo::rlnc::api::perpetual_encoder_interface;
    auto api = dynamic_cast<interface*>(encoder);
    assert(api);
    return api->pseudo_systematic();
}

void kodo_set_pseudo_systematic(kodo_coder_t encoder, uint8_t pseudo_systematic)
{
    using interface = kodo::rlnc::api::perpetual_encoder_interface;
    auto api = dynamic_cast<interface*>(encoder);
    assert(api);
    api->set_pseudo_systematic(pseudo_systematic);
}

uint8_t kodo_pre_charging(kodo_coder_t encoder)
{
    using interface = kodo::rlnc::api::perpetual_encoder_interface;
    auto api = dynamic_cast<interface*>(encoder);
    assert(api);
    return api->pre_charging();
}

void kodo_set_pre_charging(kodo_coder_t encoder, uint8_t pre_charging)
{
    using interface = kodo::rlnc::api::perpetual_encoder_interface;
    auto api = dynamic_cast<interface*>(encoder);
    assert(api);
    api->set_pre_charging(pre_charging);
}

uint32_t kodo_width(kodo_coder_t encoder)
{
    using interface = kodo::rlnc::api::perpetual_encoder_interface;
    auto api = dynamic_cast<interface*>(encoder);
    assert(api);
    return api->width();
}

void kodo_set_width(kodo_coder_t encoder, uint32_t width)
{
    using interface = kodo::rlnc::api::perpetual_encoder_interface;
    auto api = dynamic_cast<interface*>(encoder);
    assert(api);
    api->set_width(width);
}

double kodo_width_ratio(kodo_coder_t encoder)
{
    using interface = kodo::rlnc::api::perpetual_encoder_interface;
    auto api = dynamic_cast<interface*>(encoder);
    assert(api);
    return api->width_ratio();
}

void kodo_set_width_ratio(kodo_coder_t encoder, double width_ratio)
{
    using interface = kodo::rlnc::api::perpetual_encoder_interface;
    auto api = dynamic_cast<interface*>(encoder);
    assert(api);
    api->set_width_ratio(width_ratio);
}

//------------------------------------------------------------------
// GENERIC API
//------------------------------------------------------------------

uint8_t kodo_has_partial_decoding_tracker(kodo_coder_t decoder)
{
    using interface = kodo::api::final_interface;
    auto api = dynamic_cast<interface*>(decoder);
    return kodo::api::has_interface<kodo::api::partial_decoding_interface>(api);
}

uint8_t kodo_has_set_systematic_off(kodo_coder_t encoder)
{
    using interface = kodo::api::final_interface;
    auto api = dynamic_cast<interface*>(encoder);
    return kodo::api::has_interface<kodo::api::systematic_interface>(api);
}

uint8_t kodo_is_systematic_on(kodo_coder_t encoder)
{
    using interface = kodo::api::systematic_interface;
    auto api = dynamic_cast<interface*>(encoder);
    assert(api);
    return api->is_systematic_on();
}

void kodo_set_systematic_on(kodo_coder_t encoder)
{
    using interface = kodo::api::systematic_interface;
    auto api = dynamic_cast<interface*>(encoder);
    assert(api);
    api->set_systematic_on();
}

void kodo_set_systematic_off(kodo_coder_t encoder)
{
    using interface = kodo::api::systematic_interface;
    auto api = dynamic_cast<interface*>(encoder);
    assert(api);
    api->set_systematic_off();
}

//------------------------------------------------------------------
// TRACE API
//------------------------------------------------------------------

uint8_t kodo_has_set_trace_callback(kodo_coder_t coder)
{
    using interface = kodo::api::final_interface;
    auto api = dynamic_cast<interface*>(coder);
    return kodo::api::has_interface<kodo::api::trace_interface>(api);
}

uint8_t kodo_has_set_trace_stdout(kodo_coder_t coder)
{
    using interface = kodo::api::final_interface;
    auto api = dynamic_cast<interface*>(coder);
    return kodo::api::has_interface<kodo::api::trace_interface>(api);
}

uint8_t kodo_has_set_trace_off(kodo_coder_t coder)
{
    using interface = kodo::api::final_interface;
    auto api = dynamic_cast<interface*>(coder);
    return kodo::api::has_interface<kodo::api::trace_interface>(api);
}

void kodo_set_trace_callback(kodo_coder_t coder,
    kodo_trace_callback_t c_callback, void* context)
{
    assert(c_callback);
    using interface = kodo::api::trace_interface;
    auto api = dynamic_cast<interface*>(coder);
    assert(api);
    auto callback = [c_callback, context](const std::string& zone,
                                          const std::string& data)
    {
        c_callback(zone.c_str(), data.c_str(), context);
    };
    api->set_trace_callback(callback);
}

void kodo_set_trace_stdout(kodo_coder_t coder)
{
    using interface = kodo::api::trace_interface;
    auto api = dynamic_cast<interface*>(coder);
    assert(api);
    api->set_trace_stdout();
}

void kodo_set_trace_off(kodo_coder_t coder)
{
    using interface = kodo::api::trace_interface;
    auto api = dynamic_cast<interface*>(coder);
    assert(api);
    api->set_trace_off();
}
