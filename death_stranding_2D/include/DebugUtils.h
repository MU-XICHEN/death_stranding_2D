#pragma once

// Keep this in sync with GamesEngineeringBase.h.
// Todo: remove this utils.h when finishing this project
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