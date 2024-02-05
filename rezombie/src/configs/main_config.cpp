#include "rezombie/configs/main_config.h"
#include "rezombie/main/util.h"
#include <metamod/utils.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <core/strings/path.h>

namespace rz
{
    using namespace core;
    using namespace nlohmann;

    template<typename V>
    auto read(const json& data, const std::string& key, V& var) -> bool {
        const auto it = data.find(key);
        if (it == data.end()) {
            return false;
        }
        it->get_to(var);
        return true;
    }

    auto MainConfig::load() -> void {
        const std::filesystem::path path = str::BuildPathName("rezombie/configs/main.json");
        std::ifstream file(path);
        if (!file) {
            return;
        }
        const auto data = json::parse(file);
        metamod::utils::LogConsole("%s", data.dump(4).c_str());

        read(data, "server_browser_info", serverBrowserInfo_);
        read(data, "sky_name", skyName_);
        read(data, "warmup_time", warmupTime_);
        read(data, "prepare_time", prepareTime_);
        read(data, "round_time", roundTime_);
        read(data, "useless_entities", uselessEntities_);
    }
}
