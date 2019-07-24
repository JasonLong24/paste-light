#include <iostream>
#include <getopt.h>
#include <sstream>
#include <sys/stat.h>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <dirent.h>
#include "project/compiler.h"
#include "project/configuration.h"

Configuration config;
std::string paste_config = ".paste";
std::string paste_plain_output = "txt";
std::string paste_footerlink = "posts.lst";
std::string paste_post;
std::string paste_host;
std::string paste_title;
std::string paste_output;
std::string paste_style;

void copy_paste(const std::string& source, const std::string& destination)
{
    std::ifstream src(source, std::ios::binary);
    std::ofstream dst(destination,   std::ios::binary);
    dst << src.rdbuf();
}

bool is_paste_init()
{
    std::ifstream infile(".paste");
    return infile.good();
}

void show_usage(const std::string& argv)
{
    std::cout << "Usage: " << argv << " NAME"
              << std::endl;
}

void paste_add(const std::string& filename)
{
    auto time = std::time(nullptr);
    std::ofstream outfile ("posts/" + filename);
    outfile << "//*date=" << std::put_time(std::gmtime(&time), "%D")
            << "\n//*title=Placeholder Title\n"
            << "//*filetype=" << filename.substr(filename.find_last_of(".") + 1)
            << std::endl;
    outfile.close();
    std::cout << "Generated posts/" << filename << std::endl;
}

int paste_init()
{
    if(is_paste_init())
    {
        std::cout << "Project already initialized\nSee --help for usage." << std::endl;
        return 1;
    }

    mkdir("posts", 0777);
    mkdir("js", 0777);
    std::ofstream outfile (".paste");
    outfile << "paste_style=style.css" << std::endl;
    outfile << "paste_host=localhost" << std::endl;
    outfile << "paste_output=index.html" << std::endl;
    outfile << "paste_searchbar=false" << std::endl;
    outfile << "paste_title=Pastebin" << std::endl;
    outfile << "paste_plain=false" << std::endl;
    outfile << "paste_plain_output=txt" << std::endl;
    outfile << "paste_post=nothing" << std::endl;
    outfile << "paste_footerlink=posts.lst" << std::endl;
    outfile << "paste_html_view=true" << std::endl;
    outfile.close();
    copy_paste("/usr/local/share/paste-light/themes/default.css", "style.css");
    copy_paste("/usr/local/share/paste-light/themes/js/index.js", "js/index.js");
    std::cout << "Project initialized\nSee --help for usage." << std::endl;
    return 0;
}

bool paste_compile_flag = false;

int parse_arguments(const int argc, char* argv[])
{
    if (argc < 2)
    {
        show_usage(argv[0]);
    }

    const char* const short_opts = "ia:ct:o:sh";
    const option long_opts[] =
    {
        {"add", required_argument, nullptr, 'a'},
        {"init", no_argument, nullptr, 'i'},
        {"compile", no_argument, nullptr, 'c'},
        {"title", required_argument, nullptr, 't'},
        {"out", required_argument, nullptr, 'o'},
        {"searchbar", no_argument, nullptr, 's'},
        {"style", required_argument, nullptr, 160},
        {"config", required_argument, nullptr, 161},
        {"help", no_argument, nullptr, 'h'},
        {nullptr, no_argument, nullptr, 0}
    };

    if(is_paste_init())
    {
        config.Load(paste_config);

        if(!config.Get("paste_style", paste_style)               ||
           !config.Get("paste_title", paste_title)               ||
           !config.Get("paste_searchbar", sb)                    ||
           !config.Get("paste_plain", paste_plain)               ||
           !config.Get("paste_plain_output", paste_plain_output) ||
           !config.Get("paste_host", paste_host)                 ||
           !config.Get("paste_post", paste_post)                 ||
           !config.Get("paste_footerlink", paste_footerlink)     ||
           !config.Get("paste_html_view", paste_html_view)       ||
           !config.Get("paste_output", paste_output))
        {
            std::cout << "Problems loading configuration file.\nSee --help." << std::endl;
            return 1;
        }
    }

    while (true)
    {

        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

        if (-1 == opt)
            break;

        if(!is_paste_init() && opt != 'i' && opt != 'h')
        {
            std::cout << "Please initialize this project.\n See --help for usage." << std::endl;
            return 1;
        }

        switch (opt)
        {
            case 'c':
                paste_compile_flag = true;
                break;

            case 'a':
                paste_add(std::string(optarg));
                break;

            case 'i':
                paste_init();
                break;

            case 's':
                paste_searchbar();
                break;

            case 't':
                paste_title = std::string(optarg);
                break;

            case 'o':
                paste_output = std::string(optarg);
                break;

            case 160: // Style
                paste_style = std::string(optarg);
                break;

            case 161: // Config
                paste_config = std::string(optarg);
                break;

            case 'h':
            case '?':
            default:
                show_usage(argv[0]);
                break;
        }
    }

    // Be sure paste_compile() runs last.
    if (paste_compile_flag) paste_compile();

    return 0;
}

int main(int argc, char* argv[])
{
    return parse_arguments(argc, argv);
}
