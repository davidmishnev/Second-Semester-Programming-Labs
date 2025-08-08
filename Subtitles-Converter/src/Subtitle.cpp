#include "Subtitle.h"

Subtitle::Subtitle(vectorr< SubtitleLine >& info)
{
	data_ = info;
}

vectorr< Subtitle::SubtitleLine >& Subtitle::GetData()
{
	return this->data_;
}
