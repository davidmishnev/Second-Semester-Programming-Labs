#pragma once

#include "Subtitle.h"

class assSubtitle : public Subtitle {
    struct ScriptInfo {
        std::string Title_;
        std::string OriginalScript_;
        std::string ScriptType_;
        std::string Collisions_;

    };
    struct Styles {
        std::string Name_;
        std::string Fontname_;
        uint8_t Fontsize_;
        std::string PrimaryColour_;
        std::string SecondaryColour_;
        std::optional<std::string> OutlineColour_;
        std::optional<std::string> BackColour_;
        std::optional<bool> Bold_;
        std::optional<bool> Italic_;
        std::optional<bool> Underline_;
        std::optional<bool> StrikeOut_;
        std::optional<uint8_t> ScaleX_;
        std::optional<uint8_t> ScaleY_;
        std::optional<float> Spacing_;
        std::optional<float> Angle_;
        std::optional<uint8_t> BorderStyle_;
        std::optional<float> Outline_;
        std::optional<float> Shadow_;
        std::optional<uint8_t> Alignment_;
        std::optional<uint16_t> MarginL_;
        std::optional<uint16_t> MarginR_;
        std::optional<uint16_t> MarginV_;
        std::optional<uint8_t> Encoding_;
    };
public:
    void ReadFromFile(std::string& filename) override;
    void WriteToFile(std::string& filename) override;
    assSubtitle(vectorr<SubtitleLine>& info);
    assSubtitle() = default;
    ~assSubtitle() override = default;
};

std::string ConvertASSTimeToSRT(std::string& assTime);