//#include "Simulator.hpp"

#include <iostream>
#include <cstdlib>

#include "Controller.hpp"

int main()
{
	//zpr::Simulator::Create(); // to chyba tyle jak na main, moze linijka wiecej

	zpr::Controller controller;

	std::cout << "\n\nSymulacja zakonczona. Wczytanie inta konczy program." << std::endl;
	int i;
	std::cin >> i; 
	return EXIT_SUCCESS;
}