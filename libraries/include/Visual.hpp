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
namespace painter
{
    class Rsrc;
    class Drawer;
}

using namespace Party;
using namespace Objects;
using namespace Play;
using namespace BotLogic;
using namespace painter;

/**
 * @brief Agrupa los componentes que manejan el apartado visual de las clases abstractas
 */
namespace painter
{
    /**
     * @brief Representa los paquetes de recursos visuales como texturas y otros elementos que se deben cargar una sola vez
     * 
     * @note Maneja los diferentes tipos de recursos que se deben asignar a las clases abstractas
     */
    class Rsrc
    {
        private:
            static std::unordered_map<std::string, sf::Texture> textures; // Mapa de texturas
            static std::unordered_map<std::string, sf::Font> fuentes;     // Mapa de las fuentes

            // Nombres de las carpetas
            static std::string resources_fd;
            static std::string textures_fd;
            static std::string fonts_fd;

            // Name of the images files .png y .jpg
            static std::string boat_r_png;        
            static std::string boat_png;        
            static std::string boat_tail_png; 
            static std::string boat_tailV_png; 
            static std::string boat_front_png; 
            static std::string boat_frontV_png; 
            static std::string boat_body_png; 
            static std::string boat_bodyV_png; 
            static std::string cannon_png;
            static std::string heart_png;
            static std::string water_player_png;
            static std::string water_png;
            static std::string radar_png;
            static std::string wood_table_png;
            static std::string failed_png;
            static std::string menu_png;
            static std::string shield_png;
            static std::string explosion_png;

            
            // Name of the fonts files .ttf
            static std::string titulos_ttf;

        public:
            // Static Atributes
            static const sf::Vector2u WIN_SIZE;

            static size_t getButtonSize();
            static size_t getTextSize();


            // Default Constructor
            Rsrc() noexcept;

            // Getters

            /**
             * @brief Devuelve la textura que con el nombre que se paso
             * si no ha sido cargada, carga la textura de los archivos, si ya ha sido cargada previamente, sola la devuelve
             * 
             * @param texture_name Nombre de la textura, SE DEBE DE PASAR CON LAS FUNCIONES STATICAS _png(), ellas se encargan
             * de manejar los nombres de los archivos, Ejm: get_texture(Resources::boat_png());
             */
            static sf::Texture& getTexture(std::string texture_name);
            
            /**
             * @brief Devuelve la fuente que con el nombre que se paso
             * si no ha sido cargada, carga la fuente de los archivos, si ya ha sido cargada previamente, sola la devuelve
             * 
             * @param font_name Nombre de la textura, SE DEBE DE PASAR CON LAS FUNCIONES STATICAS _ttf(), ellas se encargan
             * de manejar los nombres de los archivos, Ejm: get_texture(Resources::boat_png());
             */
            static sf::Font getFont(std::string font_name);


            // Static Functions 
            static std::string boat_r_image();
            static std::string boat_image();
            static std::string boat_tail_image();
            static std::string boat_tailV_image();
            static std::string boat_front_image();
            static std::string boat_frontV_image();
            static std::string boat_body_image();
            static std::string boat_bodyV_image();
            static std::string cannon_image();
            static std::string heart_image();
            static std::string water_player_image();
            static std::string water_image();
            static std::string radar_image();
            static std::string wood_table_image();
            static std::string failed_image();
            static std::string menu_image();
            static std::string shield_image();
            static std::string destroyed_image();

            static std::string titulosFont();

            /**
             * @brief Carga las texturas de varios elementos comunes entre las ventans de juego en una sola tanda
             */
            static void load_textures(sf::Texture& water_texture, sf::Texture& ship_texture, sf::Texture& failedTexture, 
                                      sf::Texture& destroyed_texture, sf::Texture& radar_texture, sf::Texture& wood_table_texture,
                                      sf::Texture& missileTexture, sf::Texture& comodinTexture, sf::Texture& shildTexture);

            /**
             * @brief Crea una textura de un circulo gris un poco transparente con un borde grisaceo
             * para representar las casillas vacias de un mapa = Water_cell
             * 
             * @param CELL_SIZE_X el tamaño que tendra cada celda en el eje x
             * @param CELL_SIZE_y el tamaño que tendra cada celda en el eje y
             * 
             * @return la textura creada
             */
            static sf::Texture& createCircleTexture(float CELL_SIZE_X, float CELL_SIZE_Y);

            /**
             * @brief Crea una textura personalizada con un botón dentro de un círculo.
             *
             * Esta función genera una textura que contiene un círculo amarillo con borde negro
             * y un botón centrado en su interior. Se utiliza `sf::RenderTexture` para construir
             * la textura de manera dinámica y devolverla como un `sf::Texture`.
             *
             * @param CELL_SIZE_X Ancho de la textura a generar.
             * @param CELL_SIZE_Y Alto de la textura a generar.
             * @param texture Textura del botón a colocar en el centro del círculo.
             * @return sf::Texture Textura generada con el botón dentro del círculo.
             */
            static sf::Texture& create_special_button(float CELL_SIZE_X, float CELL_SIZE_Y, sf::Texture& texture);
            
    };

    class Drawer
    {
        private:

        public: 
            // Constructor por defecto
            Drawer() noexcept;


            // Adjust the size of an scalable Element, modifing its scale, base on the passed Vector
            template <typename Element>
            static void setSize(Element& element, sf::Vector2f size) 
            {
                auto eleSize = element.getTexture()->getSize();
                element.setScale(size.x / static_cast<float>(eleSize.x), size.y / static_cast<float>(eleSize.y));
            }


            // Dibuja un bote en el mapa y cambia los sprites de celdas por botes
            static void draw(std::vector<sf::Sprite>& sprites_cells, Map_ptr mapa, Boat bote, bool isBoat);

            // Dibuja un sprite segun las casillas de un mapa
            static void draw(std::vector<sf::Sprite>& sprites_cells, Map_ptr mapa, bool isRadar);
    };
}


#endif // VISUAL_HPP