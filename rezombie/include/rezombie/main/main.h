#pragma once

#include <rezombie/entity/extras.h>
#include <mhooks/amxx.h>

namespace rz
{
    auto OnAmxxAttach(const AmxxAttachMChain& chain) -> AmxxStatus;
    auto OnAmxxPluginsLoaded(const AmxxPluginsLoadedMChain& chain) -> void;
    auto OnAmxxDetach(const AmxxDetachMChain& chain) -> void;

    inline Extras GlobalExtras;
}
