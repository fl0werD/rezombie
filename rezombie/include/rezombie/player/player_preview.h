#pragma once

#include <cssdk/public/regamedll.h>
#include <string>

namespace rz::player
{
    using namespace cssdk;

    class PlayerPreview
    {
      private:
        Edict* skeleton_ = {};
        Edict* playerModel_ = {};
        Edict* weaponModel_ = {};
        std::string skeletonPath_;
        std::string playerModelPath_;
        std::string weaponModelPath_;
        float viewForwardDistance_ = 96.f;
        float buttonsRotateSpeed_ = 2.f;

      public:
        auto PreviewPrecache() -> void;
        auto CreatePreview() -> void;
        auto RemovePreview() -> void;
        auto ChangePlayerModel(int modelIndex) -> void;
        auto UpdatePosition(Vector gunPosition, Vector& viewAngle) -> void;

        auto getViewForwardDistance() const -> float
        {
            return viewForwardDistance_;
        }

        auto setViewForwardDistance(float viewForwardDistance) -> void
        {
            viewForwardDistance_ = viewForwardDistance;
        }
    };

    constexpr auto PREVIEW_SKELETON = "models/shop.mdl";
    constexpr auto PREVIEW_PLAYER = "models/player/leet/leet.mdl";
    constexpr auto PREVIEW_WEAPON = "models/p_m4a1.mdl";

    inline int g_previewSkeleton_ModelIndex = {};
    inline int g_previewPlayer_ModelIndex = {};
    inline int g_previewWeapon_ModelIndex = {};
}
