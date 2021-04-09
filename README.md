# grid-map-raycasting

C++ component including Python bindings to raycast a gridmap from a viewpoint to check for occlusions

## Prerequisites

**On Unix (Linux, OS X)**

* A compiler with C++11 support
* CMake >= 3.12
* Ninja or Pip 10+

**On Windows**

* Visual Studio 2015 or newer (required for all Python versions, see notes below)
* CMake >= 3.12 (3.8 was the first version to support VS 2015) or Pip 10+

## Installation
Please install the following C++ dependencies to use this repo:

```bash
conda install cmake pybind11 eigen
```
or alternatively on macOS with homebrew:
```bash
brew install cmake pybind11 eigen
```
or on Ubuntu with:
```bash
sudo apt install cmake python-pybind11 libeigen3-dev
```

On Windows we need to additionally install Visual Studio C++ to build Python packages and subsequently install dlib via pip:
```
pip install dlib
```

With the `setup.py` file included in this example, the `pip install` command will
invoke CMake and build the pybind11 module as specified in `CMakeLists.txt`.

## Usage
Please refer to the following sample code:
```python
import grid_map_raycasting as m
import faulthandler; faulthandler.enable()
from matplotlib import pyplot as plt
import numpy as np

# Vantage point (e.g. robot position) is at x=0, y=0, z=1
vantage_point = np.array([0., 0., 1.], dtype=np.double)
# We define a flat Digital Elevation Map (DEM) at the ground-level
grid_map = np.zeros((64, 64), dtype=np.double)
grid_map[0:30, 0:10] = 3
# We define the resolution of the grid of the DEM in x and y direction
grid_resolution = np.array([1, 1], dtype=np.double)

# we perform ray-casting to receive the occlusion mask
occlusion_mask = m.rayCastGridMap(vantage_point, grid_map, grid_resolution)

# The occlusion mask we expect to receive as the output of the ray-casting
target_occlusion_mask = np.full((64, 64), False)
target_occlusion_mask[0:29, 0:9] = True

fig, axes = plt.subplots(nrows=1, ncols=2)
axes[0].set_title("Grid map")
axes[0].matshow(grid_map)
axes[1].set_title("Occlusion mask")
axes[1].matshow(occlusion_mask)
plt.show()

assert (occlusion_mask == target_occlusion_mask).all()
```

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


