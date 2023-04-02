#include <metamod/engine.h>
#include <common/util.h>
#include <functional>

namespace message {
    using namespace cssdk;
    using namespace metamod::engine;
    using namespace common;

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
