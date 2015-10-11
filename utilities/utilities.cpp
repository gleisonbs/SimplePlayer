#include <sstream>
#include <string>
#include <vector>
#include <sys/stat.h>

#include "utilities.h"

std::vector<std::string> split(const std::string &str, char delim)
{
	std::stringstream ss(str);
	std::string word;
	std::vector<std::string> words;
	while(getline(ss, word, delim))
		words.push_back(word);

	return words;
}

std::string get_file_name(const std::string &path_file_name)
{
	std::string path_separator = "\\/";
	std::string file_name;
	size_t pos_found = path_file_name.find_last_of(path_separator);
	if (pos_found == std::string::npos)
		file_name = path_file_name;
	else
		file_name = path_file_name.substr(pos_found+1);

	return file_name;
}

FileInfo get_file_info(std::string file_name)
{
	struct stat64 file_info_buf;
	stat64(file_name.c_str(), &file_info_buf);

	FileInfo file_info_ret;
	file_info_ret.file_size = file_info_buf.st_size;

	return file_info_ret;
}
