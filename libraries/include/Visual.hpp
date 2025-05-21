#pragma once
// Archivo para desarrollar funcionalidades relacionadas al apartado visual de las estructuras de datos 

#ifndef VISUAL_HPP
#define VISUAL_HPP

#include <iostream>

#include <SFML/Graphics.hpp>		// Bibliotecas de SFML
#include <SFML/Audio.hpp>

#include <memory>
#include <string>
#include <unordered_map>

// Fowared declaration
namespace Party
{
	class Map_cell;
	class Water_cell;
	class Boat_cell;
	class Failed_cell;
	class Destroyed_cell;
	class Protected_cell;
	class Map;

	using Map_ptr = std::shared_ptr<Map>;
	using Coordinates = std::pair<size_t, size_t>;	
	using Map_cell_ptr = std::shared_ptr<Map_cell>;
	using Boat_cell_ptr = std::shared_ptr<Boat_cell>;
	using Water_cell_ptr = std::shared_ptr<Water_cell>;
	using Failed_cell_ptr = std::shared_ptr<Failed_cell>;
	using Destroyed_cell_ptr = std::shared_ptr<Destroyed_cell>;
	using Protected_cell_ptr = std::shared_ptr<Protected_cell>;
}
namespace Objects 
{
	class SingleShot;
	class ChargedShot;
	class HealCell;
	class Item;
	template <typename DamageType>
	class Rocket;
	template <typename EffectType>
	class Comodin;

	using Item_ptr = std::shared_ptr<Item>;
}
namespace Play 
{
	struct PairHash;
	class Boat;
	class Fleet;
	class Arsenal;
	class Build;
	class Player;

	using Boat_ptr = std::shared_ptr<Boat>;
}
namespace BotLogic 
{
	class Bot;

	using Movement = std::pair<Party::Coordinates, Objects::Item_ptr>;
}
namespace Painter
{
    class Resources;
    class Drawer;
}

using namespace Party;
using namespace Objects;
using namespace Play;
using namespace BotLogic;
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
            static std::string fire_png;

            
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
            static std::string fire_image();

            static std::string titulos_font();

    };

    class Drawer
    {
        private:

        public: 
            // Constructor por defecto
            Drawer() noexcept;

            // Dibuja un bote en el mapa y cambia los sprites de celdas por botes
            static void draw(std::vector<sf::Sprite>& sprites_cells, Map_ptr mapa, Boat bote, sf::Texture& texture);

            // Dibuja un sprite segun las casillas de un mapa
            static void draw(std::vector<sf::Sprite>& sprites_cells, Map_ptr mapa, bool isRadar);
    };
}


#endif // VISUAL_HPP