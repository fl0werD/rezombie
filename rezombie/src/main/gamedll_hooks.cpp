#include "rezombie/gamerules/game_rules.h"
#include "rezombie/weapons/modules/weapon.h"
#include "rezombie/player/players.h"
#include "rezombie/player/world_preview.h"
#include "rezombie/player/modules/nightvision.h"
#include "rezombie/map/environment.h"
#include "rezombie/map/map_spawns.h"
#include "rezombie/map/modules/map_cameras.h"
#include "rezombie/modelpreview/model_preview.h"
#include "rezombie/thirdcamera/third_camera.h"
#include "rezombie/main/gamedll_hooks.h"
#include "rezombie/messages/user_message.h"
#include <mhooks/metamod.h>
#include <metamod/utils.h>

namespace rz
{
    using namespace metamod;
    using namespace mhooks;

    auto RegisterGameDllHooks() -> void {
        MHookGameDllDispatchSpawn(DELEGATE_ARG<Spawn_Pre>, false);
        MHookGameDllClientPutInServer(DELEGATE_ARG<ClientPutInServer_Post>, true);
        //MHookGameDllClientKill(DELEGATE_ARG<ClientKill_Pre>, false);
        MHookGameDllServerActivate(DELEGATE_ARG<ServerActivate_Post>, true);
        MHookGameDllServerDeactivate(DELEGATE_ARG<ServerDeactivate_Pre>, false);
        MHookGameDllAddToFullPack(DELEGATE_ARG<AddToFullPack_Pre>, false);
        MHookGameDllAddToFullPack(DELEGATE_ARG<AddToFullPack_Post>, true);
        MHookGameDllUpdateClientData(DELEGATE_ARG<UpdateClientData_Post>, true);
        MHookGameDllGetWeaponData(DELEGATE_ARG<GetWeaponData_Post>, true);
    }

/*
    const std::unordered_map<std::string> USELESS_ENTITIES = {
        {"armoury_entity", 0},
        {"hostage_entity", 0},
    };
*/
    auto Spawn_Pre(const GameDllDispatchSpawnMChain& chain, Edict* entity) -> int {
        if (entity->vars.class_name == "armoury_entity") {
            RETURN_META_VALUE(Result::Supercede, -1);
        }
        if (entity->vars.class_name == "weaponbox") {
            RETURN_META_VALUE(Result::Supercede, -1);
        }
        if (entity->vars.class_name == "hostage_entity") {
            RETURN_META_VALUE(Result::Supercede, -1);
        }
        return chain.CallNext(entity);
    }

    auto ClientPutInServer_Post(
        const GameDllClientPutInServerMChain& chain,
        Edict* client
    ) -> void {
        auto& player = Players[client];
    }

    auto ClientKill_Pre(
        const GameDllClientKillMChain& chain,
        Edict* client
    ) -> void {
        RETURN_META(Result::Supercede);
    }

    auto ServerActivate_Post(
        const GameDllServerActivateMChain& chain,
        Edict* edictList,
        int edictCount,
        int clientMax
    ) -> void {
        IsServerActive = true;
        Environment.precache();
        gameRules->precache();
        initUserMessagesIds();
        VirtualHook::RegisterHooks();
        Modules.precache();
        MapSpawns.collect();
        MapCameras.collect();
        MapCameras.createEntity();
        ModelPreview.createEntities();
        ThirdCamera.createEntity();
    }

    auto ServerDeactivate_Pre(const GameDllServerDeactivateMChain& chain) -> void {
        if (!IsServerActive) {
            return;
        }
        IsServerActive = false;
        VirtualHook::UnregisterHooks();
        Modules.clear();
    }

    auto AddToFullPack_Pre(
        const GameDllAddToFullPackMChain& chain,
        EntityState* state,
        int entityId,
        Edict* entity,
        Edict* hostEntity,
        int hostFlags,
        qboolean isPlayer,
        unsigned char* set
    ) -> qboolean {
        auto& host = Players[hostEntity];
        auto& thirdCamera = host.getThirdCameraVars();
        if (!thirdCamera.isEnabled()) {
            if (entity == ThirdCamera.getCamera()) {
                RETURN_META_VALUE(Result::Supercede, false);
            }
        }
        auto& camera = host.getMapCameraVars();
        if (!camera.isEnabled()) {
            if (entity == MapCameras.getCamera()) {
                RETURN_META_VALUE(Result::Supercede, false);
            }
        }
        auto& preview = host.getPreviewVars();
        if (!preview.isEnabled()) {
            if (entity == ModelPreview[PreviewType::ParentModel] ||
                entity == ModelPreview[PreviewType::AttachModel] ||
                entity == ModelPreview[PreviewType::ExtraAttachModel]
                ) {
                RETURN_META_VALUE(Result::Supercede, false);
            }
        }
        return chain.CallNext(state, entityId, entity, hostEntity, hostFlags, isPlayer, set);
    }

    auto AddToFullPack_Post(
        const GameDllAddToFullPackMChain& chain,
        EntityState* state,
        int entityId,
        Edict* entity,
        Edict* hostEntity,
        int hostFlags,
        qboolean isPlayer,
        unsigned char* set
    ) -> qboolean {
        //metamod::utils::LogConsole("host %d ent %d %s", IndexOfEdict(hostEntity), IndexOfEdict(entity), entity->vars.class_name.CStr());
        auto& host = Players[hostEntity];
        if (hostEntity == entity) {
            if (host.getFlashlight().isEnabled()) {
                state->effects |= EF_DIM_LIGHT;
            }
            if (host.getNightVision().isEnabled()) {
                state->effects |= EF_BRIGHT_LIGHT;
            }
        }
        auto& thirdCamera = host.getThirdCameraVars();
        if (thirdCamera.isEnabled()) {
            if (entity == ThirdCamera.getCamera()) {
                state->origin = thirdCamera.getOrigin();
                state->angles = thirdCamera.getAngles();
            }
        }
        auto& mapCamera = host.getMapCameraVars();
        if (mapCamera.isEnabled()) {
            if (entity == MapCameras.getCamera()) {
                const auto& cameraRef = MapCameras[mapCamera.getCamera()];
                if (cameraRef) {
                    const auto& camera = cameraRef->get();
                    state->origin = camera.getOrigin();
                    state->angles = camera.getAngles();
                }
            }
        }
        auto& preview = host.getPreviewVars();
        if (preview.isEnabled()) {
            if (entity == ModelPreview[PreviewType::ParentModel]) {
                state->origin = preview.getOrigin();
                state->angles = preview.getAngles();
                state->velocity = host.getVelocity();
                setModel(state, preview.getModel(PreviewType::ParentModel));
            } else if (entity == ModelPreview[PreviewType::AttachModel]) {
                setModel(state, preview.getModel(PreviewType::AttachModel));
            } else if (entity == ModelPreview[PreviewType::ExtraAttachModel]) {
                setModel(state, preview.getModel(PreviewType::ExtraAttachModel));
            }
        }
        auto& worldPreview = host.getWorldPreviewVars();
        if (worldPreview.isEnabled()) {
            if (entity == WorldPreview.getEntity()) {
                state->origin = worldPreview.getOrigin();
                state->angles = worldPreview.getAngles();
                setModel(state, worldPreview.getModel());
            }
        }
        if (state->model_index == -1) {
            state->effects |= EF_NO_DRAW;
        }
        return chain.CallNext(state, entityId, entity, hostEntity, hostFlags, isPlayer, set);
    }

    auto UpdateClientData_Post(
        const GameDllUpdateClientDataMChain& chain,
        const Edict* client,
        qboolean sendWeapons,
        ClientData* data
    ) -> void {
        const auto& player = Players[client];
        const auto activeItem = player.getActiveItem();
        if (activeItem == nullptr) {
            return;
        }
        const auto weaponRef = Weapons[activeItem->vars->impulse];
        if (!weaponRef) {
            return;
        }
        data->next_attack = 1.0f;
    }

    auto GetWeaponData_Post(const GameDllGetWeaponDataMChain& chain, Edict* client, WeaponData* data) -> qboolean {
        const auto& player = Players[client];
        const auto activeItem = player.getActiveItem();
        if (activeItem == nullptr) {
            return true;
        }
        const auto weaponRef = Weapons[activeItem->vars->impulse];
        if (!weaponRef) {
            return true;
        }
        const auto itemInfo = activeItem->GetCsPlayerItem()->item_info;
        // data->id = toInt(itemInfo.id);
        data->clip = itemInfo.max_clip;
        return true;
    }
}
