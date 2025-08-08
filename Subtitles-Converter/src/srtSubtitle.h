#pragma once

#include "Subtitle.h"

class srtSubtitle : public Subtitle {
public:
    void WriteToFile(std::string& file_name) override;
    void ReadFromFile(std::string& file_name) override;
    srtSubtitle(vectorr<SubtitleLine>& info);
    srtSubtitle() = default;
    ~srtSubtitle() override = default;
};