#include <iostream>

using namespace std;

class Paste
{
  public:
    /**
     *  Checks if the project is initialized. The project is initialized if the
     *  .paste file exists in the root directory of the project.
     *
     *  @return If the project is initialized
     */
    bool is_init();

    /**
     *  Initialize the project in the current directory the user is in. It will
     *  create the .paste file with the default settings the user can change.
     *  It will also copy the default color scheme CSS file from /usr/local/share/paste-light/themes
     *
     *  @return If the project is already initialized, it will return 1.
     */
    int init();

    /**
     *  Add a new post and give it a unique hash. The name of the post will be
     *  taken from the filename argument.
     *
     *  @param Name of the new post
     *  @return 0 on success
     */
    int add(const std::string&);

    /**
     *  Compile the current project. This function just calls the paste_compile()
     *  function in compiler.hpp
     *
     *  @return 0 on success
     */
    int compile();

    /**
     *  Output a brief usage page to the user.
     *
     *  @param argv or the name of the command
     *  @return 0 on success
     */
    int usage(const std::string&);

  private:
    /**
     *  Generate the 8 digit alphanumeric hash used by the add command. This ID
     *  should be unique to that post.
     *
     *  @return Unique ID string
     */
    string gen_post_id();
};
