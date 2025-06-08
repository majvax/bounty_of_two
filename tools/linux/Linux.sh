#!/bin/bash

echo -e "\033[92m -> Generating Makefile Linux... \033[0m"

if [ -d "../../tools" ]; then
  echo -e "\033[91m -> Warning: Running script from the wrong directory, changing to root dir\033[0m"
  cd "../../"
fi

if [ -d "tools" ]; then
  echo -e "\033[92m -> Found \"tools\" directory\033[0m"
  PREMAKE_PATH="tools/linux/premake5"
else
  echo -e "\033[91m -> Error: \"tools\" directory does not exist.\033[0m"
  exit 1
fi

if [ -f "$PREMAKE_PATH" ]; then
  echo -e "\033[92m -> Found premake5 executable at $(pwd)/$PREMAKE_PATH\033[0m"
else
  echo -e "\033[91m -> Error: premake5 executable not found at $(pwd)/$PREMAKE_PATH\033[0m"
  exit 1
fi

"$PREMAKE_PATH" gmake
"$PREMAKE_PATH" export-compile-commands
