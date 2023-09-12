#include "um_dev/utils/files.h"

#include <iostream>

namespace um_dev {
namespace utils {

bool um_mkdir(const std::string &path) {
  if (access(path.c_str(), 0) == -1) {
    auto err = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);  // 777
    if (err != 0) {
      std::cout << "Enable to mkdir: " << path << ", errno = " << err
                << std::endl;
      return false;
    }
    if (access(path.c_str(), 0) == -1) {
      std::cout << "mkdir failed: " << path << ", errno = " << err << std::endl;
      return false;
    }
    return true;
  }
  return true;
}

bool um_rmdir(const std::string &path) {
  if (access(path.c_str(), 0) == -1) {
    return true;
  }
  auto err = rmdir(path.c_str());
  if (err != 0) {
    return false;
  }
  return true;
}

std::string make_csv_header(const std::vector<std::string>& fields) {
  std::string header_string;

  for (unsigned int i = 0; i < fields.size(); ++i) {
    if (i > 0) {
      header_string += ',';
    }
    header_string += fields[i];
  }

  return header_string;
  
}

}  // namespace utils
}  // namespace um_dev