cmake_minimum_required(VERSION 3.23)

add_library(${PROJECT_NAME} SHARED
        "include/rezombie/core/base_object.h"
        "include/rezombie/core/module.h"
        "include/rezombie/core/amxx_feature.h"
        "include/rezombie/core/amxx_base_feature.h"
        "include/rezombie/core/amxx_features_store.h"
        "include/rezombie/core/base_module.h"
        "include/rezombie/core/modules_store.h"
        "include/rezombie/modules/game_mode.h"
        "include/rezombie/modules/player_class.h"
        "include/rezombie/modules/player_subclass.h"
        "include/rezombie/modules/player_props.h"
        "include/rezombie/modules/player_model.h"
        "include/rezombie/modules/player_sound.h"
        "include/rezombie/modules/player_nightvision.h"
        "include/rezombie/modules/weapon.h"
        "include/rezombie/main.h"
        "include/rezombie/convars.h"
        "include/rezombie/gamerules/game_rules.h"
        "include/rezombie/gamerules/game_mode.h"
        "include/rezombie/gamerules/consts.h"
        "include/rezombie/util.h"
        "include/rezombie/player/players.h"
        "include/rezombie/player/player.h"
        "include/rezombie/player/player_class.h"
        "include/rezombie/player/player_subclass.h"
        "include/rezombie/player/player_props.h"
        "include/rezombie/player/player_model.h"
        "include/rezombie/player/player_sound.h"
        "include/rezombie/player/player_nightvision.h"
        "include/rezombie/player/player_item.h"
        "include/rezombie/player/long_jump.h"
        "include/rezombie/player/player_preview.h"
        "include/rezombie/player/weather.h"
        "include/rezombie/core/base_weapon.h"
        "include/rezombie/weapons/weapon.h"
        "include/rezombie/weapons/melee.h"
        "include/rezombie/weapons/weapons.h"
        "include/rezombie/weapons/weapon_box.h"
        "../libs/messages/include/messages/user_message.h"
        "../libs/messages/include/messages/engine_message.h"
        "../libs/messages/include/messages/hud_message_params.h"
        "../libs/messages/include/messages/temporary_entity.h"
        "include/rezombie/amxmodx/game_rules.h"
        "include/rezombie/amxmodx/player.h"
        "include/rezombie/amxmodx/player_class.h"
        "include/rezombie/amxmodx/player_subclass.h"
        "include/rezombie/amxmodx/player_props.h"
        "include/rezombie/amxmodx/player_model.h"
        "include/rezombie/amxmodx/weapon.h"
        "include/rezombie/amxmodx/melee.h"
        "include/rezombie/amxmodx/common.h"
        "include/rezombie/amxmodx/utils.h"
        "src/main.cpp"
        "src/convars.cpp"
        "src/gamerules/game_rules.cpp"
        "src/gamerules/player.cpp"
        "src/util.cpp"
        "src/player/player.cpp"
        "src/player/player_entity_vars.cpp"
        "src/player/player_vars.cpp"
        "src/player/player_base.cpp"
        "src/player/player_cstrike.cpp"
        "src/player/player_weapons.cpp"
        "src/player/extra_jumps.cpp"
        "src/player/long_jump.cpp"
        "src/player/player_preview.cpp"
        "src/player/players.cpp"
        "src/player/weather.cpp"
        "src/weapons/weapons.cpp"
        "src/weapons/melee.cpp"
        "src/weapons/weapon_box.cpp"
        "src/amxmodx/game_rules.cpp"
        "src/amxmodx/player.cpp"
        "src/amxmodx/player_class.cpp"
        "src/amxmodx/player_subclass.cpp"
        "src/amxmodx/player_props.cpp"
        "src/amxmodx/player_model.cpp"
        "src/amxmodx/weapon.cpp"
        "src/amxmodx/melee.cpp"
        "src/amxmodx/common.cpp"
        "../libs/messages/src/engine_message.cpp"
        "../libs/messages/src/user_message.cpp"
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
set(AMXX_QUERY "Main")                                 # void OnAmxxQuery()
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

FetchContent_MakeAvailable(CSSDK)
target_link_libraries(${PROJECT_NAME} PRIVATE cssdk)

FetchContent_MakeAvailable(Metamod)
target_link_libraries(${PROJECT_NAME} PRIVATE metamod)

FetchContent_MakeAvailable(AMXX)
target_link_libraries(${PROJECT_NAME} PRIVATE amxx)

FetchContent_MakeAvailable(MHooks)
target_link_libraries(${PROJECT_NAME} PRIVATE mhooks)

FetchContent_MakeAvailable(Core)
target_link_libraries(${PROJECT_NAME} PRIVATE core)

target_link_libraries(${PROJECT_NAME} PRIVATE vhooks)

target_link_libraries(${PROJECT_NAME} PRIVATE messages)

target_link_libraries(${PROJECT_NAME} PRIVATE common)
