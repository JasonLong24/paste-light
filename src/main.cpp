#include <iostream>
#include <getopt.h>

void
show_usage(const std::string& argv)
{
    std::cout << "Usage: " << argv << " NAME"
              << std::endl;
}

void
test_print()
{
    std::cout << "Test Output" << std::endl;
}

void
paste_add(std::string filename)
{
    std::cout << filename << std::endl;
}

int parse_arguments(const int argc, char* argv[])
{
    if (argc < 2)
    {
        show_usage(argv[0]);
        return 1;
    }

    const char* const short_opts = "a:ct:s:h";
    const option long_opts[] =
    {
        {"add", required_argument, nullptr, 'a'},
        {"compile", no_argument, nullptr, 'c'},
        {"title", required_argument, nullptr, 't'},
        {"searchbar", required_argument, nullptr, 's'},
        {"help", no_argument, nullptr, 'h'},
        {nullptr, no_argument, nullptr, 0}
    };

    while (true)
    {
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

        if (-1 == opt)
            break;

        switch (opt)
        {
            case 'a':
                paste_add(std::string(optarg));
                break;

            case 'c':
                std::cout << "Compile" << std::endl;
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
