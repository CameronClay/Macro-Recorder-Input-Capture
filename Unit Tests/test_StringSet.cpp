#include "pch.h"
#include "../Macros Template/StringSet.h"
#include "../Macros Template/StringSet.cpp"

TEST(StringSet, AddString_Success) {
	const auto str = "hello";
	StringSet ss{};
	ss.AddString(str);
	const auto res = ss.GetOutStrings();
	EXPECT_EQ(res.size(), 1);
	EXPECT_TRUE(res.find(str) != std::end(res));
}

TEST(StringSet, AddStringNL_Success) {
	const auto str = "hello";
	StringSet ss{};
	ss.AddStringNL(str);
	const auto res = ss.GetOutStrings();
	EXPECT_EQ(res.size(), 1);
	EXPECT_TRUE(res.find(str) != std::end(res));
}

TEST(StringSet, RemoveString_Success) {
	const auto str = "hello";
	StringSet ss{};
	ss.AddString(str);
	ss.RemoveString(str);
	const auto res = ss.GetOutStrings();
	EXPECT_EQ(res.size(), 0);
	EXPECT_TRUE(res.find(str) == std::end(res));
}

TEST(StringSet, RemoveStringNL_Success) {
	const auto str = "hello";
	StringSet ss{};
	ss.AddStringNL(str);
	ss.RemoveStringNL(str);
	const auto res = ss.GetOutStrings();
	EXPECT_EQ(res.size(), 0);
	EXPECT_TRUE(res.find(str) == std::end(res));
}