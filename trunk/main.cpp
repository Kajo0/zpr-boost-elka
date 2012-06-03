//#define TESTING

//#include "Test.hpp"
#include "Controller.hpp"

#include <boost/filesystem.hpp>
#include <iostream>
#include <cstdlib>

//#include <boost/test/minimal.hpp>
//#include <boost/test/minimal.hpp>

//#ifdef TESTING
//#include <boost/test/minimal.hpp>
//int test_main(int argc, char** const)
//#else
int main(int, char **)
//#endif 
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

	return EXIT_SUCCESS;
}