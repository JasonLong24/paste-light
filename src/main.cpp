#include <iostream>
#include <getopt.h>
#include <sstream>
#include <algorithm>
#include <dirent.h>
#include "project/compiler.hpp"
#include "project/paste.hpp"
#include "project/configuration.hpp"
#include "project/paste_config.hpp"

Configuration config;
Paste paste;
PasteConfig paste_config;

bool paste_compile_flag = false;

int parse_arguments(const int argc, char* argv[])
{
    if (argc < 2)
    {
        paste.usage(argv[0]);
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

    if(paste.is_init())
    {
        config.Load(paste_config.config);

        if(!config.Get("paste_style",     paste_config.style)     ||
           !config.Get("paste_title",     paste_config.title)     ||
           !config.Get("paste_searchbar", paste_config.searchbar) ||
           !config.Get("paste_plain",     paste_config.plain)     ||
           !config.Get("paste_host",      paste_config.host)      ||
           !config.Get("paste_post",      paste_config.post)      ||
           !config.Get("paste_html_view", paste_config.html_view) ||
           !config.Get("paste_output",    paste_config.output))
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

        if(!paste.is_init() && opt != 'i' && opt != 'h')
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
                paste.add(std::string(optarg));
                break;

            case 'i':
                paste.init();
                break;

            case 's':
                paste_config.searchbar = true;
                break;

            case 't':
                paste_config.title = std::string(optarg);
                break;

            case 'o':
                paste_config.output = std::string(optarg);
                break;

            case 160: // Style
                paste_config.style = std::string(optarg);
                break;

            case 161: // Config
                paste_config.config = std::string(optarg);
                break;

            case 'h':
            case '?':
            default:
                paste.usage(argv[0]);
                break;
        }
    }

    // Be sure paste_compile() runs last.
    if (paste_compile_flag) paste.compile();

    return 0;
}

int main(int argc, char* argv[])
{
    return parse_arguments(argc, argv);
}
