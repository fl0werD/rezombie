#pragma once

#include "rezombie/core/object.h"
#include <array>

namespace rz
{
    // cs missed body without armor?
    enum class PlayerSoundType : int {
        Body,
        Head,
        ArmoredBody,
        ArmoredHead,
        Death,
        ChangeClass,
        MAX_SOUND_TYPES
    };

    class PlayerSounds : public Object {
        std::array<std::vector<std::string>, toInt(PlayerSoundType::MAX_SOUND_TYPES)> sounds_;
        std::array<bool, toInt(PlayerSoundType::MAX_SOUND_TYPES)> isDefault_{};

        auto sounds(const PlayerSoundType soundType) -> std::vector<std::string>& {
            return sounds_[toInt(soundType)];
        }

        auto isDefault(const PlayerSoundType soundType) -> bool& {
            return isDefault_[toInt(soundType)];
        }

        auto addDefault(const PlayerSoundType soundType, const std::string& path) -> void {
            sounds_[toInt(soundType)].emplace_back(path);
        }

      public:
        explicit PlayerSounds(std::string handle) : Object(std::move(handle)) {
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

        auto add(const PlayerSoundType soundType, const std::string& path) -> void {
            if (isDefault(soundType)) {
                isDefault(soundType) = false;
                sounds(soundType).clear();
            }
            sounds(soundType).emplace_back(path);
        }

        auto getRandom(const PlayerSoundType soundType) -> const std::string& {
            // if (sounds_[toInt(soundType)].empty()) {
            //     return nullptr;
            // }
            return sounds(soundType)[RandomLong(0, sounds(soundType).size() - 1)];
        }
    };
}
