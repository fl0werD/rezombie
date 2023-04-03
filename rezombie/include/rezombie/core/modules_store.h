#pragma once

#include <string>
#include <vector>
#include <algorithm>

namespace rz
{
    class ModulesStore {
      private:
        std::vector<BaseModule*> modules_;

        ModulesStore() = default;

      public:
        ModulesStore(const ModulesStore&) = delete;
        ModulesStore(ModulesStore&&) = delete;
        auto operator=(const ModulesStore&) -> void = delete;
        auto operator=(ModulesStore&&) -> void = delete;

        static auto& instance() {
            static ModulesStore instance;
            return instance;
        }

        static auto add(BaseModule* module) -> int {
            instance().modules_.push_back(module);
            return static_cast<int>(instance().modules_.size() - 1);
        }

        static auto clear() -> void {
            forEach([](const auto& module) {
                module->clear();
            });
        }

        static auto precache() -> void {
            forEach([](const auto& module) {
                module->precache();
            });
        }

        template<typename F>
        static auto forEach(F&& action) -> void {
            std::for_each(instance().modules_.begin(), instance().modules_.end(), std::forward<F>(action));
        }
    };
}
