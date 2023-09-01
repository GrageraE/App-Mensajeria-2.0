# App-Mensajeria-2.0
[![Cliente - Linux](https://github.com/GrageraE/App-Mensajeria-2.0/actions/workflows/Cliente-linux-cmake.yml/badge.svg)](https://github.com/GrageraE/App-Mensajeria-2.0/actions/workflows/Cliente-linux-cmake.yml)
[![Servidor - Linux](https://github.com/GrageraE/App-Mensajeria-2.0/actions/workflows/Servidor-linux-cmake.yml/badge.svg)](https://github.com/GrageraE/App-Mensajeria-2.0/actions/workflows/Servidor-linux-cmake.yml)

Nueva versión del programa, escrita desde cero
## Lanzamientos
Por el momento no se lanzarán versiones precompiladas, hasta que se hallan pulido las características del programa.
## Compilación
### Requisitos
1. Compilador de C++ que soporte Qt 6
2. CMake
3. Qt 6
4. El submódulo QtWebSockets
### Windows
Lo mejor es usar Qt Creator
### Linux
```
mkdir build & cd build
cmake ..
make
```
