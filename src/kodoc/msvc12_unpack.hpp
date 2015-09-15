// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodo/api/api.hpp>

/// @todo expose in kodo
namespace kodoc
{
    template<class Interface>
    struct msvc12_unpack : public Interface::config_interface { };
}
