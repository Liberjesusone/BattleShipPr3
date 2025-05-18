# BattleShipPR3
Proyecto de Pr 3, juego de mesa battle ship, con un bot que controla los movimientos del adversario

---

## Descripción  
(Aqui colocar la descripcion del proyecto )

---

## Requisitos para windows  
Antes de ejecutar el proyecto en Windows, asegúrate de tener instalados los siguientes programas:

### **MSYS2 (mingw64)** 
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

### **Git**
Necesario para clonar el repositorio. Puedes descargarlo desde [git-scm.com](https://git-scm.com/).
```bash
pacman -S mingw-w64-x86_64-git
```
Esto instalará Git dentro de MSYS2 lo cual funcionara para nuestro caso, pero no estará disponible en el CMD o PowerShell de Windows.
(Opcional) Si deseeas instalarlo en toda tu pc sigue los pasos de este video [Descarga e Instalación de Git](https://www.youtube.com/watch?v=jdXKwLNUfmg)

---

### SFML 2.6.1**
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

### **CMake (mínimo 3.1.0)**
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

## Instalación  
Sigue estos pasos para descargar y preparar el proyecto:

1. **Clona el repositorio**
Esto genera una copia del proyecto en la capeta donde te encuentres en la terminal.
```bash
git clone https://github.com/Liberjesusone/BattleShipPr3.git
```

2. **Accede a la carpeta del proyecto**:
Estara en la direccion donde hiciste la clonacion del proyecto  
```bash
cd BattleShipPr3
```

## Compilación y Ejecución  
Dentro de la carpeta principal del juego (*BattleShipPr3*, donde se encuentra el archivo `CMakeLists.txt`), ejecuta los siguientes comandos:

1. **Generar archivos de compilación**:  
   ```bash
   cmake -S . -B build
   ```
2. **Compilar el proyecto**:  
   ```bash
   cmake --build build
   ```
3. **Ejecutar el juego**:  
   ```bash
   ./test/bin/Test.exe
   ```

## Funcionamiento del juego  
(Aqui hay que describir el funcionamiento del juego)