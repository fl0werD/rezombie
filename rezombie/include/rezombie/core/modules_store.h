#pragma once

#include <vector>
#include <algorithm>

namespace rz
{
    class ModulesStore {
        std::vector<BaseModule*> modules_{};

      public:
        auto add(BaseModule* baseModule) -> int {
            modules_.emplace_back(baseModule);
            return static_cast<int>(modules_.size() - 1);
        }

        auto clear() -> void {
            forEach([](const auto& module) {
                module->clear();
            });
        }

        auto precache() -> void {
            forEach([](const auto& module) {
                module->precache();
            });
        }

        template<typename F>
        auto forEach(F&& action) -> void {
            std::for_each(modules_.begin(), modules_.end(), std::forward<F>(action));
        }
    };

    inline ModulesStore Modules;
}
