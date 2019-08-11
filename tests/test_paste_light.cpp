#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

// TODO: Figure out why I can't include the hpp
#include "../src/configuration.cpp"

TEST_CASE("Configuration Files") {
  Configuration config;
  std::string paste_style;
  std::string paste_title;
  bool        paste_searchbar;

  REQUIRE(config.Load("config") == true);
  REQUIRE(config.Get("paste_style", paste_style) == true);
  REQUIRE(config.Get("paste_title", paste_title) == true);
  REQUIRE(config.Get("paste_searchbar", paste_searchbar) == true);

  REQUIRE(paste_style == "style.css");
  REQUIRE(paste_title == "Paste Title");
  REQUIRE(paste_searchbar == false);
}
