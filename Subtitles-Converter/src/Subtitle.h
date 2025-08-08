#pragma once

#include <exception>
#include <stdexcept>
#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include "vectorr.h"


#define SRT 0
#define SMI 1
#define ASS 2

class Subtitle {
protected:
    struct SubtitleLine {
        int counter;
        std::string timeStart, timeEnd, text;
    };
    vectorr<SubtitleLine> data_;
public:
    virtual void ReadFromFile(std::string& file_name) = 0;
    virtual void WriteToFile(std::string& file_name) = 0;
    vectorr<SubtitleLine>& GetData();
    Subtitle(vectorr<SubtitleLine>& info);
    Subtitle();
    Subtitle operator=(Subtitle& other) = delete;
    virtual ~Subtitle() = default;
};
