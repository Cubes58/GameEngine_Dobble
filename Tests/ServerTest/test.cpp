#include "pch.h"

#define private public
#include "ServerGame.h"
#include "Randomiser.h"

TEST(RandomiserTest, RealRandomNumberGenerated) {
	float min = 0.5f;
	float max = 10.0f;

	float realRandomNumber = Randomiser::getInstance().GetRealRandomNumber<float>(min, max);

	EXPECT_TRUE((realRandomNumber >= min) && (realRandomNumber <= max));
}

int main(int ac, char *av[]) {
	testing::InitGoogleTest(&ac, av);
	return RUN_ALL_TESTS();
}