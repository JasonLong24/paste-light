#!/usr/bin/env bash

function install() {
  local _current_dir=$(pwd)
  local _project_dir="/tmp/paste-light"
  sudo git clone -q https://github.com/jsnal/paste-light /tmp/paste-light 2>/dev/null
  sudo cp -fv $_project_dir/paste /usr/bin/paste

  # Setup shared files
  if [ ! -d /usr/local/share/paste-light ]; then
    sudo mkdir /usr/local/share/paste-light
    sudo cp -rv $_project_dir/themes /usr/local/share/paste-light/themes
  fi
  sudo cp -fv $_project_dir/README.md /usr/local/share/paste-light/README
}

function remove() {
  sudo rm -v /usr/bin/paste
  sudo rm -rvf /usr/local/share/paste-light
}

if [[ $1 = "-u" ]]; then
  echo "Updating paste-light... "
  remove && install
  echo "Update complete"
elif [[ $1 = "-r" ]]; then
  echo "Uninstalling paste-light... "
  remove
  echo "Uninstall complete"
else
  echo "Installing paste-light... "
  install
  echo "Install complete"
fi
