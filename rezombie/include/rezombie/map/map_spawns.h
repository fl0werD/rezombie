#pragma once

#include "rezombie/entity/map/camera_data.h"
#include <vector>

namespace rz
{
    // TODO: to module?
    class MapSpawns {
        std::vector<CameraData> data_{};

      public:
        auto collect() -> void;

        auto add(Vector origin, Vector angles) -> void {
            data_.emplace_back(origin, angles);
        }

        auto count() const -> int {
            return static_cast<int>(data_.size());
        }

        auto& operator[](int index) { return data_[index]; }
    };

    inline MapSpawns MapSpawns;
}
