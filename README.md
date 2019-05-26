# paste-light

lightweight paste system that is managed from the terminal.

### Install

```
$ git clone https://github.com/jsnal/paste-light
$ make && sudo make install
```

### Usage

```
-a, --add
       Add a new entry to the posts directory and stage it for compiling.

-c, --compile
       Compile the current project into a viewable website.

--config=FILE
       Path to the .paste config file. Default ./.paste

-i, --init
       Initialize the current working directory with the paste-light files.

-o, --out=FILE
       Path to the desired output of the html. Default ./index.html

-s, --searchbar
       Display a searchbar.

--style=FILE
       Path to style sheet. Default ./style.css

-t, --title=TITLE
       Display a title. Default Pastebin

-h, --help
       Display this help and exit.
```

See full help at `man paste-light`

### Uninstall

```
$ sudo make uninstall
```

### Update

```
$ git pull origin master
$ make clean && make && sudo make install
```
