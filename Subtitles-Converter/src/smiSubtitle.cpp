#include "smiSubtitle.h"

smiSubtitle::smiSubtitle(vectorr< SubtitleLine >& info) : Subtitle(info) {}

std::string ConvertTime(uint32_t ms) noexcept
{
	uint32_t h = ms / 3600000;
	ms %= 3600000;
	uint32_t m = ms / 60000;
	ms %= 60000;
	uint32_t s = ms / 1000;
	uint32_t ms_remainder = ms % 1000;
	char buffer[13];
	std::snprintf(buffer, sizeof(buffer), "%02u:%02u:%02u,%03u", h, m, s, ms_remainder);
	return { buffer };
}

uint32_t ConvertToMilliseconds(std::string& time) noexcept
{
	if (time.size() < 12)
	{
		return 0;
	}
	uint32_t h = (time[0] - '0') * 10 + (time[1] - '0');
	uint32_t m = (time[3] - '0') * 10 + (time[4] - '0');
	uint32_t s = (time[6] - '0') * 10 + (time[7] - '0');
	uint32_t ms = (time[9] - '0') * 100 + (time[10] - '0') * 10 + (time[11] - '0');
	return h * 3600000 + m * 60000 + s * 1000 + ms;
}

void smiSubtitle::ReadFromFile(std::string& file_name)
{
	std::ifstream in(file_name);
	if (!in.is_open())
	{
		throw std::runtime_error("Unable to open file for reading.");
	}
	in.seekg(0, std::ios_base::beg);
	std::string line;
	uint32_t lastStart = 0;
	std::string lastText;
	bool hasLast = false;
	while (!in.eof())
	{
		std::getline(in, line);
		size_t syncPos = line.find("<SYNC Start=");
		if (syncPos == std::string::npos)
		{
			continue;
		}
		size_t equalPos = line.find('=', syncPos);
		size_t closeTag = line.find('>', equalPos);
		if (equalPos == std::string::npos || closeTag == std::string::npos)
		{
			continue;
		}
		std::string startStr = line.substr(equalPos + 1, closeTag - equalPos - 1);
		int startMs = std::stoi(startStr);
		size_t pTag = line.find("<P", closeTag);
		size_t textStart = line.find('>', pTag);
		size_t textEnd = line.find("</P>", textStart);
		std::string text;
		if (textStart != std::string::npos && textEnd != std::string::npos)
		{
			text = line.substr(textStart + 1, textEnd - textStart - 1);
		}
		if (text == "&nbsp;" || text.empty())
		{
			text = "";
		}
		if (hasLast)
		{
			SubtitleLine sub;
			sub.timeStart = ConvertTime(lastStart);
			sub.timeEnd = ConvertTime(startMs);
			sub.text = lastText;
			data_.push_back(sub);
		}
		lastStart = startMs;
		lastText = text;
		hasLast = true;
	}
	if (hasLast)
	{
		SubtitleLine sub;
		sub.timeStart = ConvertTime(lastStart);
		sub.timeEnd = ConvertTime(lastStart + 2000);
		sub.text = lastText;
		data_.push_back(sub);
	}
	in.close();
}

void smiSubtitle::WriteToFile(std::string& file_name)
{
	std::ofstream out(file_name);
	if (!out.is_open())
	{
		throw std::runtime_error("Unable to open file for writing.");
	}
	out << "<SAMI>\n";
	out << "<HEAD>\n";
	out << "  <TITLE>Converted Subtitle</TITLE>\n";
	out << "  <STYLE TYPE=\"text/css\">\n";
	out << "  <!--\n";
	out << "    P{ font-size:10pt; text-align:center; font-family:Arial; color:white; background-color:black; }\n";
	out << "  -->\n";
	out << "  </STYLE>\n";
	out << "</HEAD>\n";
	out << "<BODY>\n";
	for (size_t i = 0; i < data_.size(); ++i)
	{
		uint32_t startMs = ConvertToMilliseconds(data_[i].timeStart);
		out << "<SYNC Start=" << startMs << ">\n";
		out << "<P Class=ENCC>";
		if (!data_[i].text.empty())
		{
			out << data_[i].text;
		}
		else
		{
			out << "&nbsp;";
		}
		out << "</P>\n";
	}
	uint32_t lastEndMs = ConvertToMilliseconds(data_[data_.size() - 1].timeEnd);
	out << "<SYNC Start=" << (lastEndMs + 1000) << ">\n";
	out << "<P Class=ENCC>&nbsp;</P>\n";
	out << "</BODY>\n";
	out << "</SAMI>\n";
	out.close();
}
