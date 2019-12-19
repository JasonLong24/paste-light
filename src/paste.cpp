#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <iomanip>
#include <sys/stat.h>
#include "project/paste.hpp"
#include "project/compiler.hpp"
#include "project/utils/copy.hpp"


bool Paste::is_init()
{
  std::ifstream infile(".paste");
  return infile.good();
}


int Paste::init()
{
  if(this->is_init())
  {
    std::cout << "Project already initialized\nSee --help for usage." << std::endl;
    return 1;
  }

  mkdir("posts", 0777);
  mkdir("js", 0777);

  std::ofstream outfile (".paste");
  outfile << "paste_style=style.css"    << std::endl;
  outfile << "paste_host=localhost"     << std::endl;
  outfile << "paste_output=index.html"  << std::endl;
  outfile << "paste_searchbar=false"    << std::endl;
  outfile << "paste_title=Pastebin"     << std::endl;
  outfile << "paste_plain=false"        << std::endl;
  outfile << "paste_post=nothing"       << std::endl;
  outfile << "paste_html_view=true"     << std::endl;
  outfile.close();

  copy("/usr/local/share/paste-light/themes/default.css", "style.css");
  copy("/usr/local/share/paste-light/themes/js/index.js", "js/index.js");

  std::cout << "Project initialized\nSee --help for usage." << std::endl;

  return 0;
}


int Paste::add(const std::string& filename)
{
  auto time = std::time(nullptr);
  static const std::string post_id       = this->gen_post_id();
  static const std::string post_filename = filename + "_" + post_id;

  std::ofstream outfile ("posts/" + post_filename);

  outfile << "//*date="       << std::put_time(std::gmtime(&time), "%D")
          << "\n//*id="       << post_id
          << "\n//*title="    << filename
          << "\n//*type=" << std::endl;

  outfile.close();

  std::cout << "Generated posts/" << post_filename << std::endl;
  return 0;
}


int Paste::compile()
{
  return paste_compile();
}


std::string Paste::gen_post_id()
{
  size_t length = 0;
  static const std::string allowed_chars {"123456789BCDFGHJKLMNPQRSTVWXZbcdfghjklmnpqrstvwxz"};

  static thread_local std::default_random_engine randomEngine(std::random_device{}());
  static thread_local std::uniform_int_distribution<int> randomDistribution(0, allowed_chars.size() - 1);

  std::string id(length ? length : 8, '\0');

  for (std::string::value_type& c : id) {
    c = allowed_chars[randomDistribution(randomEngine)];
  }

  return id;
}


int Paste::usage(const std::string& argv)
{
  std::cout << "Usage: " << argv << " NAME"
            << std::endl;

  return 0;
}
