#pragma once

#include "rezombie/core/object.h"
#include "amxx/api.h"

namespace rz
{
    using namespace amxx;

    class Currency : public Object {
        int getForward_{};
        int setForward_{};
        int formatForward_{};

      public:
        Currency(
            std::string handle,
            int getForward,
            int setForward,
            int formatForward
        ) : Object(std::move(handle)),
            getForward_(getForward),
            setForward_(setForward),
            formatForward_(formatForward) {}

        auto getGetForward() const { return getForward_; }

        auto executeGet(int player) const -> int {
            return ExecuteForward(getGetForward(), player);
        }

        auto getSetForward() const { return setForward_; }

        auto executeSet(int player, int amount, std::string_view reason = "") const -> int {
            return ExecuteForward(getSetForward(), player, amount, reason.data());
        }

        auto getFormatForward() const { return formatForward_; }

        auto executeFormat(int amount, bool isShort, std::string& text, int textLength) const -> int {
            return ExecuteForward(
                getFormatForward(),
                amount,
                isShort,
                PrepareCharArrayA(text.data(), textLength, true),
                textLength
            );
        }
    };
}
