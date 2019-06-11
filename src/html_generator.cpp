#include <iostream>
#include <iomanip>
#include "project/compiler.h"

using namespace std;

void html_generate_tag(ostream& stream, string tag)
{
    stream << "<" << tag << ">"
           << "</" << tag << ">"
           << endl;
}

void html_generate_tag(ostream& stream, string tag, string data)
{
    stream << "<" << tag << ">"
           << data
           << "</" << tag << ">"
           << endl;
}

void html_generate_tag_class(ostream& stream, string tag, string data, string tclass)
{
    stream << "<" << tag << " class=\"" << tclass << "\"" << ">"
           << data
           << "</" << tag << ">"
           << endl;
}

void html_generate_tag_id(ostream& stream, string tag, string data, string tid)
{
    stream << "<" << tag << " id=\"" << tid << "\"" << ">"
           << data
           << "</" << tag << ">"
           << endl;
}

void html_generate_tag_start(ostream& stream, string tag)
{
    stream << "<" << tag << ">" << endl;
}

void html_generate_tag_start_class(ostream& stream, string tag, string tclass)
{
    stream << "<" << tag << " class=\"" << tclass << "\"" << ">" << endl;
}

void html_generate_tag_start_id(ostream& stream, string tag, string tid)
{
    stream << "<" << tag << " id=\"" << tid << "\"" << ">" << endl;
}

void html_generate_tag_end(ostream& stream, string tag)
{
    stream << "</" << tag << ">" << endl;
}

void html_generate_footer(ostream& stream)
{
    auto time = std::time(nullptr);
    if(!paste_plain) {
      stream << "<footer>Last Updated: " << std::put_time(std::gmtime(&time), "%D") << "</footer>" << std::endl;
    } else {
      stream << "<footer>Last Updated: " << std::put_time(std::gmtime(&time), "%D")
             << " - <a href=\"" << paste_footerlink << "\">Shell</a>"
             << "</footer>" << std::endl;
    }
}

void html_generate_link(ostream& stream)
{
    stream << "<link rel=\"stylesheet\" type=\"text/css\" href=\"" << paste_style << "\">" << std::endl;
}

void html_generate_script(ostream& stream)
{
    stream << "<script charset='utf-8' src='js/index.js'></script>" << std::endl;
}

void html_generate_title(ostream& stream)
{
    stream << "<title>" << paste_title << "</title>" << std::endl;
}
