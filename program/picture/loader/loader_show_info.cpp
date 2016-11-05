#include "loader.h"

void LOADER::FillShowInfoData(SHOW_INFO_DATA* sid) const noexcept
{
	sid->name = (*current)->name;
}