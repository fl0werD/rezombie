#pragma once

#include "vhooks/hook_index.h"
#include "vhooks/memory_unlocker.h"
#include <metamod/engine.h>
#include <metamod/utils.h>

namespace vhooks
{
    using namespace metamod;

    class EntityVirtualTable {
        cssdk::Edict* entity_{};
        int** virtualTable_{};
        HookIndex hookIndex_{};
        bool success_{};

      public:
        EntityVirtualTable(const std::string& className, HookIndex hookIndex) {
            if (hookIndex < HookIndex::indexBegin || hookIndex > HookIndex::indexEnd) {
                return;
            }
            virtualTable_ = nullptr;
            hookIndex_ = hookIndex;
            entity_ = engine::CreateEntity();
            if (!entity_) {
                return;
            }
            if (!utils::CallGameEntity(className.c_str(), &entity_->vars)) {
                return;
            }
            const int base = 0x0;
            void** pvtable = *((void***) ((char*) entity_->private_data + base));
            if (!pvtable) {
                return;
            }
            success_ = true;
            virtualTable_ = (int**) pvtable;
        }

        EntityVirtualTable(int** virtualTable, HookIndex hookIndex) {
            if (!virtualTable) {
                return;
            }
            if (hookIndex < HookIndex::indexBegin || hookIndex > HookIndex::indexEnd) {
                return;
            }
            success_ = true;
            hookIndex_ = hookIndex;
            virtualTable_ = (int**) virtualTable;
        }

        ~EntityVirtualTable() {
            if (entity_) {
                engine::RemoveEntity(entity_);
            }
            entity_ = nullptr;
            virtualTable_ = nullptr;
        }

        void* Exchange(void* function, int*** virtualTable = nullptr) {
            const auto hookIndex = static_cast<int>(hookIndex_);
            void* pvAddressReturn = (void*) virtualTable_[hookIndex];
            MemoryUnlocker lock(&virtualTable_[hookIndex], sizeof(void*));
            if (!lock.IsValid()) {
                return nullptr;
            }
            virtualTable_[hookIndex] = (int*) function;
            if (virtualTable) {
                *virtualTable = virtualTable_;
            }
            return pvAddressReturn;
        }

        bool IsValid() const {
            return virtualTable_ != nullptr;
        }
    };
}
