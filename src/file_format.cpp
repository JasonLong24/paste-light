#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include "project/compiler.h"
#include "project/html_generator.h"
#include "maddy/parser.h"

/*
 * Called after format file, converts md to html
 */
void generate_html_view(const std::string& file)
{

  // grab the file contents
  std::ifstream ifs("build/"+file+".paste");
  std::string content( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()    ) );

  // convert to html using maddy
  std::stringstream markdownInput(content);
  std::shared_ptr<maddy::Parser> parser = std::make_shared<maddy::Parser>();
  std::string htmlOutput = parser->Parse(markdownInput);

  // put it into an html file
  std::ofstream outfile ("build/"+file+".html");
  html_generate_link(outfile);
  outfile << htmlOutput << std::endl;
  outfile.close();
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

    generate_html_view(file);
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
            << "```\n" << "wget -qO- " << paste_host << "/posts.md | sed 1,5d | grep -i \"command\"\n"
            << "curl -L "   << paste_host << "/posts.md | sed 1,5d | grep -i \"command\"\n" << "```"
            << std::endl;
    for (unsigned int i = 0; i < files.size(); i++)
    {
        outfile << "* [" << compile_get_id(files[i], "//*title=") << "]"
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
