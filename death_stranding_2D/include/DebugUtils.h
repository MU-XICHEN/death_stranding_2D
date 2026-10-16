#pragma once

// Keep this in sync with GamesEngineeringBase.h.
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>

namespace GamesEngineeringBase::Debugger
{
    inline void showAlert()
    {
        MessageBoxW(
            nullptr,
            L"Something went wrong!",
            L"Warning",
            MB_OK | MB_ICONWARNING);
    }
}