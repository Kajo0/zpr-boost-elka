#include "Simulator.hpp"

#include <iostream>
#include <cstdlib>

int main()
{
	zpr::Simulator::Create(); // to chyba tyle jak na main, moze linijka wiecej

	std::cout << "Symulacja zakonczona. Wczytanie inta konczy program." << std::endl;
	int i;
	std::cin >> i; 
	return EXIT_SUCCESS;
}