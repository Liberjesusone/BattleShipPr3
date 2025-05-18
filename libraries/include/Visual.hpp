#pragma once
#include "Types.hpp"

// Archivo para desarrollar funcionalidades relacionadas al apartado visual de las estructuras de datos 

namespace Painter
{
    class Resources;
    class Drawer;
}

using namespace Painter;

/**
 * @brief Agrupa los componentes que manejan el apartado visual de las clases abstractas
 */
namespace Painter
{
    /**
     * @brief Representa los paquetes de recursos visuales como texturas y otros elementos que se deben cargar una sola vez
     * 
     * @note Maneja los diferentes tipos de recursos que se deben asignar a las clases abstractas
     */
    class Resources
    {
        private:
            static std::unordered_map<std::string, sf::Texture> textures; // Mapa de texturas
            static std::unordered_map<std::string, sf::Font> fuentes;     // Mapa de las fuentes

            // Nombres de las carpetas
            static std::string resources_fd;
            static std::string textures_fd;
            static std::string fonts_fd;

            // Nombre de las imagenes .png y .jpg
            static std::string boat_png;
            static std::string missile_png;
            static std::string heart_png;
            static std::string water_player_png;
            static std::string radar_png;
            static std::string wood_table_png;
            static std::string failed_png;
            static std::string menu_png;
            static std::string boat_body_png;
            static std::string shield_png;

            
            // Nombre de las fuentes
            static std::string titulos_ttf;

        public:
            // Constructor por defecto
            Resources() noexcept;

            // Getters

            /**
             * @brief Devuelve la textura que con el nombre que se paso
             * si no ha sido cargada, carga la textura de los archivos, si ya ha sido cargada previamente, sola la devuelve
             * 
             * @param texture_name Nombre de la textura, SE DEBE DE PASAR CON LAS FUNCIONES STATICAS _png(), ellas se encargan
             * de manejar los nombres de los archivos, Ejm: get_texture(Resources::boat_png());
             */
            static sf::Texture& get_texture(std::string texture_name);
            
            /**
             * @brief Devuelve la fuente que con el nombre que se paso
             * si no ha sido cargada, carga la fuente de los archivos, si ya ha sido cargada previamente, sola la devuelve
             * 
             * @param font_name Nombre de la textura, SE DEBE DE PASAR CON LAS FUNCIONES STATICAS _ttf(), ellas se encargan
             * de manejar los nombres de los archivos, Ejm: get_texture(Resources::boat_png());
             */
            static sf::Font get_font(std::string font_name);


            // Static Functions 

            static std::string boat_image();
            static std::string missile_image();
            static std::string heart_image();
            static std::string water_player_image();
            static std::string radar_image();
            static std::string wood_table_image();
            static std::string failed_image();
            static std::string menu_image();
            static std::string boat_body_image();
            static std::string shield_image();

            static std::string titulos_font();

    };

    class Drawer
    {
        private:

        public: 
            // Constructor por defecto
            Drawer() noexcept;

            // Dibuja un bote en el mapa y cambia los sprites de celdas por botes
            static void draw(std::vector<sf::Sprite>& sprites_cells, Map& mapa, Boat bote, sf::Texture& texture);
    };
}