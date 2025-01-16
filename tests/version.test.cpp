// External includes
#include <gtest/gtest.h>

// Local includes
#include "../build/version.hpp"

TEST(version_test, runtime_version_matches_compiletime_version) {
    ASSERT_STREQ(
            arch::get_runtime_version(),
            arch::compiletime_version);
}