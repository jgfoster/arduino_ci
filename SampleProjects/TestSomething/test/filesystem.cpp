#include <ArduinoUnitTests.h>
#include <Arduino.h>
#include <filesystem>

namespace fs = std::filesystem;

unittest(exists)
{
  assertTrue(fs::exists("."));
}

unittest_main()
