#!/bin/bash

# Define color codes
RED="\e[31m"
ORANGE="\e[33m"
GREEN="\e[32m"
WHITE_BG="\e[47m"
RESET="\e[0m"

# Define directories
destinationDir="C:/Users/fl0wer/Desktop/hlds_win/cstrike/addons/amxmodx"
# destinationDir="D:/Games/SteamLibrary/steamapps/common/Half-Life/cstrike/addons/amxmodx"
scriptingDir="$destinationDir/scripting"
srcDir="$PWD"

# Function to print colored messages
print_color() {
    local color="$1"
    local message="$2"
    echo -e "${color}${message}${RESET}"
}

# Function to compile a .sma file
compile_sma() {
    local smaFile="$1"
    local outputPluginDir="$2"

    pluginName=$(basename "${smaFile%.sma}")
    relativeDir=$(dirname "${smaFile#$srcDir}")

    outputPlugin="$outputPluginDir/${pluginName}.amxx"

    # Create the output plugin directory if it doesn't exist
    mkdir -p "$outputPluginDir"

    # Print the name of the .sma file with white background
    # print_color $WHITE_BG " - Compiling: $(basename $smaFile)"

    # Get the last modification time of the output plugin file
    lastModTime=$(stat -c %Y "$smaFile" 2>/dev/null)
    now=$(date +%s)
    diff=$((now-lastModTime))

    # Check if the file exists and its last modification time is within the last minute
    if ! [ -f $outputPlugin ] || [ $diff -lt 60 ]; then
        # Compile the .sma file and capture its output, excluding the lines with version and copyright info
        compile_output=$("$scriptingDir/amxxpc" \
            "$smaFile" \
            -i"$srcDir" \
            -i"$srcDir/include" \
            -i"$srcDir/rezombie" \
            -o"$outputPlugin" 2>&1 | grep -vE "AMX Mod X Compiler|Copyright|Could not locate output file")

        # Check if there are any errors or warnings in the compile output
        if echo "$compile_output" | grep -qi "error"; then
            error_lines=$(echo "$compile_output" | grep -i "error" | sed 's/.*scripting\///')
            warning_lines=$(echo "$compile_output" | grep -i "warning" | sed 's/.*scripting\///')
            print_color $RED "❌ $error_lines"
            if [ -n "$warning_lines" ]; then
                print_color $ORANGE "⚠️ $warning_lines"
            fi
        elif echo "$compile_output" | grep -qi "warning"; then
            warning_lines=$(echo "$compile_output" | grep -i "warning" | sed 's/.*scripting\///')
            print_color $ORANGE "⚠️ $warning_lines"
        else
            print_color $GREEN "  ✅ Compiled: $(basename $smaFile)"
        fi
    # else
        # Skip processing the file if it has been modified within the last minute
        # print_color $ORANGE "Skipped: $(basename $smaFile) (not modified $((diff / 60)) min)"
    fi
}

# Find and compile all .sma files in the source directory and its subdirectories
find "$srcDir" -name "*.sma" -type f | while read smaFile; do
    relativeDir=$(dirname "${smaFile#$srcDir}")
    outputPluginDir="$destinationDir/plugins$relativeDir"
    compile_sma "$smaFile" "$outputPluginDir"
done

#echo ""

# Copy directories without confirmation with green messages
#print_color $GREEN " - Copying configs..."
#cp -rf "$srcDir/../configs"/* "$destinationDir/configs"
#print_color $GREEN " - Copying data..."
#cp -rf "$srcDir/../data"/* "$destinationDir/data"