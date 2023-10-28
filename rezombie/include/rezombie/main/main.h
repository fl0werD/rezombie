#pragma once

#include <mhooks/amxx.h>

namespace rz
{
    auto OnAmxxAttach(const AmxxAttachMChain& chain) -> AmxxStatus;
    auto OnAmxxPluginsLoaded(const AmxxPluginsLoadedMChain& chain) -> void;
}
