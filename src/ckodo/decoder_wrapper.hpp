// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cassert>

#include <sak/storage.hpp>

#include "decoder.hpp"


namespace kodo
{

    template<class KodoStack>
    struct decoder_wrapper : public decoder
    {
        decoder_wrapper(const typename KodoStack::pointer& decoder)
            : m_decoder(decoder)
        {
            assert(m_decoder);
        }

        virtual uint32_t recode(uint8_t *payload)
        {
            assert(payload);
            assert(m_decoder);

            return m_decoder->recode(payload);
        }

        virtual void decode(uint8_t *payload)
        {
            assert(payload);
            assert(m_decoder);

            m_decoder->decode(payload);
        }

        virtual bool is_complete() const
        {
            assert(m_decoder);
            return m_decoder->is_complete();
        }

        virtual uint32_t rank() const
        {
            assert(m_decoder);
            return m_decoder->rank();
        }

        virtual uint32_t payload_size() const
        {
            return m_decoder->payload_size();
        }

        virtual uint32_t block_size() const
        {
            return m_decoder->block_size();
        }

        virtual uint32_t symbol_size() const
        {
            return m_decoder->symbol_size();
        }

        virtual uint32_t symbols() const
        {
            return m_decoder->symbols();
        }

        virtual void copy_symbols(uint8_t* data, uint32_t size) const
        {
            auto storage = sak::mutable_storage(data, size);
            m_decoder->copy_symbols(storage);
        }

        typename KodoStack::pointer m_decoder;

    };

}


