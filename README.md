# App-Mensajeria-2.0
[![Cliente - Linux](https://github.com/GrageraE/App-Mensajeria-2.0/actions/workflows/Cliente-linux-cmake.yml/badge.svg)](https://github.com/GrageraE/App-Mensajeria-2.0/actions/workflows/Cliente-linux-cmake.yml)
[![Servidor - Linux](https://github.com/GrageraE/App-Mensajeria-2.0/actions/workflows/Servidor-linux-cmake.yml/badge.svg)](https://github.com/GrageraE/App-Mensajeria-2.0/actions/workflows/Servidor-linux-cmake.yml)

Nueva versión del programa, escrita desde cero
## Lanzamientos
Están disponibles versiones precompiladas para Windows, Linux y Android. En caso de Linux, se necesitan instalar las dependencias requeridas por Qt, incluyendo el submódulo QtWebSockets.
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
