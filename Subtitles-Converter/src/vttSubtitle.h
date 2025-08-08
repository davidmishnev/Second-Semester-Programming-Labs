#include "Subtitle.h"

class vttSubtitle : public Subtitle
{
  public:
	void WriteToFile(std::string& file_name) override;
	void ReadFromFile(std::string& file_name) override;
	vttSubtitle(vectorr< SubtitleLine >& info);
	vttSubtitle() = default;
	~vttSubtitle() override = default;
};
