cmake_minimum_required(VERSION 3.23)

add_library(${PROJECT_NAME} SHARED
        "include/rezombie/main/engine_hooks.h"
        "include/rezombie/main/gamedll_hooks.h"
        "include/rezombie/main/message_hooks.h"
        "include/rezombie/main/main.h"
        "include/rezombie/main/convars.h"
        "include/rezombie/main/util.h"
        "include/rezombie/main/api/main.h"

        "include/rezombie/core/object.h"
        "include/rezombie/core/module.h"
        "include/rezombie/core/base_module.h"
        "include/rezombie/core/modules_store.h"
        "include/rezombie/core/api/amxx_feature.h"
        "include/rezombie/core/api/amxx_base_feature.h"
        "include/rezombie/core/api/amxx_features_store.h"
        "include/rezombie/core/api/amxx_helper.h"

        "include/rezombie/entity/map/environment_data.h"
        "include/rezombie/entity/map/camera_data.h"
        "include/rezombie/entity/map/fog.h"
        "include/rezombie/entity/gamerules/mode.h"
        "include/rezombie/entity/player/player_class.h"
        "include/rezombie/entity/player/player_subclass.h"
        "include/rezombie/entity/player/player_props.h"
        "include/rezombie/entity/player/player_sounds.h"
        "include/rezombie/entity/player/flashlight.h"
        "include/rezombie/entity/player/nightvision.h"
        "include/rezombie/entity/currency/currency.h"
        "include/rezombie/entity/currency/price.h"
        "include/rezombie/entity/weapons/base_weapon.h"
        "include/rezombie/entity/weapons/weapon.h"
        "include/rezombie/entity/weapons/melee.h"
        "include/rezombie/entity/models/model.h"
        "include/rezombie/entity/models/models_pack.h"
        "include/rezombie/entity/items/item.h"
        "include/rezombie/entity/colors/color.h"
        "include/rezombie/entity/wrappers/player_item_wrapper.h"
        "include/rezombie/entity/extras.h"

        "include/rezombie/messages/engine_message.h"
        "include/rezombie/messages/user_message.h"
        "include/rezombie/messages/temporary_entity.h"
        "include/rezombie/messages/hud_message_params.h"
        "include/rezombie/messages/api/engine_message.h"
        "include/rezombie/messages/api/user_message.h"

        "include/rezombie/colors/colors.h"
        "include/rezombie/colors/api/colors.h"

        "include/rezombie/map/map_spawns.h"
        "include/rezombie/map/environment.h"
        "include/rezombie/map/extras.h"
        "include/rezombie/map/modules/map_cameras.h"
        "include/rezombie/map/modules/fog.h"
        "include/rezombie/map/api/map_cameras.h"
        "include/rezombie/map/api/environment.h"
        "include/rezombie/map/api/fog.h"
        "include/rezombie/map/api/extras.h"

        "include/rezombie/gamerules/game_rules.h"
        "include/rezombie/gamerules/consts.h"
        "include/rezombie/gamerules/api/game_rules.h"

        "include/rezombie/modes/modules/modes.h"
        "include/rezombie/modes/api/modes.h"

        "include/rezombie/player/players.h"
        "include/rezombie/player/player.h"
        "include/rezombie/player/flashlight_vars.h"
        "include/rezombie/player/nightvision_vars.h"
        "include/rezombie/player/long_jump_vars.h"
        "include/rezombie/player/join_preview_vars.h"
        "include/rezombie/player/world_preview_vars.h"
        "include/rezombie/player/third_camera_vars.h"
        "include/rezombie/player/map_camera_vars.h"
        "include/rezombie/player/modules/player_class.h"
        "include/rezombie/player/modules/player_subclass.h"
        "include/rezombie/player/modules/player_props.h"
        "include/rezombie/player/modules/player_sounds.h"
        "include/rezombie/player/modules/flashlight.h"
        "include/rezombie/player/modules/nightvision.h"
        "include/rezombie/player/api/player.h"
        "include/rezombie/player/api/player_class.h"
        "include/rezombie/player/api/player_subclass.h"
        "include/rezombie/player/api/player_props.h"
        "include/rezombie/player/api/flashlight.h"
        "include/rezombie/player/api/nightvision.h"
        "include/rezombie/player/api/jumps.h"
        "include/rezombie/player/api/long_jump.h"

        "include/rezombie/currency/modules/currency.h"
        "include/rezombie/currency/modules/price.h"
        "include/rezombie/currency/api/currency.h"
        "include/rezombie/currency/api/price.h"

        "include/rezombie/weapons/melee.h"
        "include/rezombie/weapons/weapons.h"
        "include/rezombie/weapons/modules/weapon.h"
        "include/rezombie/weapons/api/weapon.h"
        "include/rezombie/weapons/api/melee.h"

        "include/rezombie/models/modules/models.h"
        "include/rezombie/models/modules/models_pack.h"
        "include/rezombie/models/api/model.h"
        "include/rezombie/models/api/models_pack.h"

        "include/rezombie/sounds/sounds.h"
        "include/rezombie/sounds/api/sounds.h"

        "include/rezombie/items/modules/item.h"
        "include/rezombie/items/api/items.h"

        "include/rezombie/preview/join_preview.h"
        "include/rezombie/preview/world_preview.h"
        "include/rezombie/preview/api/join_preview.h"
        "include/rezombie/preview/api/world_preview.h"

        "include/rezombie/thirdcamera/third_camera.h"
        "include/rezombie/thirdcamera/api/third_camera.h"

        "include/rezombie/configs/main_config.h"

        "src/main/main.cpp"
        "src/main/engine_hooks.cpp"
        "src/main/gamedll_hooks.cpp"
        "src/main/message_hooks.cpp"
        "src/main/convars.cpp"
        "src/main/util.cpp"
        "src/main/api/main.cpp"

        "src/messages/engine_message.cpp"
        "src/messages/user_message.cpp"
        "src/messages/api/engine_message.cpp"
        "src/messages/api/user_message.cpp"

        "src/colors/colors.cpp"
        "src/colors/api/colors.cpp"

        "src/map/map_spawns.cpp"
        "src/map/map_cameras.cpp"
        "src/map/environment.cpp"
        "src/map/api/map_cameras.cpp"
        "src/map/api/environment.cpp"
        "src/map/api/fog.cpp"
        "src/map/api/extras.cpp"

        "src/gamerules/game_rules.cpp"
        "src/gamerules/player.cpp"
        "src/gamerules/api/game_rules.cpp"

        "src/modes/api/modes.cpp"

        "src/player/players.cpp"
        "src/player/player.cpp"
        "src/player/player_vars.cpp"
        "src/player/player_weapons.cpp"
        "src/player/flashlight.cpp"
        "src/player/nightvision.cpp"
        "src/player/jumps.cpp"
        "src/player/long_jump.cpp"
        "src/player/join_preview.cpp"
        "src/player/third_camera.cpp"
        "src/player/map_camera.cpp"
        "src/player/core/player_entity_vars.cpp"
        "src/player/core/player_base.cpp"
        "src/player/core/player_cstrike.cpp"
        "src/player/api/player.cpp"
        "src/player/api/player_class.cpp"
        "src/player/api/player_subclass.cpp"
        "src/player/api/player_props.cpp"
        "src/player/api/flashlight.cpp"
        "src/player/api/nightvision.cpp"
        "src/player/api/jumps.cpp"
        "src/player/api/long_jump.cpp"

        "src/currency/api/currency.cpp"
        "src/currency/api/price.cpp"

        "src/weapons/weapons.cpp"
        "src/weapons/melee.cpp"
        "src/weapons/api/weapons.cpp"
        "src/weapons/api/melee.cpp"

        "src/models/models.cpp"
        "src/models/api/models.cpp"
        "src/models/api/models_pack.cpp"

        "src/sounds/sounds.cpp"
        "src/sounds/api/sounds.cpp"

        "src/items/api/items.cpp"

        "src/preview/join_preview.cpp"
        "src/preview/world_preview.cpp"
        "src/preview/api/join_preview.cpp"
        "src/preview/api/world_preview.cpp"

        "src/thirdcamera/third_camera.cpp"
        "src/thirdcamera/api/third_camera.cpp"

        "src/wrappers/player_item_wrapper.cpp"

        "src/configs/main_config.cpp"
        )

target_include_directories(${PROJECT_NAME} PRIVATE "include")

set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_GLOBAL_ROOTNAMESPACE "${PROJECT_NAME}"
        LIBRARY_OUTPUT_DIRECTORY "${DEFAULT_OUTPUT_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${DEFAULT_OUTPUT_DIR}"
        )

if (APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX "${BINARY_NAME_SUFFIX_DARWIN}")
elseif (UNIX)
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX "${BINARY_NAME_SUFFIX_LINUX}")
elseif (WIN32)
    get_filename_component(NAME_SUFFIX_WLE "${BINARY_NAME_SUFFIX_WINDOWS}" NAME_WLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            SUFFIX "${BINARY_NAME_SUFFIX_WINDOWS}"
            PDB_NAME "${PROJECT_NAME}${NAME_SUFFIX_WLE}"
            IMPORT_SUFFIX "${NAME_SUFFIX_WLE}.lib"
            )
endif ()

setup_target_properties(${PROJECT_NAME})
setup_target_compile_options(${PROJECT_NAME})

#-------------------------------------------------------------------------------------------
# AMXX configuration
#-------------------------------------------------------------------------------------------

# Use Metamod SDK (ON/OFF)
set(AMXX_USE_METAMOD ON)

# Compatibility with AMXX v1.8.2 (ON/OFF)
set(AMXX_182_COMPATIBILITY OFF)

# Uncomment the functions you want to use in your code and specify the desired function names
set(AMXX_QUERY "Main")                                     # void OnAmxxQuery()
set(AMXX_ATTACH "__OnAmxxAttach")                          # AmxxStatus OnAmxxAttach()
#set(AMXX_DETACH "__OnAmxxDetach")                         # void OnAmxxDetach()
#set(AMXX_CHECK_GAME "__OnAmxxCheckGame")                  # AmxxGameStatus OnAmxxCheckGame(const char* game)
set(AMXX_PLUGINS_LOADED "__OnAmxxPluginsLoaded")           # void OnAmxxPluginsLoaded()
#set(AMXX_PLUGINS_UNLOADED "__OnAmxxPluginsUnloaded")      # void OnAmxxPluginsUnloaded()
#set(AMXX_PLUGINS_UNLOADING "__OnAmxxPluginsUnloading")    # void OnAmxxPluginsUnloading()

#-------------------------------------------------------------------------------------------
# Metamod configuration
#-------------------------------------------------------------------------------------------

# Flags for plug-in to indicate when it can be loaded/unloaded
set(META_PLUGIN_UNLOADABLE "Never")

# Uncomment the functions you want to use in your code and specify the desired function names
#set(META_INIT "__OnMetaInit")       # void OnMetaInit()
#set(META_QUERY "__OnMetaQuery")     # void OnMetaQuery()
#set(META_ATTACH "__OnMetaAttach")   # MetaStatus OnMetaAttach()
set(META_DETACH "__OnMetaDetach")    # void OnMetaDetach()

#-------------------------------------------------------------------------------------------
# Add dependencies to the build
#-------------------------------------------------------------------------------------------

target_link_libraries(${PROJECT_NAME} PRIVATE cssdk)

target_link_libraries(${PROJECT_NAME} PRIVATE vhooks)

target_link_libraries(${PROJECT_NAME} PRIVATE json)

FetchContent_MakeAvailable(Metamod)
target_link_libraries(${PROJECT_NAME} PRIVATE metamod)

FetchContent_MakeAvailable(AMXX)
target_link_libraries(${PROJECT_NAME} PRIVATE amxx)

FetchContent_MakeAvailable(MHooks)
target_link_libraries(${PROJECT_NAME} PRIVATE mhooks)

FetchContent_MakeAvailable(Core)
target_link_libraries(${PROJECT_NAME} PRIVATE core)
