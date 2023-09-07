# App-Mensajeria-2.0
[![Cliente - Linux](https://github.com/GrageraE/App-Mensajeria-2.0/actions/workflows/Cliente-linux-cmake.yml/badge.svg)](https://github.com/GrageraE/App-Mensajeria-2.0/actions/workflows/Cliente-linux-cmake.yml)
[![Servidor - Linux](https://github.com/GrageraE/App-Mensajeria-2.0/actions/workflows/Servidor-linux-cmake.yml/badge.svg)](https://github.com/GrageraE/App-Mensajeria-2.0/actions/workflows/Servidor-linux-cmake.yml)

Nueva versión del programa, escrita desde cero
## Lanzamientos
Están disponibles versiones precompiladas para Windows, Linux y Android. En caso de Linux, se necesitan instalar las dependencias requeridas por Qt, incluyendo el submódulo QtWebSockets.
## Cifrado -- modo seguro
Por el momento será una característica de la versión 2.0. Se puede probar una versión preeliminar compilando este repositorio. La documentación está disponible [aquí](https://github.com/GrageraE/App-Mensajeria-2.0/blob/main/docs/MODO_SEGURO.md).
## Compilación
### Requisitos
1. Compilador de C++ que soporte Qt 6
2. CMake
3. Qt 6
4. El submódulo QtWebSockets
5. OpenSSL (necesario para las funciones de crifrado - modo seguro)
6. Para compilar en Android, el NDK
### Windows y Android
Lo mejor es usar Qt Creator
### Linux
Debería compilarse el Cliente con:
```
mkdir build_Cliente && cd build_Cliente
cmake ../Cliente
make
```
Y el servidor con:
```
mkdir build_Servidor && cd build_Servidor
cmake ../Servidor
make
```
