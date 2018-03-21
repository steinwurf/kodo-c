// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <kodo_core/api/api.hpp>

namespace kodoc
{
template<class Factory>
using factory_binding =
    kodo_core::api::storage_block_info_binding<
    kodo_core::api::max_payload_size_binding<
    kodo_core::api::final_binding<Factory>>>;
}
