#include "assSubtitle.h"

assSubtitle::assSubtitle(vectorr< SubtitleLine > &info) : Subtitle(info) {}

void assSubtitle::ReadFromFile(std::string& file_name)
{
	std::ifstream in(file_name);
	if (!in.is_open()) {
		throw std::runtime_error("Unable to open file for reading.");
	}

	std::string line;
	while (std::getline(in, line)) {
		if (line.rfind("Dialogue:", 0) != 0) {
			continue;
		}
		size_t pos = 9;
		if (line[pos] == ' ') ++pos;

		vectorr<std::string> tokens;
		std::string current;
		bool inQuotes = false;

		for (; pos < line.size(); ++pos) {
			char c = line[pos];
			if (c == ',' && !inQuotes) {
				tokens.push_back(current);
				current.clear();
			} else {
				current += c;
			}
		}
		tokens.push_back(current);

		if (tokens.size() < 10) {
			throw std::runtime_error("Malformed Dialogue line");
		}

		SubtitleLine sub;
		sub.timeStart = tokens[1]; // Start
		sub.timeEnd = tokens[2];   // End
		sub.text = tokens[9];      // Text

		data_.push_back(sub);
	}

	in.close();
}


std::string ConvertASSTimeToSRT(std::string &assTime)
{
	int h = 0, m = 0, s = 0, cs = 0;
	sscanf(assTime.c_str(), "%d:%d:%d.%d", &h, &m, &s, &cs);
	int ms = cs * 10;
	std::ostringstream srtTime;
	srtTime << std::setfill('0') << std::setw(2) << h << ":" << std::setw(2) << m << ":" << std::setw(2) << s << ","
			<< std::setw(3) << ms;
	return srtTime.str();
}

void assSubtitle::WriteToFile(std::string &filename)
{
	std::ofstream out(filename);
	out << "[Script Info]\n";
	out << "This subtitle file was created by using Subtitle.h library.\n";
	out << "Title: \n";
	out << "Original Script:\n";
	out << "ScriptType:\n";
	out << "ScriptType: v4.00+\n";
	out << "Collisions: Normal\n";
	out << '\n';
	out << "[V4+ Styles]\n";
	out << "Format: Name, Fontname, Fontsize, PrimaryColour, SecondaryColour\n";
	out << "Style: Default,Arial,24,&H00FFFFFF,&H000000FF\n";
	out << '\n';
	out << "[Events]\n";
	out << "Format: Start, End, Text\n";
	for (int i = 0; i < data_.size(); i++)
	{
		out << "Dialogue: " << data_[i].timeStart << ',' << data_[i].timeEnd << ',' << data_[i].text << '\n';
	}
}
