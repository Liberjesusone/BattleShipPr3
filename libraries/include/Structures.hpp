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
        using Node_ptr = std::shared_ptr<Node<T>>;	

        public: 
            T data;
            Node_ptr up{nullptr};
            Node_ptr down{nullptr};
            Node_ptr right{nullptr};
            Node_ptr left{nullptr};

            //Constructor por defecto
            Node() : left(nullptr), right(nullptr), down(nullptr) {}

            //Constructor con valor tipo Hoja
            Node(T data) : data(data), up(nullptr), left(nullptr), right(nullptr), down(nullptr) {} 

            // Obtiene la clave 
            T get_key() { return this->data; }
            T& get_ref_key() { return this->data; }

			Node_ptr get_highest_key()
			{
				Node_ptr best_node = std::make_shared<Node>(*this); // Nodo con el valor más alto
				size_t max_probability = std::get<1>(this->data);   // Mayor número de veces que un barco puede pasar

				std::stack<Node_ptr> stack;
				stack.push(std::make_shared<Node>(*this));	
				// Revisamos todos los hijos del nodo actual
				while (!stack.empty())
				{
					Node_ptr node = stack.top();
					stack.pop();
					for (const auto& child : node->childrens()) 
					{
						if (child != nullptr && std::get<1>(child->data) > max_probability && std::get<2>(child->data)) // Ajustamos el max
						{
							max_probability = std::get<1>(child->data);
							best_node = child;
						}
						stack.push(child);	// Sumamos a su hijo para que en el llamado recursivo se vean tmb sus hijos
					}	
				}

				return best_node;
			}

            //Retorna una lista con los hijos asi apunten a nullptr, en el orden up, down, right, left
            std::list<Node_ptr> childrens() 
            {
                std::list<Node_ptr> children_list;
                children_list.push_back(up);
                children_list.push_back(down);
                children_list.push_back(right);
                children_list.push_back(left);
                return children_list;
            }

            // Inserta un nuevo nodo en el arbol
            void insert(T dato) 
            {
                // OJO POR DESARROLLAR
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
	int get_random_uniform(int n);
}


#endif // STRUCTURES_HPP