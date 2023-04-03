#include "rezombie/util.h"
#include "rezombie/gamerules/consts.h"
#include <messages/user_message.h>
#include "rezombie/player/players.h"
#include <cssdk/dll/game_rules.h>
#include <string>

namespace rz {
    using namespace cssdk;
    using namespace rz::player;
    using namespace message;

    auto isPlayableTeam(Team team) -> bool {
        return (team == Team::Human || team == Team::Zombie);
    }

    auto getPlayersCount(Team team, PlayersCountType countType) -> int {
        int playersCount = 0;
        players.forEachConnected(
            [&](const auto& player) {
                bool teamCount = (team == Team::Unassigned || team == player.getTeam());
                switch (countType) {
                    case PlayersCountType::All: {
                        if (teamCount) {
                            ++playersCount;
                        }
                        break;
                    }
                    case PlayersCountType::Dead: {
                        if (teamCount && !player.isAlive()) {
                            ++playersCount;
                        }
                        break;
                    }
                    case PlayersCountType::Alive: {
                        if (teamCount && player.isAlive()) {
                            ++playersCount;
                        }
                        break;
                    }
                }
            }
        );
        return playersCount;
    }

    auto FileExists(const char* filePath) -> bool {
        FILE* file;
        fopen_s(&file, filePath, "rb");
        if (!file) {
            return false;
        }
        fclose(file);
        return true;
    }

    auto UTIL_EmitSound(
        Edict* entity, SoundChannel channel, const std::string& sample, float volume, float attenuation,
        int flags, int pitch
    ) -> void {
        EmitSound(entity, channel, sample.c_str(), volume, attenuation, flags, pitch);
    }

    auto UTIL_SendMotd(Edict* receiver, const std::string& text) -> void {
        auto length = text.length();
        if (length > MAX_MOTD_LENGTH) {
            length = MAX_MOTD_LENGTH;
        }
        auto more = false;
        auto sent = 0;
        do {
            constexpr auto chunkSize = MAX_MOTD_CHUNK;
            const auto chunk = text.substr(sent, chunkSize);
            sent += chunkSize;
            more = sent < length;
            sendMotd(receiver, !more, chunk.c_str());
        } while (more);
    }

    auto UTIL_SendShowMenu(Edict* receiver, int keys, int time, const std::string& text) -> void {
        auto more = false;
        auto sent = 0;
        do {
            constexpr auto chunkSize = 172;
            const auto chunk = std::string{text.substr(sent, chunkSize)};
            sent += chunkSize;
            more = sent < static_cast<int>(text.length());
            sendShowMenu(receiver, keys, time, more, chunk.c_str());
        } while (more);
    }
}
