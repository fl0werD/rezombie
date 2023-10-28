#include "rezombie/main/util.h"
#include <metamod/engine.h>
#include <functional>

namespace rz
{
    using namespace cssdk;
    using namespace metamod::engine;

    auto SendNetMessage(Edict* receiver, SvcMessage message, const std::function<void()>& block) -> void {
        if (receiver) {
            MessageBegin(MessageType::One, toInt(message), nullptr, receiver);
        } else {
            MessageBegin(MessageType::All, toInt(message));
        }
        if (block) {
            block();
        }
        MessageEnd();
    }
}
