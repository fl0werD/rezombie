#include "vhooks/vhooks.h"
#include "vhooks/entity_virtual_table.h"

namespace vhooks {

    VirtualHook* VirtualHook::s_pVHookRegs = nullptr;

    bool VirtualHook::RegisterHook() {
        EntityVirtualTable table(className_, hookIndex_);
        if (!table.IsValid()) {
            return false;
        }
        originalCallback_ = table.Exchange(callback_, &virtualTableInterface_);
        if (!originalCallback_) {
            return false;
        }
        isHooked_ = true;
        return true;
    }

    bool VirtualHook::UnregisterHook() {
        EntityVirtualTable table(virtualTableInterface_, hookIndex_);
        if (!table.IsValid()) {
            return false;
        }
        if (!table.Exchange(originalCallback_)) {
            return false;
        }
        isHooked_ = false;
        return true;
    }

    bool VirtualHook::RegisterHooks() {
        bool success = true;
        for (VirtualHook* hook = s_pVHookRegs; hook; hook = hook->nextHook_) {
            if (hook->isHooked_) {
                continue;
            }
            success &= hook->RegisterHook();
        }
        return success;
    }

    bool VirtualHook::UnregisterHooks() {
        bool success = true;
        for (VirtualHook* hook = s_pVHookRegs; hook; hook = hook->nextHook_) {
            if (!hook->isHooked_) {
                continue;
            }
            success &= hook->UnregisterHook();
        }
        return success;
    }
}
