#pragma once

#include "rezombie/amxmodx/utils.h"
#include "rezombie/core/amxx_base_feature.h"
#include <amxx/api.h>

namespace rz
{
    enum DummyForward
    {
        Zero
    };

    template <class E = DummyForward, E MAX_FORWARDS = DummyForward::Zero>
    class AmxxFeature : public AmxxBaseFeature
    {
      private:
        std::array<int, static_cast<size_t>(MAX_FORWARDS)> forwards_;

      public:
        explicit AmxxFeature(std::string handle) : AmxxBaseFeature(std::move(handle))
        {
            forwards_.fill(FORWARD_INVALID);
        }

        auto RegisterForwards() -> void override {
            // To be implemented
        }

        auto RegisterNatives() -> void override {
            // To be implemented
        }

        auto getForward(E forward) const -> int
        {
            return forwards_[toInt(forward)];
        }

        auto setForward(E forward, int forwardIndex) -> void
        {
            forwards_[toInt(forward)] = forwardIndex;
        }
    };
}
