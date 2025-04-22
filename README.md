# memex

**memex** is a modern, open-source, C++23 memory management library. Designed with safety and abstraction in mind, this library procides core functionality for external process memory manipulation.

---

## Current Features
- Safe handle management
- Wraps pointers
- Modern C++ features
- Modern error handling, via std::expected
- MemoryIO class - responsible for memory operations (RPM/WPM)
- Module class - responsible for storing module information
- x86 and x64 support
- Memory Region Mapping

---

## Planned Features
- ~Module enumeration~ Done
- Pattern scanning
- Page protection modification
- Buffered writing with rollback - for safe patching
- Remote allocation and thread injection
- Patching (currently, only fill is implemented)
- Function Hooking
- Handle Hijacking

---

## Build instructions
Either build manually with CMake, or run build.bat %preset%

```bash
git clone https://github.com/yourname/memex.git
cd memex
```

### Presets:
- x64-debug
- x64-release
- x86-debug
- x86-release

---

## Documentation
Full documentation is available on: https://viktormajzus.github.io/memex/

---

## Contribution
If you want to contribute, feel free to do so. But please follow the guideline in **CONTRIBUTING.md**

---

## Example Usage

```c++
ProcessHandle process;
process.attach("target.exe");
MemoryIO mem(process);

auto mod = Module::Create(process, "target_module.dll").value();
RemotePtr<void> base = mod.base() + 0x10f4f4;

RemotePtr<int> address = mem.ResolvePointerChain<int>(base, {0x374, 0x14, 0});
mem.Write(address, 1337);
```

---

## Disclaimer

This project is intended for educational and research purposes only.

See [DISCLAIMER.md](./DISCLAIMER.md) for full legal details.
