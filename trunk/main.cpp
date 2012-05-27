#include <boost/filesystem.hpp>
#include <iostream>
#include <cstdlib>

#include "Controller.hpp"
#include "View.hpp"

int main(int argc, char **argv)
{
	boost::filesystem::path applicationPath(argv[0]);
	zpr::Controller controller(applicationPath.remove_filename());


	std::cout << "\n\nSymulacja zakonczona. Wczytanie inta konczy program." << std::endl;
	int i;
	std::cin >> i;
	return EXIT_SUCCESS;
}