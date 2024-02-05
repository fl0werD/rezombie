#pragma once

#include "rezombie/main/util.h"
#include <string>
#include <unordered_map>

namespace rz
{
    enum class ExtraType {
        Int,
        Float,
        String,
    };

    class Extras {
        std::unordered_map<std::string, int> ints_ = {};
        std::unordered_map<std::string, float> floats_ = {};
        std::unordered_map<std::string, std::string> strings_ = {};

        public:
            Extras() = default;

            auto reset() -> void {
                ints_.clear();
                floats_.clear();
                strings_.clear();
            }

            auto isKeyExists(const ExtraType type, const std::string& key) const -> bool {
                switch (type) {
                    case ExtraType::Int: {
                        if (getMapValue(ints_, key)) {
                            return true;
                        }
                        break;
                    }
                    case ExtraType::Float: {
                        if (getMapValue(floats_, key)) {
                            return true;
                        }
                        break;
                    }
                    case ExtraType::String: {
                        if (getMapValue(strings_, key)) {
                            return true;
                        }
                        break;
                    }
                }
                return false;
            }

            auto getInts() -> std::unordered_map<std::string, int>& {
                return ints_;
            }

            auto getFloats() -> std::unordered_map<std::string, float>& {
                return floats_;
            }

            auto getStrings() -> std::unordered_map<std::string, std::string>& {
                return strings_;
            }
    };
}
