#include <fstream>
#include "project/utils/copy.hpp"

int copy(const std::string& source, const std::string& destination)
{
  std::ifstream src(source, std::ios::binary);
  std::ofstream dst(destination,   std::ios::binary);
  dst << src.rdbuf();

  return 0;
}
