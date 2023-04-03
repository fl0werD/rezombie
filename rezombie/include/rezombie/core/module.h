#pragma once

#include "base_module.h"
#include "modules_store.h"
#include <core/strings/mutation.h>
#include <core/strings/path.h>
#include <metamod/engine.h>
#include <algorithm>
#include <functional>
#include <optional>
#include <utility>

namespace rz
{
    using namespace metamod::engine;

    template <class T>
    class Module : public BaseModule
    {
      private:
        std::vector<T*> data_;
        int offset_ = -1;

      public:
        explicit Module(std::string handle) : BaseModule(handle)
        {
            // check module already exists
            int index = ModulesStore::add(this);
            setOffset((index + 1) * 1000);
        }

        auto add(T* item) -> int
        {
            data_.push_back(item);
            return (data_.size() - 1) + getOffset();
        }

        auto getOffset() const -> int {
            return offset_;
        }

        auto setOffset(int offset) -> void {
            offset_ = offset;
        }

        template <typename F>
        auto find(F&& block) -> int
        {
            const auto it = std::find_if(data_.cbegin(), data_.cend(), std::forward<F>(block));
            if (it == data_.end()) {
                return 0;
            }
            return std::distance(data_.cbegin(), it) + getOffset();
        }

        auto findHandle(const std::string& handle) -> int
        {
            constexpr auto byHandle = +[](const std::string& handle)
            {
                return [&handle](const T* item)
                {
                    return item->getHandle() == handle;
                };
            };
            return find(byHandle(handle));
        }

        auto clear() -> void override
        {
            data_.clear();
        }

        auto precache() -> void override
        {
            // To be implemented
        }

        auto count() const
        {
            return data_.size();
        }

        auto begin() const
        {
            return getOffset();
        }

        auto end() const
        {
            return getOffset() + data_.size();
        }

        template <typename F>
        auto forEach(F&& action) -> void
        {
            std::for_each(data_.begin(), data_.end(), std::forward<F>(action));
        }

        auto forEachIndexed(const std::function<void(int, T&)>& action) -> void
        {
            const auto count = static_cast<int>(data_.size());
            for (auto index = 0; index < count; ++index) {
                action(index + getOffset(), *data_[index]);
            }
        }

        auto precacheModel(const std::string& modelPath) const -> int
        {
            if (modelPath.empty()) {
                return -1;
            }
            return PrecacheModel(modelPath.c_str());
        }

        auto precacheGeneric(const std::string& path) const -> int
        {
            if (path.empty()) {
                return -1;
            }
            return PrecacheGeneric(path.c_str());
        }

        auto operator[](int indexOffset) -> std::optional<std::reference_wrapper<T>>
        {
            if (getOffset() == -1) {
                return std::nullopt;
            }
            const auto index = indexOffset - getOffset();
            if (index < 0 || index >= static_cast<int>(data_.size())) {
                return std::nullopt;
            }
            return {*data_[index]};
        }

        auto operator[](const std::string& handle) -> int
        {
            return findHandle(handle);
        }
    };
}
