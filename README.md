[![CD](https://github.com/humbertodias/gl-triforce/actions/workflows/cd.yml/badge.svg)](https://github.com/humbertodias/gl-triforce/actions/workflows/cd.yml)

OpenGL Triforce
===

Ubuntu dependencies
```shell
sudo apt update
sudo apt install libgl1-mesa-dev libglew-dev
```

Build & Run
```shell
cmake -S . -B build
cmake --build build
./build/triforce
```
![output](https://github.com/humbertodias/gl-triforce/assets/9255997/d908fb9b-8213-4ff3-bc35-39022a58cf33)
<!--
ffmpeg -i input.mov -vf "fps=15,scale=900:-1:flags=lanczos" -c:v gif output.gif
-->

