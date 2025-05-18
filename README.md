# BattleShip

*Un clásico juego de estrategia naval con comodines especiales implementado en C++ con SFML para gráficos interactivos.*
##
## 📋 Requisitos Previos (Linux)  

Para compilar y ejecutar este proyecto, necesitas instalar:  

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

### 5. Compilar
```bash
cmake --build build
```

### 6. Ejecutar
```bash
./test/bin/Test
```
##
## 🎮 ¿Cómo Jugar?

### Fase de Colocación
1. **Coloca tu flota** en el tablero:
     - Barco de **4 casillas** 
     - Brcos de **3 casillas** 
     - Barcos de **2 casillas** 
     - Barcos de **1 casilla** 
  
### Fase de Batalla
1. **Dispara tus proyectiles** para hundir la flota enemiga
2. **Usa tus comodines** estratégicamente:
   - 🛡️ **Escudo**: Protege una casilla de tu barco
   - ❤️ **Curación**: Repara una casilla dañada

### Objetivo Final
**Hundir todos los barcos enemigos** antes de que destruyan los tuyos