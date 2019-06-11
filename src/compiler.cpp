#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <algorithm>
#include <fstream>
#include <dirent.h>
#include <vector>
#include <iomanip>
#include "project/compiler.h"
#include "project/html_generator.h"

bool sb = false;
bool paste_plain = false;

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
    int i = 0;

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
    html_generate_tag_start(os, "html");
    html_generate_tag_start(os, "header");
    html_generate_title(os);
    html_generate_link(os);
    html_generate_script(os);
    html_generate_tag_end(os, "header");
    html_generate_tag_start_id(os, "table", "paste-tbl-container");
    html_generate_tag_start_class(os, "tr", "paste-tbl-row");
    html_generate_tag_class(os, "th", "Name", "paste-tbl-header");
    html_generate_tag_class(os, "th", "Date", "paste-tbl-header");
    html_generate_tag_class(os, "th", "Filetype", "paste-tbl-header");
    html_generate_tag_class(os, "th", "View", "paste-tbl-header");
    html_generate_tag_end(os, "tr");
}

void compile_table_row(std::ostream& os, const std::string& value)
{
    html_generate_tag_class(os, "td", value, "paste-tbl-data");
}

void compile_table(std::vector<std::string> files, std::ostream& os)
{
    for (unsigned int i = 0; i < files.size(); i++)
    {
        std::cout << "Found -> posts/" << files[i] << std::endl;
        format_file(files[i]);
        html_generate_tag_start_class(os, "tr", "paste-tbl-row");
        compile_table_row(os, compile_get_id(files[i], "//*title="));
        compile_table_row(os, compile_get_id(files[i], "//*date="));
        compile_table_row(os, compile_get_id(files[i], "//*filetype="));
        compile_table_row(os, "<a href=\"build/" + files[i] + ".paste\">GET</a>");
        html_generate_tag_end(os, "tr");
    }

    html_generate_tag_end(os, "table");
}

void generate_plain_text(std::vector<std::string> files)
{
    std::ofstream outfile ("posts.lst");
    outfile << paste_title << " Shell Interface\n\n"
            << "wget -qO- " << paste_host << "/posts.lst | sed 1,5d | grep -i \"command\"\n"
            << "curl -L "   << paste_host << "/posts.lst | sed 1,5d | grep -i \"command\"\n"
            << std::endl;
    for (unsigned int i = 0; i < files.size(); i++)
    {
        outfile << compile_get_id(files[i], "//*title=") << " - "
                << paste_host << "/build/"
                << files[i] << ".paste" << std::endl;
    }
}

void generate_markdown_text(std::vector<std::string> files)
{
    std::ofstream outfile ("posts.md");
    outfile << "# " << paste_title << " Shell Interface\n\n"
            << "```\n" << "wget -qO- " << paste_host << "/posts.lst | sed 1,5d | grep -i \"command\"\n"
            << "curl -L "   << paste_host << "/posts.lst | sed 1,5d | grep -i \"command\"\n" << "```"
            << std::endl;
    for (unsigned int i = 0; i < files.size(); i++)
    {
        outfile << "[" << compile_get_id(files[i], "//*title=") << "]"
                << "(" << paste_host << "/build/"
                << files[i] << ".paste" << ")" << std::endl;
    }
}

void compile_plain_text(std::vector<std::string> files)
{
    if(paste_plain_output == "txt" || paste_plain_output == "text") {
          generate_plain_text(files);
    } else if(paste_plain_output == "md" || paste_plain_output == "markdown") {
          generate_markdown_text(files);
    } else if(paste_plain_output == "both") {
          generate_plain_text(files);
          generate_markdown_text(files);
    }
}

int paste_compile()
{
    std::vector<std::string> files = std::vector<std::string>();
    get_file_list("posts", files);

    std::ofstream outfile (paste_output);

    if(sb)
    {
        std::cout << "Generating Searchbar" << std::endl;
        outfile   << "<input type='text' id='table-filter' onkeyup='filter()' placeholder='Search Posts'>" << std::endl;
    }

    html_generate_tag_id(outfile, "p", paste_title, "paste-title");
    std::cout << "Generating Table" << std::endl;
    compile_table_header(outfile);
    compile_table(files, outfile);

    html_generate_footer(outfile);

    outfile.close();
    std::cout << "Compiled to " << paste_output << std::endl;

    if(paste_plain)
    {
        compile_plain_text(files);
        std::cout << "Generating Plain Text File" << std::endl;
    }
    return 0;
}
