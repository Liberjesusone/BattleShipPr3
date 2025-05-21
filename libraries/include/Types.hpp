#pragma once

#ifndef TYPES_HPP
#define TYPES_HPP

#include <iostream>

#include <SFML/Graphics.hpp>		// Bibliotecas de SFML
#include <SFML/Audio.hpp>

#include <fstream>
#include <memory>
#include <string>
#include <list>
#include <random>
#include <queue>
#include <stack>
#include <vector>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <functional>

namespace Tree 
{
	template <typename T>
	class Node;

	template <typename Tree>
	int cardinality(Tree* root);

	int capacity(int height);

	template <typename Tree>
	int heigth(Tree* root);

	template <typename Tree>
	int min_heigth(Tree* root); 

	template <typename Tree> 
	std::list<int> min_route(Tree* root);

	template <typename Tree, typename T>
	void insert_min(Tree* root, T elem);

	template <typename Tree>  
	void delete_tree(Tree* root);

	template <typename Tree, typename Action>
	void for_all(Tree* root, Action&& action);

	template <typename Tree, typename Action>
	void level_traverse(Tree* root, Action&& action);

	int get_random_uniform(int n);
}
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
	using Player_ptr = std::shared_ptr<Player>;
	using PlayerPair = std::pair<Player_ptr, Player_ptr>;
}
namespace BotLogic 
{
	class Bot;

	using Movement = std::pair<Party::Coordinates, Objects::Item_ptr>;
}

using namespace Tree;
using namespace Party;
using namespace Objects;
using namespace Play;
using namespace BotLogic;



/**
 * @brief Agrupa las estructuras relacionadas al mapa, como las celdas y sus diferentes tipos, y el mapa en si 
 */
namespace Party
{
	/**
	 * @brief Clases para representar los tipos de celdas que pueden haber en el mapa
	 */
	class Map_cell
	{
		private:
			// Representa la coordena X como first y Y como second, en el mapa donde esta 
			Coordinates location{0, 0};

		public:
			//Constructor por defecto	
			Map_cell() noexcept;

			// Constructor parametrico
			Map_cell(size_t x, size_t y) noexcept;

			virtual ~Map_cell() = default; // Make the class polymorphic

			// Getters
			Coordinates get_location() const noexcept;
			
			/**
			 * @brief Obtiene el caracter que representa el tipo de clase
			 * M = Map_cell
			 * W = Water_cell
			 * B = Boat_cell
			 * F = Failed_cell
			 * D = Destroyed_cell
			 * 
			 * @return Return 'M'
			 */
			virtual char get_type() const noexcept;

			// Setters
			void set_location(size_t x, size_t y) noexcept;
			
			// Static Functions
			static char water_type();
			static char boat_type();
			static char failed_type();
			static char destroyed_type();
			static char protected_type(); 
			static char main_type();
	};
	class Water_cell : public Map_cell
	{
		public:
			// Constructor por defecto
			Water_cell() noexcept;

			// Constructor parametrico
			Water_cell(size_t x, size_t y) noexcept;

			virtual ~Water_cell() = default; // Ensure the class is polymorphic

			// Getters
			virtual char get_type() const noexcept;
	};
	class Boat_cell : public Map_cell	
	{
		private:			
			/**
			 * @brief Representa la posicion del bote, para poder ser dibujado en el mapa
			 * U = Up cola de un barco en la parte superior
			 * D = Down cola de un barco en la parte inferior  
			 * R = Right cola de un barco en la parte derecha  
			 * L = Left cola de un barco en la parte izquierda  
			 * B = Body cuerpo de una barco horizontal
			 * V = VerticalBody cuerpo de un barco vertical
			 */  
			char position;

		public:
			// Constructor por defecto
			Boat_cell() noexcept;

			// Constructor parametrico
			Boat_cell(size_t x, size_t y) noexcept;

			virtual ~Boat_cell() = default; // Ensure the class is polymorphic

			// Getters
			virtual char get_type() const noexcept;

			char get_position() const noexcept;

			bool is_tail() const noexcept;
	};
	class Failed_cell : public Map_cell
	{
		public:
			// Constructor por defecto
			Failed_cell() noexcept;

			// Constructor parametrico
			Failed_cell(size_t x, size_t y) noexcept;
			
			virtual ~Failed_cell() = default; // Ensure the class is polymorphic

			// Getters
			virtual char get_type() const noexcept;
	};
	class Destroyed_cell : public Map_cell
	{
		public:
			// Constructor por defecto
			Destroyed_cell() noexcept;
 
			// Constructor parametrico
			Destroyed_cell(size_t x, size_t y) noexcept;

			virtual ~Destroyed_cell() = default; // Ensure the class is polymorphic

			// Getters
			virtual char get_type() const noexcept;
	};
	class Protected_cell : public Map_cell
	{
		public:
			// Constructor por defecto
			Protected_cell() noexcept;
 
			// Constructor parametrico
			Protected_cell(size_t x, size_t y) noexcept;

			virtual ~Protected_cell() = default; // Ensure the class is polymorphic

			// Getters
			virtual char get_type() const noexcept;
	};


	/**
	 * @brief Representa el mapa de una partida, con una matrix de Map_cells y diferentes metodos para operar con ellas
	 */
	class Map
	{
		using Map_cell_ptr = std::shared_ptr<Map_cell>;
		using Matrix = std::vector<std::vector<Map_cell_ptr>>;

		private:

			// Valores que representan el tamaño del mapa
			const size_t rows{0};
			const size_t columns{0};
			
			/**
			 * @brief Matriz de casillas que representa el mapa, pueden ser de los tipos, Destroyed, Failed, Water o Boat
			 * 
			 * @note Donde la casilla (x;y) = (0;0) es arriba a la izq, y = (10;10) es abajo a la derecha
			 * @note Las columnas las representa el eje x, y las filas el eje y, como si la pantalla fuese el 4to cuadrante
			 */
			Matrix matrix;

		public:
			// Constructor por defecto
			Map() noexcept;

			/** 
			 * @brief Constructor por parámetros
			 * 
			 * @param rows Número de filas del mapa
			 * @param cols Número de columnas del mapa
			 */
			Map(size_t cols, size_t rows) noexcept;


			// CHECK TYPE

			/**
			 * @brief Verifica si la celda es de tipo Map_cell, es decir que no se puede transformar en ningun otro tipo derivado
			 * 
			 * @return devuelve true si es solo tipo Map_cell, y false, si se puede transformar en otro tipo derivado
			 */
			bool is_main(size_t col, size_t row);

			/**
			 * @brief Revisa si la casilla mandada es o no agua
			 * 
			 * @param cell casilla a revisar
			 * 
			 * @return True si esta marcada como agua, False si es cualquier otra cosa
			 */
			bool is_water(Map_cell& cell);

			/**
			 * @brief Revisa si la casilla es o no agua
			 * 
			 * @param row Fila a revisar
			 * @param col Columna a revisar
			 * 
			 * @return True si esta marcada como agua, False si es cualquier otra cosa
			 */
			bool is_water(size_t col, size_t row);

			/**
			 * @brief Revisa si la casilla es o no bote
			 * 
			 * @param row Fila a revisar
			 * @param col Columna a revisar
			 * 
			 * @return True si esta marcada como bote, False si es cualquier otra cosa
			 */
			bool is_boat(size_t col, size_t row);

			/**
			 * @brief Revisa si la casilla es o no fallida
			 * 
			 * @param row Fila a revisar
			 * @param col Columna a revisar
			 * 
			 * @return True si esta marcada como failed, False si es cualquier otra cosa
			 */
			bool is_failed(size_t col, size_t row);

			/**
			 * @brief Revisa si la casilla es o no destrida
			 * 
			 * @param row Fila a revisar
			 * @param col Columna a revisar
			 * 
			 * @return True si esta marcada como destruida, False si es cualquier otra cosa
			 */
			bool is_destroyed(size_t col, size_t row);

			/**
			 * @brief Revisa si la casilla está o no protegida
			 * 
			 * @param row Fila a revisar
			 * @param col Columna a revisar
			 * 
			 * @return True si esta marcada como protegida, False si es cualquier otra cosa
			 */
			bool is_protected(size_t col, size_t row);


			// GETTERS

			size_t get_columns() const noexcept;

			size_t get_rows() const noexcept;

			/**
			 * @brief Obtiene la casilla por referencia por medio de sus coordenadas, para poder ser afectada o vista
			 * 
			 * @param row Fila a devolver
			 * @param col Columna a devolver
			 * 
			 * @note Arroja una exception si las coordenadas se salen del rango del mapa  
			 * 
			 * @return casilla seleccionada
			 */
			Map_cell& get_cell(size_t col, size_t row) const;

			/**
			 * @brief Obtiene el smart ptr de una celda por medio de sus coordenadas
			 * 
			 * @param row Fila a devolver
			 * @param col Columna a devolver
			 * 
			 * @note Arroja una exception si las coordenadas se salen del rango del mapa  
			 * 
			 * @return Smart ptr de la casilla seleccionada 
			 */
			Map_cell_ptr get_ptr_cell(size_t col, size_t row) const;

			/**
			 * @brief Obtiene la celda a la derecha de la celda mandada
			 * 
			 * @param cell La celda original
			 * 
			 * @return La celda que esta a la derecha de la original, o nullptr si la celda original esta al limite del rango
			 */
			Map_cell_ptr get_right_cell(Map_cell_ptr cell) const;

			/**
			 * @brief Obtiene la celda a la derecha de la celda mandada
			 * 
			 * @param cell La celda original
			 * 
			 * @return La celda que esta a la izquierda de la original, o nullptr si la celda original esta al limite del rango
			 */
			Map_cell_ptr get_left_cell(Map_cell_ptr cell) const;

			/**
			 * @brief Obtiene la celda arriba de la celda mandada
			 * 
			 * @param cell La celda original
			 * 
			 * @return La celda que esta arriba de la original, o nullptr si la celda original esta al limite del rango
			 */
			Map_cell_ptr get_up_cell(Map_cell_ptr cell) const;

			/**
			 * @brief Obtiene la celda a la abajo de la celda mandada
			 * 
			 * @param cell La celda original
			 * 
			 * @return La celda que esta abajo de la original, o nullptr si la celda original esta al limite del rango
			 */
			Map_cell_ptr get_down_cell(Map_cell_ptr cell) const;


			// Setters 

			/**
			 * @brief Funcion para marcar una casilla como tipo Map_cell
			 *
			 * @param cell shared_ptr de la celda a marcar
			 */
			void set_main(Map_cell_ptr cell) noexcept;

			/**
			 * @brief Funcion para marcar una casilla como agua
			 * 
			 * @param cell shared_ptr de la celda a marcar
			 */
			void set_water(Map_cell_ptr cell) noexcept;

			/**
			 * @brief Funcion para marcar una casilla como destruida
			 * 
			 * @param cell shared_ptr de la celda a marcar
			 */
			void set_destroy(Map_cell_ptr cell) noexcept;

			/**
			 * @brief Funcion para marcar una casilla como fallada, cuando un disparo no acierta
			 * 
			 * @param cell shared_ptr de la celda a marcar
			 */
			void set_fail(Map_cell_ptr cell) noexcept;

			/**
			 * @brief Funcion para marcar una casilla ocupada por un bote, cuando un disparo no acierta
			 * 
			 * @param cell shared_ptr de la celda a marcar
			 */
			void set_boat(Map_cell_ptr cell) noexcept;

			/**
			 * @brief Funcion para marcar una casilla como protegida
			 * 
			 * @param cell shared_ptr de la celda a marcar
			 */
			void set_protected(Map_cell_ptr cell) noexcept;

			/**
			 * @brief Intenta inserta un bote en el mapa, que es basicamente convertir en bote todas las casillas que ocupa el bote
			 * 
			 * @param bote a insertar
			 * 
			 * @return devulve true si se ha logrado insertar, y false si no se pudo insertar 
			 */
			bool insert_boat(Boat_ptr bote) noexcept;

			/**
			 * @brief Convierte todas las casillas del bote mandado en agua
			 * 
			 * @param bote a convertir en agua
			 */
			void delete_boat(Boat_ptr bote) noexcept;

			/**
			 * @brief Crea una safe zone marcada con casillas tipo Failed_cell alrrededor de un barco 
			 * 
			 * @param bote al cual hay que rodear con una safe zone
			 * 
			 * @note bote tiene que ser un barco previamnete perteneciente al mapa, si no el comportamiento puede ser indefinido
			 */
			void create_safe_zone(Boat_ptr bote) noexcept;

			/**
			 * @brief Crea una safe zone con Map_cell como clase base, al rededor de la celda indicada
			 */
			void create_safe_zone(Coordinates cell) noexcept;

			/**
			 * @brief Revisa si hay colision con el bote recibido y algun otro del mapa actual y si cabe dentro del mapa
			 * 
			 * @param bote al cual hay que ver si colisionaria
			 * 
			 * @return true si colosiona o se sale del mapa, false si se puede insertar sin colisionar
			 */
			bool collides(Boat_ptr bote);
	};
}

/**
 * @brief Agrupa los Objetos o consumibles de la partida, como los Items, comodines, misiles etc
 */
namespace Objects
{
	/** Functors 
	 * @brief Describen el funcionamiento de los proyectiles, comodines o items en general, se pasan como tipo de dato 
	 * en las clases genericas que hereden de Item, y estas los usaran en la casilla de origen en la que sean aplicados
	 * 
	 * @param player Es el jugador que aplica el Item 
	 * @param cell Es el smart ptr de la celda donde se empezara a efectuar el efecto 
	 * 
	 * @note Deben tener sobrescrito el operador () donde describira el comportamiento o efecto sobre la casilla
	 * @note Para agregar nuevos tipos de proyectiles solo crear nuevos functors, y al momento de instancia la clase
	 * pasarlo como tipo de dato Rocket<MyFunctor> myRocket;
	 */ 

	// Rockets Functors

	/**
	 * @brief Funtor que solo marca una casilla como destruida por uso 
	 * Si la casilla es agua, la marca como fallida
	 * Si la casilla es bote, la marca como destruida 
	 */
	class SingleShot
	{
		public: 
			// Constructor por defecto 
			SingleShot() noexcept;

			void operator() (PlayerPair& players, Party::Map_cell_ptr cell);
	};

	/**
	 * @brief Functor que destruye todas las casillas del barco si llega a impactar con uno
	 * Pero solo se esparse por un barco, sin saltar de un barco a otro  
	 */
	class ChargedShot
	{
		public:
			// Constructor por defecto
			ChargedShot() noexcept;
			
			void operator() (PlayerPair& players, Map_cell_ptr cell);
	};

	// Comodin Functors

	/**
	 * @brief Cura la celda que se le pase, si la celda es Destroyed, ahora pasara a ser Boat
	 * 
	 * @note No se espera recibir otra celda que no sea Destroyed, solo se le permitira al usuario seleccionar celdas destroyed 
	 */
	class HealCell
	{
		public: 
			// Constructor por defecto
			HealCell() noexcept;

			void operator() (PlayerPair& players, Map_cell_ptr cell);
	};

	/**
	 * @brief Protege la celda bote que se le pase 
	 * 
	 * @note No se espera recibir otra celda que no sea Boat, solo se le permitira al usuario seleccionar celdas Boat
	 */
	class ProtectCell
	{
		public: 
			// Constructor por defecto
			ProtectCell() noexcept;

			void operator() (PlayerPair& players, Map_cell_ptr cell);
	};

	/**
	 * @brief Representa un item que tiene una funcion que los jugadores pueden aplicar sobre una casilla  
	 * 
	 * @note Para usar la funcion de cada item se debe pasar un par de players donde el first sera el que ejecuta la accion,
	 * second sera su enemigo, y la casilla donde lo ejecute
	 */
	class Item
	{
		protected:
			std::string name{""};

			size_t stock{0};

		public:
			// Constructor por defecto
			Item() = default;

			// Constructor parametrico
			Item(size_t stock, std::string name) noexcept;

			// Setters
			
			// Getters
			std::string get_name() const noexcept;

			size_t get_stock() const noexcept;

			/**
			 * @brief Obtiene el caracter que representa el tipo de clase
			 * I = Item
			 * C = Comodin
			 * R = Rocket
			 * 
			 * @return Return 'I'
			 */
			virtual char get_type() const noexcept;

			// Functions

			/**
			 * @brief Aplica la funcion del item en la casilla mandada
			 *  
			 * @param row Fila a afectar
			 * @param col Columna a afectar
			 */
			virtual void use_on(PlayerPair& players, size_t col, size_t row);

			static char item_type();
			static char comodin_type();
			static char rocket_type();
	};

	/**
	 * @brief Representa los misiles balisticos o proyectiles que solo se lanzaran en el mapa enemigo o radar del Player que lo use
	 */
	template <typename DamageType = SingleShot>
	class Rocket : public Item
	{
		public:
			// Constructor por defecto
			Rocket() noexcept;

			// Constructor parametrico
			Rocket(size_t stock, std::string name) noexcept;
	
			// Getters

			virtual char get_type() const noexcept;

			// Functions
			/**
			 * @brief Dispara en la casilla mandada del radar del Player mandado, usando la logica de disparo de DamageType
			 *  
			 * @param player El jugador que disparara el rocket
			 * @param row Fila a afectar
			 * @param col Columna a afectar
			 */
			virtual void use_on(PlayerPair& players, size_t col, size_t row)
			{	
				Map_ptr radar = players.first->get_radar();	    // El mapa a afectar sera el radar del Player
				auto cell = radar->get_ptr_cell(col, row);
				DamageType shot;								// Cargamos el tipo de disparo con el que se creo el rocket
				if (stock)
				{
					shot(players, cell);
					--stock;										// Reducimos los stocks
				}
				std::cout << (stock ? "hay existencias " : "ran out of stock ") << stock << "\n";
			}
	};

	/**
	 * @brief representa los comodines/poderes que se aplican en el mapa propio del Player que los use
	 */
	template <typename EffectType = HealCell>
	class Comodin : public Item
	{
		public:
			//Constructor por defecto 
			Comodin() noexcept;

			// Constructor parametrico
			Comodin(size_t stock, std::string name) noexcept;

			// Getters

			virtual char get_type() const noexcept;

			// Functions 

			/**
			 * @brief Aplica el efecto de comodin en la casilla mandada del mapa propio del Player mandado, con la logica de
			 *  
			 * @param player El jugador que efecturara el comodin
			 * @param row Fila a afectar
			 * @param col Columna a afectar
			 */
			virtual void use_on(PlayerPair& players, size_t col, size_t row)
			{	
				Map_ptr mapa = players.first->get_map();		    // El mapa a afectar sera el mapa propio del Player
				auto cell = mapa->get_ptr_cell(col, row);
				EffectType effect;								// Cargamos el tipo de efecto con el que se creo el comodin
				if (stock)
				{
					effect(players, cell);
					--stock;     									// Reducimos los stocks
				}
				std::cout << (stock ? "hay existencias " : "ran out of stock ") << stock << "\n";
			}
	};
}

/**
 * @brief Agrupa las estruturas que forman parte de las propiedades del jugador como su Armamento, Flota, sus botes, y Build 
 */
namespace Play
{
	/**
	 * @brief Functor para generar codigos hash unicos a partir de std::pair<size_t, size_t>
	 * el cual consiste en generar un codigo hash para ambas partes del pair, luego usar el operador << 1 , con el segundo 
	 * y combinarlo con el primer hash mediante la operacion XOR ^  
	 */
	struct PairHash 
	{
		std::size_t operator()(const std::pair<size_t, size_t>& p) const noexcept;
	};

	/**
	 * @brief Representa un bote junto con todas las casillas que ocupa y su direccion
	 */
	class Boat
	{
		using Coord_list = std::unordered_set<Coordinates, PairHash>;
		private:
			Coord_list boat_coordinates;     // Lista de las coordenadas de las celdas que pertenecen al bote
			std::string name{""};	
			float distruction_per{0};		 // Porcentaje de destruccion del bote

		public: 
			// Constructor por defecto
			Boat() noexcept;

			// Constructor parametrico
			Boat(size_t size, Coordinates first_cell, bool horizontal) noexcept;

			// Functions

			/**
			 * @brief Revisa si el bote contiene la celda mandada 
			 * 
			 * @param cell El smart pointer de la celda que estamos comprobando si pertence al bote
			 * 
			 * @return true si contiene la celda, false si no la contiene
			 */
			bool contains(Map_cell_ptr cell) const noexcept;

			// Getters 

			float get_distruction_per(Map_ptr mapa);

			Coord_list& get_boat_coordinates() noexcept;

			size_t get_size() const noexcept;

			std::string get_name() const noexcept;
	};

	/**
	 * @brief Representa una flota de barcos, con los tipos de barcos de cada 
	 */
	class Fleet 
	{
		private:	
			std::vector<Boat_ptr> boats;	// Lista de los botes que pertencen a la flota

		public: 
			//Constructor por defecto
			Fleet() noexcept;

			// Getters

			std::vector<Boat_ptr>& get_boats() noexcept;

			/**
			 * @brief Obtiene el bote de esta flota el cual contiene la celda mandada
			 * 
			 * @param cell La celda que queremos comprabar si pertence a algun bote de esta flota
			 * 
			 * @return El bote al cual pertenece la celda, o nullptr si la celda no pertenece a ningun bote de esta flota
			 */
			Boat_ptr get_boat_of_cell(Map_cell_ptr cell) const noexcept;

			// Funtions

			/**
			 * @brief Agrega un bote a la lista de botes
			 * 
			 * @param bote a agregar
			 */
			void add_boat(Boat_ptr bote);

			/**
			 * @brief Elimina el barco del vector de barcos segun su direccion de memoria 
			 * 
			 * @param bote a eliminar
			 */
			void delete_boat(Boat_ptr bote);
	};

    /**
	 * @brief Representa el inventario de municiones, proyectiles e Items
	 */
	class Arsenal
	{
		private:
			std::vector<Item_ptr> items;     // Lista de consumibles 

		public:
			// Constructor por defecto
			Arsenal() noexcept;

			// Getters

			std::vector<Item_ptr>& get_items() noexcept;

			
	};

	/**
	 * @brief Representa un arsenal, una flota de barcos y un nombre de registro, los elementos que el Player dispone para jugar   
	 */
	class Build
	{
		private:
			std::string name;	// Nombre de la flota de barcos
			Arsenal arsenal;	
			Fleet flota;

		public: 
			// Constructor por defecto 
			Build() noexcept;

			// Getters
			
			Fleet& get_fleet() noexcept;

			Arsenal& get_arsenal() noexcept;
			
			std::string get_name() const noexcept;
	};

	/**
	 * @brief Representa un jugador que hace moviemientos en la partida por turnos
	 */	
	class Player
	{	
		private:
			std::string name; 	// Nombre del capitan de la flota
			Map_ptr mapa;			// Representa el mapa propio del jugador
			Map_ptr radar;			// Representa el radar donde podra hacer los disparos hacia el mapa enemigo
			Build build;		// Build con los elementos seleccionados para jugar
			

		public:
			// Constructor por defecto
			Player() noexcept;

			// Constructor parametrico
			Player(std::string name, Map_ptr mapa, Map_ptr radar) noexcept;

			// Getters

			Map_ptr get_radar() noexcept;
			
			Map_ptr get_map() noexcept;

			Build& get_build() noexcept;
	};
}

/**
 * @brief Agrupa las estructuras y funciones refentes al manejo de decisiones por parte del Bot
 */
namespace BotLogic
{
	/**
	 * @brief Tiene los mismos atributos que Player pero con las funcionalidades que manejan la logica, mediante el imput de cada turno
	 * 
	 */
	class Bot : public Player
	{
		using Node_ptr = std::shared_ptr<Node<int>>;
		private:
			// Arbol que representa las posiciones de los posibles barcos segun el disparo acertado
			Node_ptr target_boat;

			std::vector<Boat_ptr> destroyed_boats;				// Lista propia de los barcos que lleva destruidos
			Map_ptr board = std::make_shared<Map>(5, 10);	    // Matriz donde se dispara espaciadamente y se marca una safezone

			std::vector<Coordinates> successful_shots;			// vector con las celdas que ha acertado el bot
			std::stack<Coordinates> cells_to_reshot;		        // pila con las celdas que el bot deberia volver a disparar, ejem: shilds
			

		public:
			// Constructor por defecto
			Bot() noexcept;

			// Constructor parametrico
			Bot(std::string name, Map_ptr mapa, Map_ptr radar) noexcept;

			/**
			 * @brief Segun los datos del enemigo, obtiene el siguiente moviemiento
			 * 
			 * @param mapa recibe el mapa de la partida como contexto
			 * 
			 * @return Coordenadas del siguiente movimiento en los primeros dos elemtos, y el item a usar en el tercer elemento
			 */
			Movement get_next_move(Player enemy);

			/**
			 * @brief Devuelve un movimiento aleatorio 
			 */
			Movement get_random_move();

			/**
			 * @brief Le asigna al mapa mandado los botes en las posiciones segun la logica interna del bot
			 * 
			 * @param mapa al que se le agregaran los botes
			 */
			void create_map(Map_ptr mapa);

			/**
			 * @brief Hace la siguiente jugada del bot segun los datos de mapa, radar, arsenal y etc que tenga
			 * 
			 * @return devuelve true si uso un comodin, y false si uso un misil
			 */
			bool play(Player_ptr player);

			/**
			 * @brief Crea el arbol de posibilidades en funcion de un target boat alrededor de la celda 
			 * 
			 * @param coord de la celda donde se disparo y acerto, para empezara a crear desde alli el target boat
			 */
			void build_target_boat(Coordinates coord);

			/**
			 * @brief Creates a complete movement evaluating with the bot logic how should he move
			 */
			bool make_movement(Player_ptr player);
	};
}


#endif // TYPES_HPP