#pragma once

#include "rezombie/core/module.h"
#include "rezombie/entity/models/models_pack.h"

namespace rz
{
    class ModelsPackModule : public Module<ModelsPack> {
      public:
        ModelsPackModule() : Module("models_pack") {}

        auto add(std::string handle) -> int {
            return Module::add(new ModelsPack(std::move(handle)));
        }
    };

    inline ModelsPackModule ModelsPack;
}
