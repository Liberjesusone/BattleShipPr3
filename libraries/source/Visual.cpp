#include "Visual.hpp"


namespace Painter
{
    // Clase Resources

    std::unordered_map<std::string, sf::Texture> Resources::textures = {}; 
    std::unordered_map<std::string, sf::Font> Resources::fuentes = {};     

    std::string Resources::resources_fd = "resources/";
    std::string Resources::textures_fd = resources_fd + "textures/";
    std::string Resources::fonts_fd = resources_fd + "fonts/";

    std::string Resources::boat_png = "boteNaval-S.png";
    std::string Resources::missile_png = "missile.png";
    std::string Resources::heart_png = "heart.png";
    std::string Resources::water_player_png = "water_player.png";
    std::string Resources::radar_png = "water_enemy.png";
    std::string Resources::wood_table_png = "WoodTable.jpg";
    std::string Resources::failed_png = "failed.png";
    std::string Resources::menu_png = "menu.jpg";
    std::string Resources::boat_body_png = "BoteNavalBody.png";
    std::string Resources::shield_png = "shield.png";
    std::string Resources::fire_png = "fire.png";

    std::string Resources::titulos_ttf = "tittle_font.ttf";

    sf::Texture& Resources::get_texture(std::string texture_name)
    {
        if (!Resources::textures.contains(texture_name))            // Si no la hemos cargado
        {
            std::string path = Resources::textures_fd + texture_name;
            sf::Texture image;
            if (!image.loadFromFile(path))              // Cargamos la textura
            {
                std::runtime_error{"Couldn't load image from file" + path};
            }
            Resources::textures.insert({texture_name, image});
        }
        return Resources::textures[texture_name];
    }
    
    sf::Font Resources::get_font(std::string font_name)
    {
        if (!Resources::fuentes.contains(font_name))            // Si no la hemos cargado
        {
            std::string path = Resources::fonts_fd + font_name;
            sf::Font font;
            if (!font.loadFromFile(path))              // Cargamos la textura
            {
                std::runtime_error{"Couldn't load image from file" + path};
            }
            Resources::fuentes.insert({font_name, font});
        }
        return Resources::fuentes[font_name];
    }

    std::string Resources::boat_image()
    {
        return Resources::boat_png; 
    }
    std::string Resources::missile_image() 
    {
        return Resources::missile_png; 
    }
    std::string Resources::heart_image() 
    { 
        return Resources::heart_png; 
    }
    std::string Resources::water_player_image() 
    { 
        return Resources::water_player_png; 
    }
    std::string Resources::radar_image() 
    { 
        return Resources::radar_png; 
    }
    std::string Resources::wood_table_image() 
    { 
        return Resources::wood_table_png; 
    }
    std::string Resources::failed_image() 
    { 
        return Resources::failed_png; 
    }
    std::string Resources::menu_image() 
    { 
        return Resources::menu_png; 
    }
    std::string Resources::boat_body_image()
    {
        return Resources::boat_body_png;
    }
    std::string Resources::shield_image()
    {
        return Resources::shield_png;
    }
    std::string Resources::fire_image()
    {
        return Resources::fire_png;
    }

    std::string Resources::titulos_font()
    {
        return Resources::titulos_ttf;
    }


    // Clase Drawer

    void Drawer::draw(std::vector<sf::Sprite>& sprites_cells, Map& mapa, Boat bote)
    {
        for (auto boat_coord : bote.get_boat_coordinates())
        {
            // coordenada y representa filas, es decir y * cant columas (casillas por filas) = indice 1
            // coordenada x representa columnas es decir x = indice 2
            // el indice en el vector es = indice1 + indice2 
            size_t vec_indx = boat_coord.second * mapa.get_columns()  + boat_coord.first;
            std::cout << "Seting cell [" << boat_coord.first << ", " << boat_coord.second << "] as boat\n"; 

            auto scale = sprites_cells[vec_indx].getTexture()->getSize();
            auto complete_scale = sprites_cells[vec_indx].getScale();

            auto position = sprites_cells[vec_indx].getPosition();
            sf::Texture& texture = Resources::get_texture(Resources::boat_body_image());

            sprites_cells[vec_indx].setTexture(texture);
            sprites_cells[vec_indx].setPosition(position);
            sprites_cells[vec_indx].setScale(complete_scale);

            mapa.set_boat(mapa.get_ptr_cell(boat_coord.first, boat_coord.second));
        }
    }
}