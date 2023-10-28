#pragma once

#include <rezombie/core/api/amxx_base_feature.h>
#include "rezombie/main/util.h"
#include <amxx/api.h>
#include <array>

namespace rz
{
    using namespace amxx;

    constexpr auto FORWARD_INVALID = -1;

    enum class ForwardReturn {
        Continue,
        Supercede,
        Break,
    };

    enum DummyForward {
        Dummy,
    };

    template<class E = size_t, E MAX_FORWARDS = DummyForward::Dummy>
    class AmxxFeature : public AmxxBaseFeature {
        std::array<int, static_cast<size_t>(MAX_FORWARDS)> forwards_;

      public:
        explicit AmxxFeature(std::string handle) : AmxxBaseFeature(std::move(handle)) {
            forwards_.fill(FORWARD_INVALID);
        }

        auto registerForwards() -> void override {
            // To be implemented
        }

        auto registerNatives() const -> void override {
            // To be implemented
        }

        auto setForward(E forward, int forwardIndex) -> void {
            forwards_[toInt(forward)] = forwardIndex;
        }

        template<typename... TArgs>
        auto registerForward(E forward, const char* name, TArgs&& ... args) -> void {
            forwards_[toInt(forward)] = RegisterForward(name, std::forward<TArgs>(args)..., ForwardParam::Done);
        }

        template<typename... TArgs>
        auto executeForward(E forward, TArgs&& ... args) const -> ForwardReturn {
            if (forwards_[toInt(forward)] == FORWARD_INVALID) {
                // throw
            }
            return static_cast<ForwardReturn>(ExecuteForward(forwards_[toInt(forward)], std::forward<TArgs>(args)...));
        }
    };
}
