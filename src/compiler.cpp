#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <algorithm>
#include <fstream>
#include <dirent.h>
#include <vector>
#include <iomanip>
#include "compiler.h"

bool sb = false;

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

void paste_searchbar()
{
    sb = true;
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
    for (unsigned int i = 0; i <= 2; i++)
    {
        std::getline(f, cline);
        if (cline.find(id) != std::string::npos)
        {
            break;
        }
    }
    return	cline.erase(0, id.length());
}

void format_file(const std::string& file)
{
    mkdir("build", 0777);
    std::istringstream f(get_file(file));
    std::string cline;
    std::ofstream outfile ("build/" + file + ".paste");
    int i;
    while(std::getline(f, cline))
    {
        i++;
        if (i > 3)
        {
            outfile << cline << std::endl;
        }
    }
    outfile.close();
}

void compile_table_header(std::ostream& os)
{
    os << "<html>\n<header>\n<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\">\n<script charset='utf-8' src='js/index.js'></script>\n</header>\n"
       << "<table id=\"paste-tbl-container\">\n<tr class=\"paste-tbl-row\">\n"
       << "<th class=\"paste-tbl-header\">Name</th>\n<th class=\"paste-tbl-header\">Date</th>\n<th class=\"paste-tbl-header\">Filetype</th>\n<th class=\"paste-tbl-header\">View</th>\n</tr>"
       << std::endl;
}

void compile_table_row(std::ostream& os, const std::string& value)
{
    os << "<td class=\"paste-tbl-data\">" << value << "</td>" << std::endl;
}

void compile_table_footer(std::ostream& os)
{
    auto time = std::time(nullptr);
    os << "<footer>Last Updated: " << std::put_time(std::gmtime(&time), "%D") << "</footer>" << std::endl;
}

int paste_compile()
{
    std::vector<std::string> files = std::vector<std::string>();
    get_file_list("posts", files);

    std::ofstream outfile ("index.html");

    if(sb)
    {
      std::cout << "Generating Searchbar" << std::endl;
      outfile   << "<input type='text' id='table-filter' onkeyup='filter()' placeholder='Search Posts'>" << std::endl;
    }

    std::cout << paste_title << std::endl;
    std::cout << "Generating Table" << std::endl;
    compile_table_header(outfile);

    for (unsigned int i = 0; i < files.size(); i++)
    {
        std::cout << "Found -> posts/" << files[i] << std::endl;
        format_file(files[i]);
        outfile << "<tr class=\"paste-tbl-row\">" << std::endl;
        compile_table_row(outfile, compile_get_id(files[i], "//*title="));
        compile_table_row(outfile, compile_get_id(files[i], "//*date="));
        compile_table_row(outfile, compile_get_id(files[i], "//*filetype="));
        compile_table_row(outfile, "<a href=\"build/" + files[i] + ".paste\">GET</a>");
        outfile << "</tr>" << std::endl;
    }
    outfile << "</table>" << std::endl;
    compile_table_footer(outfile);
    outfile.close();
    std::cout << "Compiled to index.html" << std::endl;
    return 0;
}
