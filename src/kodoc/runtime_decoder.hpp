// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <kodo_core/api/api.hpp>
#include <kodo_core/runtime/empty_binding.hpp>
#include <kodo_core/runtime/extend_binding.hpp>
#include <kodo_core/runtime/final.hpp>
#include <kodo_core/runtime/select_field.hpp>
#include <kodo_core/runtime/use_shallow_decoder_storage.hpp>
#include <kodo_core/runtime/use_trace_enabled.hpp>

#include "factory_binding.hpp"

namespace kodoc
{
template<class Stack>
using decoder_binding =
    kodo_core::api::storage_binding<
    kodo_core::api::decoder_binding<
    kodo_core::api::rank_binding<
    kodo_core::api::read_payload_binding<
    kodo_core::api::payload_size_binding<
    kodo_core::api::coefficient_vector_size_binding<
    kodo_core::api::final_binding<Stack>>>>>>>;

template
<
    template <class,class> class Stack,
    template<class> class AdditionalCoderBindings =
    kodo_core::runtime::empty_binding,
    template<class> class AdditionalCoderFactoryBindings =
    kodo_core::runtime::empty_binding
>
struct runtime_decoder :
    kodo_core::runtime::select_field<
    kodo_core::runtime::use_shallow_decoder_storage<
    kodo_core::runtime::use_trace_enabled<
    kodo_core::runtime::final<Stack,
    kodo_core::runtime::extend_binding<AdditionalCoderFactoryBindings,
    factory_binding>::template type,
    kodo_core::runtime::extend_binding<AdditionalCoderBindings,
    decoder_binding>::template type>>>>
{ };
}
