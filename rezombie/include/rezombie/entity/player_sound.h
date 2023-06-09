#pragma once

#include "rezombie/core/base_object.h"
#include <array>
#include <utility>

namespace rz::player
{
    // cs missed body without armor?
    enum class PlayerSoundType : int {
        Body,
        Head,
        ArmoredBody,
        ArmoredHead,
        Death,
        TurnIntoClass,
        MAX_SOUND_TYPES
    };

    class PlayerSound : public BaseObject {
      private:
        std::array<std::vector<std::string>, toInt(PlayerSoundType::MAX_SOUND_TYPES)> sounds_;
        std::array<bool, toInt(PlayerSoundType::MAX_SOUND_TYPES)> isDefault_{};

        auto addDefault(PlayerSoundType soundType, const std::string& path) -> void {
            sounds_[toInt(soundType)].push_back(path);
        }

      public:
        explicit PlayerSound(std::string handle) : BaseObject(std::move(handle)) {
            isDefault_.fill(true);
            addDefault(PlayerSoundType::Body, "player/bhit_flesh-1.wav");
            addDefault(PlayerSoundType::Body, "player/bhit_flesh-2.wav");
            addDefault(PlayerSoundType::Body, "player/bhit_flesh-3.wav");
            addDefault(PlayerSoundType::Head, "player/headshot1.wav");
            addDefault(PlayerSoundType::Head, "player/headshot2.wav");
            addDefault(PlayerSoundType::Head, "player/headshot3.wav");
            addDefault(PlayerSoundType::ArmoredBody, "player/bhit_kevlar-1.wav");
            addDefault(PlayerSoundType::ArmoredHead, "player/bhit_helmet-1.wav");
            addDefault(PlayerSoundType::Death, "player/die1.wav");
            addDefault(PlayerSoundType::Death, "player/die2.wav");
            addDefault(PlayerSoundType::Death, "player/die3.wav");
            addDefault(PlayerSoundType::Death, "player/death6.wav");
        }

        auto add(PlayerSoundType soundType, const std::string& path) -> void {
            auto soundTypeInt = toInt(soundType);
            if (isDefault_[soundTypeInt]) {
                sounds_[soundTypeInt].clear();
                isDefault_[soundTypeInt] = false;
            }
            sounds_[soundTypeInt].push_back(path);
        }

        auto getRandom(PlayerSoundType soundType) const -> const std::string& {
            // if (sounds_[toInt(soundType)].empty()) {
            //     return nullptr;
            // }
            return sounds_[toInt(soundType)][RandomLong(0, sounds_[toInt(soundType)].size() - 1)];
        }
    };
}
