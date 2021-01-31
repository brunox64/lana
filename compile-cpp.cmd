@echo off

set file=%1

g++ -std=c++17 -g "%file%.cpp" -o "%file%.exe" 
