#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include "ctml.hpp"
#include "project/compiler.hpp"
#include "project/html_generator.hpp"
#include "maddy/parser.h"

void copy(const std::string& source, const std::string& destination)
{
    std::ifstream src(source, std::ios::binary);
    std::ofstream dst(destination,   std::ios::binary);
    dst << src.rdbuf();
}

/*
 * Called after format file, converts md to html
 */
void generate_html_view(const std::string& file)
{
    mkdir("build/view/", 0777);
    copy(paste_style, "build/view/style.css");

    // grab the file contents
    std::ifstream ifs("build/raw/"+file+".paste");
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()    ) );

    // convert to html using maddy
    std::stringstream markdownInput(content);
    std::shared_ptr<maddy::Parser> parser = std::make_shared<maddy::Parser>();
    std::string htmlOutput = parser->Parse(markdownInput);

    // put it into an html file
    CTML::Document document;

    document.AppendNodeToHead(CTML::Node("link").SetAttribute("rel", "stylesheet")
                                                .SetAttribute("type", "text/css")
                                                .SetAttribute("href", paste_style)
                                                .UseClosingTag(false));

    document.AppendNodeToHead(CTML::Node("title", paste_title + " - " + file));
    document.AppendNodeToBody(CTML::Node("div#raw-body", htmlOutput));

    // append to the file
    std::ofstream outfile ("build/view/"+file+".html");
    outfile << document.ToString() << std::endl;
    outfile.close();
}

void format_file(const std::string& file)
{
    mkdir("build/", 0777);
    mkdir("build/raw/", 0777);

    std::istringstream f(get_file(file));
    std::string cline;
    std::ofstream outfile ("build/raw/" + file + ".paste");

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

    if(paste_html_view) generate_html_view(file);
}

void generate_plain_text(std::vector<std::string> files)
{
    std::cout << "Generating Plain Text File" << std::endl;

    std::ofstream outfile ("posts.list");
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
