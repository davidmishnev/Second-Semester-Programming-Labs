#include "SubtitleParser.h"

int Parse_input_file_extension(std::fstream& input_file)
{
	if (!input_file.good())
	{
		throw std::invalid_argument("Invalid input file name was provided.");
	}
	std::string input;
	std::getline(input_file, input, '\n');
	if (input == "1")
	{
		return SRT;
	}
	else if (input == "<SAMI>")
	{
		return SMI;
	}
	else if (input == "[Script Info]")
	{
		return ASS;
	}
	else if (input == "WEBVTT")
	{
		return VTT;
	}
	throw std::invalid_argument("Invalid input file was provided.");
}

int Parse_output_file_extension(char*& filename)
{
	std::cmatch match;
	std::regex pattern(R"(^.+\.(srt|smi|ass|ssa|vtt)$)", std::regex::icase);
	if (std::regex_match(filename, match, pattern))
	{
		std::string ext = match[1].str();
		if (ext == "srt")
		{
			return SRT;
		}
		else if (ext == "smi")
		{
			return SMI;
		}
		else if (ext == "ass" || ext == "ssa")
		{
			return ASS;
		}
		else if (ext == "vtt")
		{
			return VTT;
		}
	}
	throw std::invalid_argument("Wrong output file was provided.");
}
