#include "Types.hpp"

using namespace Tree;
using namespace Party;
using namespace Play;
using namespace Objects;
using namespace BotLogic;

std::string num_to_str(int numero)
{
	if (numero < 10) 
		return "0" + std::to_string(numero); // Agrega un cero a la izquierda si el número tiene una cifra
	else
		return std::to_string(numero); // Devuelve el número como string si ya tiene dos cifras
}

namespace Tree
{
	int get_random_uniform(int n)
	{
		// Crear un generador de n�meros aleatorios
		static std::random_device rd;                 // Semilla aleatoria del hardware
		static std::mt19937 gen(rd());                // Motor de n�meros aleatorios (Mersenne Twister)

		// Crear una distribuci�n uniforme en el rango [0, max de int]
		static std::uniform_int_distribution<> dis(0, std::numeric_limits<int>::max());
		return dis(gen) % (n + 1);
	}
	
	int capacity(int height)
	{
		int cap = 0;
		for (int i = height - 1; i >= 0; --i)
		{
			cap += std::pow(2, i);
		}
		return cap;
	}	
}

namespace Party
{
	// Clase Map_cell

	Map_cell::Map_cell() noexcept
	{
		// Empty
	}

	Map_cell::Map_cell(size_t x, size_t y) noexcept : location(x, y) 
	{
		// Empty
	}

	Coordinates Map_cell::get_location() const noexcept
	{	
		return location;
	}

	char Map_cell::get_type() const noexcept
	{
		return 'M';
	}

	void Map_cell::set_location(size_t x, size_t y) noexcept
	{
		this->location = std::make_pair(x, y);
	}

	char Map_cell::water_type() 
	{
		return 'W';
	}
	char Map_cell::boat_type()  
	{
		return 'B';
	}
	char Map_cell::failed_type() 
	{
		return 'F';
	}
	char Map_cell::destroyed_type() 
	{
		return 'D';
	}
	char Map_cell::protected_type() 
	{
		return 'P';
	}
	char Map_cell::main_type()  
	{
		return 'M';
	}

	
	// Clase Water_cell

	Water_cell::Water_cell() noexcept : Map_cell() 
	{
		// Empty
	}

	Water_cell::Water_cell(size_t x, size_t y) noexcept : Map_cell(x, y) 
	{
		// Empty
	}

	char Water_cell::get_type() const noexcept
	{
		return 'W';
	}
	
	
	// Clase Boat_cell

	Boat_cell::Boat_cell() noexcept : Map_cell() 
	{
		// Empty
	}

	Boat_cell::Boat_cell(size_t x, size_t y) noexcept : Map_cell(x, y) 
	{
		// Empty
	}

	char Boat_cell::get_type() const noexcept
	{
		return 'B';
	}

	char Boat_cell::get_position() const noexcept 
	{
		return position;
	}

	bool Boat_cell::is_tail() const noexcept
	{
		if (position != 'V' && position != 'B')
		{
			return true;
		}
		return false;
	}

	
	// Clase Failed_Cell

	Failed_cell::Failed_cell() noexcept : Map_cell() 
	{
		// Empty	
	}

	Failed_cell::Failed_cell(size_t x, size_t y) noexcept : Map_cell(x, y) 
	{
		// Empty
	}	

	char Failed_cell::get_type() const noexcept
	{
		return 'F';
	}

	
	
	// Clase Destroyed_cell

	Destroyed_cell::Destroyed_cell() noexcept : Map_cell() 
	{
		// Empty
	}

	Destroyed_cell::Destroyed_cell(size_t x, size_t y) noexcept : Map_cell(x, y) 
	{
		// Empty
	}

	char Destroyed_cell::get_type() const noexcept
	{
		return 'D';
	}

	
	// Clase Protected_cell

	Protected_cell::Protected_cell() noexcept : Map_cell() 
	{
		// Empty
	}

	Protected_cell::Protected_cell(size_t x, size_t y) noexcept : Map_cell(x, y) 
	{
		// Empty
	}

	char Protected_cell::get_type() const noexcept
	{
		return 'P';
	}
	
		
	// Clase Map

	Map::Map() noexcept
	{
		// Empty
	}

	Map::Map(size_t cols, size_t rows) noexcept : rows(rows), columns(cols), matrix(cols, std::vector<Map_cell_ptr>(rows)) 
	{
		for (size_t x = 0; x < cols; ++x) 
		{
			for (size_t y = 0; y < rows; ++y) 
			{
				matrix[x][y] = std::make_shared<Water_cell>(x, y); // Inicializar con Water_cell todas las casillas
			}
		}
	}

	bool Map::is_water(Map_cell& cell)
	{
		return dynamic_cast<Water_cell*>(&cell) != nullptr;
	}

	bool Map::is_water(size_t col, size_t row)
	{
		Map_cell_ptr cell = get_ptr_cell(col, row);
		return std::dynamic_pointer_cast<Water_cell>(cell) != nullptr;
	}

	bool Map::is_boat(size_t col, size_t row)
	{
		Map_cell_ptr cell = get_ptr_cell(col, row);
		return std::dynamic_pointer_cast<Boat_cell>(cell) != nullptr;
	}

	bool Map::is_failed(size_t col, size_t row)
	{
		Map_cell_ptr cell = get_ptr_cell(col, row);
		return std::dynamic_pointer_cast<Failed_cell>(cell) != nullptr;
	}

	bool Map::is_destroyed(size_t col, size_t row)
	{
		Map_cell_ptr cell = get_ptr_cell(col, row);
		return std::dynamic_pointer_cast<Destroyed_cell>(cell) != nullptr;
	}

	bool Map::is_protected(size_t col, size_t row)
	{
		Map_cell_ptr cell = get_ptr_cell(col, row);
		return std::dynamic_pointer_cast<Protected_cell>(cell) != nullptr;
	}

	size_t Map::get_columns() const noexcept
	{
		return this->columns;
	}

	size_t Map::get_rows() const noexcept
	{
		return this->rows;
	}

	Map_cell& Map::get_cell(size_t col, size_t row) const
	{
		if (row >= this->rows || col >= this->columns)	// Si no esta dentro del rango
		{
			throw std::runtime_error{"Index out of range in the map"};
		}

		return *(matrix[col][row]);	
	}

	Map_cell_ptr Map::get_ptr_cell(size_t col, size_t row) const
	{
		if (row >= this->rows || col >= this->columns)	// Si no esta dentro del rango
		{
			throw std::runtime_error{"Index out of range in the map"};
		}

		return matrix[col][row];	
	}

	Map_cell_ptr Map::get_right_cell(Map_cell_ptr cell) const
	{
		auto location = cell->get_location();

		if (location.first >= columns -1) // Si ya se encuentra en el limite
		{
			return nullptr;
		}
		
		return get_ptr_cell(++location.first, location.second);
	}

	Map_cell_ptr Map::get_left_cell(Map_cell_ptr cell) const
	{
		auto location = cell->get_location();

		if (location.first == 0) // Si ya se encuentra en el limite
		{
			return nullptr;
		}
		
		return get_ptr_cell(--location.first, location.second);
	}

	Map_cell_ptr Map::get_up_cell(Map_cell_ptr cell) const
	{
		auto location = cell->get_location();

		if (location.second == 0) // Si ya se encuentra en el limite
		{
			return nullptr;
		}
		
		return get_ptr_cell(location.first, --location.second);
	}

	Map_cell_ptr Map::get_down_cell(Map_cell_ptr cell) const
	{
		auto location = cell->get_location();

		if (location.second >= rows - 1) // Si ya se encuentra en el limite
		{
			return nullptr;
		}
		
		return get_ptr_cell(location.first, ++location.second);
	}

	void Map::set_water(Map_cell_ptr cell) noexcept
	{
	    auto location = cell->get_location();
	    matrix[location.first][location.second] = std::make_shared<Water_cell>(location.first, location.second);
	}

	void Map::set_destroy(Map_cell_ptr cell) noexcept
	{
	    auto location = cell->get_location();
	    matrix[location.first][location.second] = std::make_shared<Destroyed_cell>(location.first, location.second);
	}

	void Map::set_fail(Map_cell_ptr cell) noexcept
	{
	    auto location = cell->get_location();
	    matrix[location.first][location.second] = std::make_shared<Failed_cell>(location.first, location.second);
	}

	void Map::set_boat(Map_cell_ptr cell) noexcept
	{
		auto location = cell->get_location();
	    matrix[location.first][location.second] = std::make_shared<Boat_cell>(location.first, location.second);
	}

	void Map::set_protected(Map_cell_ptr cell) noexcept
	{
		auto location = cell->get_location();
	    matrix[location.first][location.second] = std::make_shared<Protected_cell>(location.first, location.second);
	}

	bool Map::insert_boat(Boat_ptr bote) noexcept 
	{
		if (collides(bote))      // Si colosiona o no cabe en el mapa = false
		{
			return false;
		} 

		for (auto coord : bote->get_boat_coordinates())	// Si no lo insertamos
		{
			this->set_boat(this->get_ptr_cell(coord.first, coord.second));
		}
		return true;
	}

	void Map::delete_boat(Boat_ptr bote) noexcept
	{
		if (collides(bote)) 
		{
			return;
		}

		for (auto coord : bote->get_boat_coordinates())
		{
			this->set_water(this->get_ptr_cell(coord.first, coord.second));
		}
	}

	void Map::create_safe_zone(Boat_ptr bote) noexcept
	{
		// Retorna 0 si x es negativo, si no retorna x
		auto put_negative_zero = [](int x) -> size_t 
		{
			if (x < 0)
			{
				return 0; 
			}
			return x;
		};


		for (auto coord : bote->get_boat_coordinates()) // Si no asignamos las casillas safe zone como fail
		{
			// Creamos un vector con las celdas circundantes al la celda actual del bote
			std::vector<Coordinates> surrounding_cells = 		
			{
				Coordinates(put_negative_zero(coord.first - 1), coord.second),
				Coordinates(put_negative_zero(coord.first - 1), coord.second + 1),
				Coordinates(put_negative_zero(coord.first - 1), put_negative_zero(coord.second - 1)),
				Coordinates(coord.first, put_negative_zero(coord.second - 1)),
				Coordinates(coord.first, coord.second + 1),
				Coordinates(coord.first + 1, put_negative_zero(coord.second - 1)),
				Coordinates(coord.first + 1, coord.second),
				Coordinates(coord.first + 1, coord.second + 1),
			};

			// Las convertimos en fail si no hay un bote alli, si no es ya failed y si no se sale del mapa
			for (auto sur_coord : surrounding_cells)
			{
				if (sur_coord.first < this->columns && sur_coord.second < this->rows &&
					!this->is_boat(sur_coord.first, sur_coord.second) && !this->is_failed(sur_coord.first, sur_coord.second))
				{
					this->set_fail(this->get_ptr_cell(sur_coord.first, sur_coord.second));
				}
			}
		}
	}

	bool Map::collides(Boat_ptr bote)
	{
		for (auto coord : bote->get_boat_coordinates()) // confirmamos que no colisiona y que no se salga del mapa
		{
			if (!(coord.first < this->columns && coord.second < this->rows && !this->is_boat(coord.first, coord.second)))
			{
				return true;
			}
		}
		return false;
	}
	
	
}

namespace Objects
{
	// Clase SingleShot

	SingleShot::SingleShot() noexcept
	{
		// Empty
	}

	void SingleShot::operator() (Player& player, Map_cell_ptr cell) 
	{
		auto mapa = player.get_map();
		auto cell_type = cell->get_type();

		if (cell_type == Map_cell::boat_type())
		{
			mapa.set_destroy(cell);
		}
		else if (cell_type == Map_cell::water_type())
		{
			mapa.set_fail(cell);	
		}
	}


	// Clase ChargedShot

	ChargedShot::ChargedShot() noexcept
	{
		// Empty	
	}
	
	void ChargedShot::operator() (Player& player, Map_cell_ptr cell)
	{	
		Map mapa = player.get_radar();
		auto cell_type = cell->get_type();
		
		if (cell_type == Map_cell::boat_type())		  // Si la celda es un bote
		{										      // Obtenemos el bote de donde proviene la celda
			Boat_ptr enemy_boat = player.get_build().get_fleet().get_boat_of_cell(cell);	
			auto boat_cells_coord = enemy_boat->get_boat_coordinates();  // Obtenemos la lista de coordenadas de las demas celdas del bote
			for (auto coordinates : boat_cells_coord)					 // Destruimos cada una de ellas
			{
				auto cell = mapa.get_ptr_cell(coordinates.first, coordinates.second);
				mapa.set_destroy(cell);
			}
		}
		else if (cell_type == Map_cell::water_type()) // Si la celda es agua 
		{
			mapa.set_fail(cell);	
		}											  // Si no es ninguna de las anteriores no pasa nada
	}

		
	// Clase HealCell
	
	HealCell::HealCell() noexcept
	{
		// Empty
	}

	void HealCell::operator() (Player& player, Map_cell_ptr cell)
	{
		auto mapa = player.get_map();
		mapa.set_boat(cell);
	}

		
	// Clase Item

	Item::Item(size_t stock, std::string name) noexcept : stock(stock), name(name) 
	{
		// Empty
	}
	
	std::string Item::get_name() const noexcept 
	{
		return this->name;
	}

	size_t Item::get_stock() const noexcept
	{
		return this->stock;
	}

	char Item::get_type() const noexcept
	{
		return 'I';
	}

	void Item::use_on(Player& player, size_t col, size_t row)
	{
		// Pude usarce el item casual
		// OJO por desarrollar
	}

	char Item::item_type()
	{
		return 'I';
	}
	char Item::comodin_type()
	{
		return 'C';
	}
	char Item::rocket_type()
	{
		return 'R';
	}


	// Clase Rocket

	template<typename DamageType>
	Rocket<DamageType>::Rocket() noexcept : Item() 
	{
		// Empty
	}

	template<typename DamageType>
	Rocket<DamageType>::Rocket(size_t stock, std::string name) noexcept : Item(stock, name) 
	{
		// Empty
	}

	template<typename DamageType>
	char Rocket<DamageType>::get_type() const noexcept
	{
		return 'R';
	}


	// Clase Comodin
	template<typename EffectType>
	Comodin<EffectType>::Comodin() noexcept : Item() 
	{
		// Empty
	}

	template<typename EffectType>
	Comodin<EffectType>::Comodin(size_t stock, std::string name) noexcept : Item(stock, name) 
	{
		// Empty
	}

	template<typename EffectType>
	char Comodin<EffectType>::get_type() const noexcept
	{
		return 'C';
	}
}

namespace Play
{
	// Struct PairHash

	std::size_t PairHash::operator() (const std::pair<size_t, size_t>& p) const noexcept 
	{
		return std::hash<size_t>()(p.first) ^ (std::hash<size_t>()(p.second) << 1);  // Generacion de codigo hash en base a los dos valores
	}


	// Clase Boat

	Boat::Boat() noexcept
	{
		// Empty
	}

	Boat::Boat(size_t size, Coordinates first_cell, bool horizontal) noexcept
	{
		for (int i = 0; i < size && i < 5; ++i)
		{
			Coordinates coord;
			if (horizontal)
			{
				coord.first = first_cell.first + i;
				coord.second = first_cell.second;
			}
			else
			{
				coord.first = first_cell.first;
				coord.second = first_cell.second + i;
			}
			
			boat_coordinates.insert(coord);
		}
	}

	bool Boat::contains(Map_cell_ptr cell) const noexcept
	{
		return this->boat_coordinates.contains(cell->get_location());
	}

	Boat::Coord_list& Boat::get_boat_coordinates() noexcept
	{
		return this->boat_coordinates;
	}

	size_t Boat::get_size() const noexcept
	{
		return this->boat_coordinates.size();
	}

	std::string Boat::get_name() const noexcept
	{
		return this->name;
	}


	// Clase Fleet

	Fleet::Fleet() noexcept
	{
		// Empty
	}

	std::vector<Boat_ptr>& Fleet::get_boats() noexcept
	{
		return this->boats;
	}

	Boat_ptr Fleet::get_boat_of_cell(Map_cell_ptr cell) const noexcept
	{	
		for (auto bote : boats)
		{
			if (bote->contains(cell))
			{
				return bote;
			}
		}
		return nullptr;
	}

	void Fleet::add_boat(Boat_ptr bote) 
	{
		this->boats.push_back(bote);
	}

	void Fleet::delete_boat(Boat_ptr bote)
	{
		for (auto it = boats.begin(); it != boats.end(); ++it) 
		{
        	if (*it == bote)			// si las dos direcciones de memoria son iguales 
			{
				it = boats.erase(it);
				break;
        	}
    	}
	}



	// Clase Arsenal

	Arsenal::Arsenal() noexcept
	{
		// Empty		
	}

	std::vector<Item_ptr>& Arsenal::get_items() noexcept
	{
		return this->items;
	}


	// Clase Build

	Build::Build() noexcept : name("Build 1"), flota(), arsenal() 
	{
		// Emoty
	}

	Fleet& Build::get_fleet() noexcept
	{
		return this->flota;
	}

	Arsenal& Build::get_arsenal() noexcept
	{
		return this->arsenal;
	}
	
	std::string Build::get_name() const noexcept
	{
		return this->name;
	}


	// Clase Player

	Player::Player() noexcept
	{
		// Empty
	}

	Player::Player(std::string name, Map& mapa, Map& radar) noexcept : name(name), mapa(mapa), radar(radar)
	{
		// Emtpy
	}

	Map& Player::get_radar() noexcept
	{
		return this->radar;
	}
	
	Map& Player::get_map() noexcept
	{
		return this->mapa;
	}

	Build& Player::get_build() noexcept
	{
		return this->build;
	}
}

namespace BotLogic
{
	// Clase Bot

	Bot::Bot() noexcept : Player() 
	{
		// OJO por desarrollar
	}

	Bot::Bot(std::string name, Map& mapa, Map& radar) noexcept : Player(name, mapa, radar)
	{
		// Empty
	}

	Movement Bot::get_next_move(Player enemy)
	{
		// OJO por desarrollar
	}

	Movement Bot::get_random_move()
	{                      
		auto items = this->get_build().get_arsenal().get_items();			 
		Item_ptr item = items[get_random_uniform(items.size()-1)];           // Obtenemos un item aleatorio dentro del arsenal 

		std::shared_ptr<Map> selected_map;
		if (item->get_type() == Item::comodin_type())															
		{
			selected_map = std::make_shared<Map>(this->get_map());				// si es comodin lo aplicamos en el mapa nuestro 
		}
		else 
		{
			selected_map = std::make_shared<Map>(this->get_radar());				// si es rocket lo aplicamos en el mapa enemigo
		}
		
		Coordinates location{selected_map->get_columns() - 1, selected_map->get_rows() - 1};   // Obtenemos la cantidad de filas y columnas
																	          // Obtenemos una celda aleatoria dentro del rango del radar
		Coordinates selected_cell{get_random_uniform(location.first), get_random_uniform(location.second)}; 

		return Movement(selected_cell, item);		
	}
	
	void Bot::create_map(Map& mapa)
	{
		size_t rows = mapa.get_rows();
		size_t columns = mapa.get_columns();

		// Crea un bote aleatorio dentro del margen del mapa
		auto create_random_boat = [rows, columns](Map mapa, size_t size) -> Boat_ptr
		{
			// obtenemos una coordenada aleatoria para la primera celda
			bool horizontal = get_random_uniform(1);

			if (horizontal)		// Segun el size del barco podra ser asigando hacia abajo o derecha desde un segmente reducido del mapa
			{
				Coordinates first_cell(get_random_uniform(columns - size), get_random_uniform(rows - 1));

				while (mapa.is_boat(first_cell.first, first_cell.second))	// Seleccionar una celda que no sea bote
				{
					first_cell = std::make_pair(get_random_uniform(columns - size), get_random_uniform(rows - 1));
				}

				Boat_ptr bote = std::make_shared<Boat>(size, first_cell, horizontal);
				return bote;
			}
			else 
			{
				Coordinates first_cell(get_random_uniform(columns - 1), get_random_uniform(rows - size));

				while (mapa.is_boat(first_cell.first, first_cell.second))	// Seleccionar una celda que no sea bote
				{
					first_cell = std::make_pair(get_random_uniform(columns - 1), get_random_uniform(rows - size));
				}

				Boat_ptr bote = std::make_shared<Boat>(size, first_cell, horizontal);
				return bote;
			}
		};
	
		// si el barco colisiona con una safe zone devuelve true
		auto collides_with_safezone = [rows, columns](Map mapa, Boat_ptr bote) -> bool
		{
			for (auto coord : bote->get_boat_coordinates())
			{
				if (mapa.is_failed(coord.first, coord.second))
				{
					return true;
				}
			}
			return false;
		};


		std::vector<Boat_ptr> botes;
		botes.resize(5);
		size_t attempts = 0;
		bool attempted = false;		  // Si ya intento 10 veces en crear en bote respetando la safe zone sera true
		for (int i = 0; i < 5; ++i)	  // Creamos 5 botes aleatorios y los asignamos
		{
			botes[i] = create_random_boat(mapa, 5 - i);
			auto is_correct = false;
			
			if (!attempted)    // Si no ha intentado lo suficiente seguimos buscando un bote que respecte la safe zone
			{
				is_correct = !collides_with_safezone(mapa, botes[i]);		  // vemos si colisiona con la safezone
				is_correct ? is_correct = mapa.insert_boat(botes[i]) : false; // si no colisiona lo insertamos
			}
			else 			  // si ya intento mas de 10 veces, solamente trata de insertarlo
			{
				is_correct = mapa.insert_boat(botes[i]);
			}
			
			if (is_correct)
			{
				mapa.create_safe_zone(botes[i]);
				attempts = 0;
				attempted = false;
				continue;
			}

			(attempts == 10 && !attempted)? (attempts = 0, attempted = true) : 	      // intentar ahora sin respectar la safe zone
			(attempts == 10) ? (--i, attempts = 0, attempted = false) : ++attempts;	  // retroceder al barco anterior o sumar un intento
			--i;
		}

		// Convertir de nuevo las casillas failed a agua
		for (size_t col = 0; col < columns; ++col)
		{
			for (size_t row = 0; row < rows; ++row)
			{
				if (mapa.is_failed(col, row))
				{
					mapa.set_water(mapa.get_ptr_cell(col, row));
				}
			}
		}
	}
}
