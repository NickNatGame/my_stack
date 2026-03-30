# My Stack

Small C project that implements a dynamic stack of integers.

The project includes:
- a reusable stack library
- a simple interactive CLI example
- a small unit test suite
- CMake-based build configuration

## Project Structure

```text
.
├── CMakeLists.txt
├── Readme.md
├── include
│   └── stack.h
├── src
│   ├── CMakeLists.txt
│   ├── main.c
│   └── stack.c
└── test
    ├── CMakeLists.txt
    └── test.c
```

## Requirements

- CMake 3.16.3 or newer
- C compiler with C11 support

## Build

Configure the project:

```bash
cmake -S . -B build
```

Build all targets:

```bash
cmake --build build
```

By default, the project uses the `Debug` build type if none is specified.

## Build Options

The project supports several CMake options that can be set during configuration.

### `CMAKE_BUILD_TYPE`

Controls the build profile.

Available examples:
- `Debug`
- `Release`

Default:

```bash
Debug
```

Example:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

### `LOG_LEVEL`

Controls how much information is printed by `dump()`.

Supported values:
- `FULL` - prints stack contents, capacity, hash, canary check results, and errors
- `MEDIUM` - prints stack contents and capacity
- `NONE` - prints only the stack contents

Default:

```bash
FULL
```

Example:

```bash
cmake -S . -B build -DLOG_LEVEL=MEDIUM
```

### `LOG_FILE_NAME`

Controls the file used for stack error logging.

Default:

```bash
/path/to/project/log.txt
```

Example:

```bash
cmake -S . -B build -DLOG_FILE_NAME=/home/nikita/my_stack/main.txt
```

By default, the log file is written to `main.txt` in the project root.

### `BUILD_TESTING`

Enables or disables the test subdirectory and the `run-tests` target.

Default:

```bash
ON
```

Example:

```bash
cmake -S . -B build -DBUILD_TESTING=OFF
```

## Example Configurations

Release build with full logging and a custom log file:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DLOG_LEVEL=FULL -DLOG_FILE_NAME=main.txt
cmake --build build
```

Release build without tests:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DLOG_LEVEL=NONE -DBUILD_TESTING=OFF
cmake --build build
```

## Run

```bash
./build/src/stack
```

The program supports three commands:
- `1` to push a number onto the stack
- `2` to pop the top value
- `3` to exit

After each operation, the current stack state is printed.

## Run Tests

Build and run the test suite:

```bash
cmake --build build --target run-tests
```

You can also run tests directly with CTest:

```bash
cd build
ctest --output-on-failure
```
