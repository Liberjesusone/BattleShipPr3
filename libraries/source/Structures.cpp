#include "Structures.hpp"
#include "Types.hpp"

std::string num_to_str(int numero)
{
	if (numero < 10) 
		return "0" + std::to_string(numero); // Agrega un cero a la izquierda si el número tiene una cifra
	else
		return std::to_string(numero); // Devuelve el número como string si ya tiene dos cifras
}


namespace Tree
{
	int getRandomUniform(int n)
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