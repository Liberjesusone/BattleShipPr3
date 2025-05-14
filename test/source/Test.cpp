#include <SFML/Graphics.hpp>
#include "Structures.hpp"
#include "Types.hpp"

void load_textures(sf::Texture& water_texture, sf::Texture& ship_texture, sf::Texture& miss_texture, sf::Texture& destroyed_texture, 
    sf::Texture& radar_texture, sf::Texture& layout)
{
    if (!water_texture.loadFromFile("resources/textures/water.jpg") ||
        !ship_texture.loadFromFile("resources/textures/ship.png") ||
        !miss_texture.loadFromFile("resources/textures/fallo.jpg") ||
        !destroyed_texture.loadFromFile("resources/textures/destruido.jpg") ||
        !radar_texture.loadFromFile("resources/textures/radar.jpg") ||
        !layout.loadFromFile("resources/textures/layout.jpg"))
    {
        throw std::runtime_error("No se pudieron cargar las texturas");
    }
}

sf::Texture create_cell(double CELL_SIZE_X, double CELL_SIZE_Y)
{
    // Crear textura con espacio totalmente transparente
    sf::RenderTexture render_texture; 
    render_texture.create(CELL_SIZE_X, CELL_SIZE_Y);

    // Limpiar con transparencia total
    render_texture.clear(sf::Color::Transparent); 

    // Ajustar grosor del border dentro del espacio visible
    float borderr_thickness = 5.0f;                                // Grosor del border negro
    sf::RectangleShape border(sf::Vector2f(CELL_SIZE_X - borderr_thickness, CELL_SIZE_Y - borderr_thickness));
    border.setFillColor(sf::Color::Transparent);                  // Para que el interior no se opaque
    border.setOutlineColor(sf::Color::Black);         // Color del border
    border.setOutlineThickness(borderr_thickness);                  // Grosor ajustado
    border.setPosition(borderr_thickness / 2, borderr_thickness / 2); // Centrar el border dentro del espacio

    // Crear el círculo translúcido en el centro
    double radius = static_cast<double>(std::min(CELL_SIZE_X, CELL_SIZE_Y)) / 4;
    sf::CircleShape punto(radius);
    punto.setFillColor(sf::Color(128, 128, 128, 150)); // Gris translúcido
    punto.setPosition((CELL_SIZE_X - radius) / 2, (CELL_SIZE_Y - radius) / 2); 

    // Dibujar en la textura
    render_texture.draw(border);  // Primero el border negro (ahora visible)
    render_texture.draw(punto);  // Luego el círculo translúcido en el centro
    render_texture.display();

    // Extraer la textura generada
    sf::Texture cell_texture = render_texture.getTexture();
    return cell_texture;
}

sf::Texture create_special_button(double CELL_SIZE_X, double CELL_SIZE_Y, sf::Texture texture)
{
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
    return circle_texture;
}


void play_window(sf::RenderWindow& window, sf::Font& font)
{
    //MAP DIVISIONS
    sf::Vector2u windowSize = window.getSize();
    const int INFO_WIDTH = windowSize.x / 3;         // 1/3 para la información
    const int MAP_WIDTH = windowSize.x - INFO_WIDTH; // 2/3 para mapas
    const int MAP_HEIGHT = windowSize.y;
    const int RADAR_HEIGHT = MAP_HEIGHT / 2;         // Altura de ambos mapas

    //PANELS
    sf::RectangleShape info_panel(sf::Vector2f(INFO_WIDTH, MAP_HEIGHT));     // panel de informacion a la izq
    sf::RectangleShape radar_panel(sf::Vector2f(MAP_WIDTH, RADAR_HEIGHT));  // panel del radar arriba a la derecha
    sf::RectangleShape player_panel(sf::Vector2f(MAP_WIDTH, RADAR_HEIGHT)); // panel del mapa propio abajo a la derecha

    //CELLS
    Party::Map player_map(10, 10);
    Party::Map radar_map(10, 10);
    const int CELL_SIZE_X = MAP_WIDTH / radar_map.get_columns();  // Ancho y alto de las celdas
    const int CELL_SIZE_Y = RADAR_HEIGHT / radar_map.get_rows();
    sf::Texture cell_texture = create_cell(CELL_SIZE_X, CELL_SIZE_Y);

    //TEXTURES
    sf::Text exit_button; 
    sf::Texture water_texture, ship_texture, miss_texture, destroyed_texture, radar_texture, layout;
    std::vector<sf::Sprite> enemy_cells, player_cells;
    load_textures(water_texture, ship_texture, miss_texture, destroyed_texture, radar_texture, layout);
    
    //SHIPS
    std::vector<sf::Sprite> ships;

    for (int i = 0; i < 2; ++i) 
    {
        sf::Sprite ship(ship_texture);
        double scale_factor = 0.8 - (i * 0.15);    
        double scale_factor_y = 0.3 - (i * 0.05);
        ship.setScale(scale_factor, scale_factor_y); 
        ship.setPosition(0, 100 + i * 150);
        ships.push_back(ship);
    }

    //EXIT BUTTON
    exit_button.setFont(font);
    exit_button.setString("Salir");
    exit_button.setCharacterSize(30);
    exit_button.setPosition(15,50);
    exit_button.setFillColor(sf::Color::White);

    //SHOT BUTTON
    sf::Texture button_texture; 
    button_texture.loadFromFile("resources/textures/missile.png");
    sf::Texture misil_texture = create_special_button(100, 100, button_texture);
    sf::Sprite shot_button(misil_texture);
    shot_button.setPosition(15, 400);
    shot_button.setColor(sf::Color(255,255,255,128));

    //SHIELD BUTTON
    button_texture.loadFromFile("resources/textures/shield.png");
    sf::Texture shield_texture = create_special_button(100, 100, button_texture);
    sf::Sprite shield_button(shield_texture);
    shield_button.setPosition(200, 400);
    shield_button.setColor(sf::Color(255,255,255,128));

    //PANELS PROPERTIES (POSITION AND TEXTURE)
    info_panel.setTexture(&layout);
    info_panel.setPosition(0, 0);
    radar_panel.setTexture(&radar_texture);
    radar_panel.setPosition(INFO_WIDTH, 0);
    player_panel.setTexture(&water_texture);
    player_panel.setPosition(INFO_WIDTH, RADAR_HEIGHT);

    //CELLS
    for (size_t y = 0; y < radar_map.get_rows(); ++y)             
    {
        for (size_t x = 0; x < radar_map.get_columns(); ++x) 
        {
            sf::Sprite sprite;
            sprite.setTexture(cell_texture);
            sprite.setScale(static_cast<float>(CELL_SIZE_X) / sprite.getTexture()->getSize().x,
                            static_cast<float>(CELL_SIZE_Y) / sprite.getTexture()->getSize().y);
            sprite.setPosition(INFO_WIDTH + x * CELL_SIZE_X, y * CELL_SIZE_Y);
            enemy_cells.push_back(sprite);
            sprite.setPosition(INFO_WIDTH + x * CELL_SIZE_X, RADAR_HEIGHT + y * CELL_SIZE_Y);
            player_cells.push_back(sprite);
        }
    }

    //CONTROL VARIABLES
    int x = 0;
    int y = 0;
    bool is_shot_valid = false;
    bool is_shield_valid = false;
    sf::Clock clock;

    //PLAY LOOP
    while(window.isOpen())
    {
        sf::Event event;
        float current_time = clock.restart().asSeconds();
        float fps = 1.0f / current_time;

        //PLAY EVENTS LOOP
        while(window.pollEvent(event))
        {
            sf::Vector2f mouse_pos(sf::Mouse::getPosition(window));  

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                if (exit_button.getGlobalBounds().contains(mouse_pos))
                {
                    std::cout<<"SALIR JUEGO\n";
                    return;
                }   
                //ENEMY MAP
                else if (radar_panel.getGlobalBounds().contains(mouse_pos))       
                {
                    is_shield_valid = false;
                    shield_button.setColor(sf::Color(255,255,255,128));

                    for (size_t i = 0; i < enemy_cells.size(); ++i) 
                    {
                        if (enemy_cells[i].getGlobalBounds().contains(mouse_pos)) 
                        {
                            is_shot_valid = true;
                            shot_button.setColor(sf::Color(255,255,255,255));
                            x = (i / radar_map.get_columns());
                            y = (i % radar_map.get_columns());
                            
                            //GUARDAR COORD   
                            std::string tipo = radar_map.is_water(x, y) ? "Agua" :
                                                radar_map.is_boat(x, y) ? "Barco" :
                                                radar_map.is_failed(x, y) ? "Disparo fallido" :
                                                   "Barco destruido";
                            std::cout << "Radar - Casilla clickeada [" << x << ", " << y << "] - " << tipo << std::endl;
                        }
                    }   
                }
                //PLAYER MAP
                else if (player_panel.getGlobalBounds().contains(mouse_pos))
                {
                    is_shot_valid = false;
                    shot_button.setColor(sf::Color(255,255,255,128));
                    for (size_t i = 0; i < player_cells.size(); ++i) 
                    {
                        if (player_cells[i].getGlobalBounds().contains(mouse_pos)) 
                        {
                            is_shield_valid = true;
                            shield_button.setColor(sf::Color(255,255,255,255));
                            x = (i / radar_map.get_columns());
                            y = (i % radar_map.get_columns());

                            std::string tipo = radar_map.is_water(x, y) ? "Agua" :
                                                radar_map.is_boat(x, y) ? "Barco" :
                                                radar_map.is_failed(x, y) ? "Disparo fallido" :
                                                "Barco destruido";
                            std::cout << "Mapa - Casilla clickeada [" << x << ", " << y << "] - " << tipo << std::endl;
                        }
                    }  
                }
                //SHOT
                else if (shot_button.getGlobalBounds().contains(mouse_pos) && is_shot_valid)
                {
                    is_shot_valid = false;
                    shot_button.setColor(sf::Color(255,255,255,128));
                    std::cout<<"Celda ["<<x<<", "<<y<<"] disparada\n";
                }
                //SHIELD
                else if (shield_button.getGlobalBounds().contains(mouse_pos) && is_shield_valid)
                {
                    shield_button.setColor(sf::Color(255,255,255,128));
                    is_shield_valid = false;
                    std::cout<<"Celda ["<<x<<", "<<y<<"] protegida\n";
                }
            }
        }
        //PLAY DISPLAY
        window.clear();
        window.draw(info_panel);
        window.draw(radar_panel);
        window.draw(player_panel);
        window.draw(exit_button);
        window.draw(shot_button);
        window.draw(shield_button);
       
        for (const auto& casilla : enemy_cells)
        {
          window.draw(casilla);  
        } 
       
        for (const auto& casilla : player_cells)
        {
          window.draw(casilla);  
        } 
       
        for (const auto& barco : ships)
        {
            window.draw(barco);
        }

        window.display();
    }
}

void menu_window()
{
    //MAIN WINDOW
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Battle Ship", sf::Style::Fullscreen);
    sf::Vector2u windowSize = window.getSize();
    
    //BUTTONS
    std::vector<sf::Text> buttons;  
    std::vector<std::string> name_buttons = {"Jugar", "Salir"};  
    sf::Font font;
    font.loadFromFile("resources/fonts/tittle_font.ttf");

    for (size_t i = 0; i < name_buttons.size(); ++i)  
    {
        sf::Text boton;                          
        boton.setFont(font);                        
        boton.setString(name_buttons[i]);
        boton.setCharacterSize(45);
        boton.setFillColor(sf::Color::Black);
        boton.setPosition(100, 500 + i * 70);      
        buttons.push_back(boton);                
    }  

    //BACKGROUND
    sf::Sprite back_ground_sprite;             
    sf::Texture backgroundTexture; 
    backgroundTexture.loadFromFile("resources/textures/menu.jpg");
    back_ground_sprite.setTexture(backgroundTexture);

    //BACKGROUND SCALE
    sf::Vector2u textureSize = backgroundTexture.getSize();
    float windowWidth = static_cast<float>(windowSize.x);
    float windowHeight = static_cast<float>(windowSize.y);
    float textureWidth = static_cast<float>(textureSize.x);
    float textureHeight = static_cast<float>(textureSize.y);
    float scaleX = windowWidth / textureWidth; //Calcular los factores de escala (ancho_ventana / ancho_textura, alto_ventana / alto_textura)
    float scaleY = windowHeight / textureHeight;
    back_ground_sprite.setScale(scaleX, scaleY); // Aplicar la escala al sprite
     
    //MAIN LOOP
    window.setFramerateLimit(60);
    while (window.isOpen()) 
    {
        sf::Event event;

        //MENU EVENTS LOOP
        while (window.pollEvent(event)) 
        {
            sf::Vector2f mousePos(sf::Mouse::getPosition(window));

            //MENU EVENTS
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                for (size_t i = 0; i < buttons.size(); ++i) 
                {
                    if (buttons[i].getGlobalBounds().contains(mousePos)) 
                    {
                        if (buttons[i].getString() == "Salir") 
                        {
                            window.close();
                            exit(EXIT_SUCCESS);
                        }
                        else if (buttons[i].getString() == "Arsenal") 
                        {
                            std::cout << "se presiono Arsenal\n";
                        }
                        else if (buttons[i].getString() == "Tienda") 
                        {
                            std::cout << "se presiono Tienda\n";
                        }
                        else if (buttons[i].getString() == "Registros") 
                        {
                            std::cout << "se presiono Registros\n";
                        }
                        else if (buttons[i].getString() == "Jugar")
                        {
                            std::cout << "Se presiono Jugar\n";
                            play_window(window, font);
                        }
                    }
                }   
            } 

            //EXIT HOVER
            for (auto& button : buttons) 
            {
                if (button.getGlobalBounds().contains(mousePos))
                {
                        button.setFillColor(sf::Color::White);
                }
                else
                {
                    button.setFillColor(sf::Color::Black);   
                }
            }      
        }

        //MENU DISPLAY
        window.clear();
        window.draw(back_ground_sprite);    
        
        for (const auto& boton : buttons)     
        {
            window.draw(boton);         
        }
        
        window.display();
    }
}

int main()
{
    menu_window();
} 