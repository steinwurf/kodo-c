// Copyright Steinwurf ApS 2015.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cassert>

#include "encoder_wrapper.hpp"

namespace kodo
{
    template<class KodoStack, class Base>
    class perpetual_encoder_wrapper : public encoder_wrapper<KodoStack, Base>
    {
    public:

        using Super = encoder_wrapper<KodoStack, Base>;

        using coder_type = typename Super::coder_type;

    public:

        perpetual_encoder_wrapper(const coder_type& coder) :
            Super(coder)
        {
            assert(m_coder);
        }

        virtual bool pseudo_systematic() const
        {
            return m_coder->pseudo_systematic();
        }

        virtual void set_pseudo_systematic(bool pseudo_systematic)
        {
            m_coder->set_pseudo_systematic(pseudo_systematic);
        }

        virtual bool pre_charging() const
        {
            return m_coder->pre_charging();
        }

        virtual void set_pre_charging(bool pre_charging)
        {
            m_coder->set_pre_charging(pre_charging);
        }

        virtual uint32_t width() const
        {
            return m_coder->width();
        }

        virtual void set_width(uint32_t width)
        {
            m_coder->set_width(width);
        }

        virtual double width_ratio() const
        {
            return m_coder->width_ratio();
        }

        virtual void set_width_ratio(double width_ratio)
        {
            m_coder->set_width_ratio(width_ratio);
        }

    protected:

        using Super::m_coder;
    };
}
