# My stack
Mini project to show, how stack works.

## Installation and build
Installation:

```
git clone NickNatGame/my_stack
```
Build using Cmake:
````
cmake -S . -B build # only first time for configuration
cmake --build build # do this each time you need to build
````
## Run
```
./build/main
```
## Unit-Testing
```
cmake --build build --target run-tests
```
## Project tree
````
.
├── CMakeLists.txt
├── Readme.md
├── build
├── include
│   └── stack.h
├── src
│   ├── main.c
│   └── stack.c
└── test
    └── test.c
    
4 directories, 6 files
````