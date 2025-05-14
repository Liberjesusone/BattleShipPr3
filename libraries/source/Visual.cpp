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
    std::string Resources::water_png = "PixelWater.jpg";
    std::string Resources::radar_png = "PixelRadar.jpg";
    std::string Resources::wood_table_png = "WoodTable.jpg";
    std::string Resources::failed_png = "fallo.jpg";
    std::string Resources::menu_png = "menu.jpg";
    std::string Resources::boat_body_png = "BoteNavalBody.png";

    std::string Resources::titulos_ttf = "tittle_font.ttf";

    sf::Texture Resources::get_texture(std::string texture_name)
    {
        if (!Resources::textures.contains(texture_name))            // Si no la hemos cargado
        {
            std::string path = Resources::textures_fd + texture_name;
            sf::Texture image;
            if (!image.loadFromFile(path))              // Cargamos la textura
            {
                std::runtime_error{"Couldn't load image from file" + path};
            }
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
    std::string Resources::water_image() 
    { 
        return Resources::water_png; 
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

    std::string Resources::titulos_font()
    {
        return Resources::titulos_ttf;
    }



}
