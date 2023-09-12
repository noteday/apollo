#ifndef FILES_H
#define FILES_H

#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <fstream>

namespace um_dev {
namespace utils {

bool um_mkdir(const std::string &path);
bool um_rmdir(const std::string &path);
std::string make_csv_header(const std::vector<std::string>& fields);

}  // namespace utils
}  // namespace um_dev

#endif