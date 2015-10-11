#ifndef UTILITIES_H
#define UTILITIES_H

#include <sstream>
#include <string>
#include <vector>
#include <sys/stat.h>

#if defined(__WIN32__) || defined(__WIN64__)
#define stat64 _stat64
#endif

struct FileInfo {
	unsigned long long file_size;
};

std::vector<std::string> split(const std::string &str, char delim);
std::string get_file_name(const std::string &path_file_name);
FileInfo get_file_info(std::string file_name);

#endif //UTILITIES_H

