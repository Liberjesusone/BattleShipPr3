#pragma once
#include "Types.hpp"
#include "json.hpp"

#define NAME_OF(x) nameof(#x) //para obtener el nombre de una variable
std::string nameof(std::string&& nombre_variable);

namespace JsonDeserilizer
{
    using json = nlohmann::json;

    /** Estructura de datos para probar
     * @note Cada estructura de datos necesita sus funcinoes to_json y from_json para que al hacer la conversion de JSON 
     * a objeto y viceversa funcione
     */
    struct Usuario
    {
        int id;
        std::string nombre;
    };

    /**
     * @brief Convertir a JSON el tipo de dato Usuario y lo agrega a un objeto JSON
     * 
     * @param j Objeto JSON donde se guardará la información
     * @param user Objeto Usuario que se convertirá a JSON
     */
    void to_json(json& j, const Usuario& user);

    /**
     * @brief Convertir desde JSON el objeto Usuario
     * 
     * @param j Objeto JSON que contiene la información del usuario
     * @param user Objeto Usuario donde se guardará la información de get
     */
    void from_json(const json& j, Usuario& user);

    /** SEREALIZACION
     * @brief Guardar objetos en un archivo JSON sobreescribiendo el contenido por completo
     * 
     * @param usuarios Lista de objetos a guardar
     * @param nombre_archivo Nombre del archivo JSON
     */
    template <typename T>
    void guardar_en_json(const std::vector<T>& objets, const std::string& nombre_archivo) 
    {
        json j = objets;    // Convierte directamente la lista a JSON

        std::ofstream archivo(nombre_archivo);
        archivo << j.dump(4); // Escribir JSON con formato bonito (4 espacios de indentaciOn)
        archivo.close();
    }

    /** DESEREALIZACION
     * @brief Cargar objetos desde un archivo JSON
     * 
     * @param nombre_archivo Nombre del archivo JSON
     * 
     * @return std::vector<T> Lista de objetos cargados desde el archivo JSON
     */
    template <typename T>
    std::vector<T> cargar_desde_json(const std::string& nombre_archivo) 
    {
        std::ifstream archivo(nombre_archivo); // Abrir el archivo JSON
        json j;                                // Crear un objeto JSON
        archivo >> j;                          // Leer el contenido del archivo JSON   

        std::vector<T> objects = j.get<std::vector<T>>(); // Convertir de JSON a lista
        return objects;
    }

    /**
     * @brief Agregar un nuevo objeto al final de un archivo JSON sin sobreescribir el contenido
     * 
     * @param archivo Nombre del archivo JSON
     * @param new_object Objeto a agregar al final del archivo JSON
     */
    template <typename T>
    void agregar_object_at_end_json(const std::string& archivo, const T& new_object) 
    {
        std::fstream salida(archivo, std::ios::in | std::ios::out); // Lectura/escritura
        
        // Posicionarnos justo antes del último `]`
        salida.seekp(-3, std::ios::end);  // Retrocede solo 2 caracteres antes de "\n]"
    
        json j_new_object;
        to_json(j_new_object, new_object);  // Convertir objeto a JSON
    
        // Escribir correctamente: coma antes del nuevo objeto sin salto de línea
        salida << ",\n    " << j_new_object.dump(8);  
        salida.seekp(-3, std::ios::end);    // volvemos a retroceder por que el ultimo } no se imprime bien espaciado
        salida << "\n    }\n]";  // Cerrar el JSON correctamente
        salida.close();
    }

    /**
     * @brief Eliminar un objeto de un archivo JSON por medio de su ID
     * 
     * @param archivo Nombre del archivo JSON
     * @param id_eliminar ID del objeto a eliminar
     */
    void eliminar_object_json(const std::string& archivo, int id_eliminar);

    /**
     * @brief Modificar un objeto en un archivo JSON por medio de su ID
     * 
     * @param archivo Nombre del archivo JSON
     * @param id_modificar ID del objeto a modificar
     * @param attribute Atributo a modificar
     * @param new_attribute Nuevo valor del atributo
     */
    void modificar_json_parcialmente(const std::string& archivo, int id_modificar, 
                                     const std::string attribute, const std::string& new_attribute);
    
    int test();
}

namespace ReadTxt
{
    std::string read_txt(std::ifstream& id);
}
