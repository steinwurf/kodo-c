// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "kodo_coder.hpp"

extern "C"
{
    /// Base class for decoder implementation
    class kodo_decoder : public kodo_coder
    {
    public:

        virtual ~kodo_decoder()
        {}

        virtual void read_payload(uint8_t* data) = 0;
        virtual bool is_complete() const = 0;

        virtual void copy_from_symbols(uint8_t* data, uint32_t size) const = 0;

        virtual void copy_from_symbol(uint32_t index, uint8_t* data,
            uint32_t size) const = 0;

        virtual bool has_partial_decoding_tracker() const = 0;
        virtual bool is_partial_complete() const = 0;

        virtual bool is_symbol_uncoded(uint32_t) const = 0;

        virtual uint32_t symbols_uncoded() const = 0;
        virtual uint32_t symbols_seen() const = 0;
        virtual uint32_t write_feedback(uint8_t* feedback) = 0;

        virtual void read_symbol(uint8_t* symbol_data,
            uint8_t* coefficients) = 0;

        virtual void read_uncoded_symbol(uint8_t* symbol_data,
            uint32_t index) = 0;
    };
}
