// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

namespace kodo
{
    struct coder
    {
        virtual uint32_t payload_size() const = 0;
        virtual uint32_t block_size() const = 0;
        virtual uint32_t symbol_size() const = 0;
        virtual uint32_t symbols() const = 0;
        virtual uint32_t rank() const = 0;

        virtual ~coder(){}
    };
}


