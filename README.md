# grid-map-raycasting

|      CI              | status |
|----------------------|--------|
| conda.recipe         | [![Conda Actions Status][actions-conda-badge]][actions-conda-link] |
| pip builds           | [![Pip Actions Status][actions-pip-badge]][actions-pip-link] |
| [`cibuildwheel`][]   | [![Wheels Actions Status][actions-wheels-badge]][actions-wheels-link] |

[actions-badge]:           https://github.com/mstoelzle/grid-map-raycasting/workflows/Tests/badge.svg
[actions-conda-link]:      https://github.com/mstoelzle/grid-map-raycasting/actions?query=workflow%3A%22Conda
[actions-conda-badge]:     https://github.com/mstoelzle/grid-map-raycasting/workflows/Conda/badge.svg
[actions-pip-link]:        https://github.com/mstoelzle/grid-map-raycasting/actions?query=workflow%3A%22Pip
[actions-pip-badge]:       https://github.com/mstoelzle/grid-map-raycasting/workflows/Pip/badge.svg
[actions-wheels-link]:     https://github.com/mstoelzle/grid-map-raycasting/actions?query=workflow%3AWheels
[actions-wheels-badge]:    https://github.com/mstoelzle/grid-map-raycasting/workflows/Wheels/badge.svg

C++ component including Python bindings to raycast a gridmap from a viewpoint to check for occlusions
An example [pybind11](https://github.com/pybind/pybind11) module built with a
CMake-based build system. This is useful for C++ codebases that have an
existing CMake project structure. This is in many cases superseded by
[`scikit_build_example`](https://github.com/pybind/scikit_build_example), which uses
[scikit-build][], a tool from the makers of CMake designed to allow Python
packages to be driven from CMake. However, there are still cases where you
might want full control over the CMake run; and both of these approaches have
some trade-offs not present in a pure setuptools build (see
[`python_example`](https://github.com/pybind/python_example))

## Prerequisites

**On Unix (Linux, OS X)**

* A compiler with C++11 support
* CMake >= 3.4 or Pip 10+
* Ninja or Pip 10+

**On Windows**

* Visual Studio 2015 or newer (required for all Python versions, see notes below)
* CMake >= 3.8 (3.8 was the first version to support VS 2015) or Pip 10+

## Installation
Please install the following packages to use this repo:
On macOS this can be done with:
```
brew install pybind11 cmake eigen
```
Subsequently, we can install the Python module using pip:
```
pip install -e --user .
```

## Installation

Just clone this repository and pip install. Note the `--recursive` option which is
needed for the pybind11 submodule:

```bash
git clone --recursive https://github.com/pybind/cmake_example.git
pip install ./cmake_example
```

With the `setup.py` file included in this example, the `pip install` command will
invoke CMake and build the pybind11 module as specified in `CMakeLists.txt`.


## Special notes for Windows

**Compiler requirements**

Pybind11 requires a C++11 compliant compiler, i.e Visual Studio 2015 on Windows.
This applies to all Python versions, including 2.7. Unlike regular C extension
modules, it's perfectly fine to compile a pybind11 module with a VS version newer
than the target Python's VS version. See the [FAQ] for more details.

**Runtime requirements**

The Visual C++ 2015 redistributable packages are a runtime requirement for this
project. It can be found [here][vs2015_runtime]. If you use the Anaconda Python
distribution, you can add `vs2015_runtime` as a platform-dependent runtime
requirement for you package: see the `conda.recipe/meta.yaml` file in this example.


