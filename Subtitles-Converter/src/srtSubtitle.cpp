#include "srtSubtitle.h"

srtSubtitle::srtSubtitle(vectorr< SubtitleLine >& info) : Subtitle(info) {}

void srtSubtitle::ReadFromFile(std::string& file_name)
{
	std::ifstream in(file_name);
	if (!in.is_open())
	{
		throw std::runtime_error("Unable to open file for reading.");
	}
	SubtitleLine info;
	std::string temp;
	while (std::getline(in, temp))
	{
		if (temp.empty())
		{
			continue;
		}
		info.counter = std::stoi(temp);
		if (!std::getline(in, temp))
			break;
		if (temp.size() < 29 || temp.find("-->") == std::string::npos)
		{
			throw std::runtime_error("Malformed SRT time line: " + temp);
		}
		info.timeStart = temp.substr(0, 12);
		info.timeEnd = temp.substr(17, 12);
		if (!std::getline(in, info.text))
		{
			break;
		}
		std::getline(in, temp);
		data_.push_back(info);
	}
	in.close();
}


void srtSubtitle::WriteToFile(std::string& file_name)
{
	std::ofstream out(file_name);
	if (!out.is_open())
	{
		throw std::runtime_error("Unable to open file for writing.");
	}
	for (size_t i = 0; i < data_.size(); i++)
	{
		out << i + 1 << '\n';
		out << data_[i].timeStart << " --> " << data_[i].timeEnd << '\n';
		out << data_[i].text << '\n';
		out << '\n';
	}
	out.close();
}
