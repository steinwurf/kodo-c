// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include "kodoc.h"

#include <cstdint>
#include <cassert>

namespace kodoc
{
// Forward declaration of factory functions
kodoc_factory_t new_fulcrum_decoder_factory(int32_t, uint32_t, uint32_t);
kodoc_factory_t new_full_vector_decoder_factory(int32_t, uint32_t, uint32_t);
kodoc_factory_t new_on_the_fly_decoder_factory(int32_t, uint32_t, uint32_t);
kodoc_factory_t new_perpetual_decoder_factory(int32_t, uint32_t, uint32_t);
kodoc_factory_t new_seed_decoder_factory(int32_t, uint32_t, uint32_t);
kodoc_factory_t new_sliding_window_decoder_factory(int32_t, uint32_t, uint32_t);
kodoc_factory_t new_sparse_seed_decoder_factory(int32_t, uint32_t, uint32_t);
kodoc_factory_t new_reed_solomon_decoder_factory(int32_t, uint32_t, uint32_t);
}

kodoc_factory_t kodoc_new_decoder_factory(
    int32_t codec, int32_t finite_field,
    uint32_t max_symbols, uint32_t max_symbol_size)
{
    using namespace kodoc;

    kodoc_factory_t factory = 0;

#if !defined(KODOC_DISABLE_RLNC)

#if !defined(KODOC_DISABLE_FULL_VECTOR)
    if (codec == kodoc_full_vector)
    {
        factory = new_full_vector_decoder_factory(
            finite_field, max_symbols, max_symbol_size);
    }
#endif
#if !defined(KODOC_DISABLE_ON_THE_FLY)
    if (codec == kodoc_on_the_fly)
    {
        factory = new_on_the_fly_decoder_factory(
            finite_field, max_symbols, max_symbol_size);
    }
#endif
#if !defined(KODOC_DISABLE_SLIDING_WINDOW)
    if (codec == kodoc_sliding_window)
    {
        factory = new_sliding_window_decoder_factory(
            finite_field, max_symbols, max_symbol_size);
    }
#endif
#if !defined(KODOC_DISABLE_SPARSE_FULL_VECTOR)
    if (codec == kodoc_sparse_full_vector)
    {
        // The sparse_full_vector codec uses the standard full_vector decoder
        factory = new_full_vector_decoder_factory(
            finite_field, max_symbols, max_symbol_size);
    }
#endif
#if !defined(KODOC_DISABLE_SEED)
    if (codec == kodoc_seed)
    {
        factory = new_seed_decoder_factory(
            finite_field, max_symbols, max_symbol_size);
    }
#endif
#if !defined(KODOC_DISABLE_SPARSE_SEED)
    if (codec == kodoc_sparse_seed)
    {
        factory = new_sparse_seed_decoder_factory(
            finite_field, max_symbols, max_symbol_size);
    }
#endif
#if !defined(KODOC_DISABLE_PERPETUAL)
    if (codec == kodoc_perpetual)
    {
        factory = new_perpetual_decoder_factory(
            finite_field, max_symbols, max_symbol_size);
    }
#endif

#endif // !defined(KODOC_DISABLE_RLNC)

#if !defined(KODOC_DISABLE_FULCRUM)
    if (codec == kodoc_fulcrum)
    {
        factory = new_fulcrum_decoder_factory(
            finite_field, max_symbols, max_symbol_size);
    }
#endif

#if !defined(KODOC_DISABLE_REED_SOLOMON)
    if (codec == kodoc_reed_solomon)
    {
        factory = new_reed_solomon_decoder_factory(
            finite_field, max_symbols, max_symbol_size);
    }
#endif

    assert(factory && "Requested codec is unknown or unavailable");
    return factory;
}
