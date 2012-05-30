#include <boost/filesystem.hpp>
#include <iostream>
#include <cstdlib>

#include "Controller.hpp"
#include "View.hpp"
#include "Logger.hpp"

int main(int argc, char **argv)
{
	try
	{
		boost::filesystem::path applicationPath = boost::filesystem::path(__FILE__).remove_filename();
		zpr::Controller controller(applicationPath);
		controller.run();

		std::cout << "\n\nSymulacja zakonczona. Wczytanie inta konczy program." << std::endl;
	}
	catch(std::exception & e)
	{
		std::cout<<e.what()<<std::endl;
	}
	catch(...)
	{
		std::cout << "\n\nNieznany wyjatek symulacji. Program zakonczono gwaltownie." << std::endl;
	}

	int i;
	std::cin >> i;
	return EXIT_SUCCESS;
}