#include <boost/filesystem.hpp>
#include <iostream>
#include <cstdlib>

#include "Controller.hpp"

int main(int, char **)
{
	try
	{
		boost::filesystem::path applicationPath = boost::filesystem::path(__FILE__).remove_filename();
		zpr::Controller controller(applicationPath);
		controller.run();
	}
	catch(std::exception & e)
	{
		std::cout << "Wyjatek: " << e.what() << std::endl;
	}
	catch(...)
	{
		std::cout << "\n\nNieznany wyjatek symulacji. Program zakonczono gwaltownie." << std::endl;
	}

	std::cout << "\n\nWczytanie inta konczy program." << std::endl;
	int i;
	std::cin >> i;
	return EXIT_SUCCESS;
}