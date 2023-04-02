#pragma once

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#else
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <link.h>
#include <unistd.h>
#endif

namespace vhooks {
    class MemoryUnlocker {
      private:
        void* ptr_{};
        size_t size_{};
        bool error{};

#if defined(_WIN32)
        HANDLE process_{};
        DWORD oldProtection_{};
        DWORD newProtection_{};
#endif

      public:
        MemoryUnlocker(void* ptr, size_t size) : ptr_{ptr}, size_{size} {
#if defined(_WIN32)
            newProtection_ = PAGE_READWRITE;
            oldProtection_ = 0;
            if (!process_) {
                process_ = GetCurrentProcess();
            }
            FlushInstructionCache(process_, ptr_, size_);
            error = !VirtualProtect(ptr_, size_, newProtection_, &oldProtection_);
#else
#define Align(addr) ((void*)(((uint32_t)addr) & ~(sysconf(_SC_PAGESIZE) - 1)))
            uint32_t alignedSize = sysconf(_SC_PAGESIZE);
            void* alignedAddress = Align(ptr_);
            if (Align(ptr_ + size - 1) != alignedAddress) {
                alignedSize *= 2;
            }
#undef Align
            ptr_ = alignedAddress;
            size_ = alignedSize;
            error = !!mprotect(ptr_, size_, (PROT_EXEC | PROT_READ | PROT_WRITE));
#endif
        }

        ~MemoryUnlocker() {
#if defined(_WIN32)
            error = !VirtualProtect(ptr_, size_, oldProtection_, &newProtection_);
#else
            error = !!mprotect(ptr_, size_, PROT_EXEC | PROT_READ);
#endif
        }

        bool IsValid() const {
            return !error;
        }
    };
}
