#include <SFML/Graphics.hpp>
#include "Structures.hpp"
#include "Types.hpp"


void load_textures(sf::Texture& agua_texture, sf::Texture& barco_texture, sf::Texture& fallo_texture, sf::Texture& destruido_texture, 
    sf::Texture& radar_texture, sf::Texture& pirate_map)
{
    if (!agua_texture.loadFromFile("resources/textures/water copy.jpg") ||
        !barco_texture.loadFromFile("resources/textures/boteNaval-S.png") ||
        !fallo_texture.loadFromFile("resources/textures/fallo.jpg") ||
        !destruido_texture.loadFromFile("resources/textures/destruido.jpg") ||
        !radar_texture.loadFromFile("resources/textures/PixelRadar.jpg") ||
        !pirate_map.loadFromFile("resources/textures/layout.jpg")) 
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
sf::Texture create_circule_texture(double CELL_SIZE_X, double CELL_SIZE_Y)
{
    // Crear textura con espacio totalmente transparente
    sf::RenderTexture render_texture; 
    render_texture.create(CELL_SIZE_X, CELL_SIZE_Y);

    // Limpiar con transparencia total
    render_texture.clear(sf::Color::Transparent); 

    // Ajustar grosor del borde dentro del espacio visible
    float border_thickness = 5.0f;                                // Grosor del borde negro
    sf::RectangleShape borde(sf::Vector2f(CELL_SIZE_X - border_thickness, CELL_SIZE_Y - border_thickness));
    borde.setFillColor(sf::Color::Transparent);                  // Para que el interior no se opaque
    borde.setOutlineColor(sf::Color(255, 255, 255, 90));         // Color del borde
    borde.setOutlineThickness(border_thickness);                  // Grosor ajustado
    borde.setPosition(border_thickness / 2, border_thickness / 2); // Centrar el borde dentro del espacio

    // Crear el círculo translúcido en el centro
    double radius = static_cast<double>(std::min(CELL_SIZE_X, CELL_SIZE_Y)) / 4;
    sf::CircleShape punto(radius);
    punto.setFillColor(sf::Color(128, 128, 128, 150)); // Gris translúcido
    punto.setPosition((CELL_SIZE_X - radius) / 2, (CELL_SIZE_Y - radius) / 2); 

    // Dibujar en la textura
    render_texture.draw(borde);  // Primero el borde negro (ahora visible)
    render_texture.draw(punto);  // Luego el círculo translúcido en el centro
    render_texture.display();

    // Extraer la textura generada
    sf::Texture circle_texture = render_texture.getTexture();
    return circle_texture;
}


/**
 * 
 */
sf::Texture create_shot_texture(double CELL_SIZE_X, double CELL_SIZE_Y)
{
    // Crear textura con espacio totalmente transparente
    sf::RenderTexture render_texture; 
    render_texture.create(CELL_SIZE_X, CELL_SIZE_Y);

    // Limpiar con transparencia total
    render_texture.clear(sf::Color::Transparent); 

    // Ajustar grosor del borde dentro del espacio visible
    float border_thickness = 5.0f;  // Grosor del borde negro
    double radius = static_cast<double>(std::min(CELL_SIZE_X, CELL_SIZE_Y)) / 2; // Reducir el radio para que todo encaje

    // Crear círculo amarillo con borde negro
    sf::CircleShape borde(radius - border_thickness);
    borde.setFillColor(sf::Color::Yellow);       // Color interno amarillo
    borde.setOutlineColor(sf::Color::Black);     // Borde negro
    borde.setOutlineThickness(border_thickness); // Grosor ajustado
    borde.setPosition((CELL_SIZE_X - borde.getRadius() * 2) / 2, (CELL_SIZE_Y - borde.getRadius() * 2) / 2); // Centrarlo

    // Cargar textura del misil
    sf::Texture misil_texture;
    if (!misil_texture.loadFromFile("resources/textures/missile.png")) {
        throw std::runtime_error("No se pudo cargar la textura del misil.");
    }

    // Crear el círculo del misil en el centro
    sf::Sprite misil(misil_texture);
    misil.setScale((radius * 1.5) / misil_texture.getSize().x, (radius * 1.5) / misil_texture.getSize().y);
    misil.setPosition((CELL_SIZE_X - misil.getGlobalBounds().width) / 2, 
                      (CELL_SIZE_Y - misil.getGlobalBounds().height) / 2); // Centrar

    // Dibujar en la textura
    render_texture.draw(borde);  // Primero el círculo amarillo con borde negro
    render_texture.draw(misil);  // Luego el misil centrado en el círculo
    render_texture.display();

    // Extraer la textura generada
    sf::Texture circle_texture = render_texture.getTexture();
    return circle_texture;
}



int play_window(sf::RenderWindow& window) 
{
    window.clear();

    // Crear la ventana en pantalla completa
    window.setTitle("Battle Ship - Juego");

    // Tamaños y divisiones de la pantalla
    sf::Vector2u windowSize = window.getSize();
    const int INFO_WIDTH = windowSize.x / 3;         // 1/3 para la información
    const int MAP_WIDTH = windowSize.x - INFO_WIDTH; // 2/3 para mapas
    const int MAP_HEIGHT = windowSize.y;

    const int RADAR_HEIGHT = MAP_HEIGHT / 2;         // Altura de ambos mapas

    // Construimos los mapas
    Party::Map player_map(5, 10);
    Party::Map radar_map(5, 10);

    
    // ---- CARGAR TEXTURAS ---- //
    sf::Texture agua_texture, barco_texture, fallo_texture, destruido_texture, radar_texture, pirate_map;
    load_textures(agua_texture, barco_texture, fallo_texture, destruido_texture, radar_texture, pirate_map);
    
    // Creacion del tamaño de cada celda y la textura de celda vacia = circle_texture
    const int CELL_SIZE_X = MAP_WIDTH / radar_map.get_columns();  // Ancho y alto de las celdas
    const int CELL_SIZE_Y = RADAR_HEIGHT / radar_map.get_rows();

    sf::Texture circle_texture = create_circule_texture(CELL_SIZE_X, CELL_SIZE_Y);


    // ---- PANELES ---- //
    // Posicionamiento de paneles
    sf::RectangleShape info_panel(sf::Vector2f(MAP_WIDTH, MAP_HEIGHT));     // panel de informacion a la izq
    info_panel.setPosition(0, 0);
    info_panel.setTexture(&pirate_map);

    sf::RectangleShape radar_panel(sf::Vector2f(MAP_WIDTH, RADAR_HEIGHT));  // panel del radar arriba a la derecha
    radar_panel.setPosition(INFO_WIDTH, 0);
    radar_panel.setTexture(&radar_texture);

    sf::RectangleShape player_panel(sf::Vector2f(MAP_WIDTH, RADAR_HEIGHT)); // panel del mapa propio abajo a la derecha
    player_panel.setPosition(INFO_WIDTH, RADAR_HEIGHT);
    player_panel.setTexture(&agua_texture);


    // Asignar una textura a cada casilla del mapa
    std::vector<sf::Sprite> radar_casillas, player_casillas;
    for (int y = 0; y < radar_map.get_rows(); ++y)              // mapa del radar
    {
        for (int x = 0; x < radar_map.get_columns(); ++x) 
        {
            sf::Sprite sprite;
            if (radar_map.is_water(x, y))
                sprite.setTexture(circle_texture);
            else if (radar_map.is_boat(x, y))
                sprite.setTexture(barco_texture);
            else if (radar_map.is_failed(x, y))
                sprite.setTexture(fallo_texture);
            else if (radar_map.is_destroyed(x, y))
                sprite.setTexture(destruido_texture);

            sprite.setScale(static_cast<float>(CELL_SIZE_X) / sprite.getTexture()->getSize().x,
                            static_cast<float>(CELL_SIZE_Y) / sprite.getTexture()->getSize().y);

            sprite.setPosition(INFO_WIDTH + x * CELL_SIZE_X, y * CELL_SIZE_Y);
            radar_casillas.push_back(sprite);
        }
    }

    for (int y = 0; y < player_map.get_rows(); ++y)            // mapa del jugador
    {
        for (int x = 0; x < player_map.get_columns(); ++x)
        {
            sf::Sprite sprite;
            if (player_map.is_water(x, y))
                sprite.setTexture(circle_texture);
            else if (player_map.is_boat(x, y))
                sprite.setTexture(barco_texture);
            else if (player_map.is_failed(x, y))
                sprite.setTexture(fallo_texture);
            else if (player_map.is_destroyed(x, y))
                sprite.setTexture(destruido_texture);

            sprite.setScale(static_cast<float>(CELL_SIZE_X) / sprite.getTexture()->getSize().x,
                            static_cast<float>(CELL_SIZE_Y) / sprite.getTexture()->getSize().y);

            sprite.setPosition(INFO_WIDTH + x * CELL_SIZE_X, RADAR_HEIGHT + y * CELL_SIZE_Y);
            player_casillas.push_back(sprite);
        }
    }


    // ---- BOTONES ---- // 
    sf::Font font;
    if (!font.loadFromFile("resources/fonts/tittle_font.ttf")) 
    {
        throw std::runtime_error("No se pudo cargar la fuente.");
    }

    // Botón de Salida
    sf::Text exit_button("Salir", font, 50);        //OJO darle a los bones un tamaño escalable
    exit_button.setFillColor(sf::Color::Yellow);
    exit_button.setPosition(20, 20);

    // Botón de Terminar Partida
    sf::Text end_game_button("Terminar Partida", font, 50);
    end_game_button.setFillColor(sf::Color::Yellow);
    end_game_button.setPosition(190, 20);

    // Boton de Disparar
    sf::Texture misil_texture = create_shot_texture(100, 100);
    sf::Sprite shot_button(misil_texture);
    shot_button.setPosition((MAP_WIDTH / 2) - 100, MAP_HEIGHT / 2);


    // ---- BARCOS ---- // 
    std::vector<sf::Sprite> barcos;
    sf::Texture texture;
    texture.loadFromFile("resources/textures/boteNaval-S.png");
    for (int i = 0; i < 5; ++i) // 5 barcos con tamaños decrecientes
    {
        sf::Sprite barco(texture);
    
        // Factor de reducción progresiva
        double scale_factor = 1.30 - (i * 0.15);    // Reduce cada barco un 15% más pequeño que el anterior
        double scale_factor_y = 0.8 - (i * 0.05);
        barco.setScale(scale_factor, scale_factor_y); // Mantiene proporción correcta
    
        // Ajustar posición para que los barcos se alineen correctamente
        barco.setPosition(10, i * (texture.getSize().y - 7) + 90); // Espaciado vertical dinámico
        barcos.push_back(barco);
    }

    // ---- COMODINES ---- //  
    std::vector<sf::Sprite> comodines;
    std::vector<sf::Text> comodinCantidad;
    sf::Texture comodinTexture;
    comodinTexture.loadFromFile("resources/textures/heart.png");
    for (int i = 0; i < 2; ++i) 
    {
        sf::Sprite comodin(comodinTexture);
        comodin.setScale(static_cast<double>(MAP_WIDTH / 7) / comodinTexture.getSize().x,
                         static_cast<double>(MAP_HEIGHT / 7) / comodinTexture.getSize().y);
        comodin.setPosition(i * (MAP_WIDTH / 7) * 1.20, MAP_HEIGHT / 2);
        comodines.push_back(comodin);
        
        sf::Text cantidad("3", font, 50); // Ejemplo: 3 comodines restantes
        cantidad.setFillColor(sf::Color::White);
        float padding_x = comodin.getTexture()->getSize().x / 45;
        float padding_y = comodin.getTexture()->getSize().y / 45;
        cantidad.setPosition(comodin.getPosition().x + padding_x, comodin.getPosition().y + padding_y);
        comodinCantidad.push_back(cantidad);
    }

    // ---- PROYECTILES ---- //  
    std::vector<sf::Sprite> proyectiles;
    std::vector<sf::Text> proyectilCantidad;
    sf::Texture projectile_texture;
    projectile_texture.loadFromFile("resources/textures/fallo.jpg");

    int x_position = 25;
    int y_position = MAP_HEIGHT * 2 / 3 ;
    int j = 0;
    for (int i = 0; i < 4; ++i) 
    {
        if (i == 2)
        {
            x_position = 35 + MAP_WIDTH / 7 * 1.2;
            j = 0;
        }

        sf::Sprite proyectil(projectile_texture);
        proyectil.setScale(static_cast<double>(MAP_WIDTH / 7) / projectile_texture.getSize().x,
                           static_cast<double>(MAP_HEIGHT / 7) / projectile_texture.getSize().y);
        proyectil.setPosition(x_position, y_position + j * (MAP_HEIGHT / 7 * 1.2));
        proyectiles.push_back(proyectil);

        sf::Text cantidad("5", font, 50); // Ejemplo: 5 proyectiles restantes
        cantidad.setFillColor(sf::Color::White);
        cantidad.setPosition(proyectil.getPosition().x, proyectil.getPosition().y);
        proyectilCantidad.push_back(cantidad);

        ++j;
    }

    sf::Clock clock;
    window.setFramerateLimit(60);
    // Bucle principal del juego
    while (window.isOpen()) 
    {
        // Compute de frame rate
        float current_time = clock.restart().asSeconds();
        float fps = 1.0f / current_time;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            // Detección de clic izquierdo
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mouse_pos(event.mouseButton.x, event.mouseButton.y);

                if (radar_panel.getGlobalBounds().contains(mouse_pos))       // En el panel del radar
                {
                    for (size_t i = 0; i < radar_casillas.size(); ++i) 
                    {
                        if (radar_casillas[i].getGlobalBounds().contains(mouse_pos)) 
                        {
                            int x = (i % radar_map.get_columns());
                            int y = (i / radar_map.get_columns());

                            std::string tipo = radar_map.is_water(x, y) ? "Agua" :
                                               radar_map.is_boat(x, y) ? "Barco" :
                                               radar_map.is_failed(x, y) ? "Disparo fallido" :
                                               "Barco destruido";

                            std::cout << "Radar - Casilla clickeada [" << x << ", " << y << "] - " << tipo << std::endl;
                            //radar_map.setSelected();
                        }
                    }   
                }
                else if (player_panel.getGlobalBounds().contains(mouse_pos)) // En el panel del mapa 
                {
                    for (size_t i = 0; i < player_casillas.size(); ++i) 
                    {
                        if (player_casillas[i].getGlobalBounds().contains(mouse_pos)) 
                        {
                            int x = (i % radar_map.get_columns());
                            int y = (i / radar_map.get_columns());

                            std::string tipo = radar_map.is_water(x, y) ? "Agua" :
                                            radar_map.is_boat(x, y) ? "Barco" :
                                            radar_map.is_failed(x, y) ? "Disparo fallido" :
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
                    else if (end_game_button.getGlobalBounds().contains(mouse_pos)) // Clic en "Terminar Partida"
                    {
                        std::cout << "Partida terminada. Volviendo al menú principal..." << std::endl;
                        window.close();
                        //menu_window();
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
        window.draw(radar_panel);
        window.draw(player_panel);
        window.draw(exit_button);
        window.draw(end_game_button);
        window.draw(shot_button);

        // Dibujar los items del panel de info
        for (const auto& barco : barcos) window.draw(barco);
        for (const auto& comodin : comodines) window.draw(comodin);
        for (const auto& cantidad : comodinCantidad) window.draw(cantidad);
        for (const auto& proyectil : proyectiles) window.draw(proyectil);
        for (const auto& cantidad : proyectilCantidad) window.draw(cantidad);

        // Dibujar los vectores de sprites
        for (const auto& casilla : radar_casillas) window.draw(casilla);
        for (const auto& casilla : player_casillas) window.draw(casilla);

        window.display();
    }
}

int menu_window()
{
    // Crea una ventana Con los atributos pasados, el nombre, y el como pantalla completa
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Battle Ship", sf::Style::Fullscreen);


    // Cargar la imagen de fondo
    sf::Texture backgroundTexture;                              // Tipo de dato para las imagenes o texturas
    if (!backgroundTexture.loadFromFile("Resources/textures/menu.jpg")) // Si no se logra leer 
    {
        throw std::runtime_error("Coudln't load background image");
    }
    sf::Sprite back_ground_sprite(backgroundTexture);             // Si se logra leer creamos un sprite de la textura

    // Tipo de dato para la fuente de texto
    sf::Font font;                                              
    if (!font.loadFromFile("Resources/fonts/tittle_font.ttf"))              // Si no se logra leer
    {
        throw std::runtime_error("Coudln't load font file");
    }


    // Vector con los textos y con los nombres para crear nuestros botones
    std::vector<sf::Text> botones;               
    std::vector<std::string> nombres = {"Jugar", "Arsenal", "Tienda", "Registros", "Salir"};


    // Creacion de los botones
    for (size_t i = 0; i < nombres.size(); ++i)  
    {
        sf::Text boton;                          // Asignacion de propiedades de cada boton  
        boton.setFont(font);                        
        boton.setString(nombres[i]);
        boton.setCharacterSize(40);
        boton.setFillColor(sf::Color::White);
        boton.setPosition(50, 600 + i * 60);      // Posición arriba a la izquierda
        botones.push_back(boton);                // Se va llenando nuestro vector de Textos
    }   
    

    // Dibujar los elementos del bucle principal
    window.setFramerateLimit(60);
    while (window.isOpen()) 
    {
        // Evento que representa las cosas que ocurren en el menu
        sf::Event event;

        // Evento para cerrar la ventana
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                exit(EXIT_SUCCESS);
            }

            // Obtenemos un vector 2 d con la direccion de mause
            sf::Vector2f mousePos(sf::Mouse::getPosition(window));

            // Cambiar el color de los botones si la direccion corresponde a alguna de las coordenadas de nuestros botones
            for (auto& boton : botones) 
            {
                if (boton.getGlobalBounds().contains(mousePos)) 
                {
                    boton.setFillColor(sf::Color::Yellow); // Color cuando el mouse está encima
                } 
                else
                {
                    boton.setFillColor(sf::Color::White); // Color normal
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) 
            {    
                for (size_t i = 0; i < botones.size(); ++i) // Recorremos los botones para ver cual coordenada coincide
                {
                    if (botones[i].getGlobalBounds().contains(mousePos)) 
                    {
                        if (nombres[i] == "Salir") 
                        {
                            window.close();
                            exit(EXIT_SUCCESS);
                        }
                        else if (nombres[i] == "Arsenal") 
                        {
                            std::cout << "se presiono Arsenal\n";
                        }
                        else if (nombres[i] == "Tienda") 
                        {
                            std::cout << "se presiono Tienda\n";
                        }
                        else if (nombres[i] == "Registros") 
                        {
                            std::cout << "se presiono Registros\n";
                        }
                        else if (nombres[i] == "Jugar")
                        {
                            std::cout << "Se presiono Jugar\n";
                            play_window(window);
                        }
                    }
                }
            }
        }


        // Cargar la ventana
        window.clear();                 // Limpia la ventana
        window.draw(back_ground_sprite);// Dibuja la imagen de fondo
        for (const auto& boton : botones) 
        {
            window.draw(boton);         // Dibuja cada boton
        }
        window.display();               // Mostramos la ventana
    }
}

int main()
{
    menu_window();
} 