#!/bin/bash
echo "Generating GNU Make project files..."

cd ../../

PREMAKE_PATH="tools/linux/premake5"

if [ -f "$PREMAKE_PATH" ]; then
    echo "Found premake5 executable at $PREMAKE_PATH"
else
    echo "Error: premake5 executable not found at $PREMAKE_PATH"
    exit 1
fi

"$PREMAKE_PATH" gmake
