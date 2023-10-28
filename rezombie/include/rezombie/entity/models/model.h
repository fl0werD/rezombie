#pragma once

#include "rezombie/core/object.h"
#include "rezombie/main/util.h"
#include "metamod/engine.h"
#include <core/strings.h>
#include <algorithm>
#include <utility>

namespace rz
{
    using namespace core;

    class Model : public Object {
        std::string path_{};
        std::string texturesPath_{};
        int body_{};
        int skin_{};
        int precacheId_{};

        auto setTexturesPath(const std::string& path) -> void {
            auto texturesPath = path;
            str::ReplaceAll(texturesPath, ".mdl", "T.mdl");
            if (!isFileExists(texturesPath)) {
                return;
            }
            texturesPath_ = texturesPath;
        }

      public:
        explicit Model(
            std::string handle,
            std::string path,
            int body,
            int skin
        ) : Object(std::move(handle)),
            path_(std::move(path)),
            body_(body),
            skin_(skin) {
            setTexturesPath(getPath());
        }

        auto getPath() const -> const std::string& { return path_; }

        auto getTexturesPath() const -> const std::string& { return texturesPath_; }

        auto getBody() const -> int { return body_; }

        auto setBody(int body) -> void { body_ = body; }

        auto getSkin() const -> int { return skin_; }

        auto setSkin(int skin) -> void { skin_ = skin; }

        auto getPrecacheId() const -> int { return precacheId_; }

        auto setPrecacheId(int precacheId) -> void { precacheId_ = precacheId; }
    };
}
