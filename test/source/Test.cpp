#include "Visual.hpp"
#include "Types.hpp"
#include <map>
#include <chrono>
#include <thread>

void load_textures(sf::Texture& water_texture, sf::Texture& ship_texture, sf::Texture& miss_texture, sf::Texture& destroyed_texture, 
    sf::Texture& radar_texture, sf::Texture& wood_table_texture)
{
    water_texture = Resources::get_texture(Resources::water_player_image());
    ship_texture = Resources::get_texture(Resources::boat_image());
    miss_texture = Resources::get_texture(Resources::failed_image());
    destroyed_texture = Resources::get_texture(Resources::failed_image());
    radar_texture = Resources::get_texture(Resources::radar_image());
    wood_table_texture = Resources::get_texture(Resources::wood_table_image());
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


void message_window(sf::RenderWindow& window, const std::string& message_str, sf::Color color)
{
    sf::Font font = Resources::get_font(Resources::titulos_font());

    //MESSAGE
    sf::Text message;
    message.setFont(font);
    message.setString(message_str);
    message.setCharacterSize(50); // Ajusta el tamaño para que se vea bien
    message.setFillColor(sf::Color::White);

    //Centrar el mensaje en la pantalla
    sf::FloatRect messageRect = message.getLocalBounds();
    message.setOrigin(messageRect.left + messageRect.width / 2.0f, messageRect.top + messageRect.height / 2.0f);
    message.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 50); 

    //EXIT BUTTON
    sf::Text exit_button;
    exit_button.setFont(font);
    exit_button.setString("Salir");
    exit_button.setCharacterSize(30);
    exit_button.setFillColor(sf::Color::White);

    //Posicionar el botón de salir debajo del mensaje y centrado
    sf::FloatRect buttonRect = exit_button.getLocalBounds();
    exit_button.setOrigin(buttonRect.left + buttonRect.width / 2.0f, buttonRect.top + buttonRect.height / 2.0f);
    exit_button.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 50); 

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            sf::Vector2f mouse_pos(static_cast<float>(sf::Mouse::getPosition(window).x),
                                   static_cast<float>(sf::Mouse::getPosition(window).y));

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                if (exit_button.getGlobalBounds().contains(mouse_pos))
                {
                    std::cout << "SALIR JUEGO\n";
                    return; 
                }
            }
        }
        window.clear(color); 
        window.draw(message);          
        window.draw(exit_button);      
        window.display();              
    }
}
    
//Actualiza la lista de los barcos del bot
void update_list_ships(Bot& bot, Map_ptr& radar_map, std::vector<sf::Text>& boat_list)
{
    auto ships = bot.get_build().get_fleet().get_boats();
    
    for(int i = 0; i < ships.size(); ++i)
    {
        if (ships[i]->get_distruction_per(radar_map) >= 100.f)
        {
            if (i == 0)
            {
                boat_list[4].setFillColor(sf::Color(255,255,255,128));
            }
            else if(i == 1)
            {
                boat_list[3].setFillColor(sf::Color(255,255,255,128));
            }
            else if(i == 2)
            {
                boat_list[2].setFillColor(sf::Color(255,255,255,128));
            }
            else if(i == 3)
            {
                boat_list[1].setFillColor(sf::Color(255,255,255,128));
            }
            else if(i == 4)
            {
                boat_list[0].setFillColor(sf::Color(255,255,255,128));
            }
        }
    }
}

//Retorna la cantidad de barcos que tienen un 100% de destrucción
int winner_verification(Player& player, Map_ptr& map)
{
    auto ships = player.get_build().get_fleet().get_boats();
    int result = 0;
    
    for(int i = 0; i < ships.size(); ++i)
    {
        if (ships[i]->get_distruction_per(map) == 100.f)
        {
            ++result;
        }
    }
    return result;
}


void play_window(sf::RenderWindow& window, Player& player, Bot& bot) 
{
    sf::Font font = Resources::get_font(Resources::titulos_font());

    //MAP DIVISIONS
    sf::Vector2u windowSize = window.getSize();
    const size_t INFO_WIDTH = windowSize.x / 3;         // 1/3 para la información
    const size_t MAP_WIDTH = windowSize.x - INFO_WIDTH; // 2/3 para mapas
    const size_t MAP_HEIGHT = windowSize.y;
    const size_t RADAR_HEIGHT = MAP_HEIGHT / 2;         // Altura de ambos mapas

    //PANELS
    sf::RectangleShape info_panel(sf::Vector2f(INFO_WIDTH, MAP_HEIGHT));     // panel de informacion a la izq
    sf::RectangleShape radar_panel(sf::Vector2f(MAP_WIDTH, RADAR_HEIGHT));  // panel del radar arriba a la derecha
    sf::RectangleShape player_panel(sf::Vector2f(MAP_WIDTH, RADAR_HEIGHT)); // panel del mapa propio abajo a la derecha
    
    //MAPS
    Map_ptr player_map = player.get_map();
    Map_ptr radar_map = player.get_radar();
    PlayerPair player_pair = std::make_pair(std::make_shared<Player>(player), std::make_shared<Player>(bot));

    const float CELL_SIZE_X = static_cast<float>(MAP_WIDTH) / player_map->get_columns();
    const float CELL_SIZE_Y = static_cast<float>(RADAR_HEIGHT) / player_map->get_rows();

    //TEXTURES
    sf::Text exit_button; 
    sf::Texture water_player_texture, ship_texture, miss_texture, destroyed_texture, water_enemy_texture, wood_table_texture;
    std::vector<sf::Sprite> enemy_cells, player_cells;
    load_textures(water_player_texture, ship_texture, miss_texture, destroyed_texture, water_enemy_texture, wood_table_texture);

    //EXIT BUTTON
    exit_button.setFont(font);
    exit_button.setString("Salir");
    exit_button.setCharacterSize(40);
    exit_button.setPosition(INFO_WIDTH * 0.05f, MAP_HEIGHT * 0.05f);
    exit_button.setFillColor(sf::Color::White);

    //SHOT BUTTON
    sf::Texture button_texture = Resources::get_texture(Resources::missile_image());
    sf::Texture misil_texture = create_special_button(100, 100, button_texture);
    sf::Sprite shot_button(misil_texture);
    shot_button.setPosition(INFO_WIDTH * 0.1f, MAP_HEIGHT * 0.7f - shot_button.getGlobalBounds().height / 2);
    shot_button.setColor(sf::Color(255,255,255,128));

    //SHIELD BUTTON
    button_texture = Resources::get_texture(Resources::shield_image());
    sf::Texture shield_texture = create_special_button(100, 100, button_texture);
    sf::Sprite shield_button(shield_texture);
    shield_button.setPosition(INFO_WIDTH * 0.5f - shield_button.getGlobalBounds().width / 2, MAP_HEIGHT * 0.7f - shield_button.getGlobalBounds().height / 2);
    shield_button.setColor(sf::Color(255,255,255,128));

    //HEAL BUTTON
    button_texture = Resources::get_texture(Resources::heart_image());
    sf::Texture heal_texture = create_special_button(100, 100, button_texture);
    sf::Sprite heal_button(heal_texture);
    heal_button.setPosition(INFO_WIDTH * 0.9f - heal_button.getGlobalBounds().width, MAP_HEIGHT * 0.7f - heal_button.getGlobalBounds().height / 2);
    heal_button.setColor(sf::Color(255,255,255,128));

    //PANELS PROPERTIES (POSITION AND TEXTURE)
    info_panel.setTexture(&wood_table_texture);
    info_panel.setPosition(0, 0);
    radar_panel.setFillColor(sf::Color::Black);
    radar_panel.setPosition(INFO_WIDTH, 0);
    player_panel.setFillColor(sf::Color::Black);
    player_panel.setPosition(INFO_WIDTH, RADAR_HEIGHT);

    //CELLS - RADAR (ENEMY MAP)
    for (int y = 0; y < radar_map->get_rows(); ++y)
    {
        for (int x = 0; x < radar_map->get_columns(); ++x)
        {
            sf::Sprite enemy_sprite;
            enemy_sprite.setTexture(water_enemy_texture);    
            enemy_sprite.setScale(CELL_SIZE_X / enemy_sprite.getTexture()->getSize().x, CELL_SIZE_Y / enemy_sprite.getTexture()->getSize().y);
            enemy_sprite.setPosition(INFO_WIDTH + x * CELL_SIZE_X, y * CELL_SIZE_Y); // Posición relativa al radar_panel
            enemy_cells.push_back(enemy_sprite);
        }
    }

    //CELLS - PLAYER MAP
    for (int y = 0; y < player_map->get_rows(); ++y)
    {
        for (int x = 0; x < player_map->get_columns(); ++x)
        {
            sf::Sprite player_sprite;
            if (player_map->is_boat(x,y))
            {
                player_sprite.setTexture(Resources::get_texture(Resources::boat_body_image()));
            }
            else
            {
                player_sprite.setTexture(water_player_texture);
                
            }
            player_sprite.setScale(CELL_SIZE_X / player_sprite.getTexture()->getSize().x, CELL_SIZE_Y / player_sprite.getTexture()->getSize().y);
            player_sprite.setPosition(INFO_WIDTH + x * CELL_SIZE_X, RADAR_HEIGHT + y * CELL_SIZE_Y); // Posición relativa al player_panel
            player_cells.push_back(player_sprite);
        }
    }
    
    //CONTROL VARIABLES
    int x = 0;
    int y = 0;
    bool is_shot_valid = false;
    bool is_shield_valid = false;
    bool is_heal_valid = false;
    bool player_won = false;
    bool bot_won = false;

    //IA MESSAGE
    sf::Text bot_shot_message("IA DISPARA", font, 30);
    sf::Text bot_comodin_message("IA USA COMODIN", font, 30);
    bot_shot_message.setFillColor(sf::Color(255, 255, 255, 0));
    bot_shot_message.setPosition(shot_button.getPosition().x, shot_button.getPosition().y + shot_button.getGlobalBounds().height + 50);
    bot_comodin_message.setFillColor(sf::Color(255, 255, 255, 0));
    bot_comodin_message.setPosition(shot_button.getPosition().x, shot_button.getPosition().y + shot_button.getGlobalBounds().height + 50);

    //BOAT LIST 
    std::vector<sf::Text> boat_list;
    for (int i = 0; i < 5; ++i) 
    {
        sf::Text boat_text;
        boat_text.setFont(font);
        boat_text.setString("Barco [" + std::to_string(i + 1) + "]");
        boat_text.setCharacterSize(30); 
        boat_text.setFillColor(sf::Color::White);
        boat_text.setPosition(exit_button.getPosition().x, exit_button.getPosition().y + exit_button.getGlobalBounds().height + 50 + (i * 40));
        boat_list.push_back(boat_text);
    } 

    //BOT SHIPS
    auto bot_ships = bot.get_build().get_fleet().get_boats();
  
    //PLAY LOOP
    while(window.isOpen())
    {
        sf::Event event;

        //PLAY EVENTS LOOP
        while(window.pollEvent(event))
        {
            sf::Vector2f mouse_pos(sf::Mouse::getPosition(window));  
            
            if (player_won)
            {
                message_window(window, "Congratulations, you have won", sf::Color::Green);
                std::cout<<"JUGADOR GANA\n";
                return;
            }
            else if (bot_won)
            {
                message_window(window, "You have lost!", sf::Color::Red);
                std::cout<<"IA GANA\n";
                return;
            }

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
                            x = (i % radar_map->get_columns());
                            y = (i / radar_map->get_columns());  

                            if (radar_map->is_boat(x,y) || radar_map->is_water(x,y) || radar_map->is_protected(x,y))
                            {
                                shot_button.setColor(sf::Color(255,255,255,255));
                                is_shot_valid = true;
                            }

                            if (radar_map->is_failed(x,y) || radar_map->is_destroyed(x,y))
                            {
                                shot_button.setColor(sf::Color(255,255,255,128));
                                is_shot_valid = false;
                            }      
                    
                            std::string tipo;
                            
                            if (radar_map->is_water(x, y)) 
                            {
                                tipo = "Agua";
                            } 
                            else if (radar_map->is_boat(x, y)) 
                            {
                                tipo = "Barco";
                            }
                            else if (radar_map->is_failed(x, y)) 
                            {
                                tipo = "Disparo fallido";
                            }
                            else 
                            {
                                tipo = "Barco destruido";
                            }

                            std::cout << "Radar - Casilla clickeada [" << x << ", " << y << "] - " << tipo << std::endl;
                        }
                    }   
                }
                //PLAYER MAP
                else if (player_panel.getGlobalBounds().contains(mouse_pos))
                {
                    is_shot_valid = false;
                    shot_button.setColor(sf::Color(255,255,255,128));
                    is_heal_valid = false;
                    heal_button.setColor(sf::Color(255,255,255,128));
                    is_heal_valid = false;
                    shield_button.setColor(sf::Color(255,255,255,128));
                    
                    for (size_t i = 0; i < player_cells.size(); ++i) 
                    {
                        if (player_cells[i].getGlobalBounds().contains(mouse_pos)) 
                        {                                            
                            x = (i % player_map->get_columns());
                            y = (i / player_map->get_columns());
                            
                            //El comodin SHIELD solo se activa si la casilla es un bote
                            if (player_map->is_boat(x,y))
                            {
                                shield_button.setColor(sf::Color(255,255,255,255));
                                is_shield_valid = true;
                            }
                            else
                            {
                                shield_button.setColor(sf::Color(255,255,255,128));   
                                is_shield_valid = false;
                            }

                            //El comodin HEAL solo se activa si la casilla es un bote destruido
                            if (player_map->is_destroyed(x,y))
                            {
                                is_heal_valid = true;
                                heal_button.setColor(sf::Color(255,255,255,255));
                            }
                            else
                            {
                                heal_button.setColor(sf::Color(255,255,255,128));
                                is_heal_valid = false;   
                            }
                            
                            std::string tipo;
                            
                            if (player_map->is_water(x, y)) 
                            {
                                tipo = "Agua";
                            } 
                            else if (player_map->is_boat(x, y)) 
                            {
                                tipo = "Barco";
                            }
                            else if (player_map->is_failed(x, y)) 
                            {
                                tipo = "Disparo fallido";
                            }
                            else 
                            {
                                tipo = "Barco destruido";
                            }

                            std::cout << "Player - Casilla clickeada [" << x << ", " << y << "] - " << tipo << std::endl;
                        }
                    }  
                }
                //SHOT
                else if (shot_button.getGlobalBounds().contains(mouse_pos) && is_shot_valid)
                {
                    is_shot_valid = false;
                    size_t index = y * radar_map->get_columns() + x;
                    Map_cell_ptr cell = radar_map->get_ptr_cell(x, y);                    
                    shot_button.setColor(sf::Color(255,255,255,128));
                    
                    player.get_build().get_arsenal().get_items()[0]->use_on(player_pair, x, y);   
                    
                    if (radar_map->is_destroyed(x, y))
                    {
                        enemy_cells[index].setTexture(Resources::get_texture(Resources::fire_image()));
                    }
                    else 
                    {
                        enemy_cells[index].setTexture(Resources::get_texture(Resources::failed_image()));
                    }
                    enemy_cells[index].setScale(CELL_SIZE_X / enemy_cells[index].getTexture()->getSize().x, CELL_SIZE_Y / enemy_cells[index].getTexture()->getSize().y);

                    //Verificando si el jugador ganó
                    update_list_ships(bot, radar_map, boat_list); 
                    if (bot.get_build().get_fleet().get_boats().size() == winner_verification(bot, radar_map)+1)
                    {
                        player_won = true;
                        break;
                    }

                    // Ejecutamos el bot
                    bool was_comodin = bot.play(std::make_shared<Player>(player));      // jugamos y vemos que tipo fue
                    if (was_comodin)
                    {
                        std::cout << "jugo comodin\n";
                        
                        bot_comodin_message.setFillColor(sf::Color(255,255,255,255));
                        bot_shot_message.setFillColor(sf::Color(255,255,255,0));

                        Drawer::draw(enemy_cells, radar_map, was_comodin);                        
                    }
                    else 
                    {
                        std::cout << "jugo misil\n";
                        
                        bot_comodin_message.setFillColor(sf::Color(255,255,255,0));
                        bot_shot_message.setFillColor(sf::Color(255,255,255,255));

                        Drawer::draw(player_cells, player_map, was_comodin);                    
                    }
                }
                //SHIELD
                else if (shield_button.getGlobalBounds().contains(mouse_pos) && is_shield_valid)
                {
                    size_t index = y * player_map->get_columns() + x;
                    Map_cell_ptr cell = player_map->get_ptr_cell(x, y);                    
                    is_shield_valid = false;
                    is_heal_valid = false;                                   

                    shield_button.setColor(sf::Color(255,255,255,128));
                    heal_button.setColor(sf::Color(255,255,255,128));                                         
                
                    std::cout<<"Celda ["<<x<<", "<<y<<"] protegida\n";
                    
                    player.get_build().get_arsenal().get_items()[3]->use_on(player_pair, x, y);   
                    
                    if (player_map->is_protected(x, y))
                    {        
                        player_cells[index].setTexture(Resources::get_texture(Resources::shield_image())); 
                        player_cells[index].setScale(CELL_SIZE_X / player_cells[index].getTexture()->getSize().x, CELL_SIZE_Y / player_cells[index].getTexture()->getSize().y);
                    }
    
                    // Ejecutamos el bot
                    bool was_comodin = bot.play(std::make_shared<Player>(player));      // jugamos y vemos que tipo fue
                    if (was_comodin)
                    {
                        std::cout << "jugo comodin\n";
                        Drawer::draw(enemy_cells, radar_map, was_comodin);
  
                        bot_comodin_message.setFillColor(sf::Color(255,255,255,255));
                        bot_shot_message.setFillColor(sf::Color(255,255,255,0));
                    }
                    else 
                    {
                        std::cout << "jugo misil\n";

                        bot_comodin_message.setFillColor(sf::Color(255,255,255,0));
                        bot_shot_message.setFillColor(sf::Color(255,255,255,255));

                        Drawer::draw(player_cells, player_map, was_comodin);                       
                    }          
                }
                //HEAL
                else if (heal_button.getGlobalBounds().contains(mouse_pos) && is_heal_valid)
                {
                    size_t index = y * player_map->get_columns() + x;
                    Map_cell_ptr cell = player_map->get_ptr_cell(x, y);                    
                    is_shield_valid = false;
                    is_heal_valid = false;            

                    shield_button.setColor(sf::Color(255,255,255,128));
                    heal_button.setColor(sf::Color(255,255,255,128));                                         
                        
                    std::cout<<"Celda ["<<x<<", "<<y<<"] curada\n";    
                
                    player.get_build().get_arsenal().get_items()[2]->use_on(player_pair, x, y);   
                   
                    if (player_map->is_boat(x, y))
                    {        
                        player_cells[index].setTexture(Resources::get_texture(Resources::boat_body_image())); 
                        player_cells[index].setScale(CELL_SIZE_X / player_cells[index].getTexture()->getSize().x, CELL_SIZE_Y / player_cells[index].getTexture()->getSize().y);
                    }

                    // Ejecutamos el bot
                    bool was_comodin = bot.play(std::make_shared<Player>(player));      // jugamos y vemos que tipo fue
                    if (was_comodin)
                    {
                        std::cout << "jugo comodin\n";
                        Drawer::draw(enemy_cells, radar_map, was_comodin);
                        
                        bot_comodin_message.setFillColor(sf::Color(255,255,255,255));
                        bot_shot_message.setFillColor(sf::Color(255,255,255,0));
                    }
                    else 
                    {
                        std::cout << "jugo misil\n";
                        Drawer::draw(player_cells, player_map, was_comodin);

                        bot_comodin_message.setFillColor(sf::Color(255,255,255,0));
                        bot_shot_message.setFillColor(sf::Color(255,255,255,255));
                    }
                }
                //Verificando si el bot ganó
                update_list_ships(bot, radar_map, boat_list); 
                if (player.get_build().get_fleet().get_boats().size() == winner_verification(player, player_map)+1)
                {
                    bot_won = true;
                    break;
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
        window.draw(bot_shot_message);
        window.draw(bot_comodin_message);
       
        for (const auto& casilla : enemy_cells)
        {
          window.draw(casilla);  
        } 
       
        for (const auto& casilla : player_cells)
        {
          window.draw(casilla);  
        } 

        for (const auto& boat_text : boat_list) 
        {
           window.draw(boat_text);
        }
       
        window.display();
    }
}

void build_window(sf::RenderWindow& window)
{
    // Crear la ventana en pantalla completa
    window.clear();
    window.setTitle("Battle Ship - Build");

    // Construimos los mapas y el jugador
    Map_ptr mapa = std::make_shared<Map>(5, 10);
    Map_ptr radar = std::make_shared<Map>(5, 10);    
    Bot bot("Bot", radar, mapa);
    Player player("Jugador", mapa, radar);

    // TAMAÑOS Y DIVISIONES DE PANTALLA
    sf::Vector2u window_size = window.getSize();
    const size_t WIN_WIDTH = window_size.x;
    const size_t WIN_HEIGHT = window_size.y;
    
    const size_t INFO_WIDTH = window_size.x / 3;         // 1/3 para la información

    const size_t MAP_WIDTH = window_size.x - INFO_WIDTH; // 2/3 para el mapa
    const size_t MAP_HEIGHT = WIN_HEIGHT * 0.8;

    const size_t CELL_SIZE_X = MAP_WIDTH / mapa->get_columns();  // Ancho y alto de las celdas
    const size_t CELL_SIZE_Y = MAP_HEIGHT / mapa->get_rows();

    const size_t TEXT_SIZE = WIN_HEIGHT * 0.05;
    const size_t BOTON_SIZE_X = WIN_WIDTH * 0.08;
    const size_t BOTON_SIZE_Y = BOTON_SIZE_X;


    // ---- CARGAR TEXTURAS ---- //
    sf::Texture misil_texture = create_special_button(MAP_WIDTH * 0.10, MAP_WIDTH * 0.10, Resources::get_texture(Resources::missile_image()));
    sf::Texture circle_texture = create_cell(CELL_SIZE_X, CELL_SIZE_Y);
    sf::Texture comodin_texture = Resources::get_texture(Resources::heart_image());
    sf::Texture water_texture, barco_texture, fallo_texture, destruido_texture, radar_texture, wood_table_texture;
    load_textures(water_texture, barco_texture, fallo_texture, destruido_texture, radar_texture, wood_table_texture);


    // ---- PANELES ---- //
    sf::RectangleShape info_panel(sf::Vector2f(WIN_WIDTH, WIN_HEIGHT));     // panel de informacion a la izq
    info_panel.setPosition(0, 0);
    info_panel.setTexture(&wood_table_texture);

    sf::RectangleShape player_panel(sf::Vector2f(MAP_WIDTH, MAP_HEIGHT)); // panel del mapa propio abajo a la derecha
    player_panel.setPosition(INFO_WIDTH, WIN_HEIGHT * 0.1);
    player_panel.setTexture(&water_texture);


    // CASILLAS DEL MAPA
    std::vector<sf::Sprite> player_casillas;
    for (int y = 0; y < mapa->get_rows(); ++y)            // mapa del jugador
    {
        for (int x = 0; x < mapa->get_columns(); ++x)
        {
            sf::Sprite sprite;
            if (mapa->is_water(x, y))
                sprite.setTexture(circle_texture);
            else if (mapa->is_boat(x, y))
                sprite.setTexture(barco_texture);
            else if (mapa->is_failed(x, y))
                sprite.setTexture(fallo_texture);
            else if (mapa->is_destroyed(x, y))
                sprite.setTexture(destruido_texture);
            
            sprite.setScale(static_cast<float>(CELL_SIZE_X) / sprite.getTexture()->getSize().x,
                            static_cast<float>(CELL_SIZE_Y) / sprite.getTexture()->getSize().y);

            sprite.setPosition(INFO_WIDTH + x * CELL_SIZE_X, WIN_HEIGHT * 0.1 + y * CELL_SIZE_Y);
            player_casillas.push_back(sprite);
        }
    }

    // ---- BOTONES ---- // 
    sf::Font font = Resources::get_font(Resources::titulos_font());

    // Botón de Salida
    sf::Text exit_button("Salir", font, TEXT_SIZE);        //OJO darle a los bones un tamaño escalable
    exit_button.setFillColor(sf::Color::Yellow);
    exit_button.setPosition(MAP_WIDTH * 0.05, WIN_HEIGHT * 0.05);

    // Botón de jugar
    sf::Text play_button("Jugar", font, TEXT_SIZE);
    play_button.setFillColor(sf::Color::Yellow);
    play_button.setPosition(MAP_WIDTH * 0.10 + exit_button.getGlobalBounds().width, WIN_HEIGHT * 0.05);

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
    
    // BARCOS VACIOS PARA REPRESENTAR QUE FUERON TOMADOS //
    std::vector<sf::Sprite> empty_boats;
    empty_boats.resize(5);
    for (int i = 0; i < 5; ++i) // Al tomarlo crea un sprite vacio que ocupa su posicion 
    {
        sf::Sprite empty_boat(barco_texture);
        empty_boat.setScale(barcos[i].getScale());
        empty_boat.setColor(sf::Color(255, 255, 255, 128));
        empty_boat.setPosition(barcos[i].getPosition());
        empty_boats[i] = empty_boat;
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
    bool horizontal = true;
    bool holding = false;
    size_t boat_indx;
    window.setFramerateLimit(60);
    while (window.isOpen()) 
    {
        sf::Vector2f mouse_pos;
        sf::Event event;
        while (window.pollEvent(event))
        {
            mouse_pos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);

            if (event.type == sf::Event::Closed)                        // Cerrar la ventana
            {
                return;
            }

            if (event.type == sf::Event::MouseMoved && holding)         // Mover el bote siguiendo el mouse 
            {
                barcos[boat_indx].setPosition(event.mouseMove.x, event.mouseMove.y);
            }

            // Detección de clic izquierdo
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {                
                if (player_panel.getGlobalBounds().contains(mouse_pos))      // En el panel del mapa 
                {
                    for (size_t i = 0; i < player_casillas.size(); ++i)      // Vemos que casilla fue clickeada
                    {
                        if (player_casillas[i].getGlobalBounds().contains(mouse_pos) && holding) // si clickeo una celda con el barco tomado
                        {
                            size_t x = (i % mapa->get_columns());
                            size_t y = (i / mapa->get_columns());

                            std::string tipo = mapa->is_water(x, y) ? "Agua" :
                                            mapa->is_boat(x, y) ? "Barco" :
                                            mapa->is_failed(x, y) ? "Disparo fallido" :
                                            "Barco destruido";
                            
                            // Confirmar que cabe en el mapa
                            size_t boat_size = 5 - boat_indx; 
                            if ((boat_size + x > mapa->get_columns() && horizontal) || (boat_size + y > mapa->get_rows() && !horizontal))
                            {
                                break;
                            }

                            // No poner un barco sobre otro barco
                            bool colision = false;
                            for (int i = x, j = y; i < mapa->get_columns() && i < x + boat_size && j < mapa->get_rows() && j < y + boat_size; )
                            {   
                                if (mapa->is_boat(i, j))
                                {
                                    colision = true;
                                    break;
                                }
                                horizontal ? ++i : ++j;
                            }
                            if (colision)
                            {
                                break;
                            } 

                            Coordinates coord(x, y);
                            Boat bote(boat_size, coord, horizontal);
                            sf::Texture& texture = Resources::get_texture(Resources::boat_body_image());
                            Drawer::draw(player_casillas, mapa, bote, texture);  
                            player.get_build().get_fleet().add_boat(std::make_shared<Boat>(bote)); // agregamos el barco al player

                            // Hacerlo invisible    
                            barcos[boat_indx].setPosition(-100, -100);                             // Lo sacamos de la pantalla
                            holding = false;
                            horizontal = true;
                        }
                        else if (player_casillas[i].getGlobalBounds().contains(mouse_pos))       // si la clickeo sin ningun bote
                        {
                            int x = (i % mapa->get_columns());
                            int y = (i / mapa->get_columns());

                            std::string tipo = mapa->is_water(x, y) ? "Agua" :
                                            mapa->is_boat(x, y) ? "Barco" :
                                            mapa->is_failed(x, y) ? "Disparo fallido" :
                                            "Barco destruido";

                            std::cout << "Mapa - Casilla clickeada [" << x << ", " << y << "] - " << tipo << std::endl;

                            if (mapa->is_boat(x, y))      // si clickeo el barco devolverlo a su posicion
                            {
                                auto boat = player.get_build().get_fleet().get_boat_of_cell(mapa->get_ptr_cell(x, y));   
                                size_t indx = 5 - boat->get_size();             // indice correspondiente al vector de sprite

                                Drawer::draw(player_casillas, mapa, *boat, circle_texture);
                                for (auto coord : boat->get_boat_coordinates()) // devolvemos a agua todas las casillas
                                {
                                    mapa->set_water(mapa->get_ptr_cell(coord.first, coord.second));
                                }

                                barcos[indx].setRotation(0);                    // ponemos el sprite de nuevo en su posicion
                                barcos[indx].setPosition(empty_boats[indx].getPosition());
                                horizontal = true;

                                player.get_build().get_fleet().delete_boat(boat);
                            }
                        }
                    }  
                }
                else if (info_panel.getGlobalBounds().contains(mouse_pos))   // En el panel de informacion
                {
                    if (holding)                                    // Soltar de nuevo el bote en su lugar
                    {
                        barcos[boat_indx].setRotation(0);
                        barcos[boat_indx].setPosition(empty_boats[boat_indx].getPosition());
                        holding = false;
                        horizontal = true;
                    }
                    else if (exit_button.getGlobalBounds().contains(mouse_pos)) // Click en "Salir"
                    {
                        return;
                    }
                    else if (play_button.getGlobalBounds().contains(mouse_pos)) // Click en "Jugar"
                    {
                        std::cout << "Jugar. Empezando partida..." << std::endl;
                        Player jugador("Jugador", mapa, radar); 
                        play_window(window, player, bot);
                        return;
                    }
                    for (size_t i = 0; i < comodines.size(); ++i)   // Click sobre algun comodin
                    {
                        if (comodines[i].getGlobalBounds().contains(mouse_pos))
                        {
                            std::cout << "Comodin " << i << "\n";
                        }
                    }
                    for (size_t i = 0; i < barcos.size(); ++i)      // Click sobre algun barco
                    {
                        if (barcos[i].getGlobalBounds().contains(mouse_pos))
                        {                            
                            barcos[i].setPosition(mouse_pos);
                            boat_indx = i;
                            holding = !holding;
                        }
                    }
                    for (size_t i = 0; i < proyectiles.size(); ++i) // Click sobre algun proyectil
                    {
                        if (proyectiles[i].getGlobalBounds().contains(mouse_pos)) 
                        {
                            std::cout << "Proyectil " << i << std::endl;
                        }
                    }  
                }
            }

            // Rotar el barco
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && holding)
            {
                horizontal ? barcos[boat_indx].rotate(90) : barcos[boat_indx].rotate(-90);  
                horizontal = !horizontal;
            }
        }
        window.clear();
        window.draw(info_panel);
        window.draw(player_panel);
        window.draw(exit_button);
        window.draw(play_button);

        // Dibujar los items del panel de info
        for (const auto& empty_boat : empty_boats) window.draw(empty_boat);
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
    sf::Font font = Resources::get_font(Resources::titulos_font());

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
    sf::Texture background_texture = Resources::get_texture(Resources::menu_image());
    back_ground_sprite.setTexture(background_texture);

    //BACKGROUND SCALE
    sf::Vector2u textureSize = background_texture.getSize();
    float windowWidth = static_cast<float>(windowSize.x);
    float windowHeight = static_cast<float>(windowSize.y);
    float textureWidth = static_cast<float>(textureSize.x);
    float textureHeight = static_cast<float>(textureSize.y);
    float scaleX = windowWidth / textureWidth; //Calcular los factores de escala (ancho_ventana / ancho_textura, alto_ventana / alto_textura)
    float scaleY = windowHeight / textureHeight;
    back_ground_sprite.setScale(scaleX, scaleY); // Aplicar la escala al sprite
     
    window.setFramerateLimit(60);
    
    //MAIN LOOP
    while (window.isOpen()) 
    {
        sf::Event event;

        //MENU EVENTS LOOP
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                exit(EXIT_SUCCESS);
            }
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