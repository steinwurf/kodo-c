// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include "kodoc.h"

#include <cstdint>
#include <cassert>

//------------------------------------------------------------------
// ENCODER FACTORY FOR SHALLOW STORAGE STACKS
//------------------------------------------------------------------

kodo_factory_t
kodo_new_shallow_encoder_factory(int32_t code_type, int32_t finite_field,
                                 uint32_t max_symbols, uint32_t max_symbol_size,
                                 int32_t trace_mode)
{
    kodo_factory_t factory = 0;

    // Unknown code type
    assert(factory);

    return factory;
}
