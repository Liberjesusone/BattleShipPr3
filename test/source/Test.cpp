#include "Visual.hpp"

void load_textures(sf::Texture& water_texture, sf::Texture& ship_texture, sf::Texture& miss_texture, sf::Texture& destroyed_texture, 
    sf::Texture& radar_texture, sf::Texture& layout)
{
    if (!water_texture.loadFromFile("resources/textures/water_player.png") ||
        !ship_texture.loadFromFile("resources/textures/ship.png") ||
        !miss_texture.loadFromFile("resources/textures/fallo.jpg") ||
        !destroyed_texture.loadFromFile("resources/textures/destruido.jpg") ||
        !radar_texture.loadFromFile("resources/textures/water_enemy.png") ||
        !layout.loadFromFile("resources/textures/layout.jpg"))
    {
        throw std::runtime_error("No se pudieron cargar las texturas");
    }
}

/**
 * @brief Crea una textura de un circulo gris un poco transparente con un borde grisaceo
 * para representar las casillas vacias de un mapa = Water_cell
 * 
 * @param CELL_SIZE_X el tamaño que tendra cada celda en el eje x
 * @param CELL_SIZE_y el tamaño que tendra cada celda en el eje y
 * 
 * @return la textura creada
 */
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
    border.setOutlineColor(sf::Color::Blue);         // Color del border
    border.setOutlineThickness(borderr_thickness);                 // Grosor ajustado
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
    //sf::Texture cell_texture = create_cell(CELL_SIZE_X, CELL_SIZE_Y);

    //TEXTURES
    sf::Text exit_button; 
    sf::Texture water_texture, ship_texture, miss_texture, destroyed_texture, radar_texture, layout;
    std::vector<sf::Sprite> enemy_cells, player_cells;
    load_textures(water_texture, ship_texture, miss_texture, destroyed_texture, radar_texture, layout);
    sf::Texture water_enemy;
    sf::Texture water_player;
    water_player.loadFromFile("resources/textures/water_player.png");
    water_enemy.loadFromFile("resources/textures/water_enemy.png");
    
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
    shield_button.setPosition(130, 400);
    shield_button.setColor(sf::Color(255,255,255,128));

    //HEAL BUTTON
    button_texture.loadFromFile("resources/textures/heart.png");
    sf::Texture heal_texture = create_special_button(100, 100, button_texture);
    sf::Sprite heal_button(heal_texture);
    heal_button.setPosition(245, 400);
    heal_button.setColor(sf::Color(255,255,255,128));

    //PANELS PROPERTIES (POSITION AND TEXTURE)
    info_panel.setTexture(&layout);
    info_panel.setPosition(0, 0);
    //radar_panel.setTexture(&radar_texture);
    radar_panel.setFillColor(sf::Color::Black);
    radar_panel.setPosition(INFO_WIDTH, 0);
    //player_panel.setTexture(&water_texture);
    player_panel.setFillColor(sf::Color::Black);
    player_panel.setPosition(INFO_WIDTH, RADAR_HEIGHT);

    //CELLS
    for (size_t x = 0; x < radar_map.get_rows(); ++x)             
    {
        for (size_t y = 0; y < radar_map.get_columns(); ++y) 
        {
            // Sprite para el radar (enemigo)
            sf::Sprite enemy_sprite;
            enemy_sprite.setTexture(water_enemy);
            enemy_sprite.setScale(static_cast<float>(CELL_SIZE_X) / water_enemy.getSize().x,
                                static_cast<float>(CELL_SIZE_Y) / water_enemy.getSize().y);
            enemy_sprite.setPosition(INFO_WIDTH + x * CELL_SIZE_X, y * CELL_SIZE_Y);
            enemy_cells.push_back(enemy_sprite);
            
            // Sprite para el mapa del jugador
            sf::Sprite player_sprite;
            player_sprite.setTexture(water_player);
            player_sprite.setScale(static_cast<float>(CELL_SIZE_X) / water_player.getSize().x,
                                 static_cast<float>(CELL_SIZE_Y) / water_player.getSize().y);
            player_sprite.setPosition(INFO_WIDTH + x * CELL_SIZE_X, RADAR_HEIGHT + y * CELL_SIZE_Y);
            player_cells.push_back(player_sprite);
        }
    }

    //CONTROL VARIABLES
    int x = 0;
    int y = 0;
    bool is_shot_valid = false;
    bool is_shield_valid = false;
    bool is_heal_valid = false;
    //sf::Clock clock;

    //PLAY LOOP
    while(window.isOpen())
    {
        sf::Event event;
        //float current_time = clock.restart().asSeconds();
        //float fps = 1.0f / current_time;

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
                    is_heal_valid = false;
                    shield_button.setColor(sf::Color(255,255,255,128));
                    heal_button.setColor(sf::Color(255,255,255,128));

                    for (size_t i = 0; i < enemy_cells.size(); ++i) 
                    {
                        if (enemy_cells[i].getGlobalBounds().contains(mouse_pos)) 
                        {
                            is_shot_valid = true;
                            shot_button.setColor(sf::Color(255,255,255,255));
                            x = (i % radar_map.get_columns());
                            y = (i / radar_map.get_columns());        
                    
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
                            is_heal_valid = true;
                            shield_button.setColor(sf::Color(255,255,255,255));
                            heal_button.setColor(sf::Color(255,255,255,255));
                            x = (i % radar_map.get_columns());
                            y = (i / radar_map.get_columns());
                            

                            std::string tipo = player_map.is_water(x, y) ? "Agua" :
                                                player_map.is_boat(x, y) ? "Barco" :
                                                player_map.is_protected(x, y) ? "Protected" :
                                                player_map.is_failed(x, y) ? "Disparo fallido" :
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
                    Map_cell_ptr cell = radar_map.get_ptr_cell(x, y);

                    if (radar_map.is_water(x, y)) 
                    { 
                        radar_map.set_fail(cell);
                        std::cout<<"Celda ["<<x<<", "<<y<<"] Fallaste\n"; 
                    }
                    else if (radar_map.is_boat(x,y))
                    {
                        radar_map.set_destroy(cell); 
                        std::cout<<"Celda ["<<x<<", "<<y<<"] Acertaste\n"; 
                    }                
                }
                //SHIELD
                else if (shield_button.getGlobalBounds().contains(mouse_pos) && is_shield_valid)
                {
                    shield_button.setColor(sf::Color(255,255,255,128));
                    heal_button.setColor(sf::Color(255,255,255,128));
                    
                    is_shield_valid = false;
                    is_heal_valid = false;

                    Map_cell_ptr cell = player_map.get_ptr_cell(x, y);
                    player_map.set_protected(cell);

                    std::cout<<"Celda ["<<x<<", "<<y<<"] protegida\n";                                        
                }
                //HEAL
                else if (heal_button.getGlobalBounds().contains(mouse_pos) && is_heal_valid)
                {
                    heal_button.setColor(sf::Color(255,255,255,128));
                    shield_button.setColor(sf::Color(255,255,255,128));

                    is_heal_valid = false;
                    is_shield_valid = false;
                    
                    Map_cell_ptr cell = player_map.get_ptr_cell(x, y);
                    //Establecer la casilla como heal

                    std::cout<<"Celda ["<<x<<", "<<y<<"] curada\n";                                        
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
        window.draw(heal_button);
       
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


int build_window(sf::RenderWindow& window)
{
    // Crear la ventana en pantalla completa
    window.clear();
    window.setTitle("Battle Ship - Build");


    // Construimos los mapas y el jugador
    Map mapa(5, 10);
    Player player("Jugador", mapa);


    // Tamaños y divisiones de la pantalla
    sf::Vector2u window_size = window.getSize();
    const size_t WIN_WIDTH = window_size.x;
    const size_t WIN_HEIGHT = window_size.y;
    
    const size_t INFO_WIDTH = window_size.x / 3;         // 1/3 para la información

    const size_t MAP_WIDTH = window_size.x - INFO_WIDTH; // 2/3 para el mapa
    const size_t MAP_HEIGHT = WIN_HEIGHT * 0.8;

    const size_t CELL_SIZE_X = MAP_WIDTH / mapa.get_columns();  // Ancho y alto de las celdas
    const size_t CELL_SIZE_Y = MAP_HEIGHT / mapa.get_rows();

    const size_t TEXT_SIZE = WIN_HEIGHT * 0.05;
    const size_t BOTON_SIZE_X = WIN_WIDTH * 0.08;
    const size_t BOTON_SIZE_Y = BOTON_SIZE_X;

    // ---- CARGAR TEXTURAS ---- //
    sf::Texture misil_texture = create_shot_texture(MAP_WIDTH * 0.10, MAP_WIDTH * 0.10);
    sf::Texture circle_texture = create_circule_texture(CELL_SIZE_X, CELL_SIZE_Y);
    sf::Texture comodin_texture = Painter::Resources::get_texture(Painter::Resources::heart_image());
    sf::Texture agua_texture, barco_texture, fallo_texture, destruido_texture, radar_texture, pirate_map;
    load_textures(agua_texture, barco_texture, fallo_texture, destruido_texture, radar_texture, pirate_map);


    // ---- PANELES ---- //
    sf::RectangleShape info_panel(sf::Vector2f(WIN_WIDTH, WIN_HEIGHT));     // panel de informacion a la izq
    info_panel.setPosition(0, 0);
    info_panel.setTexture(&pirate_map);

    sf::RectangleShape player_panel(sf::Vector2f(MAP_WIDTH, MAP_HEIGHT)); // panel del mapa propio abajo a la derecha
    player_panel.setPosition(INFO_WIDTH, WIN_HEIGHT * 0.1);
    player_panel.setTexture(&agua_texture);

    // Asignar una textura a cada casilla del mapa
    std::vector<sf::Sprite> player_casillas;
    for (int y = 0; y < mapa.get_rows(); ++y)            // mapa del jugador
    {
        for (int x = 0; x < mapa.get_columns(); ++x)
        {
            sf::Sprite sprite;
            if (mapa.is_water(x, y))
                sprite.setTexture(circle_texture);
            else if (mapa.is_boat(x, y))
                sprite.setTexture(barco_texture);
            else if (mapa.is_failed(x, y))
                sprite.setTexture(fallo_texture);
            else if (mapa.is_destroyed(x, y))
                sprite.setTexture(destruido_texture);

            sprite.setScale(static_cast<float>(CELL_SIZE_X) / sprite.getTexture()->getSize().x,
                            static_cast<float>(CELL_SIZE_Y) / sprite.getTexture()->getSize().y);

            sprite.setPosition(INFO_WIDTH + x * CELL_SIZE_X, WIN_HEIGHT * 0.1 + y * CELL_SIZE_Y);
            player_casillas.push_back(sprite);
        }
    }

    // ---- BOTONES ---- // 
    sf::Font font = Painter::Resources::get_font(Painter::Resources::titulos_font());

    // Botón de Salida
    sf::Text exit_button("Salir", font, TEXT_SIZE);        //OJO darle a los bones un tamaño escalable
    exit_button.setFillColor(sf::Color::Yellow);
    exit_button.setPosition(MAP_WIDTH * 0.05, WIN_HEIGHT * 0.05);

    // Botón de jugar
    sf::Text play_button("Jugar", font, TEXT_SIZE);
    play_button.setFillColor(sf::Color::Yellow);
    play_button.setPosition(MAP_WIDTH * 0.10 + exit_button.getGlobalBounds().getSize().x, WIN_HEIGHT * 0.05);

    // Boton de Disparar
    sf::Sprite shot_button(misil_texture);
    shot_button.setPosition((MAP_WIDTH / 2) - shot_button.getGlobalBounds().getSize().x, WIN_HEIGHT / 2);


    // ---- BARCOS ---- // 
    std::vector<sf::Sprite> barcos;
    for (int i = 0; i < 5; ++i) // 5 barcos con tamaños decrecientes
    {
        sf::Sprite barco(barco_texture);
    
        // Factor de reducción progresiva
        double scale_factor = 1.30 - (i * 0.15);    // Reduce cada barco un 15% más pequeño que el anterior
        double scale_factor_y = 0.8 - (i * 0.05);
        barco.setScale(scale_factor, scale_factor_y); // Mantiene proporción correcta
    
        // Ajustar posición para que los barcos se alineen correctamente
        barco.setPosition(MAP_WIDTH * 0.01, i * (barco_texture.getSize().y - 7) + 90); // Espaciado vertical dinámico
        barcos.push_back(barco);
    }


    // ---- COMODINES ---- //  
    std::vector<sf::Sprite> comodines;
    comodines.resize(2);
    std::vector<sf::Text> cant_comodin;
    
    sf::Sprite comodin(comodin_texture);
    comodin.setScale(static_cast<double>(BOTON_SIZE_X) / comodin_texture.getSize().x,
                         static_cast<double>(BOTON_SIZE_Y) / comodin_texture.getSize().y);
    comodines[0] = comodines [1] = comodin;
    comodines[0].setPosition(WIN_WIDTH * 0.01, WIN_HEIGHT * 0.51);
    comodines[1].setPosition(WIN_WIDTH * 0.04 + BOTON_SIZE_X, WIN_HEIGHT * 0.51);

    for (int i = 0; i < comodines.size(); ++i)      // Texto de cantidad
    {
        sf::Text cantidad("2", font, TEXT_SIZE);
        cantidad.setFillColor(sf::Color::White);
        sf::FloatRect text_bounds = cantidad.getLocalBounds();   // lo asingamos dentro del espacio del sprite
        cantidad.setOrigin(text_bounds.left, text_bounds.top);
        cantidad.setPosition(comodines[i].getPosition());
        cant_comodin.push_back(cantidad);
    }


    // ---- PROYECTILES ---- //  
    std::vector<sf::Sprite> proyectiles;
    proyectiles.resize(4);
    std::vector<sf::Text> cant_proyectiles;
    
    sf::Sprite proyectile(fallo_texture);
    proyectile.setScale(static_cast<double>(BOTON_SIZE_X) / fallo_texture.getSize().x,
                        static_cast<double>(BOTON_SIZE_Y) / fallo_texture.getSize().y);
    proyectiles[0] = proyectiles[1] = proyectiles [2] = proyectiles[3] = proyectile;
    proyectiles[0].setPosition(WIN_WIDTH * 0.04, WIN_HEIGHT * 0.67);
    proyectiles[1].setPosition(WIN_WIDTH * 0.04, WIN_HEIGHT * 0.69 + BOTON_SIZE_Y);
    proyectiles[2].setPosition(WIN_WIDTH * 0.08 + BOTON_SIZE_X, WIN_HEIGHT * 0.67);
    proyectiles[3].setPosition(WIN_WIDTH * 0.08 + BOTON_SIZE_X, WIN_HEIGHT * 0.69 + BOTON_SIZE_Y);

    for (int i = 0; i < proyectiles.size(); ++i)        // Texto de cantidad 
    {
        sf::Text cantidad("5", font, TEXT_SIZE);
        cantidad.setFillColor(sf::Color::White);
        sf::FloatRect text_bounds = cantidad.getLocalBounds();
        cantidad.setOrigin(text_bounds.left, text_bounds.top);
        cantidad.setPosition(proyectiles[i].getPosition());
        cant_proyectiles.push_back(cantidad);
    }

    // Bucle principal del juego
    bool holding = false;
    size_t boat_indx;
    window.setFramerateLimit(60);
    while (window.isOpen()) 
    {
        sf::Vector2f mouse_pos;
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            
            mouse_pos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);

            if (event.type == sf::Event::MouseMoved && holding) 
            {
                barcos[boat_indx].setPosition(event.mouseMove.x, event.mouseMove.y);
            }

            // Detección de clic izquierdo
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {                
                if (player_panel.getGlobalBounds().contains(mouse_pos)) // En el panel del mapa 
                {
                    for (size_t i = 0; i < player_casillas.size(); ++i) 
                    {
                        if (player_casillas[i].getGlobalBounds().contains(mouse_pos) && holding) // si clickeo una celda con el barco tomado
                        {
                            int x = (i % mapa.get_columns());
                            int y = (i / mapa.get_columns());

                            std::string tipo = mapa.is_water(x, y) ? "Agua" :
                                            mapa.is_boat(x, y) ? "Barco" :
                                            mapa.is_failed(x, y) ? "Disparo fallido" :
                                            "Barco destruido";

                            Boat bote(4 - boat_indx, Coordinates(x, y));
                            Painter::Drawer::draw(player_casillas, mapa, bote);    
                        }
                        else if (player_casillas[i].getGlobalBounds().contains(mouse_pos)) // si solo la clickeo 
                        {
                            int x = (i % mapa.get_columns());
                            int y = (i / mapa.get_columns());

                            std::string tipo = mapa.is_water(x, y) ? "Agua" :
                                            mapa.is_boat(x, y) ? "Barco" :
                                            mapa.is_failed(x, y) ? "Disparo fallido" :
                                            "Barco destruido";

                            std::cout << "Mapa - Casilla clickeada [" << x << ", " << y << "] - " << tipo << std::endl;
                        }
                    }  
                }
                else if (info_panel.getGlobalBounds().contains(mouse_pos))   // En el panel de informacion
                {
                    if (exit_button.getGlobalBounds().contains(mouse_pos)) // Clic en "Salir"
                    {
                        window.close(); // Cierra el juego
                        exit(EXIT_SUCCESS);
                    }
                    else if (play_button.getGlobalBounds().contains(mouse_pos)) // Clic en "Terminar Partida"
                    {
                        std::cout << "Jugar. Empezando partida..." << std::endl;
                        play_window(window);
                    }
                    for (size_t i = 0; i < comodines.size(); ++i)
                    {
                        if (comodines[i].getGlobalBounds().contains(mouse_pos))
                        {
                            std::cout << "Comodin " << i << "\n";
                        }
                    }
                    for (size_t i = 0; i < barcos.size(); ++i)
                    {
                        if (barcos[i].getGlobalBounds().contains(mouse_pos))
                        {
                            std::cout << "Barco de " << 4 - i << " posiciones\n";    
                            barcos[i].setPosition(mouse_pos);
                            boat_indx = i;
                            holding = !holding;
                        }
                    }
                    for (size_t i = 0; i < proyectiles.size(); ++i) 
                    {
                        if (proyectiles[i].getGlobalBounds().contains(mouse_pos)) 
                        {
                            std::cout << "Proyectil " << i << std::endl;
                        }
                    }  
                }
            }
        }

        window.clear();
        window.draw(info_panel);
        window.draw(player_panel);
        window.draw(exit_button);
        window.draw(play_button);
        window.draw(shot_button);

        // Dibujar los items del panel de info
        for (const auto& barco : barcos) window.draw(barco);
        for (const auto& comodin : comodines) window.draw(comodin);
        for (const auto& cantidad : cant_comodin) window.draw(cantidad);
        for (const auto& proyectil : proyectiles) window.draw(proyectil);
        for (const auto& cantidad : cant_proyectiles) window.draw(cantidad);

        // Dibujar los vectores de sprites
        for (const auto& casilla : player_casillas) window.draw(casilla);

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
                            build_window(window);
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