#pragma once

#include <string>
#include <utility>

namespace rz
{
    class Object {
        std::string handle_;
        std::string name_;

      public:
        explicit Object(
            std::string handle,
            std::string name = ""
        ) : handle_(std::move(handle)),
            name_(std::move(name)) {}

        auto getHandle() const -> const std::string& { return handle_; }

        auto setHandle(std::string handle) -> void { handle_ = std::move(handle); }

        auto getName() const -> const std::string& { return name_; }

        auto setName(std::string name) -> void { name_ = std::move(name); }
    };
}
