#include "Visual.hpp"
#include "Types.hpp"

namespace painter
{
    /**
     * @brief Paleta de colores
     * E7F5FD -> Blanco Nuve
     * 26547A -> Azul Mar oscuro
     * 754C2D -> Marron Madera Oscuro
     * 736961 -> Marron Madera Descolorado
     * 215687 -> Azul Mar
     * 333636 -> Sombra Oscuro 
     * 60B3F0 -> Azul celeste
     * A48B44 -> Madera Amarilla clara
     */



    // Clase Resources

    std::unordered_map<std::string, sf::Texture> Rsrc::textures = {}; 
    std::unordered_map<std::string, sf::Font> Rsrc::fuentes = {};     

    std::string Rsrc::resources_fd = "resources/";
    std::string Rsrc::textures_fd = resources_fd + "textures/";
    std::string Rsrc::fonts_fd = resources_fd + "fonts/";

    std::string Rsrc::boat_r_png = "PixelArtBoatRemoved.png";
    std::string Rsrc::boat_png = "PixelArtBoat.png";
    std::string Rsrc::boat_tail_png = "PixelArtBoatTail.png";
    std::string Rsrc::boat_tailV_png = "PixelArtBoatTailV.png";
    std::string Rsrc::boat_front_png = "PixelArtBoatFront.png";
    std::string Rsrc::boat_frontV_png = "PixelArtBoatFrontV.png";
    std::string Rsrc::boat_body_png = "PixelArtBoatBody.png";
    std::string Rsrc::boat_bodyV_png = "PixelArtBoatBodyV.png";
    std::string Rsrc::cannon_png = "PixelArtCannon.jpeg";
    std::string Rsrc::heart_png = "PixelArtTools.png";
    std::string Rsrc::water_player_png = "PixelArtWaterMap.jpg";
    std::string Rsrc::water_png = "PixelArtWater.jpg";
    std::string Rsrc::radar_png = "PixelArtWater.jpg";
    std::string Rsrc::wood_table_png = "PixelWoodTable.jpg";
    std::string Rsrc::failed_png = "PixelWater.jpg";
    std::string Rsrc::menu_png = "PixelArtMenu.jpg";
    std::string Rsrc::shield_png = "PixelArtShild.png";
    std::string Rsrc::explosion_png = "PixelArtExplosion.png";

    std::string Rsrc::titulos_ttf = "tittle_font.ttf";

    const sf::Vector2u Rsrc::WIN_SIZE = sf::Vector2u(1920, 1080);
    
    size_t Rsrc::getButtonSize() 
    {
        return WIN_SIZE.x * 0.08; 
    }
    size_t Rsrc::getTextSize()
    {
        return WIN_SIZE.y * 0.05;
    }

    Rsrc::Rsrc() noexcept
    {
        // Empty
    }

    sf::Texture& Rsrc::getTexture(std::string texture_name)
    {
        if (!Rsrc::textures.contains(texture_name))            // Si no la hemos cargado
        {
            std::string path = Rsrc::textures_fd + texture_name;
            sf::Texture image;
            if (!image.loadFromFile(path))              // Cargamos la textura
            {
                std::runtime_error{"Couldn't load image from file" + path};
            }
            Rsrc::textures.insert({texture_name, image});
        }
        return Rsrc::textures[texture_name];
    }
    
    sf::Font Rsrc::getFont(std::string font_name)
    {
        if (!Rsrc::fuentes.contains(font_name))            // Si no la hemos cargado
        {
            std::string path = Rsrc::fonts_fd + font_name;
            sf::Font font;
            if (!font.loadFromFile(path))              // Cargamos la textura
            {
                std::runtime_error{"Couldn't load image from file" + path};
            }
            Rsrc::fuentes.insert({font_name, font});
        }
        return Rsrc::fuentes[font_name];
    }

    std::string Rsrc::boat_r_image()
    {
        return Rsrc::boat_r_png; 
    }
    std::string Rsrc::boat_image()
    {
        return Rsrc::boat_png; 
    }
    std::string Rsrc::boat_tail_image()
    {
        return Rsrc::boat_tail_png; 
    }
    std::string Rsrc::boat_tailV_image()
    {
        return Rsrc::boat_tailV_png; 
    }
    std::string Rsrc::boat_front_image()
    {
        return Rsrc::boat_front_png; 
    }
    std::string Rsrc::boat_frontV_image()
    {
        return Rsrc::boat_frontV_png; 
    }
    std::string Rsrc::boat_body_image()
    {
        return Rsrc::boat_body_png; 
    }
    std::string Rsrc::boat_bodyV_image()
    {
        return Rsrc::boat_bodyV_png; 
    }
    std::string Rsrc::cannon_image() 
    {
        return Rsrc::cannon_png; 
    }
    std::string Rsrc::heart_image() 
    { 
        return Rsrc::heart_png; 
    }
    std::string Rsrc::water_player_image() 
    { 
        return Rsrc::water_player_png; 
    }
    std::string Rsrc::water_image() 
    { 
        return Rsrc::water_png; 
    }
    std::string Rsrc::radar_image() 
    { 
        return Rsrc::radar_png; 
    }
    std::string Rsrc::wood_table_image() 
    { 
        return Rsrc::wood_table_png; 
    }
    std::string Rsrc::failed_image() 
    { 
        return Rsrc::failed_png; 
    }
    std::string Rsrc::menu_image() 
    { 
        return Rsrc::menu_png; 
    }
    std::string Rsrc::shield_image()
    {
        return Rsrc::shield_png;
    }
    std::string Rsrc::destroyed_image()
    {
        return Rsrc::explosion_png;
    }

    std::string Rsrc::titulosFont()
    {
        return Rsrc::titulos_ttf;
    }

    void Rsrc::load_textures(sf::Texture& water_texture, sf::Texture& ship_texture, sf::Texture& failedTexture, 
                             sf::Texture& destroyed_texture, sf::Texture& radar_texture, sf::Texture& wood_table_texture,
                             sf::Texture& missileTexture, sf::Texture& comodinTexture, sf::Texture& shildTexture)
    {
        water_texture = Rsrc::getTexture(Rsrc::water_player_image());
        ship_texture = Rsrc::getTexture(Rsrc::boat_r_image());
        failedTexture = Rsrc::getTexture(Rsrc::failed_image());
        destroyed_texture = Rsrc::getTexture(Rsrc::destroyed_image());
        radar_texture = Rsrc::getTexture(Rsrc::radar_image());
        wood_table_texture = Rsrc::getTexture(Rsrc::wood_table_image());
        missileTexture = Rsrc::create_special_button(Rsrc::WIN_SIZE.x * 0.05,  Rsrc::WIN_SIZE.y * 0.05, Rsrc::getTexture(Rsrc::cannon_image()));
        comodinTexture = Rsrc::getTexture(Rsrc::heart_image());
        shildTexture = Rsrc::getTexture(Rsrc::shield_image());
    }

    sf::Texture& Rsrc::createCircleTexture(float CELL_SIZE_X, float CELL_SIZE_Y)
    {
        if (Rsrc::textures.contains("Circle_image.png"))
        {
            return Rsrc::textures["Circle_image.png"];
        }
        
        // Crear textura con espacio totalmente transparente
        sf::RenderTexture render_texture; 
        render_texture.create(CELL_SIZE_X, CELL_SIZE_Y);

        // Limpiar con transparencia total
        render_texture.clear(sf::Color::Transparent); 

        // Ajustar grosor del border dentro del espacio visible
        float borderr_thickness = 5.0f;                                // Grosor del border negro
        sf::RectangleShape border(sf::Vector2f(CELL_SIZE_X - borderr_thickness, CELL_SIZE_Y - borderr_thickness));
        border.setFillColor(sf::Color::Transparent);                  // Para que el interior no se opaque
        border.setOutlineColor(sf::Color(125, 125, 125, 125));         // Color del border
        border.setOutlineThickness(borderr_thickness);                 // Grosor ajustado
        border.setPosition(borderr_thickness / 2, borderr_thickness / 2); // Centrar el border dentro del espacio

        // Crear el círculo translúcido en el centro
        double radius = static_cast<double>(std::min(CELL_SIZE_X, CELL_SIZE_Y)) / 4;
        sf::CircleShape punto(radius);
        punto.setFillColor(sf::Color(128, 128, 128, 150)); // Gris translúcido
        punto.setPosition((CELL_SIZE_X - radius) * 0.4, (CELL_SIZE_Y - radius) * 0.4); 

        // Dibujar en la textura
        render_texture.draw(border);  // Primero el border negro (ahora visible)
        render_texture.draw(punto);  // Luego el círculo translúcido en el centro
        render_texture.display();

        // Extraer la textura generada
        sf::Texture cell_texture = render_texture.getTexture();
        textures.insert({"Circle_image.png", cell_texture});
        return textures["Circle_image.png"];
    }

    sf::Texture& Rsrc::create_special_button(float CELL_SIZE_X, float CELL_SIZE_Y, sf::Texture& texture)
    {
        if (Rsrc::textures.contains("Special_button.png"))
        {
            return textures["Special_button.png"];
        }

        // Crear textura con espacio totalmente transparente
        sf::RenderTexture render_texture; 
        render_texture.create(CELL_SIZE_X, CELL_SIZE_Y);

        // Limpiar con transparencia total
        render_texture.clear(sf::Color::Transparent); 

        // Ajustar grosor del border dentro del espacio visible
        float borderr_thickness = 5.0f;  // Grosor del border negro
        double radius = static_cast<double>(std::min(CELL_SIZE_X, CELL_SIZE_Y)) / 2; // Reducir el radio para que todo encaje

        // Crear círculo amarillo con border negro
        sf::CircleShape border(radius - borderr_thickness);
        border.setFillColor(sf::Color::Yellow);       // Color interno amarillo
        border.setOutlineColor(sf::Color::Black);     // border negro
        border.setOutlineThickness(borderr_thickness); // Grosor ajustado
        border.setPosition((CELL_SIZE_X - border.getRadius() * 2) / 2, (CELL_SIZE_Y - border.getRadius() * 2) / 2); // Centrarlo

        // Crear el círculo del boton en el centro
        sf::Sprite button(texture);
        button.setScale((radius * 1.5) / texture.getSize().x, (radius * 1.5) / texture.getSize().y);
        button.setPosition((CELL_SIZE_X - button.getGlobalBounds().width) / 2, 
                        (CELL_SIZE_Y - button.getGlobalBounds().height) / 2); // Centrar

        // Dibujar en la textura
        render_texture.draw(border);  // Primero el círculo amarillo con border negro
        render_texture.draw(button);  // Luego el boton centrado en el círculo
        render_texture.display();

        // Extraer la textura generada
        sf::Texture circle_texture = render_texture.getTexture();
        Rsrc::textures.insert({"Special_button.png", circle_texture});
        return textures["Special_button.png"];
    }





    // Clase Drawer

    void Drawer::draw(std::vector<sf::Sprite>& spritesCells, Map_ptr mapa, Boat bote, bool isBoat)
    {
        size_t boatSize = bote.get_size();
        bool isHorizontal = bote.isHorizontal();
        auto cellSize = spritesCells[0].getGlobalBounds();

        int i = 0;
        for (auto boat_coord : bote.get_boat_coordinates())
        {
            // coordenada y representa filas, es decir y * cant columas (casillas por filas) = indice 1
            // coordenada x representa columnas es decir x = indice 2
            // el indice en el vector es = indice1 + indice2 
            size_t vecIndx = boat_coord.second * mapa->get_columns()  + boat_coord.first; 

            sf::Sprite sprite;
            auto position = spritesCells[vecIndx].getPosition();

            if (!isBoat)
            {
                sprite.setTexture(Rsrc::createCircleTexture(cellSize.width, cellSize.height));
            }
            else if (boatSize == 1)
            {
                sprite.setTexture(Rsrc::getTexture(Rsrc::boat_image())); 
            }
            else if (!i)
            {
                isHorizontal? sprite.setTexture(Rsrc::getTexture(Rsrc::boat_tail_image())) : 
                              sprite.setTexture(Rsrc::getTexture(Rsrc::boat_tailV_image()));
            }
            else if (i == boatSize - 1)
            {
                isHorizontal? sprite.setTexture(Rsrc::getTexture(Rsrc::boat_front_image())) : 
                              sprite.setTexture(Rsrc::getTexture(Rsrc::boat_frontV_image()));
            }
            else 
            {
                isHorizontal? sprite.setTexture(Rsrc::getTexture(Rsrc::boat_body_image())) : 
                              sprite.setTexture(Rsrc::getTexture(Rsrc::boat_bodyV_image()));
            }

            sprite.setPosition(position);
            Drawer::setSize(sprite, sf::Vector2f(cellSize.width, cellSize.height));

            spritesCells[vecIndx] = sprite;
            mapa->set_boat(mapa->get_ptr_cell(boat_coord.first, boat_coord.second));
            ++i;
        }
    }

    void Drawer::draw(std::vector<sf::Sprite>& sprites_cells, Map_ptr mapa, bool isRadar)
    {
        for (int y = 0; y < mapa->get_rows(); ++y)
        {
            for (int x = 0; x < mapa->get_columns(); ++x)
            {
                size_t vec_indx = y * mapa->get_columns() + x;
                auto position = sprites_cells[vec_indx].getPosition();
                auto scale = sprites_cells[vec_indx].getScale();

                if (mapa->is_boat(x, y) && !isRadar)
                {
                    sprites_cells[vec_indx].setTexture(Rsrc::getTexture(Rsrc::boat_body_image()));
                }
                else if (mapa->is_water(x, y))
                {
                    sprites_cells[vec_indx].setTexture(Rsrc::getTexture( (isRadar ? Rsrc::radar_image() : Rsrc::water_player_image()) ) );
                }
                else if (mapa->is_failed(x, y))
                {
                    sprites_cells[vec_indx].setTexture(Rsrc::getTexture(Rsrc::failed_image()));
                }
                else if (mapa->is_destroyed(x, y))
                {
                    sprites_cells[vec_indx].setTexture(Rsrc::getTexture(Rsrc::destroyed_image()));
                }
                else if (!isRadar)
                {
                    sprites_cells[vec_indx].setTexture(Rsrc::getTexture(Rsrc::shield_image()));
                }
                sprites_cells[vec_indx].setScale(scale);
                sprites_cells[vec_indx].setPosition(position);
            }
        }
    }
}