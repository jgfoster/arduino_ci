#include <ArduinoUnitTests.h>
#include <Arduino.h>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

unittest(exists)
{
  assertTrue(fs::exists("."));
  std::string path = fs::current_path();
  assertEqual("/home/runner/work/arduino_ci/arduino_ci/SampleProjects/TestSomething", path);
}

unittest_main()
