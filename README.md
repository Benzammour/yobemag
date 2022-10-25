# yobemag

[![build](https://github.com/benzammour/yobemag/actions/workflows/quality-control.yml/badge.svg)](https://github.com/Benzammour/yobemag)
[![Coverage](https://sonarcloud.io/api/project_badges/measure?project=Benzammour_yobemag&metric=coverage)](https://sonarcloud.io/summary/new_code?id=Benzammour_yobemag)
[![License](https://img.shields.io/github/license/Benzammour/yobemag)](https://github.com/Benzammour/yobemag/blob/main/LICENSE)

## Build

```bash
rm -rf build
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=RELEASE -DOPTIMIZE=3 -DCMAKE_C_COMPILER=<gcc|clang> ..
make
```

### CMake Options

Use these options with `-D<Option>=<Value>`.

| Option             | Values                                                   | Explanation                                                                                                                                       | Requires         |
|--------------------|----------------------------------------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------|------------------|
| `CMAKE_BUILD_TYPE` | `DEBUG`,`RELEASE`                                        | `DEBUG` uses `-g` and adds the `YOBEMAG_DEBUG` compile definition                                                                                 | -                |
| `OPTIMIZE`         | `0`, `1`, `2`, `3`, `fast`, `s`, `z`                     | see, e.g., [GCC optimization options](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html)                                                   | Compiler support |
| `CMAKE_C_COMPILER` | Supported: `gcc` (11), `clang` (11, 12, 13, 14)          | The supported versions are used by our pipeline and guarantee compatibility                                                                       | -                |
| `TEST`             | `0`, `1`                                                 | Disables/enables building tests                                                                                                                   | -                |
| `COVERAGE`         | `0`, `1`                                                 | Removes/Adds instrumentation required for coverage reports                                                                                        | `TEST=1`         |
| `SANITIZE`         | gcc: `valgrind`, clang: `address`, `memory`, `undefined` | For `valgrind`, it runs the test executable with valgrind.<br>For the rest, the tests are instrumented with sanitizers but just run as executable | `clang` OR `gcc` |

### Build Targets

Use these targets with `make <Target>`

| Target     | Explanation                                                                                                                    |
|------------|--------------------------------------------------------------------------------------------------------------------------------|
|            | Default target, builds the `yobemag` executable                                                                                |
| `test`     | Builds the `yobemag_test` executable that runs unit tests from [`test/`](https://github.com/Benzammour/yobemag/tree/main/test) |
| `sanitize` | Runs `yobemag` or `yobemag_test` (depending on `TEST=<0/1>`) with the specified sanitizer (see [`OPTIMIZE`](###CMake-Options)) |
| `install`  |                                                                                                                                |

## Test

```bash
rm -rf build
mkdir build && cd build
cmake -DTEST=1 -DCMAKE_BUILD_TYPE=DEBUG -DOPTIMIZE=3 -DCMAKE_C_COMPILER=<gcc|clang> ..
make test
```

## Contributing

TODO

### Pre-commit hooks

We strongly encourage you to install pre-commit hooks that make sure your commit is clean.
The hook runs the following things:

- YAML lint
- new line at EOF check
- merge conflict check
- clang-format (in-place)

You can install the hook as follows:

```bash
pip install pre-commit
pre-commit install
```
