/*****************************************************************//**
 * \file   memex.cpp
 * \brief  Testing file. Check CMakeLists if you want to run tests in main().
 * 
 * \author Viktor M.
 * \date   April 2025
 *********************************************************************/
#include <memex/memex.hpp>

#include "pch.hpp"

int main()
{
  ProcessHandle process{};
  process.attach("ac_client.exe");
  MemoryIO mem(process);

  auto moduleResult{ Module::Create(process, "ac_client.exe") };
  if (!moduleResult)
    return 1;

  Module client = moduleResult.value();
  auto localPlayer{ (client.base() + 0x10f4f4).as<void>() };

  RemotePtr<int> ammoAddress{ mem.ResolvePointerChain<int>(localPlayer, {0x374, 0x14, 0x0}) };
  std::int32_t ammoBuffer{ 1000 };

  while (true)
  {
    if (GetAsyncKeyState(VK_INSERT) & 1)
      break;

    mem.Write<std::int32_t>(ammoAddress, ammoBuffer);
  }

  return 0;
}