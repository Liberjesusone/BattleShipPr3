# BattleShip

*Un clásico juego de estrategia naval con comodines especiales implementado en C++ con SFML para gráficos interactivos.*
##
## 📋 Requisitos Previos (Linux)  

Para compilar y ejecutar este proyecto, se necesita instalar:  

### 1. Compilador C++ (g++)
```bash
# Debian/Ubuntu
sudo apt install g++

# Arch Linux
sudo pacman -S gcc

# Fedora/RHEL
sudo dnf install gcc-c++
```

### 2. Cmake
```bash
# Debian/Ubuntu
sudo apt install cmake

# Arch Linux
sudo pacman -S cmake

# Fedora/RHEL
sudo dnf install cmake
```
### 3. Sfml
```bash
# Debian/Ubuntu
sudo apt install libsfml-dev

# Arch Linux
sudo pacman -S sfml

# Fedora/RHEL
sudo dnf install SFML-devel
```
##
## 🛠️ Ejecución

### 1. Descargar el archivo .zip

### 2. Descomprimir el .zip:

### 3. Abrir la terminal en la carpeta 'BattleShipPr3-main'

### 4. Crear los archivos de compilación
```bash
cmake -S. -B build
```
*NOTA: Si la versión de cmake instalada provoca un error de incompatibildad, debe ir al archivo 'CMakeLists.txt' y cambiarla:*
```bash
// Para saber la versión instalada de cmake

cmake --version
```
```bash
// En el archivo 'CMakeLists.txt' cambiar:

cmake_minimum_required(VERSION 'version')
```

### 5. Compilar
```bash
cmake --build build
```

### 6. Ejecutar
```bash
./test/bin/Test
```
##
## 📋 Requisitos Previos (Windows)  


### 1. MSYS2 (mingw64)
Entorno necesario para la compilación. Mas informacion de este en [MSYS2](https://www.msys2.org/).

**- 1 Descargar la terminal de MSYS" mingw64**
Para descargar este entorno puedes seguir los pasos del siguiente video para descargarlo desde la pagina original 
[Compilar y Depurar C/C++ en Visual Studio Code con MinGW y MSYS2](https://www.youtube.com/watch?v=bvNDbo347b8)

Una vez descargado el instalador, ejecutarlo para descargar la aplicacion y abrir la terminal MSYS2 mingw64 o MSYS2 mingw32 
segun la arquitectura de tu pc, si es de 64 o 32 bits

**- 2 Una vez instalado**
Actualizar los paquetes de MSYS2
Antes de instalar cualquier cosa en MSYS2, asegúrate de que tu entorno está actualizado:

```bash
pacman -Syu
```
Si se te pide cerrar y reiniciar la terminal, hazlo y vuelve a ejecutar el comando.

---

### 2. Git
Necesario para clonar el repositorio. Puedes descargarlo desde [git-scm.com](https://git-scm.com/).
```bash
pacman -S mingw-w64-x86_64-git
```
Esto instalará Git dentro de MSYS2 lo cual funcionara para nuestro caso, pero no estará disponible en el CMD o PowerShell de Windows.
(Opcional) Si deseeas instalarlo en toda tu pc sigue los pasos de este video [Descarga e Instalación de Git](https://www.youtube.com/watch?v=jdXKwLNUfmg)

---

### 3. SFML 2.6.1
Biblioteca gráfica utilizada en el juego. Mas informacion sobre esta en [SFML](https://www.sfml-dev.org/).

**Busca los paquetes disponibles para SFML (esto es opcional, solo para confirmar que puedes decargar sfml desde tu entorno):**
```bash
pacman -Ss sfml
```

En la terminal se desplegaran todas las bibliotecas disponibles, revisa que entre ellas este la llamada:
```bash
mingw64/mingw-w64-x86_64-sfml 2.6.1-1 
    A simple, fast, cross-platform, and object-oriented multimedia API (mingw-w64)
```

**-1 Eliminar smfml para evitar conflictos con posibles anteriores versiones**
```bash
pacman -R mingw-w64-x86_64-sfml
```

**-2 Instalar la biblioteca**
```bash
pacman -S mingw-w64-x86_64-sfml
```

**-3 Verifica que la instalación fue exitosa con:**
```bash
pacman -Q mingw-w64-x86_64-sfml
```
Si el resultado muestra algo como:
mingw-w64-x86_64-sfml 2.6.1-1
¡Entonces ya está listo para usarse en el proyecto!

---

### 4. CMake (mínimo 3.1.0)
Herramienta de generación de archivos de construcción. Mas informacion en [CMake](https://cmake.org/download/).
El proyecto requiere de CMake 3.16.3 o superior, en MSYS2 mingw64 usa el siguiente comando en la terminal:

```bash
pacman -S mingw-w64-x86_64-cmake
```
Este comando instalará la versión más reciente disponible en los repositorios de MSYS2. 

Para verificar qué versión tienes instalada, usa:
```bash
cmake --version
```
Si el numero de la version es 3.16.3 o superior podemos continuar 

---

### **GCC compatible con C++20**
Compilador que soporte la versión requerida, este proyecto requiere de la version de gcc 10 o superior. 
Puedes obtenerlo a través de MSYS2 con el siguiente comando:

```bash
pacman -S mingw-w64-x86_64-gcc
```

Para verificar qué versión tienes instalada, usa:
```bash
gcc --version
```
Si el resultado muestra algo como:
gcc.exe (Rev4, Built by MSYS2 project) 15.1.0
Copyright (C) 2025 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

Se decargo correctamente 

##
## 🎮 ¿Cómo Jugar?

### Fase de Colocación
1. **Coloca tu flota** en el tablero:
     - Barco de **5 casillas** 
     - Brcos de **4 casillas** 
     - Barcos de **3 casillas** 
     - Barcos de **2 casillas** 
     - Barcos de **1 casillas** 
  
### Fase de Batalla
1. **Dispara tus proyectiles** para hundir la flota enemiga
2. **Usa tus comodines** estratégicamente:
   - 🛡️ **Escudo**: Protege una casilla de tu barco
   - ❤️ **Curación**: Repara una casilla dañada
   - **Super Proyectil**: Destruye un barco entero.

### Objetivo Final
**Hundir todos los barcos del enemigo** antes de que destruyan los tuyos