#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <algorithm>
#include <fstream>
#include <dirent.h>
#include <vector>
#include <iomanip>
#include <memory>
#include <string>
#include "project/compiler.hpp"
#include "project/file_format.hpp"
#include "project/paste_config.hpp"
#include "project/utils/file.hpp"
#include "maddy/parser.h"
#include "ctml.hpp"


std::string compile_get_id(std::string file, std::string id)
{
    std::istringstream f(get_file(file));
    std::string cline;

    for (unsigned int i = 0; i <= 3; i++)
    {
        std::getline(f, cline);

        if (cline.find(id) != std::string::npos)
        {
            break;
        }
    }

    return	cline.erase(0, id.length());
}

void compile_table_header(CTML::Node& node_table_container)
{
    CTML::Node node_table_row("tr#paste-tbl-row");

    // Append the table rows to the table container
    node_table_row.AppendChild(CTML::Node("th.paste-tbl-header", "ID"))
                  .AppendChild(CTML::Node("th.paste-tbl-header", "Name"))
                  .AppendChild(CTML::Node("th.paste-tbl-header", "Date"))
                  .AppendChild(CTML::Node("th.paste-tbl-header", "Type"))
                  .AppendChild(CTML::Node("th.paste-tbl-header", "Raw"));

    // If paste_config.html_view is on make that table header
    if (paste_config.html_view)
        node_table_row.AppendChild(CTML::Node("th.paste-tbl-header", "View"));

    node_table_container.AppendChild(node_table_row);
}

void compile_table(std::vector<std::string> files, CTML::Node& node_table_container)
{
    for (unsigned int i = 0; i < files.size(); i++)
    {
        std::cout << "Found -> posts/" << files[i] << std::endl;

        // Format the file name and copy it to build/raw
        format_file(files[i]);

        // Create a new table row
        CTML::Node node_table_row("tr.paste-tbl-row");

        // Append all of the default table data cells
        node_table_row.AppendChild(CTML::Node("td.paste-tbl-data",
                                   compile_get_id(files[i], "//*id=")))

                      .AppendChild(CTML::Node("td.paste-tbl-data",
                                   compile_get_id(files[i], "//*title=")))

                      .AppendChild(CTML::Node("td.paste-tbl-data",
                                   compile_get_id(files[i], "//*date=")))

                      .AppendChild(CTML::Node("td.paste-tbl-data",
                                   compile_get_id(files[i], "//*type=")))

                      .AppendChild(CTML::Node("td.paste-tbl-data")
                                              .AppendChild(CTML::Node("a", "RAW")
                                              .SetAttribute("href", "build/raw/"+files[i]+".paste")));

        // If paste_config.html_view is on make that table cell
        if (paste_config.html_view)
          node_table_row.AppendChild(CTML::Node("td.paste-tbl-data").AppendChild(CTML::Node("a", "VIEW")
                                     .SetAttribute("href", "build/view/"+files[i]+".html")));

        node_table_container.AppendChild(node_table_row);
    }
}

void compile_footer(CTML::Document& document)
{
    auto time = std::time(nullptr);
    std::stringstream ss;
    ss << std::put_time(std::gmtime(&time), "%D");

    std::string last_updated = "Last Updated: " + ss.str();

    if (!paste_config.plain)
    {
        document.AppendNodeToBody(CTML::Node("footer", last_updated));
    }
    else
    {
        CTML::Node footer("footer", last_updated + " - ");
        footer.AppendChild(CTML::Node("a", "Shell").SetAttribute("href", "posts.list"));
        document.AppendNodeToBody(footer);
    }
}

int paste_compile()
{
    std::vector<std::string> files = std::vector<std::string>();
    get_file_list("posts", files);

    CTML::Document document;

    document.AppendNodeToHead(CTML::Node("link").SetAttribute("rel", "stylesheet")
                                                .SetAttribute("type", "text/css")
                                                .SetAttribute("href", paste_config.style)
                                                .UseClosingTag(false));

    document.AppendNodeToHead(CTML::Node("script").SetAttribute("charset", "utf-8")
                                                  .SetAttribute("src", "js/index.js"));

    document.AppendNodeToHead(CTML::Node("title", paste_config.title));

    document.AppendNodeToBody(CTML::Node("p#paste-title", paste_config.title));
    CTML::Node node_table_container("table#paste-tbl-container");

    if(paste_config.searchbar)
    {
        std::cout << "Generating Searchbar" << std::endl;
        document.AppendNodeToBody(CTML::Node("input").SetAttribute("placeholder", "Search Posts")
                                                     .SetAttribute("type",        "text")
                                                     .SetAttribute("id",          "table-filter")
                                                     .SetAttribute("onkeyup",     "filter()")
                                                     .UseClosingTag(false));
    }

    std::cout << "Generating Table" << std::endl;

    // Compile the table header and every table row
    compile_table_header(node_table_container);
    compile_table(files, node_table_container);
    document.AppendNodeToBody(node_table_container);

    // Compile footer
    compile_footer(document);

    // Dump the html into the html file
    std::ofstream outfile (paste_config.output);
    outfile << document.ToString(CTML::StringFormatting::MULTIPLE_LINES) << std::endl;
    outfile.close();

    std::cout << "Compiled to " << paste_config.output << std::endl;

    if (paste_config.plain)
        generate_plain_text(files);

    if (paste_config.post != "nothing")
        system(paste_config.post.c_str());

    return 0;
}
