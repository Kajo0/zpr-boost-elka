#include <iostream>
#include <cstdlib>

#include "Controller.hpp"
#include "View.hpp"

int main()
{

	zpr::Controller controller;

	zpr::View *view = new zpr::View();	// taki tescik
	view->loop();
	delete view;	// zeby x wylazcal okienko tu ;)

	std::cout << "\n\nSymulacja zakonczona. Wczytanie inta konczy program." << std::endl;
	int i;
	std::cin >> i;
	return EXIT_SUCCESS;
}