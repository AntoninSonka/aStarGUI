#!/bin/bash

# Compile all .cpp files
cd "src" || exit

for file in *.cpp; do
    # Compile each file and generate corresponding .o file
    g++ -c "$file" -o "${file%.cpp}.o"
done

if [ "$1" = "sfbuild" ]; then
    g++ *.o -o ../builds/main -lsfml-graphics -lsfml-window -lsfml-system
fi

if [ "$1" = "sfrun" ]; then
    g++ *.o -o ../builds/main -lsfml-graphics -lsfml-window -lsfml-system
    ./../builds/main
fi

if [ "$1" = "build" ]; then
    g++ *.o -o ../builds/main
fi

if [ "$1" = "run" ]; then
    g++ *.o -o ../builds/main
    ./../builds/main
fi

if [ "$1" = "nbuild" ]; then
    g++ *.o -o ../builds/main -lncurses
fi

if [ "$1" = "nrun" ]; then
    g++ *.o -o ../builds/main -lncurses
    ./../builds/main
fi

# Clean up .o files
rm *.o

cd .. || exit
