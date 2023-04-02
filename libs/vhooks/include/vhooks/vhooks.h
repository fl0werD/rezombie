#pragma once

#include "vhooks/hook_index.h"
#include <type_traits>
#include <string>

namespace vhooks {

    class VirtualHook {
      private:
        std::string className_{};
        HookIndex hookIndex_{};
        void* callback_{};
        void* originalCallback_{};
        bool isHooked_{};
        int** virtualTableInterface_{};
        VirtualHook* nextHook_{};

      protected:
        bool RegisterHook();

        bool UnregisterHook();

      public:
        static VirtualHook* s_pVHookRegs;

        static bool RegisterHooks();

        static bool UnregisterHooks();

        VirtualHook() = default;

        template<typename F = void (*)(), typename T>
        VirtualHook(const std::string_view className, HookIndex hookIndex, F(T::*callback)) {
            className_ = className;
            hookIndex_ = hookIndex;
            callback_ = (void*) (size_t&) callback;
            nextHook_ = s_pVHookRegs;
            s_pVHookRegs = this;
        }

        template<typename F = void (*)(), typename T>
        auto CreateHook(const std::string_view className, HookIndex hookIndex, F(T::*callback)) -> void {
            className_ = className;
            hookIndex_ = hookIndex;
            callback_ = (void*) (size_t&) callback;
            if (!isHooked_) {
                RegisterHook();
            }
        }

        auto DestroyHook() -> void {
            if (isHooked_) {
                UnregisterHook();
            }
        }

        template<typename R = void, typename C, typename... A>
        typename std::enable_if_t<std::is_same<R, void>::value == false, R> Call(C* context, A... args) {
            using originalFunction = R(__thiscall*)(C*, A...);
            originalFunction function = (originalFunction&) originalCallback_;
            if (!function) {
                return R(0);
            }
            return function(context, args...);
        }

        template<typename R = void, typename C, typename... A>
        typename std::enable_if_t<std::is_same<R, void>::value == true, R> Call(C* context, A... args) {
            using originalFunction = R(__thiscall*)(C*, A...);
            originalFunction function = (originalFunction&) originalCallback_;
            if (!function) {
                return;
            }
            function(context, args...);
        }
    };
}
