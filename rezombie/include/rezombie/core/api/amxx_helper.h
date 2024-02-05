#pragma once

namespace rz
{
#define PARAMS_COUNT            (params[0] / sizeof(cell))

#define CHECK_PLAYER(id)       \
    if (!IsClient(id)) { \
        LogError(amx, AmxError::Native, "invalid player index %d", id); \
        return false; \
    }

#define CHECK_VAR_EXISTS(format, ...) \
    if (!var) { \
        LogError(amx, AmxError::Native, format, __VA_ARGS__); \
        return false; \
    }
}
