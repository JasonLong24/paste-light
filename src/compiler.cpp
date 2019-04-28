#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <dirent.h>
#include <vector>
#include "compiler.h"

int get_file_list(std::string dir, std::vector<std::string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        std::cout << "Error(" << errno << ") opening " << dir << std::endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(std::string(dirp->d_name));
    }
    files.erase(std::remove(files.begin(), files.end(), "."), files.end());
    files.erase(std::remove(files.begin(), files.end(), ".."), files.end());
    closedir(dp);
    return 0;
}

std::string get_file(std::string file)
{
    std::ifstream ifs("posts/" + file);
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()    ) );
    return content;
}

std::string compile_get_id(std::string file, std::string id)
{
		std::istringstream f(get_file(file));
		std::string cline;
		for (unsigned int i = 0; i <= 2; i++) {
				std::getline(f, cline);
				if (cline.find(id) != std::string::npos) {
					break;
				}
		}
	 return	cline.erase(0, id.length());
}

int paste_compile()
{
    // Gen header
    // For loop of each file
    // End Loop
    // Footer

    std::vector<std::string> files = std::vector<std::string>();
    get_file_list("posts", files);

    for (unsigned int i = 0;i < files.size();i++) {
        std::cout << compile_get_id(files[i], "//*title=")
									<< compile_get_id(files[i], "//*date=")
									<< compile_get_id(files[i], "//*filetype=")
                  << std::endl;
    }
    return 0;
}
