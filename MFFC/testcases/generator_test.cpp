//
// Created by tripack on 17-1-5.
//

#include <gtest/gtest.h>
#include "../pattern_gen.h"

TEST(PATTERN_GENERATOR_FULL, I1) {
    FullPatternGenerator pg(1);
    ASSERT_EQ(pg.nInputs(), 1);
    ASSERT_FALSE(pg.hasEnded());
    ASSERT_EQ(pg.generate(), std::vector<int>({0}));
    ASSERT_EQ(pg.generate(), std::vector<int>({1}));
    ASSERT_TRUE(pg.hasEnded());
}

TEST(PATTERN_GENERATOR_FULL, I3) {
    FullPatternGenerator pg(3);
    ASSERT_EQ(pg.nInputs(), 3);
    ASSERT_FALSE(pg.hasEnded());
    ASSERT_EQ(pg.generate(), std::vector<int>({0, 0, 0}));
    ASSERT_EQ(pg.generate(), std::vector<int>({1, 0, 0}));
    ASSERT_EQ(pg.generate(), std::vector<int>({0, 1, 0}));
    ASSERT_EQ(pg.generate(), std::vector<int>({1, 1, 0}));
    ASSERT_EQ(pg.generate(), std::vector<int>({0, 0, 1}));
    ASSERT_EQ(pg.generate(), std::vector<int>({1, 0, 1}));
    ASSERT_EQ(pg.generate(), std::vector<int>({0, 1, 1}));
    ASSERT_EQ(pg.generate(), std::vector<int>({1, 1, 1}));
    ASSERT_TRUE(pg.hasEnded());
}

TEST(PATTERN_GENERATOR_FINITE_RAND, I3) {
    RandomPatternGenerator pg(3, 8);
    ASSERT_EQ(pg.nInputs(), 3);
    ASSERT_FALSE(pg.hasEnded());
    //std::cout << "\n\n";
    for (int i = 0; i < 8; ++i) {
        auto v = pg.generate();
        for (auto i : v)
            std::cout << i << " ";
        std::cout << std::endl;
    }
    ASSERT_TRUE(pg.hasEnded());
}