#pragma once

#include "rezombie/core/object.h"
#include "rezombie/entity/models/model.h"
#include "rezombie/models/modules/models.h"
#include "rezombie/main/util.h"
#include <metamod/engine.h>
#include <algorithm>

namespace rz
{
    class ModelsPack : public Object {
        std::vector<int> models_{};

      public:
        explicit ModelsPack(std::string handle) : Object(std::move(handle)) {}

        auto add(int modelIndex) -> void {
            models_.emplace_back(modelIndex);
        }

        auto count() const -> size_t {
            return models_.size();
        }

        auto getRandom() const -> int {
            using namespace metamod::engine;
            return models_[RandomLong(0, static_cast<int>(count() - 1))];
        }
    };
}
