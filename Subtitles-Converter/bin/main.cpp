#include "Subtitle.h"
#include "SubtitleParser.h"
#include "assSubtitle.h"
#include "smiSubtitle.h"
#include "srtSubtitle.h"
#include "vttSubtitle.h"

#define ARGUMENTS_COUNT 3

int main(int argc, char* argv[])
{
	if (argc != ARGUMENTS_COUNT)
	{
		throw std::invalid_argument("Invalid number of arguments were provided.");
	}
	std::string filename_in = static_cast< std::string >(argv[1]);
	std::string filename_out = static_cast< std::string >(argv[2]);
	std::fstream input_file(filename_in);
	int input_file_extension = Parse_input_file_extension(input_file);
	int output_file_extension = Parse_output_file_extension(argv[2]);
	std::fstream output_file(filename_out);
	switch (input_file_extension)
	{
	case SRT:
	{
		srtSubtitle input_srt;
		input_srt.ReadFromFile(filename_in);
		switch (output_file_extension)
		{
		case SRT:
			throw std::invalid_argument("Input and output formats match.");
		case SMI:
		{
			smiSubtitle output_smi(input_srt.GetData());
			output_smi.WriteToFile(filename_out);
			break;
		}
		case ASS:
		{
			assSubtitle output_ass(input_srt.GetData());
			output_ass.WriteToFile(filename_out);
			break;
		}
		case VTT:
		{
			vttSubtitle output_vtt(input_srt.GetData());
			output_vtt.WriteToFile(filename_out);
			break;
		}
		default:
			throw std::invalid_argument("Unsupported output format.");
		}
		break;
	}

	case SMI:
	{
		smiSubtitle input_smi;
		input_smi.ReadFromFile(filename_in);
		switch (output_file_extension)
		{
		case SRT:
		{
			srtSubtitle output_srt(input_smi.GetData());
			output_srt.WriteToFile(filename_out);
			break;
		}
		case SMI:
			throw std::invalid_argument("Input and output formats match.");
		case ASS:
		{
			assSubtitle output_ass(input_smi.GetData());
			output_ass.WriteToFile(filename_out);
			break;
		}
		case VTT:
		{
			vttSubtitle output_vtt(input_smi.GetData());
			output_vtt.WriteToFile(filename_out);
			break;
		}
		default:
			throw std::invalid_argument("Unsupported output format.");
		}
		break;
	}
	case ASS:
	{
		assSubtitle input_ass;
		input_ass.ReadFromFile(filename_in);
		switch (output_file_extension)
		{
		case SRT:
		{
			srtSubtitle output_srt(input_ass.GetData());
			output_srt.WriteToFile(filename_out);
			break;
		}
		case SMI:
		{
			smiSubtitle output_smi(input_ass.GetData());
			output_smi.WriteToFile(filename_out);
			break;
		}
		case ASS:
			throw std::invalid_argument("Input and output formats match.");
		case VTT:
		{
			vttSubtitle output_vtt(input_ass.GetData());
			output_vtt.WriteToFile(filename_out);
			break;
		}
		default:
			throw std::invalid_argument("Unsupported output format.");
		}
		break;
	}
	case VTT:
	{
		assSubtitle input_ass;
		input_ass.ReadFromFile(filename_in);
		switch (output_file_extension)
		{
		case SRT:
		{
			srtSubtitle output_srt(input_ass.GetData());
			output_srt.WriteToFile(filename_out);
			break;
		}
		case SMI:
		{
			smiSubtitle output_smi(input_ass.GetData());
			output_smi.WriteToFile(filename_out);
			break;
		}
		case ASS:
		{
			assSubtitle output_ass(input_ass.GetData());
			output_ass.WriteToFile(filename_out);
			break;
		}
		case VTT:
		{
			throw std::invalid_argument("Input and output formats match.");
		}
		default:
			throw std::invalid_argument("Unsupported output format.");
		}
		break;
	}
	default:
		throw std::invalid_argument("Invalid input file format.");
	}

	input_file.close();
	output_file.close();
	return 0;
}
