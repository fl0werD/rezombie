#pragma once

#include "rezombie/main/util.h"
#include <cssdk/public/regamedll.h>
#include <string>
#include <vector>
#include <array>

namespace rz
{
    using namespace cssdk;

    enum class PreviewType : int {
        ParentModel,
        AttachModel,
        ExtraAttachModel,
        MAX_TYPES
    };

    class ModelPreview {
        std::array<Edict*, toInt(PreviewType::MAX_TYPES)> entities_{};
        float viewForwardDistance_ = 96;
        float buttonsRotateSpeed_ = 2;

      public:
        auto createEntities() -> void;

        auto getEntity(PreviewType previewEntity) -> Edict* { return entities_[toInt(previewEntity)]; }

        auto setEntity(PreviewType previewEntity, Edict* entity) { entities_[toInt(previewEntity)] = entity; }

        auto getViewForwardDistance() const -> float { return viewForwardDistance_; }

        auto setViewForwardDistance(float viewForwardDistance) -> void { viewForwardDistance_ = viewForwardDistance; }

        auto& operator[](PreviewType previewEntity) { return entities_[toInt(previewEntity)]; }
    };

    inline ModelPreview ModelPreview;
}
