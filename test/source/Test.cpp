#include "Visual.hpp"
#include "Types.hpp"
#include <map>
#include <chrono>
#include <thread>


void message_window(sf::RenderWindow& window, const std::string& message_str, sf::Color color)
{
    sf::Font font = Rsrc::getFont(Rsrc::titulosFont());

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


// void play_window(sf::RenderWindow& window, Player& player, Bot& bot) 
// {
//     sf::Font font = Rsrc::getFont(Rsrc::titulosFont());

//     //MAP DIVISIONS
//     sf::Vector2u windowSize = window.getSize();
//     const size_t INFO_WIDTH = windowSize.x / 3;         // 1/3 para la información
//     const size_t MAP_WIDTH = windowSize.x - INFO_WIDTH; // 2/3 para mapas
//     const size_t MAP_HEIGHT = windowSize.y;
//     const size_t RADAR_HEIGHT = MAP_HEIGHT / 2;         // Altura de ambos mapas

//     //PANELS
//     sf::RectangleShape info_panel(sf::Vector2f(INFO_WIDTH, MAP_HEIGHT));     // panel de informacion a la izq
//     sf::RectangleShape radar_panel(sf::Vector2f(MAP_WIDTH, RADAR_HEIGHT));  // panel del radar arriba a la derecha
//     sf::RectangleShape player_panel(sf::Vector2f(MAP_WIDTH, RADAR_HEIGHT)); // panel del mapa propio abajo a la derecha
    
//     //MAPS
//     Map_ptr player_map = player.get_map();
//     Map_ptr radar_map = player.get_radar();
//     PlayerPair player_pair = std::make_pair(std::make_shared<Player>(player), std::make_shared<Player>(bot));

//     const float CELL_SIZE_X = static_cast<float>(MAP_WIDTH) / player_map->get_columns();
//     const float CELL_SIZE_Y = static_cast<float>(RADAR_HEIGHT) / player_map->get_rows();

//     //TEXTURES
//     sf::Text exit_button; 
//     std::vector<sf::Sprite> enemy_cells, player_cells;
//     sf::Texture water_texture, barco_texture, fallo_texture, destruido_texture, radar_texture, wood_table_texture, misil_texture, circle_texture, comodin_texture;
//     Rsrc::load_textures(water_texture, barco_texture, fallo_texture, destruido_texture, radar_texture, wood_table_texture, misil_texture, circle_texture, comodin_texture);

//     //EXIT BUTTON
//     exit_button.setFont(font);
//     exit_button.setString("Salir");
//     exit_button.setCharacterSize(40);
//     exit_button.setPosition(INFO_WIDTH * 0.05f, MAP_HEIGHT * 0.05f);
//     exit_button.setFillColor(sf::Color::White);

//     //SHOT BUTTON
//     sf::Texture button_texture = Rsrc::get_texture(Rsrc::missile_image());
//     sf::Texture misil_texture = create_special_button(100, 100, button_texture);
//     sf::Sprite shot_button(misil_texture);
//     shot_button.setPosition(INFO_WIDTH * 0.1f, MAP_HEIGHT * 0.7f - shot_button.getGlobalBounds().height / 2);
//     shot_button.setColor(sf::Color(255,255,255,128));

//     //SHIELD BUTTON
//     button_texture = Rsrc::get_texture(Rsrc::shield_image());
//     sf::Texture shield_texture = create_special_button(100, 100, button_texture);
//     sf::Sprite shield_button(shield_texture);
//     shield_button.setPosition(INFO_WIDTH * 0.5f - shield_button.getGlobalBounds().width / 2, MAP_HEIGHT * 0.7f - shield_button.getGlobalBounds().height / 2);
//     shield_button.setColor(sf::Color(255,255,255,128));

//     //HEAL BUTTON
//     button_texture = Rsrc::get_texture(Rsrc::heart_image());
//     sf::Texture heal_texture = create_special_button(100, 100, button_texture);
//     sf::Sprite heal_button(heal_texture);
//     heal_button.setPosition(INFO_WIDTH * 0.9f - heal_button.getGlobalBounds().width, MAP_HEIGHT * 0.7f - heal_button.getGlobalBounds().height / 2);
//     heal_button.setColor(sf::Color(255,255,255,128));

//     //PANELS PROPERTIES (POSITION AND TEXTURE)
//     info_panel.setTexture(&wood_table_texture);
//     info_panel.setPosition(0, 0);
//     radar_panel.setFillColor(sf::Color::Black);
//     radar_panel.setPosition(INFO_WIDTH, 0);
//     player_panel.setFillColor(sf::Color::Black);
//     player_panel.setPosition(INFO_WIDTH, RADAR_HEIGHT);

//     //CELLS - RADAR (ENEMY MAP)
//     for (int y = 0; y < radar_map->get_rows(); ++y)
//     {
//         for (int x = 0; x < radar_map->get_columns(); ++x)
//         {
//             sf::Sprite enemy_sprite;
//             enemy_sprite.setTexture(water_enemy_texture);    
//             enemy_sprite.setScale(CELL_SIZE_X / enemy_sprite.getTexture()->getSize().x, CELL_SIZE_Y / enemy_sprite.getTexture()->getSize().y);
//             enemy_sprite.setPosition(INFO_WIDTH + x * CELL_SIZE_X, y * CELL_SIZE_Y); // Posición relativa al radar_panel
//             enemy_cells.push_back(enemy_sprite);
//         }
//     }

//     //CELLS - PLAYER MAP
//     for (int y = 0; y < player_map->get_rows(); ++y)
//     {
//         for (int x = 0; x < player_map->get_columns(); ++x)
//         {
//             sf::Sprite player_sprite;
//             if (player_map->is_boat(x,y))
//             {
//                 player_sprite.setTexture(Rsrc::get_texture(Rsrc::boat_body_image()));
//             }
//             else
//             {
//                 player_sprite.setTexture(water_player_texture);
                
//             }
//             player_sprite.setScale(CELL_SIZE_X / player_sprite.getTexture()->getSize().x, CELL_SIZE_Y / player_sprite.getTexture()->getSize().y);
//             player_sprite.setPosition(INFO_WIDTH + x * CELL_SIZE_X, RADAR_HEIGHT + y * CELL_SIZE_Y); // Posición relativa al player_panel
//             player_cells.push_back(player_sprite);
//         }
//     }
    
//     //CONTROL VARIABLES
//     int x = 0;
//     int y = 0;
//     bool is_shot_valid = false;
//     bool is_shield_valid = false;
//     bool is_heal_valid = false;
//     bool player_won = false;
//     bool bot_won = false;

//     //IA MESSAGE
//     sf::Text bot_shot_message("IA DISPARA", font, 30);
//     sf::Text bot_comodin_message("IA USA COMODIN", font, 30);
//     bot_shot_message.setFillColor(sf::Color(255, 255, 255, 0));
//     bot_shot_message.setPosition(shot_button.getPosition().x, shot_button.getPosition().y + shot_button.getGlobalBounds().height + 50);
//     bot_comodin_message.setFillColor(sf::Color(255, 255, 255, 0));
//     bot_comodin_message.setPosition(shot_button.getPosition().x, shot_button.getPosition().y + shot_button.getGlobalBounds().height + 50);

//     //BOAT LIST 
//     std::vector<sf::Text> boat_list;
//     for (int i = 0; i < 5; ++i) 
//     {
//         sf::Text boat_text;
//         boat_text.setFont(font);
//         boat_text.setString("Barco [" + std::to_string(i + 1) + "]");
//         boat_text.setCharacterSize(30); 
//         boat_text.setFillColor(sf::Color::White);
//         boat_text.setPosition(exit_button.getPosition().x, exit_button.getPosition().y + exit_button.getGlobalBounds().height + 50 + (i * 40));
//         boat_list.push_back(boat_text);
//     } 

//     //BOT SHIPS
//     auto bot_ships = bot.get_build().get_fleet().get_boats();
  
//     //PLAY LOOP
//     while(window.isOpen())
//     {
//         sf::Event event;

//         //PLAY EVENTS LOOP
//         while(window.pollEvent(event))
//         {
//             sf::Vector2f mouse_pos(sf::Mouse::getPosition(window));  
            
//             if (player_won)
//             {
//                 message_window(window, "Congratulations, you have won", sf::Color::Green);
//                 std::cout<<"JUGADOR GANA\n";
//                 return;
//             }
//             else if (bot_won)
//             {
//                 message_window(window, "You have lost!", sf::Color::Red);
//                 std::cout<<"IA GANA\n";
//                 return;
//             }

//             if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
//             {
//                 if (exit_button.getGlobalBounds().contains(mouse_pos))
//                 {
//                     std::cout<<"SALIR JUEGO\n";
//                     return;
//                 }   
//                 //ENEMY MAP
//                 else if (radar_panel.getGlobalBounds().contains(mouse_pos))       
//                 {
//                     is_shield_valid = false;
//                     is_heal_valid = false;
//                     shield_button.setColor(sf::Color(255,255,255,128));
//                     heal_button.setColor(sf::Color(255,255,255,128));

//                     for (size_t i = 0; i < enemy_cells.size(); ++i) 
//                     {
//                         if (enemy_cells[i].getGlobalBounds().contains(mouse_pos)) 
//                         {
//                             x = (i % radar_map->get_columns());
//                             y = (i / radar_map->get_columns());  

//                             if (radar_map->is_boat(x,y) || radar_map->is_water(x,y) || radar_map->is_protected(x,y))
//                             {
//                                 shot_button.setColor(sf::Color(255,255,255,255));
//                                 is_shot_valid = true;
//                             }

//                             if (radar_map->is_failed(x,y) || radar_map->is_destroyed(x,y))
//                             {
//                                 shot_button.setColor(sf::Color(255,255,255,128));
//                                 is_shot_valid = false;
//                             }      
                    
//                             std::string tipo;
                            
//                             if (radar_map->is_water(x, y)) 
//                             {
//                                 tipo = "Agua";
//                             } 
//                             else if (radar_map->is_boat(x, y)) 
//                             {
//                                 tipo = "Barco";
//                             }
//                             else if (radar_map->is_failed(x, y)) 
//                             {
//                                 tipo = "Disparo fallido";
//                             }
//                             else 
//                             {
//                                 tipo = "Barco destruido";
//                             }

//                             std::cout << "Radar - Casilla clickeada [" << x << ", " << y << "] - " << tipo << std::endl;
//                         }
//                     }   
//                 }
//                 //PLAYER MAP
//                 else if (player_panel.getGlobalBounds().contains(mouse_pos))
//                 {
//                     is_shot_valid = false;
//                     shot_button.setColor(sf::Color(255,255,255,128));
//                     is_heal_valid = false;
//                     heal_button.setColor(sf::Color(255,255,255,128));
//                     is_heal_valid = false;
//                     shield_button.setColor(sf::Color(255,255,255,128));
                    
//                     for (size_t i = 0; i < player_cells.size(); ++i) 
//                     {
//                         if (player_cells[i].getGlobalBounds().contains(mouse_pos)) 
//                         {                                            
//                             x = (i % player_map->get_columns());
//                             y = (i / player_map->get_columns());
                            
//                             //El comodin SHIELD solo se activa si la casilla es un bote
//                             if (player_map->is_boat(x,y))
//                             {
//                                 shield_button.setColor(sf::Color(255,255,255,255));
//                                 is_shield_valid = true;
//                             }
//                             else
//                             {
//                                 shield_button.setColor(sf::Color(255,255,255,128));   
//                                 is_shield_valid = false;
//                             }

//                             //El comodin HEAL solo se activa si la casilla es un bote destruido
//                             if (player_map->is_destroyed(x,y))
//                             {
//                                 is_heal_valid = true;
//                                 heal_button.setColor(sf::Color(255,255,255,255));
//                             }
//                             else
//                             {
//                                 heal_button.setColor(sf::Color(255,255,255,128));
//                                 is_heal_valid = false;   
//                             }
                            
//                             std::string tipo;
                            
//                             if (player_map->is_water(x, y)) 
//                             {
//                                 tipo = "Agua";
//                             } 
//                             else if (player_map->is_boat(x, y)) 
//                             {
//                                 tipo = "Barco";
//                             }
//                             else if (player_map->is_failed(x, y)) 
//                             {
//                                 tipo = "Disparo fallido";
//                             }
//                             else 
//                             {
//                                 tipo = "Barco destruido";
//                             }

//                             std::cout << "Player - Casilla clickeada [" << x << ", " << y << "] - " << tipo << std::endl;
//                         }
//                     }  
//                 }
//                 //SHOT
//                 else if (shot_button.getGlobalBounds().contains(mouse_pos) && is_shot_valid)
//                 {
//                     is_shot_valid = false;
//                     size_t index = y * radar_map->get_columns() + x;
//                     Map_cell_ptr cell = radar_map->get_ptr_cell(x, y);                    
//                     shot_button.setColor(sf::Color(255,255,255,128));
                    
//                     player.get_build().get_arsenal().get_items()[0]->use_on(player_pair, x, y);   
                    
//                     if (radar_map->is_destroyed(x, y))
//                     {
//                         enemy_cells[index].setTexture(Rsrc::get_texture(Rsrc::fire_image()));
//                     }
//                     else 
//                     {
//                         enemy_cells[index].setTexture(Rsrc::get_texture(Rsrc::failed_image()));
//                     }
//                     enemy_cells[index].setScale(CELL_SIZE_X / enemy_cells[index].getTexture()->getSize().x, CELL_SIZE_Y / enemy_cells[index].getTexture()->getSize().y);

//                     //Verificando si el jugador ganó
//                     update_list_ships(bot, radar_map, boat_list); 
//                     if (bot.get_build().get_fleet().get_boats().size() == winner_verification(bot, radar_map)+1)
//                     {
//                         player_won = true;
//                         break;
//                     }

//                     // Ejecutamos el bot
//                     bool was_comodin = bot.play(std::make_shared<Player>(player));      // jugamos y vemos que tipo fue
//                     if (was_comodin)
//                     {
//                         std::cout << "jugo comodin\n";
                        
//                         bot_comodin_message.setFillColor(sf::Color(255,255,255,255));
//                         bot_shot_message.setFillColor(sf::Color(255,255,255,0));

//                         Drawer::draw(enemy_cells, radar_map, was_comodin);                        
//                     }
//                     else 
//                     {
//                         std::cout << "jugo misil\n";
                        
//                         bot_comodin_message.setFillColor(sf::Color(255,255,255,0));
//                         bot_shot_message.setFillColor(sf::Color(255,255,255,255));

//                         Drawer::draw(player_cells, player_map, was_comodin);                    
//                     }
//                 }
//                 //SHIELD
//                 else if (shield_button.getGlobalBounds().contains(mouse_pos) && is_shield_valid)
//                 {
//                     size_t index = y * player_map->get_columns() + x;
//                     Map_cell_ptr cell = player_map->get_ptr_cell(x, y);                    
//                     is_shield_valid = false;
//                     is_heal_valid = false;                                   

//                     shield_button.setColor(sf::Color(255,255,255,128));
//                     heal_button.setColor(sf::Color(255,255,255,128));                                         
                
//                     std::cout<<"Celda ["<<x<<", "<<y<<"] protegida\n";
                    
//                     player.get_build().get_arsenal().get_items()[3]->use_on(player_pair, x, y);   
                    
//                     if (player_map->is_protected(x, y))
//                     {        
//                         player_cells[index].setTexture(Rsrc::get_texture(Rsrc::shield_image())); 
//                         player_cells[index].setScale(CELL_SIZE_X / player_cells[index].getTexture()->getSize().x, CELL_SIZE_Y / player_cells[index].getTexture()->getSize().y);
//                     }
    
//                     // Ejecutamos el bot
//                     bool was_comodin = bot.play(std::make_shared<Player>(player));      // jugamos y vemos que tipo fue
//                     if (was_comodin)
//                     {
//                         std::cout << "jugo comodin\n";
//                         Drawer::draw(enemy_cells, radar_map, was_comodin);
  
//                         bot_comodin_message.setFillColor(sf::Color(255,255,255,255));
//                         bot_shot_message.setFillColor(sf::Color(255,255,255,0));
//                     }
//                     else 
//                     {
//                         std::cout << "jugo misil\n";

//                         bot_comodin_message.setFillColor(sf::Color(255,255,255,0));
//                         bot_shot_message.setFillColor(sf::Color(255,255,255,255));

//                         Drawer::draw(player_cells, player_map, was_comodin);                       
//                     }          
//                 }
//                 //HEAL
//                 else if (heal_button.getGlobalBounds().contains(mouse_pos) && is_heal_valid)
//                 {
//                     size_t index = y * player_map->get_columns() + x;
//                     Map_cell_ptr cell = player_map->get_ptr_cell(x, y);                    
//                     is_shield_valid = false;
//                     is_heal_valid = false;            

//                     shield_button.setColor(sf::Color(255,255,255,128));
//                     heal_button.setColor(sf::Color(255,255,255,128));                                         
                        
//                     std::cout<<"Celda ["<<x<<", "<<y<<"] curada\n";    
                
//                     player.get_build().get_arsenal().get_items()[2]->use_on(player_pair, x, y);   
                   
//                     if (player_map->is_boat(x, y))
//                     {        
//                         player_cells[index].setTexture(Rsrc::get_texture(Rsrc::boat_body_image())); 
//                         player_cells[index].setScale(CELL_SIZE_X / player_cells[index].getTexture()->getSize().x, CELL_SIZE_Y / player_cells[index].getTexture()->getSize().y);
//                     }

//                     // Ejecutamos el bot
//                     bool was_comodin = bot.play(std::make_shared<Player>(player));      // jugamos y vemos que tipo fue
//                     if (was_comodin)
//                     {
//                         std::cout << "jugo comodin\n";
//                         Drawer::draw(enemy_cells, radar_map, was_comodin);
                        
//                         bot_comodin_message.setFillColor(sf::Color(255,255,255,255));
//                         bot_shot_message.setFillColor(sf::Color(255,255,255,0));
//                     }
//                     else 
//                     {
//                         std::cout << "jugo misil\n";
//                         Drawer::draw(player_cells, player_map, was_comodin);

//                         bot_comodin_message.setFillColor(sf::Color(255,255,255,0));
//                         bot_shot_message.setFillColor(sf::Color(255,255,255,255));
//                     }
//                 }
//                 //Verificando si el bot ganó
//                 update_list_ships(bot, radar_map, boat_list); 
//                 if (player.get_build().get_fleet().get_boats().size() == winner_verification(player, player_map)+1)
//                 {
//                     bot_won = true;
//                     break;
//                 }
//             }
//         }

//         //PLAY DISPLAY
//         window.clear();
//         window.draw(info_panel);
//         window.draw(radar_panel);
//         window.draw(player_panel);
//         window.draw(exit_button);
//         window.draw(shot_button);
//         window.draw(shield_button);
//         window.draw(heal_button);
//         window.draw(bot_shot_message);
//         window.draw(bot_comodin_message);
       
//         for (const auto& casilla : enemy_cells)
//         {
//           window.draw(casilla);  
//         } 
       
//         for (const auto& casilla : player_cells)
//         {
//           window.draw(casilla);  
//         } 

//         for (const auto& boat_text : boat_list) 
//         {
//            window.draw(boat_text);
//         }
       
//         window.display();
//     }
// }

void build_window(sf::RenderWindow& window)
{
    window.clear();
    window.setTitle("Battle Ship - Build");

    // ---- Construct player and bot maps ---- //
    Map_ptr map = std::make_shared<Map>(10, 10);
    Map_ptr radar = std::make_shared<Map>(10, 10);    
    Bot bot("Bot", radar, map);
    Player player("Jugador", map, radar);

    // ---- WINDOW DIVISIONS AND SIZES ---- //
    const size_t INFO_WIDTH = Rsrc::WIN_SIZE.x / 3;              // 1/3 para la información

    const size_t MAP_WIDTH = Rsrc::WIN_SIZE.x - INFO_WIDTH;      // 2/3 para el mapa
    const size_t MAP_HEIGHT = Rsrc::WIN_SIZE.y * 0.8;

    const size_t CELL_SIZE_X = MAP_WIDTH / map->get_columns();  // Ancho y alto de las celdas
    const size_t CELL_SIZE_Y = MAP_HEIGHT / map->get_rows();

    // ---- LOAD TEXTURES ---- //
    sf::Texture waterTexture, barcoTexture, falloTexture, destruidoTexture, radarTexture, woodTableTexture, misilTexture, circleTexture, comodinTexture, shildTexture;
    circleTexture = Rsrc::createCircleTexture(CELL_SIZE_X, CELL_SIZE_Y);
    Rsrc::load_textures(waterTexture, barcoTexture, falloTexture, destruidoTexture, radarTexture, woodTableTexture, misilTexture, comodinTexture, shildTexture);

    // ---- PANELS ---- //
    sf::Sprite infoPanel;                                               // Background
    infoPanel.setPosition(0, 0);
    infoPanel.setTexture(woodTableTexture);
    Drawer::setSize(infoPanel, sf::Vector2f(Rsrc::WIN_SIZE.x, Rsrc::WIN_SIZE.y));

    sf::RectangleShape playerPanel(sf::Vector2f(MAP_WIDTH, MAP_HEIGHT)); // Panel where the player map is going to be
    playerPanel.setPosition(INFO_WIDTH, Rsrc::WIN_SIZE.y * 0.1);
    playerPanel.setTexture(&waterTexture);

    // ---- MAP CELLS ---- //
    std::vector<sf::Sprite> playerCells;
    for (int y = 0; y < map->get_rows(); ++y)            
    {
        for (int x = 0; x < map->get_columns(); ++x)
        {
            sf::Sprite sprite;
            if (map->is_water(x, y))
                sprite.setTexture(circleTexture);
            else if (map->is_boat(x, y))
                sprite.setTexture(barcoTexture);
            else if (map->is_failed(x, y))
                sprite.setTexture(falloTexture);
            else if (map->is_destroyed(x, y))
                sprite.setTexture(destruidoTexture);
            
            sprite.setScale(static_cast<float>(CELL_SIZE_X) / sprite.getTexture()->getSize().x,
                            static_cast<float>(CELL_SIZE_Y) / sprite.getTexture()->getSize().y);

            sprite.setPosition(INFO_WIDTH + x * CELL_SIZE_X, Rsrc::WIN_SIZE.y * 0.1 + y * CELL_SIZE_Y);
            playerCells.push_back(sprite);
        }
    }

    // ---- BUTONS ---- // 
    sf::Font font = Rsrc::getFont(Rsrc::titulosFont());

    // EXIT BUTTON
    sf::Text exitButton("Salir", font, Rsrc::getTextSize());        
    exitButton.setFillColor(sf::Color::White);
    exitButton.setPosition(Rsrc::WIN_SIZE.x * 0.03, Rsrc::WIN_SIZE.y * 0.05);

    // PLAY BUTTON
    sf::Text playButton("Jugar", font, Rsrc::getTextSize());
    playButton.setFillColor(sf::Color::White);
    playButton.setPosition(Rsrc::WIN_SIZE.x * 0.03 + exitButton.getGlobalBounds().width * 1.10, Rsrc::WIN_SIZE.y * 0.05);

    // ---- SHIPS ---- // 
    std::vector<sf::Sprite> barcos;
    for (int i = 0; i < 5; ++i)                 // 5 ships with decrecent sizes
    {
        sf::Sprite barco(barcoTexture);
        Drawer::setSize(barco, sf::Vector2f(CELL_SIZE_X * (5 - i), CELL_SIZE_Y));

        // Ajustar posición para que los barcos se alineen correctamente
        barco.setPosition(Rsrc::WIN_SIZE.x * 0.01, playButton.getGlobalBounds().height * 1.5 + (i * CELL_SIZE_Y) + ((i+1) * CELL_SIZE_Y * 0.10));
        barcos.push_back(barco);
    }
    
    // ---- EMPTY SHIPS  ---- //
    std::vector<sf::Sprite> emptyBoats;
    emptyBoats.resize(5);
    for (int i = 0; i < 5; ++i)         // When a ship is taken this will fill he empty space with a opaque ship
    {
        sf::Sprite emptyBoat(barcoTexture);
        emptyBoat.setScale(barcos[i].getScale());
        emptyBoat.setColor(sf::Color(255, 255, 255, 128));
        emptyBoat.setPosition(barcos[i].getPosition());
        emptyBoats[i] = emptyBoat;
    }


    // ---- COMODINES ---- //  
    std::vector<sf::Sprite> comodines;
    std::vector<sf::Text> cantComodin;
    comodines.resize(2);

    comodines[0].setPosition(Rsrc::WIN_SIZE.x * 0.01, Rsrc::WIN_SIZE.y * 0.51);
    comodines[0].setTexture(comodinTexture);
    comodines[1].setPosition(Rsrc::WIN_SIZE.x * 0.04 + Rsrc::getButtonSize(), Rsrc::WIN_SIZE.y * 0.51);
    comodines[1].setTexture(shildTexture);
    Drawer::setSize(comodines[0], sf::Vector2f(Rsrc::getButtonSize(), Rsrc::getButtonSize()));
    Drawer::setSize(comodines[1], sf::Vector2f(Rsrc::getButtonSize(), Rsrc::getButtonSize()));

    for (int i = 0; i < comodines.size(); ++i)      // Texto de cantidad
    {
        sf::Text cantidad("3", font, Rsrc::getTextSize());
        cantidad.setFillColor(sf::Color::White);
        sf::FloatRect text_bounds = cantidad.getLocalBounds();   // lo asingamos dentro del espacio del sprite
        cantidad.setOrigin(text_bounds.left, text_bounds.top);
        cantidad.setPosition(comodines[i].getPosition());
        cantComodin.push_back(cantidad);
    }


    // ---- PROYECTILES ---- //  
    std::vector<sf::Sprite> proyectiles;
    std::vector<sf::Text> cant_proyectiles;
    proyectiles.resize(4);
    
    sf::Sprite proyectile(Rsrc::getTexture(Rsrc::cannon_image()));
    Drawer::setSize(proyectile, sf::Vector2f(Rsrc::getButtonSize(), Rsrc::getButtonSize()));
    proyectiles[0] = proyectiles[1] = proyectiles [2] = proyectiles[3] = proyectile;
    proyectiles[0].setPosition(Rsrc::WIN_SIZE.x * 0.04, Rsrc::WIN_SIZE.y * 0.67);
    proyectiles[1].setPosition(Rsrc::WIN_SIZE.x * 0.04, Rsrc::WIN_SIZE.y * 0.69 + Rsrc::getButtonSize());
    proyectiles[2].setPosition(Rsrc::WIN_SIZE.x * 0.08 + Rsrc::getButtonSize(), Rsrc::WIN_SIZE.y * 0.67);
    proyectiles[3].setPosition(Rsrc::WIN_SIZE.x * 0.08 + Rsrc::getButtonSize(), Rsrc::WIN_SIZE.y * 0.69 + Rsrc::getButtonSize());

    for (int i = 0; i < proyectiles.size(); ++i)        // Texto de cantidad 
    {
        sf::Text cantidad("5", font, Rsrc::getTextSize());
        cantidad.setFillColor(sf::Color::White);
        sf::FloatRect text_bounds = cantidad.getLocalBounds();
        cantidad.setOrigin(text_bounds.left, text_bounds.top);
        cantidad.setPosition(proyectiles[i].getPosition());
        cant_proyectiles.push_back(cantidad);
    }

    // ---- MAIN LOOP ---- //
    bool horizontal = true;
    bool holding = false;
    size_t boatIndx;
    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (event.type == sf::Event::Closed)                        // Cerrar la ventana
            {
                return;
            }

            if (event.type == sf::Event::MouseMoved && holding)         // Mover el bote siguiendo el mouse 
            {
                barcos[boatIndx].setPosition(mousePos.x, mousePos.y);
            }

            // LEFT MOUSE CLICK
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {                
                if (playerPanel.getGlobalBounds().contains(mousePos))      // En el panel del mapa 
                {
                    for (size_t i = 0; i < playerCells.size(); ++i)      // Vemos que casilla fue clickeada
                    {
                        if (playerCells[i].getGlobalBounds().contains(mousePos) && holding) // si clickeo una celda con el barco tomado
                        {
                            size_t x = (i % map->get_columns());
                            size_t y = (i / map->get_columns());

                            std::string tipo = map->is_water(x, y) ? "Agua" :
                                            map->is_boat(x, y) ? "Barco" :
                                            map->is_failed(x, y) ? "Disparo fallido" :
                                            "Barco destruido";
                            
                            Coordinates coord(x, y);
                            Boat_ptr bote = std::make_shared<Boat>(5 - boatIndx, coord, horizontal);
                            if (!map->insert_boat(bote))
                            {
                                break;
                            }
                            
                            Drawer::draw(playerCells, map, *bote, true);  
                            player.get_build().get_fleet().add_boat(bote); // agregamos el barco al player

                            // Hacerlo invisible    
                            barcos[boatIndx].setPosition(-100, -100);                      // Lo sacamos de la pantalla
                            holding = false;
                            horizontal = true;
                        }
                        else if (playerCells[i].getGlobalBounds().contains(mousePos))       // si la clickeo sin ningun bote
                        {
                            int x = (i % map->get_columns());
                            int y = (i / map->get_columns());

                            std::string tipo = map->is_water(x, y) ? "Agua" :
                                            map->is_boat(x, y) ? "Barco" :
                                            map->is_failed(x, y) ? "Disparo fallido" :
                                            "Barco destruido";

                            std::cout << "Mapa - Casilla clickeada [" << x << ", " << y << "] - " << tipo << std::endl;

                            if (map->is_boat(x, y))      // Si clickeo el barco devolverlo a su posicion
                            {
                                auto boat = player.get_build().get_fleet().get_boat_of_cell(map->get_ptr_cell(x, y));   
                                size_t indx = 5 - boat->get_size();             // indice correspondiente al vector de sprite

                                Drawer::draw(playerCells, map, *boat, false);
                                map->delete_boat(boat);

                                // ponemos el sprite de nuevo en su posicion
                                barcos[indx].setRotation(0);                    
                                barcos[indx].setPosition(emptyBoats[indx].getPosition());
                                Drawer::setSize(barcos[indx], sf::Vector2f(CELL_SIZE_X * (5 - indx), CELL_SIZE_Y));
                                horizontal = true;

                                player.get_build().get_fleet().delete_boat(boat);
                            }
                        }
                    }  
                }
                else if (infoPanel.getGlobalBounds().contains(mousePos))   // En el panel de informacion
                {
                    if (holding)                                    // Soltar de nuevo el bote en su lugar
                    {
                        barcos[boatIndx].setRotation(0);
                        barcos[boatIndx].setPosition(emptyBoats[boatIndx].getPosition());
                        Drawer::setSize(barcos[boatIndx], sf::Vector2f(CELL_SIZE_X * (5 - boatIndx), CELL_SIZE_Y));
                        holding = false;
                        horizontal = true;
                    }
                    else if (exitButton.getGlobalBounds().contains(mousePos)) // Click en "Salir"
                    {
                        return;
                    }
                    else if (playButton.getGlobalBounds().contains(mousePos)) // Click en "Jugar"
                    {
                        std::cout << "Jugar. Empezando partida..." << std::endl;
                        Player jugador("Jugador", map, radar); 
                        //play_window(window, player, bot);
                        return;
                    }
                    for (size_t i = 0; i < comodines.size(); ++i)   // Click sobre algun comodin
                    {
                        if (comodines[i].getGlobalBounds().contains(mousePos))
                        {
                            std::cout << "Comodin " << i << "\n";
                        }
                    }
                    for (size_t i = 0; i < barcos.size(); ++i)      // Click sobre algun barco
                    {
                        if (barcos[i].getGlobalBounds().contains(mousePos))
                        {                            
                            barcos[i].setPosition(mousePos);
                            boatIndx = i;
                            holding = !holding;
                        }
                    }
                    for (size_t i = 0; i < proyectiles.size(); ++i) // Click sobre algun proyectil
                    {
                        if (proyectiles[i].getGlobalBounds().contains(mousePos)) 
                        {
                            std::cout << "Proyectil " << i << std::endl;
                        }
                    }  
                }
            }

            // Rotar el barco
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && holding)
            {
                horizontal ? barcos[boatIndx].rotate(90) : barcos[boatIndx].rotate(-90);  
                horizontal = !horizontal;
                if (horizontal)
                {
                    Drawer::setSize(barcos[boatIndx], sf::Vector2f(CELL_SIZE_X * (5 - boatIndx), CELL_SIZE_Y));
                }
                else 
                {
                    Drawer::setSize(barcos[boatIndx], sf::Vector2f(CELL_SIZE_Y  * (5 - boatIndx), CELL_SIZE_X));
                }
                std::cout << (horizontal? "horizontal" : "vertical") << "\n";
            }
        }
        window.clear();
        window.draw(infoPanel);
        window.draw(playerPanel);
        window.draw(exitButton);
        window.draw(playButton);

        // Dibujar los items del panel de info
        for (const auto& empty_boat : emptyBoats) window.draw(empty_boat);
        for (const auto& barco : barcos) window.draw(barco);
        for (const auto& comodin : comodines) window.draw(comodin);
        for (const auto& cantidad : cantComodin) window.draw(cantidad);
        for (const auto& proyectil : proyectiles) window.draw(proyectil);
        for (const auto& cantidad : cant_proyectiles) window.draw(cantidad);

        // Dibujar los vectores de sprites
        for (const auto& casilla : playerCells) window.draw(casilla);

        window.display();
    }
}

void menu_window()
{
    //MAIN WINDOW
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Battle Ship");  // Creamos la ventana con 
    window.setView(sf::View(sf::FloatRect(0, 0, 1920, 1080)));
    
    // BUTTONS
    sf::Font font = Rsrc::getFont(Rsrc::titulosFont());
    
    std::vector<sf::Text> buttons;  
    std::vector<std::string> nameButtons = {"Jugar", "Salir"};  
    for (size_t i = 0; i < nameButtons.size(); ++i)  
    {
        sf::Text boton;                          
        boton.setFont(font);                        
        boton.setString(nameButtons[i]);
        boton.setCharacterSize(Rsrc::getTextSize());
        boton.setFillColor(sf::Color::Black);
        boton.setPosition(Rsrc::WIN_SIZE.x * 0.05, (Rsrc::WIN_SIZE.y * 0.5) + (i * Rsrc::getTextSize() * 1.2));      
        buttons.push_back(boton);                
    }  

    // BACKGROUND SPRITE
    sf::Sprite backgroundSprite;             
    backgroundSprite.setPosition(0, 0);
    backgroundSprite.setTexture(Rsrc::getTexture(Rsrc::menu_image()));
    Drawer::setSize(backgroundSprite, sf::Vector2f(Rsrc::WIN_SIZE.x, Rsrc::WIN_SIZE.y));
     
    //MAIN LOOP
    window.setFramerateLimit(60);
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
            
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            
            // MENU EVENTS
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                for (size_t i = 0; i < buttons.size(); ++i) 
                {
                    if (buttons[i].getGlobalBounds().contains(mousePos)) 
                    {
                        if (i == 0)
                        {
                            std::cout << "Se presiono Jugar\n";
                            build_window(window);
                        }
                        else if (i == 1) 
                        {
                            window.close();
                            exit(EXIT_SUCCESS);
                        }
                    }
                }   
            } 
            // HIGTH LIGTH BOTONS
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
        window.draw(backgroundSprite);    
        
        for (const auto& boton : buttons) window.draw(boton);
        
        window.display();
    }
}

int main()
{
    menu_window();
} 