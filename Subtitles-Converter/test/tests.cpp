#include "../Subtitle.h"
#include "../SubtitleParser.h"
#include "../assSubtitle.h"
#include "../smiSubtitle.h"
#include "../srtSubtitle.h"
#include "../vttSubtitle.h"

#include <gtest/gtest.h>

TEST(srt_format, srt_to_vtt)
{
	srtSubtitle input_srt;
	std::string in = "test.srt";
	std::string out = "fromSrtToVtt.vtt";

	input_srt.ReadFromFile(in);

	vttSubtitle output_vtt(input_srt.GetData());
	output_vtt.WriteToFile(out);

	vttSubtitle check_vtt;
	check_vtt.ReadFromFile(out);

	auto& original = input_srt.GetData();
	auto& converted = check_vtt.GetData();

	ASSERT_EQ(original.size(), converted.size());

	for (size_t i = 0; i < original.size(); ++i)
	{
		EXPECT_EQ(original[i].text, converted[i].text) << "Text mismatch at line " << i;
		EXPECT_EQ(original[i].timeStart, converted[i].timeStart) << "Start time mismatch at line " << i;
		EXPECT_EQ(original[i].timeEnd, converted[i].timeEnd) << "End time mismatch at line " << i;
	}
}

TEST(ass_format, ass_to_srt) {
	assSubtitle input_ass;
	std::string in = "test.ass";
	std::string out = "fromAssToSrt.srt";

	input_ass.ReadFromFile(in);

	srtSubtitle output_srt(input_ass.GetData());

	auto& original = input_ass.GetData();
	auto& converted = output_srt.GetData();

	ASSERT_EQ(original.size(), converted.size());

	for (size_t i = 0; i < original.size(); ++i) {
		EXPECT_EQ(original[i].text, converted[i].text) << "Text mismatch at line " << i;
		EXPECT_EQ(original[i].timeStart, converted[i].timeStart) << "Start time mismatch at line " << i;
		EXPECT_EQ(original[i].timeEnd, converted[i].timeEnd) << "End time mismatch at line " << i;
	}
}


TEST(ass_format, ass_to_vtt)
{
	assSubtitle input_ass;
	std::string in = "test.ass";
	std::string out = "fromAssToVtt.vtt";

	input_ass.ReadFromFile(in);

	vttSubtitle output_vtt(input_ass.GetData());

	auto& original = input_ass.GetData();
	auto& converted = output_vtt.GetData();

	ASSERT_EQ(original.size(), converted.size());

	for (size_t i = 0; i < original.size(); ++i)
	{
		EXPECT_EQ(original[i].text, converted[i].text) << "Text mismatch at line " << i;
		EXPECT_EQ(original[i].timeStart, converted[i].timeStart) << "Start time mismatch at line " << i;
		EXPECT_EQ(original[i].timeEnd, converted[i].timeEnd) << "End time mismatch at line " << i;
	}
}

TEST(srt_format, srt_to_ass)
{
	srtSubtitle input_srt;
	std::string in = "test.srt";
	std::string out = "fromSrtToAss.ass";

	input_srt.ReadFromFile(in);

	assSubtitle output_ass(input_srt.GetData());

	auto& original = input_srt.GetData();
	auto& converted = output_ass.GetData();

	ASSERT_EQ(original.size(), converted.size());

	for (size_t i = 0; i < original.size(); ++i)
	{
		EXPECT_EQ(original[i].text, converted[i].text) << "Text mismatch at line " << i;
		EXPECT_EQ(original[i].timeStart, converted[i].timeStart) << "Start time mismatch at line " << i;
		EXPECT_EQ(original[i].timeEnd, converted[i].timeEnd) << "End time mismatch at line " << i;
	}
}

TEST(smi_format, smi_to_srt)
{
	smiSubtitle input_smi;
	std::string in = "test.smi";
	std::string out = "fromSmiToSrt.srt";

	input_smi.ReadFromFile(in);

	smiSubtitle output_smi(input_smi.GetData());

	auto& original = input_smi.GetData();
	auto& converted = output_smi.GetData();

	ASSERT_EQ(original.size(), converted.size());

	for (size_t i = 0; i < original.size(); ++i)
	{
		EXPECT_EQ(original[i].text, converted[i].text) << "Text mismatch at line " << i;
		EXPECT_EQ(original[i].timeStart, converted[i].timeStart) << "Start time mismatch at line " << i;
	}
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
