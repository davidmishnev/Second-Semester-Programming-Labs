#pragma once

#include "Subtitle.h"

#define SRT 0
#define SMI 1
#define ASS 2

int Parse_input_file_extension(std::fstream& input_file);

int Parse_output_file_extension(char*& filename);
