cmake_minimum_required(VERSION 3.16.3)
project(BattleShipPR3 LANGUAGES CXX)

# Configurar estándar de C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -m64 -fdiagnostics-color=always -pedantic -Ofast")

# Definir carpetas del proyecto
set(LIB_DIR ${CMAKE_SOURCE_DIR}/libraries)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/test)

include_directories(${LIB_DIR}/include)

# Encontrar archivos fuente y encabezados
file(GLOB SOURCES "${LIB_DIR}/source/*.cpp")
file(GLOB INCLUDES "${LIB_DIR}/include/*.hpp")
file(GLOB TESTS "${TEST_DIR}/source/*.cpp")

# Definir carpeta de salida para archivos objeto (.o) y ejecutables (.exe)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR}/obj)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TEST_DIR}/bin)

# Crear la librería estática con los archivos en Libraries/Source/
add_library(battleship STATIC ${SOURCES})

# Vincular SFML a la librería
target_link_libraries(battleship PRIVATE sfml-graphics sfml-window sfml-system)

# Compilar los ejecutables dentro de Test/Source/
foreach(TEST_FILE ${TESTS})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME} PRIVATE battleship sfml-graphics sfml-window sfml-system)
endforeach()