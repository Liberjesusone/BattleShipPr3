#include "Visual.hpp"
#include "Types.hpp"
#include <map>
#include <chrono>
#include <thread>
 
void winnerWindow(sf::RenderWindow& window, bool playerWon)
{
    window.clear();
    window.setTitle((playerWon ? "Has ganado" : "Has perdido"));

    sf::Font font = Rsrc::getFont(Rsrc::titulosFont());
    sf::Texture texture = Rsrc::getTexture(Rsrc::winner_image());
    sf::Sprite background(texture);
    Drawer::setSize(background, sf::Vector2f(Rsrc::WIN_SIZE.x, Rsrc::WIN_SIZE.y));

    sf::Text messageText((playerWon ? "Has ganado" : "Has perdido"), font, Rsrc::getTextSize());
    messageText.setFillColor(sf::Color::White);
    messageText.setPosition(Rsrc::WIN_SIZE.x * 0.39, Rsrc::WIN_SIZE.y * 0.8);    // Posicionando el botón en el centro

    sf::Text returnButton("Regresar", font, Rsrc::getTextSize());
    returnButton.setFillColor(sf::Color::White);
    returnButton.setPosition(Rsrc::WIN_SIZE.x * 0.41, messageText.getGlobalBounds().height + Rsrc::WIN_SIZE.y * 0.8);    // Posicionando el botón en el centro

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            if (event.type == sf::Event::Closed)
            {    
                return;
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (returnButton.getGlobalBounds().contains(mousePos))
                { 
                    return;
                }
            }
            
            if (returnButton.getGlobalBounds().contains(mousePos))
            {
                returnButton.setFillColor(sf::Color::Black);  // Cambia a negro
            }
            else
            {
                returnButton.setFillColor(sf::Color::White); // Mantiene el color original
            }            
        }

        window.clear();
        window.draw(background);
        window.draw(returnButton);
        window.draw(messageText);
        window.display();
    }
}

void play_window(sf::RenderWindow& window, Player& player, Bot& bot) 
{
    window.clear();
    window.setTitle("Battle Ship - Jugar");

    // ---- MAPS ---- //
    Map_ptr playerMap = player.get_map();
    Map_ptr radarMap = player.get_radar();
    PlayerPair playerPair = std::make_pair(std::make_shared<Player>(player), std::make_shared<Player>(bot));

    // ---- WINDOW DIVISIONS AND SIZES ---- //
    const size_t INFO_WIDTH = Rsrc::WIN_SIZE.x / 3;              // 1/3 para la información
    
    const size_t MAP_WIDTH = Rsrc::WIN_SIZE.x - INFO_WIDTH;      // 2/3 para el mapa
    const size_t MAP_HEIGHT = Rsrc::WIN_SIZE.y * 0.5;            // 1/2 de altura  
    
    const size_t CELL_SIZE_X = MAP_WIDTH / playerMap->get_columns();  // Ancho y alto de las celdas
    const size_t CELL_SIZE_Y = MAP_HEIGHT / playerMap->get_rows();
        
    // ---- TEXTURES ---- //
    sf::Texture circleTexture, waterTexture, barcoTexture, falloTexture, destruidoTexture, radarTexture, woodTableTexture, cannonTexture, comodinTexture, shildTexture;
    circleTexture = Rsrc::createCircleTexture(CELL_SIZE_X, CELL_SIZE_Y);
    Rsrc::load_textures(waterTexture, barcoTexture, falloTexture, destruidoTexture, radarTexture, woodTableTexture, cannonTexture, comodinTexture, shildTexture);
    
    // ---- PANELS ---- //                                                  // Background
    sf::RectangleShape infoPanel(sf::Vector2f(INFO_WIDTH, Rsrc::WIN_SIZE.y));     // panel de informacion a la izq
    infoPanel.setPosition(0, 0);
    infoPanel.setTexture(&woodTableTexture);
    Drawer::setSize(infoPanel, sf::Vector2f(Rsrc::WIN_SIZE.x, Rsrc::WIN_SIZE.y));

    // ---- RADAR PANEL ---- //
    sf::RectangleShape radarPanel(sf::Vector2f(MAP_WIDTH, MAP_HEIGHT));  // panel del radar arriba a la derecha
    radarPanel.setPosition(INFO_WIDTH, 0);
    radarPanel.setTexture(&Rsrc::getTexture(Rsrc::water_radar_image()));

    // ---- PLAYER PANEL ---- //
    sf::RectangleShape playerPanel(sf::Vector2f(MAP_WIDTH, MAP_HEIGHT)); // panel del mapa propio abajo a la derecha    
    playerPanel.setPosition(INFO_WIDTH, MAP_HEIGHT);
    playerPanel.setTexture(&waterTexture);    

    // ---- CELLS - RADAR ---- //
    std::vector<sf::Sprite> enemyCells, playerCells;
    for (int y = 0; y < radarMap->get_rows(); ++y)
    {
        for (int x = 0; x < radarMap->get_columns(); ++x)
        {   
            sf::Sprite sprite;
            sf::Sprite sprite2;
            sprite.setTexture(circleTexture);
            sprite2.setTexture(circleTexture);
            
            Drawer::setSize(sprite, sf::Vector2f(CELL_SIZE_X, CELL_SIZE_Y));
            Drawer::setSize(sprite2, sf::Vector2f(CELL_SIZE_X, CELL_SIZE_Y));

            sprite.setPosition(INFO_WIDTH + x * CELL_SIZE_X, y * CELL_SIZE_Y);
            sprite2.setPosition(INFO_WIDTH + x * CELL_SIZE_X, MAP_HEIGHT + y * CELL_SIZE_Y);
            enemyCells.push_back(sprite);
            playerCells.push_back(sprite2);
        }
    }

    for (auto boat : player.get_build().get_fleet().get_boats())
    {
        Drawer::draw(playerCells, playerMap, *boat, true);
    }
    
    
    sf::Font font = Rsrc::getFont(Rsrc::titulosFont());
    // ---- EXIT BUTTON ---- //
    sf::Text exitButton("Salir", font, Rsrc::getTextSize());        
    exitButton.setFillColor(sf::Color::White);
    exitButton.setPosition(Rsrc::WIN_SIZE.x * 0.03, Rsrc::WIN_SIZE.y * 0.05);
    
     // ---- COMODINES ---- //  
    std::vector<sf::Sprite> comodines;
    std::vector<sf::Text> cantComodin;
    comodines.resize(2);

    comodines[0].setPosition(Rsrc::WIN_SIZE.x * 0.04, Rsrc::WIN_SIZE.y * 0.43);
    comodines[0].setTexture(comodinTexture);
    comodines[1].setPosition(Rsrc::WIN_SIZE.x * 0.08 + Rsrc::getButtonSize(), Rsrc::WIN_SIZE.y * 0.43);
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
    std::vector<sf::Text> cantProyectiles;
    proyectiles.resize(2);
    
    sf::Sprite proyectile(Rsrc::getTexture(Rsrc::cannon_image()));
    Drawer::setSize(proyectile, sf::Vector2f(Rsrc::getButtonSize(), Rsrc::getButtonSize()));
    proyectiles[0] = proyectiles[1] = proyectile;
    proyectiles[0].setPosition(Rsrc::WIN_SIZE.x * 0.04, Rsrc::WIN_SIZE.y * 0.59);
    proyectiles[1].setPosition(Rsrc::WIN_SIZE.x * 0.08 + Rsrc::getButtonSize(), Rsrc::WIN_SIZE.y * 0.59);

    for (int i = 0; i < proyectiles.size(); ++i)        // Texto de cantidad 
    {
        sf::Text cantidad("3", font, Rsrc::getTextSize());
        cantidad.setFillColor(sf::Color::White);
        sf::FloatRect text_bounds = cantidad.getLocalBounds();
        cantidad.setOrigin(text_bounds.left, text_bounds.top);
        cantidad.setPosition(proyectiles[i].getPosition());
        cantProyectiles.push_back(cantidad);
    }
    cantProyectiles[0].setString("");

    // ---- INFORMATIVE BOARD ---- //
    sf::RectangleShape board(sf::Vector2f(INFO_WIDTH * 0.95, Rsrc::getButtonSize() * 1.2)); // panel del mapa propio abajo a la derecha    
    board.setPosition(Rsrc::WIN_SIZE.x * 0.005, proyectiles[0].getGlobalBounds().height * 1.1 + Rsrc::WIN_SIZE.y * 0.59);
    board.setTexture(&Rsrc::getTexture(Rsrc::sign_image()));  

    sf::Text information("El bot no ha jugado", font, Rsrc::getTextSize() * 0.55);
    information.setPosition(Rsrc::WIN_SIZE.x * 0.035, board.getPosition().y * 1.1);
    information.setColor(sf::Color::Black);

    // ---- SHIPS ---- // 
    std::vector<sf::Sprite> barcos;
    for (int i = 0; i < 5; ++i)                 // 5 ships with decrecent sizes
    {
        sf::Sprite barco(barcoTexture);
        Drawer::setSize(barco, sf::Vector2f((CELL_SIZE_X * (5 - i)) * 0.8, CELL_SIZE_Y));

        barco.setPosition(Rsrc::WIN_SIZE.x * 0.01, exitButton.getGlobalBounds().height * 1.5 + (i * CELL_SIZE_Y) + ((i+1) * CELL_SIZE_Y * 0.10));
        barcos.push_back(barco);
    }
  
    //DEBUG  si se quiere debuguear comentar la linea  117 enemyCells.push_back(sprite);
    // ---- MAP CELLS ---- //
    // for (int y = 0; y < radarMap->get_rows(); ++y)            
    // {
    //     for (int x = 0; x < radarMap->get_columns(); ++x)
    //     {
    //         sf::Sprite sprite;
    //         if (radarMap->is_water(x, y))
    //             sprite.setTexture(circleTexture);
    //         else if (radarMap->is_boat(x, y))
    //             sprite.setTexture(barcoTexture);
    //         else if (radarMap->is_failed(x, y))
    //             sprite.setTexture(falloTexture);
    //         else if (radarMap->is_destroyed(x, y))
    //             sprite.setTexture(destruidoTexture);
            
    //         Drawer::setSize(sprite, sf::Vector2f(CELL_SIZE_X, CELL_SIZE_Y));
    //         sprite.setPosition(INFO_WIDTH + x * CELL_SIZE_X, y * CELL_SIZE_Y);
    //         enemyCells.push_back(sprite);
    //     }
    // }


    //CONTROL VARIABLES
    int x = 0;
    int y = 0;
    bool isShotValid = false;
    bool isShieldValid = false;
    bool isHealValid = false;
    
    auto lockButtons = [&comodines, &isHealValid, &isShieldValid, &proyectiles, &isShotValid]()
    {
        isShotValid = false;
        isShieldValid = false;
        isHealValid = false;
        proyectiles[0].setColor(sf::Color(255,255,255,128));
        proyectiles[1].setColor(sf::Color(255,255,255,128));
        comodines[0].setColor(sf::Color(255,255,255,128));
        comodines[1].setColor(sf::Color(255,255,255,128));
    };

    // Retorna true si se acabo el juego
    auto checkWinner = [&window, &playerPair, &barcos]() -> bool
    {
        if (playerPair.first->get_build().get_fleet().isDestroyed(playerPair.first->get_map()))
        {
            winnerWindow(window, false);
            return true;
        }
        bool playerWon = true;
        size_t i = 0;
        for (auto boat : playerPair.second->get_build().get_fleet().get_boats())
        {
            if (boat->get_distruction_per(playerPair.second ->get_map()) == 100.00f)
            {
                barcos[i].setColor(sf::Color(255, 100, 100));
            }
            else 
            {
                playerWon = false;
                barcos[i].setColor(sf::Color::White);
            }
            ++i;
        }
        if (playerWon)
        {
            winnerWindow(window, true);
            return true;
        }   
        return false;
    };

    //PLAY LOOP
    while(window.isOpen())
    {
        sf::Event event;

        //PLAY EVENTS LOOP
        while(window.pollEvent(event))
        {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            // CLOSE WINDOW
            if (event.type == sf::Event::Closed)                        
            {
                return;
            }

            // DETECTING LEFT MOUSE CLICK
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {   
                // ENEMY MAP - Select cell
                if (radarPanel.getGlobalBounds().contains(mousePos))       
                {
                    lockButtons();

                    for (size_t i = 0; i < enemyCells.size(); ++i) 
                    {
                        if (enemyCells[i].getGlobalBounds().contains(mousePos)) 
                        {
                            x = (i % radarMap->get_columns());
                            y = (i / radarMap->get_columns());  

                            if (!radarMap->is_failed(x, y))
                            {
                                proyectiles[0].setColor(sf::Color(255,255,255,255));
                                cantProyectiles[1].getString() != "0" ? proyectiles[1].setColor(sf::Color(255,255,255,255)) : void();
                                isShotValid = true;                            
                            }      
                        }
                    }   
                }
                // PLAYER MAP - Select cell
                else if (playerPanel.getGlobalBounds().contains(mousePos))
                {
                    lockButtons();

                    for (size_t i = 0; i < playerCells.size(); ++i) 
                    {
                        if (playerCells[i].getGlobalBounds().contains(mousePos)) 
                        {                                            
                            x = (i % playerMap->get_columns());
                            y = (i / playerMap->get_columns());
                            
                            //El comodin SHIELD solo se activa si la casilla es un bote
                            if (playerMap->is_boat(x, y))
                            {
                                isShieldValid = true;
                                cantComodin[1].getString() != "0" ? comodines[1].setColor(sf::Color(255,255,255,255)) : void();
                            }
                            //El comodin HEAL solo se activa si la casilla es un bote destruido
                            else if (playerMap->is_destroyed(x, y))
                            {
                                isHealValid = true;
                                cantComodin[0].getString() != "0" ? comodines[0].setColor(sf::Color(255,255,255,255)): void();
                            }
                        }
                    }  
                }
                // INFO PANEL - Buttons
                else if (infoPanel.getGlobalBounds().contains(mousePos))
                {
                    // Salir
                    if (exitButton.getGlobalBounds().contains(mousePos))
                    {
                        std::cout<<"SALIR JUEGO\n";
                        return;
                    }
                    // Click sobre algun comodin
                    for (size_t i = 0; i < comodines.size() && (isHealValid || isShieldValid); ++i)   
                    {
                        if (comodines[i].getGlobalBounds().contains(mousePos))
                        {   
                            auto item = player.get_build().get_arsenal().get_items()[2 + i]; 
                            if ((i == 0 && !isHealValid) || (i == 1 && !isShieldValid))
                            {
                                break;
                            }  
                            if (!item->get_stock())
                            {
                                break;
                            }

                            lockButtons();
                            size_t index = y * playerMap->get_columns() + x;
                            Map_cell_ptr cell = playerMap->get_ptr_cell(x, y);                    

                            item->use_on(playerPair, x, y);
                            cantComodin[i].setString(std::to_string(item->get_stock())); 
                            
                            // Visual Adjust
                            sf::Sprite sprite;
                            if (playerMap->is_protected(x, y))
                            {        
                                sprite.setTexture(Rsrc::getTexture(Rsrc::shield_image())); 
                                sprite.setPosition(playerCells[index].getPosition());
                            }
                            else if (playerMap->is_boat(x, y))
                            {

                                sprite.setTexture(Rsrc::getTexture(Rsrc::boat_body_image())); 
                                sprite.setPosition(playerCells[index].getPosition());
                            }
                            Drawer::setSize(sprite, sf::Vector2f(CELL_SIZE_X, CELL_SIZE_Y));
                            playerCells[index] = sprite;
            
                            // Ejecutamos el bot
                            bool was_comodin = bot.make_movement(std::make_shared<Player>(player));      // jugamos y vemos que tipo fue
                            was_comodin ? Drawer::draw(enemyCells, radarMap, was_comodin) : Drawer::draw(playerCells, playerMap, was_comodin);                       
                            information.setString(bot.getInformation());

                            if (checkWinner())
                            {
                                return;
                            }
                        }
                    }
                    // Click sobre algun proyectil
                    for (size_t i = 0; i < proyectiles.size() && isShotValid; ++i)
                    {
                        if (proyectiles[i].getGlobalBounds().contains(mousePos)) 
                        {
                            auto item = player.get_build().get_arsenal().get_items()[i];
                            if (!item->get_stock())
                            {
                                break;
                            }

                            lockButtons();

                            size_t index = y * radarMap->get_columns() + x;
                            Map_cell_ptr cell = radarMap->get_ptr_cell(x, y);                    
                            
                            item->use_on(playerPair, x, y);   
                            if (i)                              // Actualizamos la cantidad si no es el misil infinito
                            {
                                cantProyectiles[i].setString(std::to_string(item->get_stock())); 
                            }

                            // Ejecutamos el bot
                            bool wasComodin = bot.make_movement(std::make_shared<Player>(player));      // jugamos y vemos que tipo fue
                            Drawer::draw(enemyCells, radarMap, true);
                            Drawer::draw(playerCells, playerMap, false);
                            information.setString(bot.getInformation());

                            if (checkWinner())
                            {
                                return;
                            }
                        }
                    } 
                }
            }
        }

        //PLAY DISPLAY
        window.clear();
        window.draw(infoPanel);
        window.draw(board);
        window.draw(information);
        window.draw(radarPanel);
        window.draw(playerPanel);
        window.draw(exitButton);
       
        for (const auto& barco : barcos) window.draw(barco);
        for (const auto& casilla : enemyCells) window.draw(casilla);  
        for (const auto& casilla : playerCells) window.draw(casilla);  
        for (const auto& comodin : comodines) window.draw(comodin);
        for (const auto& cantidad : cantComodin) window.draw(cantidad);
        for (const auto& proyectil : proyectiles) window.draw(proyectil);
        for (const auto& cantidad : cantProyectiles) window.draw(cantidad);

        window.display();
    }
}

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
            
            Drawer::setSize(sprite, sf::Vector2f(CELL_SIZE_X, CELL_SIZE_Y));

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
    playButton.setPosition(Rsrc::WIN_SIZE.x * 0.06 + exitButton.getGlobalBounds().width * 1.10, Rsrc::WIN_SIZE.y * 0.05);

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

    comodines[0].setPosition(Rsrc::WIN_SIZE.x * 0.04, Rsrc::WIN_SIZE.y * 0.53);
    comodines[0].setTexture(comodinTexture);
    comodines[1].setPosition(Rsrc::WIN_SIZE.x * 0.08 + Rsrc::getButtonSize(), Rsrc::WIN_SIZE.y * 0.53);
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
    proyectiles.resize(2);
    
    sf::Sprite proyectile(Rsrc::getTexture(Rsrc::cannon_image()));
    Drawer::setSize(proyectile, sf::Vector2f(Rsrc::getButtonSize(), Rsrc::getButtonSize()));
    proyectiles[0] = proyectiles[1] = proyectile;
    proyectiles[0].setPosition(Rsrc::WIN_SIZE.x * 0.04, Rsrc::WIN_SIZE.y * 0.69);
    proyectiles[1].setPosition(Rsrc::WIN_SIZE.x * 0.08 + Rsrc::getButtonSize(), Rsrc::WIN_SIZE.y * 0.69);

    for (int i = 0; i < proyectiles.size(); ++i)        // Texto de cantidad 
    {
        sf::Text cantidad("3", font, Rsrc::getTextSize());
        cantidad.setFillColor(sf::Color::White);
        sf::FloatRect text_bounds = cantidad.getLocalBounds();
        cantidad.setOrigin(text_bounds.left, text_bounds.top);
        cantidad.setPosition(proyectiles[i].getPosition());
        cant_proyectiles.push_back(cantidad);
    }
    cant_proyectiles[0].setString("");

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
            // Cerrar la ventana
            if (event.type == sf::Event::Closed)                        
            {
                return;
            }
            // Mover el bote siguiendo el mouse
            if (event.type == sf::Event::MouseMoved && holding)          
            {
                barcos[boatIndx].setPosition(mousePos.x, mousePos.y);
            }

            // LEFT MOUSE CLICK - Buttons
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {                
                // MAP PANEL 
                if (playerPanel.getGlobalBounds().contains(mousePos))      
                {
                    for (size_t i = 0; i < playerCells.size(); ++i)      // Vemos que casilla fue clickeada
                    {
                        // Si clickeo una celda con el barco tomado
                        if (playerCells[i].getGlobalBounds().contains(mousePos) && holding) 
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
                            barcos[boatIndx].setPosition(-1000, -1000);      // Lo sacamos de la pantalla
                            holding = false;
                            horizontal = true;
                        }
                        // Si la clickeo sin ningun bote
                        else if (playerCells[i].getGlobalBounds().contains(mousePos))       
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
                // INFO PANEL
                else if (infoPanel.getGlobalBounds().contains(mousePos))   
                {
                    // Soltar de nuevo el bote en su lugar
                    if (holding)                                    
                    {
                        barcos[boatIndx].setRotation(0);
                        barcos[boatIndx].setPosition(emptyBoats[boatIndx].getPosition());
                        Drawer::setSize(barcos[boatIndx], sf::Vector2f(CELL_SIZE_X * (5 - boatIndx), CELL_SIZE_Y));
                        holding = false;
                        horizontal = true;
                    }
                    // Click en "Salir"
                    else if (exitButton.getGlobalBounds().contains(mousePos)) 
                    {
                        return;
                    }
                    // Click en "Jugar"
                    else if (playButton.getGlobalBounds().contains(mousePos)) 
                    {
                        std::cout << "Jugar. Empezando partida..." << std::endl;
                        Player jugador("Jugador", map, radar); 
                        play_window(window, player, bot);
                        return;
                    }
                    // Click sobre algun barco
                    for (size_t i = 0; i < barcos.size(); ++i)      
                    {
                        if (barcos[i].getGlobalBounds().contains(mousePos))
                        {                            
                            barcos[i].setPosition(mousePos);
                            boatIndx = i;
                            holding = !holding;
                        }
                    }
                }
            }

            // RIGHT MOUSE CLICK - Rotar el barco
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