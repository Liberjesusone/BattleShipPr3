#include "Types.hpp"
#include "Structures.hpp"

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

	bool Map::is_main(size_t col, size_t row)
	{
		Map_cell_ptr cell = get_ptr_cell(col, row);
		bool is_main = !this->is_boat(col, row) && !this->is_destroyed(col, row) && !this->is_failed(col, row) &&
					   !this->is_water(col, row) && !this->is_protected(col, row);
		return is_main;
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

	void Map::set_main(Map_cell_ptr cell) noexcept
	{
	    auto location = cell->get_location();
	    matrix[location.first][location.second] = std::make_shared<Map_cell>(location.first, location.second);
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

	void Map::create_safe_zone(Coordinates coord) noexcept
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

		for (auto sur_coord : surrounding_cells)
		{
			if (sur_coord.first < this->columns && sur_coord.second < this->rows &&
				!this->is_boat(sur_coord.first, sur_coord.second) && !this->is_failed(sur_coord.first, sur_coord.second))
			{
				this->set_main(this->get_ptr_cell(sur_coord.first, sur_coord.second));
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

	void SingleShot::operator() (PlayerPair& players, Map_cell_ptr cell) 
	{
		Map_ptr mapa = players.first->get_radar();
		auto cell_type = cell->get_type();

		if (cell_type == Map_cell::boat_type())
		{
			std::cout << "destroying cell {" << cell->get_location().first << " " << cell->get_location().second << "}\n";
			mapa->set_destroy(cell);

		}
		else if (cell_type == Map_cell::protected_type())
		{
			std::cout << "Shoting shild {" << cell->get_location().first << " " << cell->get_location().second << "}\n";
			mapa->set_boat(cell);
		}
		else if (cell_type == Map_cell::water_type())
		{
			std::cout << "Failing cell {" << cell->get_location().first << " " << cell->get_location().second << "}\n";
			mapa->set_fail(cell);	
		}
	}


	// Clase ChargedShot

	ChargedShot::ChargedShot() noexcept
	{
		// Empty	
	}
	
	void ChargedShot::operator() (PlayerPair& players, Map_cell_ptr cell)
	{	
		Map_ptr mapa = players.first->get_radar();
		auto cell_type = cell->get_type();
		std::cout << "shoting charged\n";
		
		if (cell_type == Map_cell::boat_type())		  // Si la celda es un bote
		{										      // Obtenemos el bote de donde proviene la celda
			Boat_ptr enemy_boat = players.second->get_build().get_fleet().get_boat_of_cell(cell);	
			for (auto coord : enemy_boat->get_boat_coordinates())					 // Destruimos cada una de ellas
			{
				auto boat_cell = mapa->get_ptr_cell(coord.first, coord.second);
				std::cout << "Charged destroyin cell {" << boat_cell->get_location().first << " " << boat_cell->get_location().second << "}\n";
				
				boat_cell->get_type() == Map_cell::protected_type() ? mapa->set_boat(boat_cell) : mapa->set_destroy(boat_cell);
			}
		}
		else if (cell_type == Map_cell::water_type()) // Si la celda es agua 
		{
			std::cout << "Charged Failing cell {" << cell->get_location().first << " " << cell->get_location().second << "}\n";
			mapa->set_fail(cell);	
		}											  // Si no es ninguna de las anteriores no pasa nada
	}

		
	// Clase HealCell
	
	HealCell::HealCell() noexcept
	{
		// Empty
	}

	void HealCell::operator() (PlayerPair& players, Map_cell_ptr cell)
	{
		Map_ptr mapa = players.first->get_map();
		std::cout << "Healing cell {" << cell->get_location().first << " " << cell->get_location().second << "}\n";
		if (cell->get_type() == Map_cell::destroyed_type())
		{
			std::cout << "was Heal\n";
			mapa->set_boat(cell);
		}
	}


	// Clase ProtectCell
	
	ProtectCell::ProtectCell() noexcept
	{
		// Empty
	}

	void ProtectCell::operator() (PlayerPair& players, Map_cell_ptr cell)
	{
		Map_ptr mapa = players.first->get_map();
		std::cout << "Protecting cell {" << cell->get_location().first << " " << cell->get_location().second << "}\n";
		if (cell->get_type() == Map_cell::boat_type())
		{
			std::cout << "was protected\n";
			mapa->set_protected(cell);
		}
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

	void Item::use_on(PlayerPair& players, size_t col, size_t row)
	{
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

	float Boat::get_distruction_per(Map_ptr mapa)
	{
		size_t destroyed_cells = 0;
		for (auto coord : this->get_boat_coordinates())
		{
			if (mapa->is_destroyed(coord.first, coord.second))
			{
				++destroyed_cells;
			}
		}
		return this->distruction_per = (100 * static_cast<float>(destroyed_cells)) / static_cast<float>(this->get_size());
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
		// se cargan por defecto los items
		this->items.push_back(std::make_shared<Rocket<SingleShot>>(1000, "Missile"));
		this->items.push_back(std::make_shared<Rocket<ChargedShot>>(3, "Torpedo"));
		this->items.push_back(std::make_shared<Comodin<HealCell>>(3, "Corazon"));
		this->items.push_back(std::make_shared<Comodin<ProtectCell>>(3, "Shild"));
	}

	std::vector<Item_ptr>& Arsenal::get_items() noexcept
	{
		return this->items;
	}


	// Clase Build

	Build::Build() noexcept : name("Build 1"), flota(), arsenal() 
	{
		// Empty
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

	Player::Player(std::string name, Map_ptr mapa, Map_ptr radar) noexcept : name(name), mapa(mapa), radar(radar)
	{
		// Emtpy
	}

	Map_ptr Player::get_radar() noexcept
	{
		return this->radar;
	}
	
	Map_ptr Player::get_map() noexcept
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

	Bot::Bot(std::string name, Map_ptr mapa, Map_ptr radar) noexcept : Player(name, mapa, radar)
	{
		this->create_map(mapa);	 // Mandamos a crear el mapa
	}

	Movement Bot::get_next_move(Player enemy)
	{
		
	}

	Movement Bot::get_random_move()
	{                      
		auto items = this->get_build().get_arsenal().get_items();
		Item_ptr item = items[get_random_uniform(items.size()-1)];           // Obtenemos un item aleatorio dentro del arsenal 

		Map_ptr selected_map;
		if (item->get_type() == Item::comodin_type())															
		{
			selected_map = this->get_map();				// si es comodin lo aplicamos en el mapa nuestro 
		}
		else 
		{
			selected_map = this->get_radar();			// si es rocket lo aplicamos en el mapa enemigo
		}
		
		Coordinates location{selected_map->get_columns() - 1, selected_map->get_rows() - 1};   // Obtenemos la cantidad de filas y columnas
																	          // Obtenemos una celda aleatoria dentro del rango del radar
		Coordinates selected_cell{get_random_uniform(location.first), get_random_uniform(location.second)}; 
		std::cout << "\nobteniendo el moviemiento aleatorio\n";
		return Movement(selected_cell, item);		
	}
	
	void Bot::create_map(Map_ptr mapa)
	{
		size_t rows = mapa->get_rows();
		size_t columns = mapa->get_columns();

		// Crea un bote aleatorio dentro del margen del mapa
		auto create_random_boat = [rows, columns](Map_ptr mapa, size_t size) -> Boat_ptr
		{
			// obtenemos una coordenada aleatoria para la primera celda
			bool horizontal = get_random_uniform(1);

			if (horizontal)		// Segun el size del barco podra ser asigando hacia abajo o derecha desde un segmente reducido del mapa
			{
				Coordinates first_cell(get_random_uniform(columns - size), get_random_uniform(rows - 1));

				while (mapa->is_boat(first_cell.first, first_cell.second))	// Seleccionar una celda que no sea bote
				{
					first_cell = std::make_pair(get_random_uniform(columns - size), get_random_uniform(rows - 1));
				}

				Boat_ptr bote = std::make_shared<Boat>(size, first_cell, horizontal);
				return bote;
			}
			else 
			{
				Coordinates first_cell(get_random_uniform(columns - 1), get_random_uniform(rows - size));

				while (mapa->is_boat(first_cell.first, first_cell.second))	// Seleccionar una celda que no sea bote
				{
					first_cell = std::make_pair(get_random_uniform(columns - 1), get_random_uniform(rows - size));
				}

				Boat_ptr bote = std::make_shared<Boat>(size, first_cell, horizontal);
				return bote;
			}
		};
	
		// si el barco colisiona con una safe zone devuelve true
		auto collides_with_safezone = [rows, columns](Map_ptr mapa, Boat_ptr bote) -> bool
		{
			for (auto coord : bote->get_boat_coordinates())
			{
				if (mapa->is_failed(coord.first, coord.second))
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
				is_correct = !collides_with_safezone(mapa, botes[i]);
				is_correct ? is_correct = mapa->insert_boat(botes[i]) : false; 
			}
			else 
			{
				is_correct = mapa->insert_boat(botes[i]);
			}
			

			if (is_correct)
			{
				mapa->create_safe_zone(botes[i]);
				attempts = 0;
				attempted = false;
				continue;
			}

			(attempts == 10 && !attempted)? (attempts = 0, attempted = true) : 	      // intentar ahora sin respectar la safe zone
			(attempts == 10) ? (--i, attempts = 0, attempted = false) : ++attempts;	  // retroceder al barco anterior o sumar un intento
			--i;
		}
		
		for (auto boat : botes)			// Agregamos la lista de botes al bot
		{
			this->get_build().get_fleet().add_boat(boat);
		}

		// Convertir de nuevo las casillas failed a agua
		for (size_t col = 0; col < columns; ++col)
		{
			for (size_t row = 0; row < rows; ++row)
			{
				if (mapa->is_failed(col, row))
				{
					mapa->set_water(mapa->get_ptr_cell(col, row));
				}
			}
		}
	}

	bool Bot::play(Player_ptr player)
	{
		auto movement = this->get_random_move();
		auto cell = movement.first;
		auto item = movement.second;

		std::cout << "jugo en la casilla {" << cell.first << " " << cell.second << "} con un " << item->get_type() << "\n";
		PlayerPair pair = std::make_pair(std::make_shared<Player>(*this), player);
		item->use_on(pair, cell.first, cell.second);
		return item->get_type() == Item::comodin_type();	
	}

	void Bot::build_target_boat(Coordinates cell)
	{	
		// Construir todas las celdas
		target_boat = 
		for ()
		{

		}

		// Asignarles cantidades, y luego calcular el porcentaje
	}

	bool Bot::make_movement(Player_ptr player)
	{	
		PlayerPair player_pair = std::make_pair(std::make_shared<Player>(*this), player);
		Map_ptr player_map = player->get_map();
		Map_ptr bot_map = player->get_radar();

		Item_ptr missile = this->get_build().get_arsenal().get_items()[0];
		Item_ptr charged_missile = this->get_build().get_arsenal().get_items()[1];
		Item_ptr heal = this->get_build().get_arsenal().get_items()[2];
		Item_ptr shild = this->get_build().get_arsenal().get_items()[3];

		std::vector<Boat_ptr>& bot_boats = this->get_build().get_fleet().get_boats();

		auto get_relative_position = [](Coordinates coord1, Coordinates coord2) -> char 
		{
			if (coord1.first == coord2.first) 
			{ // Misma Columna
				return (coord1.second < coord2.second) ? 'U' : 'D';
			}
			else if (coord1.second == coord2.second) 
			{ // Misma fila
				return (coord1.first < coord2.first) ? 'R' : 'L';
			}

			return 'N'; // No debería ocurrir, pues al menos una coordenada es igual
		};



		auto cal_des_boats = [this, player]() -> size_t
		{
			size_t destroyed_boats = 0;
			size_t indx = 0;
			for (auto boat : player->get_build().get_fleet().get_boats())
			{
				if (boat->get_distruction_per(player->get_map()) == 100.00f)
				{
					this->destroyed_boats[indx] = boat;		// Agregamos el barco a nuestra lista de destruidos
					++destroyed_boats;
				}
				++indx;
			}
			return destroyed_boats;
		};
		size_t destroyed_boats = cal_des_boats();
		

		// Una vez se le hayan acabado los heals al enemigo poenemos nuestras celdas acertadas para repasar
		static bool already_reshoted = false; // para que solo se mande a repasar los shots una vez 
		if (player->get_build().get_arsenal().get_items()[2]->get_stock() == 0 && !already_reshoted && cells_to_reshot.size() == 0)
		{
			for (auto cell : successful_shots)
			{
				this->cells_to_reshot.push(cell);
			}
			already_reshoted = true;
		}

		bool have_to_cure = false; 
		std::vector<size_t> damaged_boat_indx; 			// Indices de los botes que estan dañados
		std::vector<size_t> good_boats_indx;
		size_t indx = 0; 
		for (auto boat : bot_boats)	// Obtenemos la lista de botes que tienen mas del 50% de destruccion y los que tienen menos o igual 50%
		{
			if (boat->get_distruction_per(bot_map) > 0)							
			{	
				damaged_boat_indx.push_back(indx);
			}
			if (boat->get_distruction_per(bot_map) <= 50.00f)
			{
				good_boats_indx.push_back(indx);
			}
			if (boat->get_distruction_per(bot_map) > 50.00f)  
			{
				have_to_cure = true;						
			}
			++indx;
		}

		// Si uno de nuestros botes supera el 50% de destruccion priorizamos la curacion
		if (have_to_cure && heal->get_stock() > 0)			// Si no hay heals pasamos al siguiente
		{
			if (damaged_boat_indx.size() > 0)			    // Si hay botes destruidos 
			{
				size_t random_indx = get_random_uniform(damaged_boat_indx.size() - 1); 
				auto boat_coordinates = bot_boats[damaged_boat_indx[random_indx]]->get_boat_coordinates(); // bote aleatorio

				// Buscamos aleatoriamente una celda destruida del bote aleatorio
				indx = 0;
				std::vector<std::unordered_set<Coordinates, PairHash>::iterator> damaged_cells;
				for (auto it = boat_coordinates.begin(); it != boat_coordinates.end(); ++it) 
				{
					if (bot_map->is_destroyed(it->first, it->second)) 
					{
						damaged_cells.push_back(it); 					// Guardamos el iterador 
					}
				}

				random_indx = get_random_uniform(damaged_cells.size() - 1);		// Obtenemos un indice para los iteradores, aleatorio
				Coordinates random_coordinates = *(damaged_cells[random_indx]); // celda aleatoria


				heal->use_on(player_pair, random_coordinates.first, random_coordinates.second);
				return true;			
			}
		}
		
		// Si no, vemos si tenemos un objetivo en nuestro arbol (target_boat)
		if (target_boat == nullptr)			// si no hay un target boat
		{
			bool we_should_shot = true;

			// Si quedan escudos hacemos un 50/50 a ver si disparamos o protegemos
			if (shild->get_stock() > 0)
			{
				bool we_should_shot = get_random_uniform(1);
			}

			if (we_should_shot)	// Disparamos espaciadamente
			{
				auto select_shot_cell = [this]() -> Coordinates
				{
					auto player_map = this->get_radar();
					// Comprobar si hay que repasar un disparo
					if (this->cells_to_reshot.size() > 0)
					{
						auto cell = this->cells_to_reshot.top();
						this->cells_to_reshot.pop();
						return cell;
					}
					else // Si no hay que repasar uno, disparar espaciadamente
					{
						// Intentar diez veces buscar un celda agua, que no sea safe_zone
						Coordinates ran_cell(get_random_uniform(player_map->get_columns() - 1), get_random_uniform(player_map->get_rows() - 1));

						for (int i = 0; i < 10 && !this->board->is_water(ran_cell.first, ran_cell.second); ++i)
						{
							ran_cell = std::make_pair(get_random_uniform(player_map->get_columns() - 1), get_random_uniform(player_map->get_rows() - 1));

							if (this->board->is_destroyed(ran_cell.first, ran_cell.second) || this->board->is_failed(ran_cell.first, ran_cell.second))
							{
								--i; 	// Solo sumamos un intento si fue tipo Map_cell = safe zone
							}
						}	
						if (this->board->is_water(ran_cell.first, ran_cell.second))
						{
							return ran_cell;
						}

						// Si no se encontro ninguna celda water, disparar en una safe Zone
						for (int i = 0; i < 10 && !this->board->is_water(ran_cell.first, ran_cell.second) && 
												  !this->board->is_main(ran_cell.first, ran_cell.second); ++i)
						{
							ran_cell = std::make_pair(get_random_uniform(player_map->get_columns() - 1), get_random_uniform(player_map->get_rows() - 1));
						}

						// Se supone que en este punto, la celda ran_cell, sera siempre o agua o safe zone
						return ran_cell;
					}
				};

				Coordinates cell_to_shot = select_shot_cell();
				
				missile->use_on(player_pair, cell_to_shot.first, cell_to_shot.second);
				
				// Asiganar al bot los datos obtenidos del disparo
				if (player_map->is_failed(cell_to_shot.first, cell_to_shot.second))				// fallamos
				{
					board->set_fail(board->get_ptr_cell(cell_to_shot.first, cell_to_shot.second));
					board->create_safe_zone(cell_to_shot);
				}
				else if (player_map->is_destroyed(cell_to_shot.first, cell_to_shot.second))      // Acerto
				{
					// Marcar nuestra board como destruida, y agregar el tiro a successful shots, agregar barcos si se destruyeron 
					// a la lista de barcos y mandar a construir el targetboat
					board->set_destroy(board->get_ptr_cell(cell_to_shot.first, cell_to_shot.second));
					successful_shots.push_back(cell_to_shot);
					build_target_boat(cell_to_shot);	
					
					size_t new_des_boats = cal_des_boats();
					if (new_des_boats > destroyed_boats)	// Si antes del disparo teniamos menos barcos destruidos significa que destruimos un con el disparo
					{
						target_boat = nullptr;		// Entonces eliminamos el arbol de posibilidades
					}
					
				}
				else if (player_map->is_boat(cell_to_shot.first, cell_to_shot.second))			 // Disparo a un escudo
				{
					// Marcar nuestra board como bote, y agregar el tiro a cells-to-reshot
					board->set_boat(board->get_ptr_cell(cell_to_shot.first, cell_to_shot.second));
					cells_to_reshot.push(cell_to_shot);
				}
				return false;
			}
			else 				// Protegemos una de nuestras casillas 
			{	
				size_t random_indx = get_random_uniform(good_boats_indx.size() - 1); 
				auto boat_coordinates = bot_boats[good_boats_indx[random_indx]]->get_boat_coordinates(); // bote aleatorio con menos del 50%

				// Buscamos aleatoriamente una celda buena del bote aleatorio
				indx = 0;
				std::vector<std::unordered_set<Coordinates, PairHash>::iterator> boat_cells;
				for (auto it = boat_coordinates.begin(); it != boat_coordinates.end(); ++it) 
				{
					if (bot_map->is_boat(it->first, it->second)) 
					{
						boat_cells.push_back(it); 					// Guardamos el iterador 
					}
				}

				random_indx = get_random_uniform(boat_cells.size() - 1);     // Obtenemos un indice para los iteradores, aleatorio
				Coordinates random_coordinates = *(boat_cells[random_indx]); // celda aleatoria

				shild->use_on(player_pair, random_coordinates.first, random_coordinates.second);
				return true;
			}
		}
		else					// Si hay un target boat, seguimos disparando en funcion de el 
		{
			Node_ptr selected_node = target_boat->get_highest_key();
			Data& clave = selected_node->get_ref_key();
			auto coord = std::get<0>(clave);
			auto percent = std::get<1>(clave);

			bool use_normal_missile = true;

			// Seleccionar entre misil normal o super misil
			if (charged_missile->get_stock() > 0 && percent >= 90.00f)
			{
				use_normal_missile = false;
			}
			else if (charged_missile->get_stock() > 0 && percent > 75.00f)
			{
				use_normal_missile = get_random_uniform(1);
			}

			use_normal_missile ? missile->use_on(player_pair, coord.first, coord.second) : charged_missile->use_on(player_pair, coord.first, coord.second);

			if (player_map->is_failed(coord.first, coord.second))				// fallamos
			{
				board->set_fail(board->get_ptr_cell(coord.first, coord.second));
				board->create_safe_zone(coord);

				// Cortar esta rama del arbol 
				auto initial_coord = std::get<0>(target_boat->get_ref_key());
				char relative_pos = get_relative_position(initial_coord, coord);
				
				if (relative_pos == 'R')	    // Esta a la derecha
				{
					selected_node->left->right = nullptr;
				}
				else if (relative_pos == 'L')	// Esta a la izq
				{
					selected_node->right->left = nullptr;
				}
				else if (relative_pos == 'D')	// Esta abajo
				{
					selected_node->up->down = nullptr;
				}
				else if (relative_pos == 'U')	// Esta arriba
				{
					selected_node->down->up = nullptr;
				}
			}
			else if (player_map->is_destroyed(coord.first, coord.second))      // Acerto
			{
				// Marcar nuestra board como destruida, y agregar el tiro a successful shots, agregar barcos si se destruyeron 
				// a la lista de barcos y mandar a construir el targetboat
				std::get<2>(clave) = false;
				board->set_destroy(board->get_ptr_cell(coord.first, coord.second));
				successful_shots.push_back(coord);	
				
				size_t new_des_boats = cal_des_boats();
				if (new_des_boats > destroyed_boats)	// Si antes del disparo teniamos menos barcos destruidos significa que destruimos un con el disparo
				{
					target_boat = nullptr;		// Entonces eliminamos el arbol de posibilidades
				}
			}
			else if (player_map->is_boat(coord.first, coord.second))			 // Disparo a un escudo
			{
				// Marcar nuestra board como bote, y agregar el tiro a cells-to-reshot
				board->set_boat(board->get_ptr_cell(coord.first, coord.second));
				cells_to_reshot.push(coord);
			}
			return false;
		}
	}
}
