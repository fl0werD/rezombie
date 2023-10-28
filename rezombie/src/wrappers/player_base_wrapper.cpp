#include "rezombie/entity/wrappers/player_base_wrapper.h"
#include "rezombie/gamerules/game_rules.h"
#include "rezombie/player/players.h"
#include "rezombie/player/modules/player_props.h"

namespace rz
{
    auto PlayerBaseWrapper::PlayerDeathThink() -> void {
        vars->dead_flag = DeathState::Alive;
        return;
        if (joining_state != JoinState::Joined) {
            return;
        }
        if (vars->flags & FL_ON_GROUND) {
            const float forward = vars->velocity.Length() - 20;
            if (forward <= 0) {
                vars->velocity = VECTOR_ZERO;
            } else {
                vars->velocity = forward * vars->velocity.Normalize();
            }
        }

        if (HasWeapons()) {
            GetCsPlayer()->RemoveAllItems(true);
            //PackDeadPlayerItems();
        }

        vars->angles.x = 0;

        if (vars->model_index && !sequence_finished && vars->dead_flag == DeathState::Dying) {
            //StudioFrameAdvance();
            return;
        }

        if (vars->move_type != MoveTypeEntity::None && (vars->flags & FL_ON_GROUND)) {
            vars->move_type = MoveTypeEntity::None;
        }

        if (vars->dead_flag == DeathState::Dying) {
            dead_time = g_global_vars->time;
            vars->dead_flag = DeathState::Dead;
        }

        //StopAnimation();
        vars->effects |= EF_NO_INTERPOLATE;
/*
        if (vars->dead_flag != DeathState::Respawnable) {
            if (HasTimePassedSinceDeath(CGameRules::GetDyingTime()) && !(physics_flags & PFLAG_OBSERVER)) {
                StartDeathCam();
            }
        }
*/
        if (vars->dead_flag == DeathState::Dead && team != TeamName::Unassigned && team != TeamName::Spectator) {
            if (gameRules->PlayerCanRespawn(this)) {
                //if (forcerespawn.value <= 0 || (m_iTeam != CT && m_iTeam != TERRORIST)) {
                vars->dead_flag = DeathState::Respawnable;
                //}
            }
            vars->next_think = g_global_vars->time + 0.1f;
            return;
        }

        if (vars->dead_flag == DeathState::Respawnable) {
            /*if (GetObserverMode() != OBS_NONE && (m_iTeam == UNASSIGNED || m_iTeam == SPECTATOR)) {
                return;
            }
            if (m_iMenu == Menu_ChooseAppearance || m_iJoiningState == SHOWTEAMSELECT) {
                return;
            }*/
            Spawn();
            vars->button = 0;
            vars->next_think = -1;
        }
    }

    auto PlayerBaseWrapper::HasWeapons() -> bool {
        for (const auto& item: player_items) {
            if (item) {
                return true;
            }
        }
        return false;
    }
}
