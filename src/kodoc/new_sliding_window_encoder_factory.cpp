// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include "kodoc.h"

#include <cstdint>

#include <kodo/api/api.hpp>
#include <kodo/runtime/default_encoder_binding.hpp>
#include <kodo/runtime/default_encoder_factory_binding.hpp>
#include <kodo/runtime/extend_binding.hpp>

#include <kodo/rlnc/sliding_window_encoder.hpp>

#include "create_factory.hpp"

namespace kodoc
{
    template<class Stack>
    using extra_bindings =
        kodo::api::read_feedback_binding<
        kodo::api::feedback_size_binding<
        kodo::api::systematic_binding<
        kodo::api::trace_binding<
        kodo::api::const_storage_binding<Stack>>>>>;

    using sliding_window_encoder_binding = kodo::runtime::extend_binding<
        extra_bindings, kodo::runtime::default_encoder_binding>;

    kodo_factory_t new_sliding_window_encoder_factory(int32_t finite_field,
        uint32_t max_symbols, uint32_t max_symbol_size)
    {
        using namespace kodo;

        return create_factory<
            rlnc::sliding_window_encoder,
            meta::typelist<storage_type<tag::const_shallow_storage>>,
            sliding_window_encoder_binding::template type,
            runtime::default_encoder_factory_binding>(
                finite_field, max_symbols, max_symbol_size);
    }
}
