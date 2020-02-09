#include <gtest/gtest.h>
#include "lib.h"

TEST(, test2)
{
	ASSERT_TRUE(version() > 1);
}