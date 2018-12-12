#include "pch.h"

#define private public
#include "Game.h"

TEST(TestCaseName, TestName) {
  //EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

int main(int ac, char *av[]) {
	testing::InitGoogleTest(&ac, av);
	return RUN_ALL_TESTS();
}