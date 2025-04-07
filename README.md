[![CD](https://github.com/humbertodias/gl-triforce/actions/workflows/cd.yml/badge.svg)](https://github.com/humbertodias/gl-triforce/actions/workflows/cd.yml)
![GitHub all releases](https://img.shields.io/github/downloads/humbertodias/gl-triforce/total)

# OpenGL Triforce

Download the [latest release](https://github.com/humbertodias/gl-triforce/releases) for Windows, Linux, or macOS — or build it yourself:

### Build from Source

1. **Clone the Repository**

```bash
git clone --recurse-submodules https://github.com/humbertodias/gl-triforce.git
cd gl-triforce
```

2. **Build and Run**

```bash
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=$PWD/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build
./build/triforce
```


![output](https://github.com/humbertodias/gl-triforce/assets/9255997/d908fb9b-8213-4ff3-bc35-39022a58cf33)
<!--
ffmpeg -i input.mov -vf "fps=15,scale=900:-1:flags=lanczos" -c:v gif output.gif
-->

