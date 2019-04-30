#include <iostream>
#include <getopt.h>
#include <sys/stat.h>
#include <fstream>
#include <iomanip>
#include "compiler.h"

std::string paste_title = "";

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

int paste_add(const std::string& filename)
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
    if(is_paste_init()) { std::cout << "Project already initialized\nSee --help for usage." << std::endl; return 1; }

    mkdir("posts", 0777);
    mkdir("js", 0777);
    std::ofstream outfile (".paste");
    outfile.close();
    copy_paste("/usr/local/share/paste-light/themes/light.css", "style.css");
    copy_paste("/usr/local/share/paste-light/themes/js/index.js", "js/index.js");
    std::cout << "Project initialized\nSee --help for usage." << std::endl;
}

int parse_arguments(const int argc, char* argv[])
{
    if (argc < 2)
    {
        show_usage(argv[0]);
    }

    const char* const short_opts = "ia:ct:sh";
    const option long_opts[] =
    {
        {"add", required_argument, nullptr, 'a'},
        {"init", no_argument, nullptr, 'i'},
        {"compile", no_argument, nullptr, 'c'},
        {"title", required_argument, nullptr, 't'},
        {"searchbar", no_argument, nullptr, 's'},
        {"help", no_argument, nullptr, 'h'},
        {nullptr, no_argument, nullptr, 0}
    };

    while (true)
    {
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

        if (-1 == opt)
            break;

        if(!is_paste_init() && opt != 'i')
        {
            std::cout << "Please initialize this project.\n See --help for usage." << std::endl;
            return 1;
        }

        switch (opt)
        {

            // TODO: Make paste_compile always execute last instead of the first time it appears.
            case 'c':
                paste_compile();
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
                std::cout << paste_title << std::endl;
                break;

            case 'h':
            case '?':
            default:
                show_usage(argv[0]);
                break;
        }
    }

    return 0;
}

int main(int argc, char* argv[])
{
    return parse_arguments(argc, argv);
}
