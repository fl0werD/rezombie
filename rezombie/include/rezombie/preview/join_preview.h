#pragma once

#include "rezombie/main/util.h"
#include "cssdk/public/regamedll.h"
#include <string>
#include <vector>
#include <array>

namespace rz
{
    using namespace cssdk;

    enum class JoinPreviewType : int {
        ParentModel,
        AttachModel,
        ExtraAttachModel,
        MAX_TYPES
    };

    class ModelPreview {
        std::array<Edict*, toInt(JoinPreviewType::MAX_TYPES)> entities_{};
        float viewForwardDistance_ = 96;
        float buttonsRotateSpeed_ = 2;

      public:
        auto createEntities() -> void;

        auto getEntity(JoinPreviewType previewEntity) -> Edict* { return entities_[toInt(previewEntity)]; }

        auto setEntity(JoinPreviewType previewEntity, Edict* entity) { entities_[toInt(previewEntity)] = entity; }

        auto getViewForwardDistance() const -> float { return viewForwardDistance_; }

        auto setViewForwardDistance(float viewForwardDistance) -> void { viewForwardDistance_ = viewForwardDistance; }

        auto& operator[](JoinPreviewType previewEntity) { return entities_[toInt(previewEntity)]; }
    };

    inline ModelPreview ModelPreview;
}
