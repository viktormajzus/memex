#include <memex/utility.hpp>
#include <iostream>

int main()
{
  std::string_view proc{ "Notepad.exe" };
  auto procId{ utility::FindPidByName(proc) };
  if (!procId)
  {
    std::cout << memex::parseError(procId.error());
  } else
  {
    std::cout << "Process ID: " << procId.value();
  }
}