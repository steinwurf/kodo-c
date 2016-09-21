// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>
#include <cstdlib>
#include <ctime>

#include <platform/config.hpp>
#include <gtest/gtest.h>

// The main function should not be defined on Windows Phone
#if defined(PLATFORM_WINDOWS_PHONE)
    int invoke_gtest(int argc, char** argv)
#else
    GTEST_API_ int main(int argc, char** argv)
#endif
{
    srand(static_cast<uint32_t>(time(0)));

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
