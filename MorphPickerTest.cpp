#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "MorphPicker.hpp"

TEST(Morph, pickMorph_first__only_4) {
	const std::vector<float> in1 = {0.1, 0.2, 0.3, 0.4};
	EXPECT_THAT(pickMorphs(in1), ::testing::UnorderedElementsAre(0, 1, 2, 3));

	const std::vector<float> in2 = {0.4, 0.2, 0.5, 0.1};
	EXPECT_THAT(pickMorphs(in2), ::testing::UnorderedElementsAre(0, 1, 2, 3));
}

TEST(Morph, pickMorph_less_than_4) {
	const std::vector<float> in1 = {0.1};
	EXPECT_THAT(pickMorphs(in1), ::testing::UnorderedElementsAre(0, -1, -1, -1));

	const std::vector<float> in2 = {0.4, 0.2, 0.5};
	EXPECT_THAT(pickMorphs(in2), ::testing::UnorderedElementsAre(0, 1, 2, -1));

}

TEST(Morph, pickMorph_empty) {
	EXPECT_THAT(pickMorphs({}), ::testing::UnorderedElementsAre(-1, -1, -1, -1));
}

TEST(Morph, pickMorph_end_of_vector) {
	const std::vector<float> in = {0.1, 0.3, 0.7, 0.2, 0.99, 0.8, 0.9, 0.88};
	EXPECT_THAT(pickMorphs(in), ::testing::UnorderedElementsAre(4, 5, 6, 7));
}
