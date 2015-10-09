// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include "kodoc.h"

#include <cstdint>

#include <kodo/api/api.hpp>
#include <kodo/runtime/default_decoder_binding.hpp>
#include <kodo/runtime/default_decoder_factory_binding.hpp>
#include <kodo/runtime/extend_binding.hpp>

#include <kodo/rlnc/sliding_window_decoder.hpp>

#include "create_factory.hpp"

namespace kodoc
{
    template<class Stack>
    using extra_bindings =
        kodo::api::write_feedback_binding<
        kodo::api::feedback_size_binding<
        kodo::api::partial_decoding_binding<
        kodo::api::write_payload_binding<
        kodo::api::trace_binding<
        kodo::api::mutable_storage_binding<Stack>>>>>>;

    using sliding_window_decoder_binding = kodo::runtime::extend_binding<
        extra_bindings, kodo::runtime::default_decoder_binding>;

    kodo_factory_t new_sliding_window_decoder_factory(int32_t finite_field,
        uint32_t max_symbols, uint32_t max_symbol_size)
    {
        using namespace kodo;

        return create_factory<
            rlnc::sliding_window_decoder,
            meta::typelist<storage_type<tag::mutable_shallow_storage>>,
            sliding_window_decoder_binding::template type,
            runtime::default_decoder_factory_binding>(
                finite_field, max_symbols, max_symbol_size);
    }
}
