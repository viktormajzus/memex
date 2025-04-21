#pragma once
#include <Windows.h>

namespace memex
{
  /**
   * @brief Enum representing different access masks for process memory operations.
   *
   */
  enum class AccessMask : DWORD
  {
    Read = PROCESS_VM_READ,
    Write = PROCESS_VM_WRITE,
    Execute = PROCESS_CREATE_THREAD,
    Full = PROCESS_ALL_ACCESS
  };
}