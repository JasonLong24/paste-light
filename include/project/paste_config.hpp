#ifndef PASTE_CONFIG_H
#define PASTE_CONFIG_H

struct PasteConfig
{
  std::string config { ".paste" };
  std::string post;
  std::string host;
  std::string title;
  std::string output;
  std::string style;
  bool searchbar;
  bool plain;
  bool html_view;
};

extern struct PasteConfig paste_config;
#endif
