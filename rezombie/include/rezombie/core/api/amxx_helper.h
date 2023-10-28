#pragma once

namespace rz
{
#define CHECK_VAR_EXISTS(format, ...) \
    if (!var) { \
        LogError(amx, AmxError::Native, format, __VA_ARGS__); \
        return false; \
    }
}
