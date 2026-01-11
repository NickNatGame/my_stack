# My stack
Mini project to show, how stack works.

## Installation and build
Installation:

```
git clone NickNatGame/my_stack
```
Build using Cmake:

````cmake -S . -B build # only first time for configuration
cmake --build build # do this each time you need to build
````
## Run
```
./build/main
```
## Unit-Testing
```
gcc -I./include test/test.c src/stack.c -o test/test
```
## Project tree
````.
├── CMakeLists.txt
├── Readme
├── build
│   ├── CMakeCache.txt
│   ├── CMakeFiles
│   │   ├── 3.16.3
│   │   │   ├── CMakeCCompiler.cmake
│   │   │   ├── CMakeCXXCompiler.cmake
│   │   │   ├── CMakeDetermineCompilerABI_C.bin
│   │   │   ├── CMakeDetermineCompilerABI_CXX.bin
│   │   │   ├── CMakeSystem.cmake
│   │   │   ├── CompilerIdC
│   │   │   │   ├── CMakeCCompilerId.c
│   │   │   │   ├── a.out
│   │   │   │   └── tmp
│   │   │   └── CompilerIdCXX
│   │   │       ├── CMakeCXXCompilerId.cpp
│   │   │       ├── a.out
│   │   │       └── tmp
│   │   ├── CMakeDirectoryInformation.cmake
│   │   ├── CMakeOutput.log
│   │   ├── CMakeTmp
│   │   ├── Makefile.cmake
│   │   ├── Makefile2
│   │   ├── TargetDirectories.txt
│   │   ├── cmake.check_cache
│   │   ├── progress.marks
│   │   └── stack.dir
│   │       ├── C.includecache
│   │       ├── DependInfo.cmake
│   │       ├── build.make
│   │       ├── cmake_clean.cmake
│   │       ├── depend.internal
│   │       ├── depend.make
│   │       ├── flags.make
│   │       ├── link.txt
│   │       ├── progress.make
│   │       └── src
│   │           ├── main.c.o
│   │           └── stack.c.o
│   ├── Makefile
│   ├── cmake_install.cmake
│   └── stack
├── include
│   └── stack.h
├── src
│   ├── main.c
│   └── stack.c
└── test
    ├── test
    └── test.c

13 directories, 38 files
````