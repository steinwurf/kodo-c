// Copyright Steinwurf ApS 2015.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "kodo_encoder.hpp"

extern "C"
{
    /// Interface class for a perpetual encoder implementation
    struct kodo_perpetual_encoder : public kodo_encoder
    {
    public:

        virtual ~kodo_perpetual_encoder() {}

        virtual bool pseudo_systematic() const = 0;
        virtual void set_pseudo_systematic(bool pseudo_systematic) = 0;
        virtual bool pre_charging() const = 0;
        virtual void set_pre_charging(bool pre_charging) = 0;
        virtual uint32_t width() const = 0;
        virtual void set_width(uint32_t width) = 0;
        virtual double width_ratio() const = 0;
        virtual void set_width_ratio(double width_ratio) = 0;
    };
}
