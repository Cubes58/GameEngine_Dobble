#include "pch.h"

#define private public

#include "RenderComponent.h"

TEST(RenderComponentTest, TestInitialization) {
	RenderComponent renderComponent;

	EXPECT_TRUE(true);
}

int main(int ac, char *av[]) {
	testing::InitGoogleTest(&ac, av);
	return RUN_ALL_TESTS();
}