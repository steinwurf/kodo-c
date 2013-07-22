// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cassert>

#include <sak/storage.hpp>

#include "encoder.hpp"

namespace kodo
{

    template<class KodoStack>
    struct encoder_wrapper : public encoder
    {
        encoder_wrapper(const typename KodoStack::pointer& encoder)
            : m_encoder(encoder)
        {
            assert(m_encoder);
        }

        virtual uint32_t encode(uint8_t *payload)
        {
            return m_encoder->encode(payload);
        }

        virtual void set_symbols(
            const uint8_t* data, uint32_t size)
        {
            auto storage = sak::const_storage(data, size);
            m_encoder->set_symbols(storage);
        }

        virtual void set_symbol(
            uint32_t index, const uint8_t* data, uint32_t size)
        {
            auto storage = sak::const_storage(data, size);
            m_encoder->set_symbol(index, storage);
        }

        virtual uint32_t payload_size() const
        {
            return m_encoder->payload_size();
        }

        virtual uint32_t block_size() const
        {
            return m_encoder->block_size();
        }

        virtual uint32_t symbol_size() const
        {
            return m_encoder->symbol_size();
        }

        virtual uint32_t symbols() const
        {
            return m_encoder->symbols();
        }

        virtual uint32_t rank() const
        {
            return m_encoder->rank();
        }


        typename KodoStack::pointer m_encoder;

    };
}


