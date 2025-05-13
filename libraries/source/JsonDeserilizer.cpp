#include "JsonDeserilizer.hpp"

/**
 * @brief Construccion del metodo nameof para obtener el nombre de una variable
 * USO: nameof(NAME(this->espacio.variable))
 * la idea es poder llamar a este metodo con la variable en si, para en caso de refactorizar el nombre de la variable se
 * actualice automaticamente en el metodo nameof, sin necesidad de cambiarlo manualmente
 * 
 * @note Se usa el preprocesador para obtener el string completo de la variable ejemplo: this->espacio.variable
 * @note Ese valor se le pasa al metodo nameof, que busca el ultimo delimitador (., ->) y devuelve el nombre de la variable
 */
std::string nameof(std::string&& nombre_variable) 
{
    // Buscar el último delimitador (::, ., ->)
    size_t pos = nombre_variable.find_last_of(".>-:");
    
    if (pos != std::string::npos) 
    {
        return nombre_variable.substr(pos + 1); // Extraer la última palabra después del delimitador
    }
    
    return nombre_variable;                     // Si no hay delimitadores, devolver el string original
}

namespace JsonDeserilizer
{
    void to_json(json& j, const Usuario& user) 
    {
        j = json{ {"id", user.id}, {"nombre", user.nombre} };
    }

    void from_json(const json& j, Usuario& user) 
    {
        j.at("id").get_to(user.id);         // Obtener el id del JSON
        j.at("nombre").get_to(user.nombre); // Obtener el nombre del JSON
    }

    void eliminar_object_json(const std::string& archivo, int id_eliminar) 
    {
        std::ifstream entrada(archivo);
        json j;
        entrada >> j;
        entrada.close();

        auto my_fuct = [&](const json& object) {
            return object["id"] == id_eliminar;
        };
        j.erase(std::remove_if(j.begin(), j.end(), my_fuct));
    
        std::ofstream salida(archivo);
        salida << j.dump(4);
        salida.close();
    }

    void modificar_json_parcialmente(const std::string& archivo, int id_modificar, 
        const std::string attribute, const std::string& new_attribute) 
    {
        std::ifstream entrada(archivo);  // Abrir archivo JSON
        json j;
        entrada >> j;  // Cargar parcialmente el JSON en memoria
        entrada.close();

        // Modificar solo el objeto con el ID específico
        for (auto& object : j) 
        {
            if (object["id"] == id_modificar) 
            {
                object[attribute] = new_attribute;
                break;  // Detener cuando se encuentre el objecto
            }
        }

        std::ofstream salida(archivo);  // Escribir el archivo modificado
        salida << j.dump(4);
        salida.close();
    }    

    int test() 
    {
        std::vector<Usuario> usuarios = { {1, "Liber"}, {2, "Maria"}, {3, "Carlos"} };
        guardar_en_json(usuarios, "usuarios.json");

        usuarios = cargar_desde_json<Usuario>("usuarios.json");

        auto my_funct = [](std::vector<Usuario>& usuarios)
        {
            for (const auto& usuario : usuarios) 
            {
                std::cout << "ID: " << usuario.id << ", Nombre: " << usuario.nombre << std::endl;
            }
            std::cout << std::endl;
        };
        my_funct(usuarios); // Mostrar los usuarios cargados desde el JSON

        std::cout << "Modificando el usuario dos ahora con Nombre Maria Jose..." << std::endl;
        modificar_json_parcialmente("usuarios.json", 2, "nombre", "Maria Jose");
        usuarios = cargar_desde_json<Usuario>("usuarios.json"); // Cargar nuevamente el JSON
        my_funct(usuarios);                                     // Mostrar los usuarios modificados desde el JSON

        std::cout << "Elimando el usuario con ID 3..." << std::endl;
        eliminar_object_json("usuarios.json", 3); 
        usuarios = cargar_desde_json<Usuario>("usuarios.json"); // Cargar nuevamente el JSON
        my_funct(usuarios);                                     // Mostrar los usuarios después de la eliminación

        std::cout << "Agregando de nuevo el usuario con Carlos..." << std::endl;
        agregar_object_at_end_json("usuarios.json", Usuario{3, "Carlos"});
        usuarios = cargar_desde_json<Usuario>("usuarios.json"); // Cargar nuevamente el JSON
        my_funct(usuarios);                                     // Mostrar los usuarios después de la adición

        std::cout << "Guardado y cargado correctamente desde JSON." << std::endl;
        return 0;
    }
}

namespace ReadTxt
{
    std::string read_txt(std::ifstream& id)
    {
        std::string my_text;
        std::string line;
        
        while (std::getline(id, line))    // Leer línea por línea
        {  
            my_text += line + "\n";       // Agregar al string con salto de línea
        }
    
        return my_text;
    }
}