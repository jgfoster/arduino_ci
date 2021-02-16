#include <ArduinoUnitTests.h>
#include <Arduino.h>
#include <experimental/filesystem>

namespace fs = std::__fs::filesystem;

unittest(exists)
{
  assertTrue(fs::exists("."));
}

unittest_main()
