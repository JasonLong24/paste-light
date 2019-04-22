# paste-light

lightweight paste system that supports pushing pastes from the terminal.

### Install

```
bash <(wget -qO- https://raw.githubusercontent.com/jsnal/paste-light/master/install.sh)
```

### Usage

```
-init, --initialize         Initialize a new project.
-a, --add [NAME]            Create a new paste with filename.
-c, --compile               Compile the current project into a viewable website.
-t, --title [TITLE]         Title your website.
-l, --listen                Make current server a listner for potential pushes
-p, --port                  Set the current server port.
-h, --help                  Show this screen.
--search-bar [BOOLEAN]      Create a search bar for your website. Needs JS.
--send                      Send a paste to a listening server.
--host                      Set the current server ip address.
```

### Uninstall

```
bash <(wget -qO- https://raw.githubusercontent.com/jsnal/paste-light/master/install.sh) -r
```

### Update

```
bash <(wget -qO- https://raw.githubusercontent.com/jsnal/paste-light/master/install.sh) -u
```
