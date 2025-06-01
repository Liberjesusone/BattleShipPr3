#pragma once

#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP
#include "Types.hpp"

/**
 * @brief Agrupa la estructura de datos para el arbol que se usara como arbol de jugadas para el Bot
 * 
 * @note Cuenta con funcionalidades genericas para el manejo de arboles y la implementacion espeficia de un arbol de grado 4
 */
namespace Tree
{
    /**
	 * @brief Estructura de datos para arboles con 4 hijos
	 * 
	 * @note Esta estructura tiene el metodo childrens()
	 */
	template <typename T>
	class Node
	{
        using TreePtr = std::shared_ptr<Node<T>>;	

        public: 
            T data;
            TreePtr up{nullptr};
            TreePtr down{nullptr};
            TreePtr right{nullptr};
            TreePtr left{nullptr};

            //Constructor por defecto
            Node() : left(nullptr), right(nullptr), down(nullptr) 
			{
				// Empty
			}

            //Constructor con valor tipo Hoja
            Node(T data) : data(data), up(nullptr), left(nullptr), right(nullptr), down(nullptr) 
			{
				// Empty
			} 

            // Obtiene la clave 
            T get_key() { return this->data; }
            T& get_ref_key() { return this->data; }

			TreePtr get_highest_key()
			{
				TreePtr bestNode = std::make_shared<Node>(*this); // Nodo con el valor más alto
				float maxProbability = std::get<1>(this->data);   // Mayor número de veces que un barco puede pasar

				float den = 0;
				TreePtr aux = std::make_shared<Node>(*this);
				while (aux->up != nullptr)
				{
					aux = aux->up;
					if (std::get<2>(aux->get_key()))		// vemos si no ha sido usado antes
					{
						if (std::get<1>(aux->get_key()) > maxProbability) // actualizar max
						{
							std::cout << "coord : " << std::get<0>(aux->get_key()).first << " " << std::get<0>(aux->get_key()).second << " \n";
							std::cout << "aux value: " << std::get<1>(aux->get_key()) << "  max: " << maxProbability << "\n"; 
							bestNode = aux;
							maxProbability = std::get<1>(aux->get_key());
						}
						den += std::get<1>(aux->get_key());
						break;
					}
				}
				aux = std::make_shared<Node>(*this);
				while (aux->right != nullptr)
				{
					aux = aux->right;
					if (std::get<2>(aux->get_key()))
					{
						if (std::get<1>(aux->get_key()) > maxProbability) // actualizar max
						{
							std::cout << "coord : " << std::get<0>(aux->get_key()).first << " " << std::get<0>(aux->get_key()).second << " \n";
							std::cout << "aux value: " << std::get<1>(aux->get_key()) << "  max: " << maxProbability << "\n"; 
							bestNode = aux;
							maxProbability = std::get<1>(aux->get_key());
						}
						den += std::get<1>(aux->get_key());
						break;
					}
				}
				aux = std::make_shared<Node>(*this);
				while (aux->left != nullptr)
				{
					aux = aux->left;
					if (std::get<2>(aux->get_key()))
					{
						if (std::get<1>(aux->get_key()) > maxProbability) // actualizar max
						{
							std::cout << "coord : " << std::get<0>(aux->get_key()).first << " " << std::get<0>(aux->get_key()).second << " \n";
							std::cout << "aux value: " << std::get<1>(aux->get_key()) << "  max: " << maxProbability << "\n"; 
							bestNode = aux;
							maxProbability = std::get<1>(aux->get_key());
						}
						den += std::get<1>(aux->get_key());
						break;
					}
				}
				aux = std::make_shared<Node>(*this);
				while (aux->down != nullptr)
				{
					aux = aux->down;
					if (std::get<2>(aux->get_key()))
					{
						if (std::get<1>(aux->get_key()) > maxProbability) // actualizar max
						{
							std::cout << "coord : " << std::get<0>(aux->get_key()).first << " " << std::get<0>(aux->get_key()).second << " \n";
							std::cout << "aux value: " << std::get<1>(aux->get_key()) << "  max: " << maxProbability << "\n"; 
							bestNode = aux;
							maxProbability = std::get<1>(aux->get_key());
						}
						den += std::get<1>(aux->get_key());
						break;
					}
				}
				std::cout << "Besto node finded max: " << std::get<1>(bestNode->get_key()) << "   coord : " << std::get<0>(bestNode->get_key()).first << " " << std::get<0>(bestNode->get_key()).second << " \n";
				
				std::get<1>(bestNode->get_ref_key()) = maxProbability / den;
				std::cout << "probability " << (std::get<1>(bestNode->get_key())) << "\n";
				std::get<2>(bestNode->get_ref_key()) = false;
				return bestNode;
			}

			void print()
			{
				std::string upStr;
				std::string rightStr;
				std::string downStr;
				std::string leftStr;
			
				TreePtr best_node = std::make_shared<Node>(*this); // Nodo con el valor más alto
				float max_probability = std::get<1>(this->data);   // Mayor número de veces que un barco puede pasar

				TreePtr aux = std::make_shared<Node>(*this);
				while (aux->up != nullptr)
				{
					aux = aux->up;
					std::string coordinates = std::string("[") + std::to_string(std::get<0>(aux->get_key()).first) + std::string("-") + std::to_string(std::get<0>(aux->get_key()).second) + std::string("] ");
					std::string number = std::to_string(std::get<1>(aux->get_key())) + std::string(" ");
					std::string boolean = (std::get<2>(aux->get_key()) ? std::string("true") : std::string("false")) + std::string(" ");
					upStr += coordinates + number + boolean + std::string("; "); 
				}
				aux = std::make_shared<Node>(*this);
				while (aux->right != nullptr)
				{
					aux = aux->right;
					std::string coordinates = std::string("[") + std::to_string(std::get<0>(aux->get_key()).first) + std::string("-") + std::to_string(std::get<0>(aux->get_key()).second) + std::string("] ");
					std::string number = std::to_string(std::get<1>(aux->get_key())) + std::string(" ");
					std::string boolean = (std::get<2>(aux->get_key()) ? std::string("true") : std::string("false")) + std::string(" ");
					rightStr += coordinates + number + boolean + std::string("; ");  
				}
				aux = std::make_shared<Node>(*this);
				while (aux->left != nullptr)
				{
					aux = aux->left;
					std::string coordinates = std::string("[") + std::to_string(std::get<0>(aux->get_key()).first) + std::string("-") + std::to_string(std::get<0>(aux->get_key()).second) + std::string("] ");
					std::string number = std::to_string(std::get<1>(aux->get_key())) + std::string(" ");
					std::string boolean = (std::get<2>(aux->get_key()) ? std::string("true") : std::string("false")) + std::string(" ");
					leftStr += coordinates + number + boolean + std::string("; "); 
				}
				aux = std::make_shared<Node>(*this);
				while (aux->down != nullptr)
				{
					aux = aux->down;
					std::string coordinates = std::string("[") + std::to_string(std::get<0>(aux->get_key()).first) + std::string("-") + std::to_string(std::get<0>(aux->get_key()).second) + std::string("] ");
					std::string number = std::to_string(std::get<1>(aux->get_key())) + std::string(" ");
					std::string boolean = (std::get<2>(aux->get_key()) ? std::string("true") : std::string("false")) + std::string(" ");
					downStr += coordinates + number + boolean + std::string("; "); 
				}		
				std::cout << "UpBranch: " << upStr << "\n";		
				std::cout << "rightBranch: " << rightStr << "\n";		
				std::cout << "downBranch: " << downStr << "\n";		
				std::cout << "leftBranch: " << leftStr << "\n";		
			}

            //Retorna una lista con los hijos asi apunten a nullptr, en el orden up, down, right, left
            std::list<TreePtr> childrens() 
            {
                std::list<TreePtr> children_list;
                children_list.push_back(up);
                children_list.push_back(down);
                children_list.push_back(right);
                children_list.push_back(left);
                return children_list;
            }

			size_t get_heigth()
			{
				size_t heigth = 1;
				TreePtr aux = std::make_shared<Node>(*this);
				while (aux->up != nullptr)
				{
					aux = aux->up;
					++heigth;
				}
				aux = std::make_shared<Node>(*this);
				while (aux->down != nullptr)
				{
					aux = aux->down;
					++heigth;
				}

				return heigth;
			}
			
			/**
			 * @brief devuelven el tamaño de la rama mas corta, y un bool para saber que rama es 
			 * 
			 * @param vertical especifica en que sentido se quiere analizar las ramas, vertical o horizontal(vertical = false)
			 * 
			 * @return devuelve el tamaño, y true si es arriba o derecha, y false si es abajo o izq, respecto a la dirección vertical/horizontal
			 */
			std::pair<size_t, bool> get_short_branch(bool vertical)
			{
				size_t size = 1;
				size_t size2 = 1;
				TreePtr aux = std::make_shared<Node>(*this);
				while ((vertical ? aux->up != nullptr : aux->right != nullptr))
				{
					aux = (vertical ? aux->up : aux->right);
					++size;
				}
				aux = std::make_shared<Node>(*this);
				while ((vertical ? aux->down != nullptr : aux->left != nullptr))
				{
					aux = (vertical ? aux->down : aux->left);
					++size2;
				}

				return std::make_pair((size > size2 ? size2 : size), size < size2);
			}

			size_t get_width()
			{
				size_t width = 1;
				TreePtr aux = std::make_shared<Node>(*this);
				while (aux->right != nullptr)
				{
					aux = aux->right;
					++width;
				}
				aux = std::make_shared<Node>(*this);
				while (aux->left != nullptr)
				{
					aux = aux->left;
					++width;
				}

				return width;
			}
	};

	/**
	 * @brief Obtiene la cantidad de nodos en el arbol de manera recursiva 
	 * 
	 * @return int Cantidad de nodos en el arbol, si el ptr es null retorna 0
	 */
	template <typename Tree>
	int cardinality(std::shared_ptr<Tree> root) 
	{
		if (root == nullptr) return 0;
		
		int count = 1;
		for (auto child : root->childrens())
		{
			if (child == nullptr) continue;   // No nos interesan los nodos con hijos libres
			count += cardinality(child);
		}
		return count;
	}

	/**
	 * @brief Obtiene la capacidad total de nodos que puede tener el arbol con una altura dada
	 * 
	 * @return int Capacidad total de nodos que puede tener el arbol con una altura dada
	 */
	int capacity(int height);

	/**
	 * @brief Obtiene la altura del arbol de manera recursiva 
	 * 
	 * @note si el arbol es vacio devuelve -1
	 * 
	 * @return int Altura del arbol
	 */
	template <typename Tree>
	int heigth(std::shared_ptr<Tree> root) 
	{
		if (root == nullptr) return -1;
		
		int max_of = 0;
		for (auto child : root->childrens())
		{
			if (child == nullptr) continue;   // No nos interesan los nodos con hijos libres
			max_of = std::max(max_of, heigth(child));
		}
		return max_of + 1;
	}

	/**
	 * @brief Devuelve la cantidad de nodos minima hasta el primer espacio libre en el arbol
	 * 
	 * @note si el arbol es vacio devuelve -1
	 * 
	 * @return int Cantidad de nodos minima hasta el primer espacio libre en el arbol
	 */
	template <typename Tree>
	int min_heigth(std::shared_ptr<Tree> root)  
	{
		if (root == nullptr) return -1;

		int min_of = 0;

		int i = 0;
		for (auto child : root->childrens())
		{
			if (child == nullptr) // Si es null es por que el nodo actual tiene un espacio libre
			{
				min_of = 0;   
				break;
			}

			if (i == 0)   //primera iteracion
			{
				min_of = min_heigth(child);
				++i;
				continue;
			}
			
			min_of = std::min(min_of, min_heigth(child));
		}

		return min_of + 1;
	}

	/**
	 * @brief Devuelve una lista con el numero de hijo, que se debe tomar para llegar a la ruta mas corta
	 * 
	 * @note si el arbol es vacio devuelve 1 
	 * 
	 * @return std::list<int> Lista con el numero de hijo, que se debe tomar para llegar a la ruta mas corta
	 */
	template <typename Tree>
	std::list<int> min_route(std::shared_ptr<Tree> root) 
	{
		if (root == nullptr) return {1};

		std::list<int> route;    //tiene los numeros en forma de indx de los hijos que marcan la ruta mas corta donde no hay nodo
		auto list_of_childrens = root->childrens();

		int min_of = 0;         
		int child_min_route = 0;        // Tiene el indice en la lista de child, del hijo que tiene la ruta mas corta 
		Tree* child_min_ptr = nullptr;  // Tiene el apuntador a ese child

		int i = 0;
		for (auto child : list_of_childrens)
		{
			if (child == nullptr) // Si es null es por que el nodo actual tiene un espacio libre
			{
				route.push_back(i);
				return route;
			}

			if (i == 0)  // primera iteracion
			{
				min_of = min_heigth(child);
				child_min_ptr = child;
				++i;
				continue;
			}

			int min_heigth = min_heigth(child);
			min_of = std::min(min_of, min_heigth);

			if (min_heigth < min_of) //si se consigue un nodo menor al minimo actual se actualiza 
			{
				child_min_route = i;
				child_min_ptr = child;
			}
			++i;
		}

		route.push_back(child_min_route);		// Agregamos a la lista el hijo de el nodo actual
		route.merge(min_route(child_min_ptr));  // le concatenamos la lista de los hijos de ese nodo
		
		return route;
	}

	/**
	 * @brief Inserta un elemento en la ruta mas corta del arbol
	 * 
	 * @note si el arbol es vacio se crea el primer nodo
	 */
	template <typename Tree, typename T>
	void insert_min(std::shared_ptr<Tree> root, T elem)  
	{
		if (root == nullptr) // Si el arbol esta vacio se crea el primer nodo
		{
			root = new Tree(elem);
			return;
		}

		auto list_of_childrens = root->childrens();

		int min_of = 0;
		Tree* child_min = nullptr;  // Tiene el apuntador a el child con la ruta mas corta

		int i = 0;
		for (auto child : list_of_childrens)
		{
			if (child == nullptr) // Si es null es por que el nodo actual tiene un espacio libre
			{
				root->insert(elem);
				return;
			}

			if (i == 0)  // primera iteracion
			{
				min_of = min_heigth(child);
				child_min = child;
				++i;
				continue;
			}

			int min_heigth = min_heigth(child);
			min_of = std::min(min_of, min_heigth);

			if (min_heigth < min_of) //si se consigue un nodo menor al minimo actual se actualiza 
				child_min = child;
			
			++i;
		}

		insert_min(child_min, elem);		
	}

	/**
	 * @brief Elimina todos los nodos del arbol
	 * 
	 * @note si el arbol es vacio no hace nada
	 */
	template <typename Tree>
	void delete_tree(Tree* root)
	{
		if (root == nullptr) return;
		for (auto child : root->childrens())
		{
			delete_tree(child);
		}
		delete root;
	}



	//----------------------------- FUNCIONES DE RECORRIDO DEL ARBOL ----------------------------- //

	/**
	 * @brief Recorre el arbol en preorden y aplica una accion a cada nodo
	 * 
	 * @note si el arbol es vacio no hace nada
	 */
	template <typename Tree, typename Action>
	void for_all(std::shared_ptr<Tree> root, Action&& action)	
	{
		if (root == nullptr) return;
		action(root->dato);
		for (auto child : root->childrens())
		{
			for_all(child, action);
		}
	}

	/**
	 * @brief Recorre el arbol en level traverse y aplica una accion a cada nodo
	 */
	template <typename Tree, typename Action>
	void level_traverse(std::shared_ptr<Tree> root, Action&& action)  
	{
		std::queue<Tree*> myQueue;
		myQueue.push(root);
		
		while (!myQueue.empty())		// Recorremos el arbol en anchura  
		{
			auto myRoot = myQueue.front(); // Obtenemos el frente de la cola 
			myQueue.pop();				   // Lo desencolamos 
			action(myRoot);

			for (auto child : myRoot->childrens())  // Apilamos a la cola de ejecucion cada hijo 
			{
				if (child == nullptr) continue;
				myQueue.push(child);
			}
		}
	}

    
	/**
	 * @brief Genera un numero aleatorio entre [0, n]
	 */
	int getRandomUniform(int n);
}


#endif // STRUCTURES_HPP