#include <ArduinoUnitTests.h>
#include <Arduino.h>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

unittest(exists)
{
  assertTrue(fs::exists("."));
  std::string path = fs::current_path();
  std::cout << "path = " << path << std::endl;
}

unittest_main()
