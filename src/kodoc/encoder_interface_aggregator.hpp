// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodo/api/api.hpp>

#include "msvc12_unpack.hpp"

namespace kodoc
{
    template<class... Interfaces>
    struct encoder_interface_aggregator :
        kodo::api::build_interface,
        kodo::api::trace_interface,
        kodo::api::rank_interface,
        kodo::api::storage_interface,
        kodo::api::const_storage_interface,
        kodo::api::encoder_interface,
        kodo::api::write_payload_interface,
        kodo::api::payload_size_interface,
        Interfaces...
    {
        struct config_interface :
            kodo::api::build_interface::config_interface,
            kodo::api::trace_interface::config_interface,
            kodo::api::rank_interface::config_interface,
            kodo::api::storage_interface::config_interface,
            kodo::api::const_storage_interface::config_interface,
            kodo::api::encoder_interface::config_interface,
            kodo::api::write_payload_interface::config_interface,
            kodo::api::payload_size_interface::config_interface,
            msvc12_unpack<Interfaces>...
        { };
    };
}
