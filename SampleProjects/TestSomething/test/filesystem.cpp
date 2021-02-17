#include <ArduinoUnitTests.h>
#include <Arduino.h>

// #include "filesystem.h"

#if defined(__APPLE__)
#include <filesystem>
namespace fs = std::__fs::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

unittest(filesystem)
{
  assertTrue(fs::exists("."));
  std::string path = fs::current_path();
  assertTrue(path.size() > 0);
}

unittest_main()
