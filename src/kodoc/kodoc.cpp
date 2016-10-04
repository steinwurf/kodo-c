// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include "kodoc.h"

#include <cstring>
#include <cstdint>
#include <cassert>
#include <string>

#include <storage/storage.hpp>
#include <kodo_core/api/api.hpp>

#if !defined(KODOC_DISABLE_RLNC)
    #include <kodo_rlnc/api/pre_charging.hpp>
    #include <kodo_rlnc/api/pseudo_systematic.hpp>
    #include <kodo_rlnc/api/set_pre_charging.hpp>
    #include <kodo_rlnc/api/set_pseudo_systematic.hpp>
    #include <kodo_rlnc/api/set_width.hpp>
    #include <kodo_rlnc/api/set_width_ratio.hpp>
    #include <kodo_rlnc/api/width.hpp>
    #include <kodo_rlnc/api/width_ratio.hpp>
#endif // !defined(KODOC_DISABLE_RLNC)

#if !defined(KODOC_DISABLE_FULCRUM)
    #include <kodo_fulcrum/api/expansion.hpp>
    #include <kodo_fulcrum/api/max_expansion.hpp>
    #include <kodo_fulcrum/api/set_expansion.hpp>
    #include <kodo_fulcrum/api/inner_symbols.hpp>
    #include <kodo_fulcrum/api/max_inner_symbols.hpp>
    #include <kodo_fulcrum/api/nested_symbols.hpp>
    #include <kodo_fulcrum/api/nested_symbol_size.hpp>
#endif // !defined(KODOC_DISABLE_FULCRUM)

struct kodoc_factory { };
struct kodoc_coder { };

using namespace kodo_core::api;

//------------------------------------------------------------------
// CONFIGURATION API
//------------------------------------------------------------------

uint8_t kodoc_has_codec(int32_t codec)
{
    bool available = false;

#if !defined(KODOC_DISABLE_RLNC)

#if !defined(KODOC_DISABLE_FULL_VECTOR)
    if (codec == kodoc_full_vector)
    {
        available = true;
    }
#endif
#if !defined(KODOC_DISABLE_ON_THE_FLY)
    if (codec == kodoc_on_the_fly)
    {
        available = true;
    }
#endif
#if !defined(KODOC_DISABLE_SLIDING_WINDOW)
    if (codec == kodoc_sliding_window)
    {
        available = true;
    }
#endif
#if !defined(KODOC_DISABLE_SPARSE_FULL_VECTOR)
    if (codec == kodoc_sparse_full_vector)
    {
        available = true;
    }
#endif
#if !defined(KODOC_DISABLE_SEED)
    if (codec == kodoc_seed)
    {
        available = true;
    }
#endif
#if !defined(KODOC_DISABLE_SPARSE_SEED)
    if (codec == kodoc_sparse_seed)
    {
        available = true;
    }
#endif
#if !defined(KODOC_DISABLE_PERPETUAL)
    if (codec == kodoc_perpetual)
    {
        available = true;
    }
#endif

#endif // !defined(KODOC_DISABLE_RLNC)

#if !defined(KODOC_DISABLE_FULCRUM)
    if (codec == kodoc_fulcrum)
    {
        available = true;
    }
#endif

#if !defined(KODOC_DISABLE_REED_SOLOMON)
    if (codec == kodoc_reed_solomon)
    {
        available = true;
    }
#endif

    return available;
}


//------------------------------------------------------------------
// FACTORY API
//------------------------------------------------------------------

void kodoc_delete_factory(kodoc_factory_t factory)
{
    auto api = (final_interface*) factory;
    assert(api);
    api->reset();
}

uint32_t kodoc_factory_max_symbols(kodoc_factory_t factory)
{
    auto api = (final_interface*) factory;
    assert(api);
    return max_symbols(api);
}

uint32_t kodoc_factory_max_symbol_size(kodoc_factory_t factory)
{
    auto api = (final_interface*) factory;
    assert(api);
    return max_symbol_size(api);
}

uint32_t kodoc_factory_max_block_size(kodoc_factory_t factory)
{
    auto api = (final_interface*) factory;
    assert(api);
    return max_block_size(api);
}

uint32_t kodoc_factory_max_payload_size(kodoc_factory_t factory)
{
    auto api = (final_interface*) factory;
    assert(api);
    return max_payload_size(api);
}

void kodoc_factory_set_symbols(kodoc_factory_t factory, uint32_t symbols)
{
    auto api = (final_interface*) factory;
    assert(api);
    set_symbols(api, symbols);
}

void kodoc_factory_set_symbol_size(
    kodoc_factory_t factory, uint32_t symbol_size)
{
    auto api = (final_interface*) factory;
    assert(api);
    set_symbol_size(api, symbol_size);
}

kodoc_coder_t kodoc_factory_build_coder(kodoc_factory_t factory)
{
    auto api = (final_interface*) factory;
    assert(api);
    return (kodoc_coder_t) build(api)->keep_alive();
}

void kodoc_delete_coder(kodoc_coder_t coder)
{
    auto api = (final_interface*) coder;
    assert(api);
    api->reset();
}

//------------------------------------------------------------------
// PAYLOAD API
//------------------------------------------------------------------

uint32_t kodoc_payload_size(kodoc_coder_t coder)
{
    auto api = (final_interface*) coder;
    assert(api);
    return payload_size(api);
}

void kodoc_read_payload(kodoc_coder_t decoder, uint8_t* payload)
{
    auto api = (final_interface*) decoder;
    assert(api);
    read_payload(api, payload);
}

uint32_t kodoc_write_payload(kodoc_coder_t coder, uint8_t* payload)
{
    auto api = (final_interface*) coder;
    assert(api);
    return write_payload(api, payload);
}

uint8_t kodoc_has_write_payload(kodoc_coder_t coder)
{
    auto api = (final_interface*) coder;
    assert(api);
    return has_interface<write_payload_interface>(api);
}

//------------------------------------------------------------------
// SYMBOL STORAGE API
//------------------------------------------------------------------

uint32_t kodoc_block_size(kodoc_coder_t coder)
{
    auto api = (final_interface*) coder;
    assert(api);
    return block_size(api);
}

void kodoc_set_const_symbols(kodoc_coder_t coder, uint8_t* data, uint32_t size)
{
    auto api = (final_interface*) coder;
    assert(api);
    set_const_symbols(api, storage::storage(data, size));
}

void kodoc_set_const_symbol(
    kodoc_coder_t coder, uint32_t index, uint8_t* data, uint32_t size)
{
    auto api = (final_interface*) coder;
    assert(api);
    set_const_symbol(api, index, storage::storage(data, size));
}

void kodoc_set_mutable_symbols(
    kodoc_coder_t coder, uint8_t* data, uint32_t size)
{
    auto api = (final_interface*) coder;
    assert(api);
    set_mutable_symbols(api, storage::storage(data, size));
}

void kodoc_set_mutable_symbol(
    kodoc_coder_t coder, uint32_t index, uint8_t* data, uint32_t size)
{
    auto api = (final_interface*) coder;
    assert(api);
    set_mutable_symbol(api, index, storage::storage(data, size));
}

uint32_t kodoc_symbol_size(kodoc_coder_t coder)
{
    auto api = (final_interface*) coder;
    assert(api);
    return symbol_size(api);
}

uint32_t kodoc_symbols(kodoc_coder_t coder)
{
    auto api = (final_interface*) coder;
    assert(api);
    return symbols(api);
}

uint32_t kodoc_coefficient_vector_size(kodoc_coder_t coder)
{
    auto api = (final_interface*) coder;
    assert(api);
    return coefficient_vector_size(api);
}

//------------------------------------------------------------------
// CODEC API
//------------------------------------------------------------------

uint8_t kodoc_is_complete(kodoc_coder_t decoder)
{
    auto api = (final_interface*) decoder;
    assert(api);
    return is_complete(api);
}

uint8_t kodoc_is_partially_complete(kodoc_coder_t decoder)
{
    auto api = (final_interface*) decoder;
    assert(api);
    return is_partially_complete(api);
}

uint8_t kodoc_has_feedback_size(kodoc_coder_t coder)
{
    auto api = (final_interface*) coder;
    assert(api);
    return has_interface<feedback_size_interface>(api);
}

uint8_t kodoc_feedback_size(kodoc_coder_t coder)
{
    auto api = (final_interface*) coder;
    assert(api);
    return feedback_size(api);
}

void kodoc_read_feedback(kodoc_coder_t encoder, uint8_t* feedback)
{
    auto api = (final_interface*) encoder;
    assert(api);
    read_feedback(api, feedback);
}

uint32_t kodoc_write_feedback(kodoc_coder_t decoder, uint8_t* feedback)
{
    auto api = (final_interface*) decoder;
    assert(api);
    return write_feedback(api, feedback);
}

uint32_t kodoc_rank(kodoc_coder_t coder)
{
    auto api = (final_interface*) coder;
    assert(api);
    return rank(api);
}

uint8_t kodoc_is_symbol_pivot(kodoc_coder_t decoder, uint32_t index)
{
    auto api = (final_interface*) decoder;
    assert(api);
    return is_symbol_pivot(api, index);
}

uint8_t kodoc_is_symbol_missing(kodoc_coder_t decoder, uint32_t index)
{
    auto api = (final_interface*) decoder;
    assert(api);
    return is_symbol_missing(api, index);
}

uint8_t kodoc_is_symbol_partially_decoded(kodoc_coder_t decoder, uint32_t index)
{
    auto api = (final_interface*) decoder;
    assert(api);
    return is_symbol_partially_decoded(api, index);
}

uint8_t kodoc_is_symbol_uncoded(kodoc_coder_t decoder, uint32_t index)
{
    auto api = (final_interface*) decoder;
    assert(api);
    return is_symbol_uncoded(api, index);
}

uint32_t kodoc_symbols_missing(kodoc_coder_t decoder)
{
    auto api = (final_interface*) decoder;
    assert(api);
    return symbols_missing(api);
}

uint32_t kodoc_symbols_partially_decoded(kodoc_coder_t decoder)
{
    auto api = (final_interface*) decoder;
    assert(api);
    return symbols_partially_decoded(api);
}

uint32_t kodoc_symbols_uncoded(kodoc_coder_t decoder)
{
    auto api = (final_interface*) decoder;
    assert(api);
    return symbols_uncoded(api);
}

void kodoc_set_status_updater_on(kodoc_coder_t decoder)
{
    auto api = (final_interface*) decoder;
    assert(api);
    set_status_updater_on(api);
}

void kodoc_set_status_updater_off(kodoc_coder_t decoder)
{
    auto api = (final_interface*) decoder;
    assert(api);
    set_status_updater_off(api);
}

void kodoc_update_symbol_status(kodoc_coder_t decoder)
{
    auto api = (final_interface*) decoder;
    assert(api);
    update_symbol_status(api);
}

uint8_t kodoc_is_status_updater_enabled(kodoc_coder_t decoder)
{
    auto api = (final_interface*) decoder;
    assert(api);
    return is_status_updater_enabled(api);
}

void kodoc_read_symbol(kodoc_coder_t decoder, uint8_t* symbol_data,
                       uint8_t* coefficients)
{
    auto api = (final_interface*) decoder;
    assert(api);
    return read_symbol(api, symbol_data, coefficients);
}

void kodoc_read_uncoded_symbol(
    kodoc_coder_t decoder, uint8_t* symbol_data, uint32_t index)
{
    auto api = (final_interface*) decoder;
    assert(api);
    return read_uncoded_symbol(api, symbol_data, index);
}

uint32_t kodoc_write_symbol(
    kodoc_coder_t encoder, uint8_t* symbol_data, uint8_t* coefficients)
{
    auto api = (final_interface*) encoder;
    assert(api);
    return write_symbol(api, symbol_data, coefficients);
}

uint32_t kodoc_write_uncoded_symbol(
    kodoc_coder_t encoder, uint8_t* symbol_data, uint32_t index)
{
    auto api = (final_interface*) encoder;
    assert(api);
    return write_uncoded_symbol(api, symbol_data, index);
}

//------------------------------------------------------------------
// GENERIC API
//------------------------------------------------------------------

uint8_t kodoc_has_symbol_decoding_status_updater_interface(
    kodoc_coder_t decoder)
{
    auto api = (final_interface*) decoder;
    assert(api);
    return has_interface<symbol_decoding_status_updater_interface>(api);
}

uint8_t kodoc_has_partial_decoding_interface(kodoc_coder_t decoder)
{
    auto api = (final_interface*) decoder;
    assert(api);
    return has_interface<partial_decoding_interface>(api);
}

uint8_t kodoc_has_systematic_interface(kodoc_coder_t encoder)
{
    auto api = (final_interface*) encoder;
    assert(api);
    return has_interface<systematic_interface>(api);
}

uint8_t kodoc_is_systematic_on(kodoc_coder_t encoder)
{
    auto api = (final_interface*) encoder;
    assert(api);
    return is_systematic_on(api);
}

void kodoc_set_systematic_on(kodoc_coder_t encoder)
{
    auto api = (final_interface*) encoder;
    assert(api);
    set_systematic_on(api);
}

void kodoc_set_systematic_off(kodoc_coder_t encoder)
{
    auto api = (final_interface*) encoder;
    assert(api);
    set_systematic_off(api);
}

//------------------------------------------------------------------
// TRACE API
//------------------------------------------------------------------

uint8_t kodoc_has_trace_interface(kodoc_coder_t coder)
{
    auto api = (final_interface*) coder;
    assert(api);
    return has_interface<trace_interface>(api);
}

void kodoc_set_trace_callback(
    kodoc_coder_t coder, kodoc_trace_callback_t c_callback, void* context)
{
    assert(c_callback);
    auto api = (final_interface*) coder;
    assert(api);
    auto callback = [c_callback, context](const std::string& zone,
                                          const std::string& data)
    {
        c_callback(zone.c_str(), data.c_str(), context);
    };
    set_trace_callback(api, callback);
}

void kodoc_set_trace_stdout(kodoc_coder_t coder)
{
    auto api = (final_interface*) coder;
    assert(api);
    set_trace_stdout(api);
}

void kodoc_set_trace_off(kodoc_coder_t coder)
{
    auto api = (final_interface*) coder;
    assert(api);
    set_trace_off(api);
}

void kodoc_set_zone_prefix(kodoc_coder_t coder, const char* prefix)
{
    auto api = (final_interface*) coder;
    assert(api);
    set_zone_prefix(api, std::string(prefix));
}

//------------------------------------------------------------------
// SPARSE ENCODER API
//------------------------------------------------------------------

double kodoc_density(kodoc_coder_t encoder)
{
    auto api = (final_interface*) encoder;
    assert(api);
    return density(api);
}

void kodoc_set_density(kodoc_coder_t encoder, double density)
{
    auto api = (final_interface*) encoder;
    assert(api);
    set_density(api, density);
}

//------------------------------------------------------------------
// PERPETUAL ENCODER API
//------------------------------------------------------------------

uint8_t kodoc_pseudo_systematic(kodoc_coder_t encoder)
{
#if !defined(KODOC_DISABLE_RLNC)
    auto api = (final_interface*) encoder;
    assert(api);
    return kodo_rlnc::api::pseudo_systematic(api);
#else
    (void)encoder;
    return 0;
#endif
}

void kodoc_set_pseudo_systematic(
    kodoc_coder_t encoder, uint8_t pseudo_systematic)
{
#if !defined(KODOC_DISABLE_RLNC)
    auto api = (final_interface*) encoder;
    assert(api);
    kodo_rlnc::api::set_pseudo_systematic(api, pseudo_systematic != 0);
#else
    (void)encoder;
    (void)pseudo_systematic;
#endif
}

uint8_t kodoc_pre_charging(kodoc_coder_t encoder)
{
#if !defined(KODOC_DISABLE_RLNC)
    auto api = (final_interface*) encoder;
    assert(api);
    return kodo_rlnc::api::pre_charging(api);
#else
    (void)encoder;
    return 0;
#endif
}

void kodoc_set_pre_charging(kodoc_coder_t encoder, uint8_t pre_charging)
{
#if !defined(KODOC_DISABLE_RLNC)
    auto api = (final_interface*) encoder;
    assert(api);
    kodo_rlnc::api::set_pre_charging(api, pre_charging != 0);
#else
    (void)encoder;
    (void)pre_charging;
#endif
}

uint32_t kodoc_width(kodoc_coder_t encoder)
{
#if !defined(KODOC_DISABLE_RLNC)
    auto api = (final_interface*) encoder;
    assert(api);
    return kodo_rlnc::api::width(api);
#else
    (void)encoder;
    return 0;
#endif
}

void kodoc_set_width(kodoc_coder_t encoder, uint32_t width)
{
#if !defined(KODOC_DISABLE_RLNC)
    auto api = (final_interface*) encoder;
    assert(api);
    kodo_rlnc::api::set_width(api, width);
#else
    (void)encoder;
    (void)width;
#endif
}

double kodoc_width_ratio(kodoc_coder_t encoder)
{
#if !defined(KODOC_DISABLE_RLNC)
    auto api = (final_interface*) encoder;
    assert(api);
    return kodo_rlnc::api::width_ratio(api);
#else
    (void)encoder;
    return 0.0;
#endif
}

void kodoc_set_width_ratio(kodoc_coder_t encoder, double width_ratio)
{
#if !defined(KODOC_DISABLE_RLNC)
    auto api = (final_interface*) encoder;
    assert(api);
    kodo_rlnc::api::set_width_ratio(api, width_ratio);
#else
    (void)encoder;
    (void)width_ratio;
#endif
}

//------------------------------------------------------------------
// FULCRUM CODER API
//------------------------------------------------------------------

uint32_t kodoc_expansion(kodoc_coder_t coder)
{
#if !defined(KODOC_DISABLE_FULCRUM)
    auto api = (final_interface*) coder;
    assert(api);
    return kodo_fulcrum::api::expansion(api);
#else
    (void)coder;
    return 0;
#endif
}

uint32_t kodoc_inner_symbols(kodoc_coder_t coder)
{
#if !defined(KODOC_DISABLE_FULCRUM)
    auto api = (final_interface*) coder;
    assert(api);
    return kodo_fulcrum::api::inner_symbols(api);
#else
    (void)coder;
    return 0;
#endif
}

//------------------------------------------------------------------
// FULCRUM ENCODER API
//------------------------------------------------------------------
uint32_t kodoc_nested_symbols(kodoc_coder_t encoder)
{
#if !defined(KODOC_DISABLE_FULCRUM)
    auto api = (final_interface*) encoder;
    assert(api);
    return kodo_fulcrum::api::nested_symbols(api);
#else
    (void)encoder;
    return 0;
#endif
}

uint32_t kodoc_nested_symbol_size(kodoc_coder_t encoder)
{
#if !defined(KODOC_DISABLE_FULCRUM)
    auto api = (final_interface*) encoder;
    assert(api);
    return kodo_fulcrum::api::nested_symbol_size(api);
#else
    (void)encoder;
    return 0;
#endif
}

//------------------------------------------------------------------
// FULCRUM FACTORY API
//------------------------------------------------------------------

uint32_t kodoc_factory_max_expansion(kodoc_factory_t factory)
{
#if !defined(KODOC_DISABLE_FULCRUM)
    auto api = (final_interface*) factory;
    assert(api);
    return kodo_fulcrum::api::max_expansion(api);
#else
    (void)factory;
    return 0;
#endif
}

void kodoc_factory_set_expansion(kodoc_factory_t factory, uint32_t expansion)
{
#if !defined(KODOC_DISABLE_FULCRUM)
    auto api = (final_interface*) factory;
    assert(api);
    kodo_fulcrum::api::set_expansion(api, expansion);
#else
    (void)factory;
    (void)expansion;
#endif
}

uint32_t kodoc_factory_max_inner_symbols(kodoc_factory_t factory)
{
#if !defined(KODOC_DISABLE_FULCRUM)
    auto api = (final_interface*) factory;
    assert(api);
    return kodo_fulcrum::api::max_inner_symbols(api);
#else
    (void)factory;
    return 0;
#endif
}
