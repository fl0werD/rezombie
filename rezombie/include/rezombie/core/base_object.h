#pragma once

#include <string>

namespace rz
{
    class BaseObject
    {
      private:
        std::string handle_;
        std::string name_;

      public:
        explicit BaseObject(std::string handle, std::string name = "") :
          handle_(std::move(handle)),
          name_(std::move(name))
        {
        }

        auto getHandle() const -> const std::string&
        {
            return handle_;
        }

        auto getName() const -> const std::string&
        {
            return name_;
        }

        auto setName(std::string name) -> void
        {
            name_ = std::move(name);
        }
    };
}
