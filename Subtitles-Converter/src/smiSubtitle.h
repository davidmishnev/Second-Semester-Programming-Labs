#pragma once

#include "Subtitle.h"

class smiSubtitle : public Subtitle
{
  public:
	void ReadFromFile(std::string& file_name) override;
	void WriteToFile(std::string& file_name) override;
	smiSubtitle(vectorr< SubtitleLine >& info);
	smiSubtitle() = default;
	~smiSubtitle() override = default;
};

std::string ConvertTime(uint32_t ms) noexcept;

uint32_t ConvertToMilliseconds(std::string& time) noexcept;
