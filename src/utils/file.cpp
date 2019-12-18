#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <dirent.h>


int get_file_list(std::string dir, std::vector<std::string> &files)
{
  DIR *dp;
  struct dirent *dirp;

  if((dp  = opendir(dir.c_str())) == NULL)
  {
    std::cout << "Error(" << errno << ") opening " << dir << std::endl;
    return errno;
  }

  while ((dirp = readdir(dp)) != NULL)
  {
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
