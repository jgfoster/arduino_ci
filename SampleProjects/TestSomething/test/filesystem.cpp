#include <ArduinoUnitTests.h>
#include <Arduino.h>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

unittest(filesystem)
{
  assertTrue(fs::exists("."));
  std::string expected = std::string("/home/runner/work/arduino_ci/arduino_ci");
  std::string actual = fs::current_path();
  int size = expected.size();
  assertTrue(path.size() >= size);
  path = path.substr(0, size);
  assertEqual("/home/runner/work/arduino_ci/arduino_ci", path);
}

unittest_main()
