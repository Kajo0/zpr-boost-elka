#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <boost\lexical_cast.hpp>
#include <boost\thread\mutex.hpp>
#include "Logger.hpp"

/**
 * Private constructor to make only one
 * instance of logger - Singleton
 *
 * Creates file named as ${actual timestamp}.txt
 * if succeeded write log there, otherwise write onto std::cout
 */
Logger::Logger(): count_(0), output_(&std::cout) {

	std::ofstream *out = new std::ofstream();

	//TODO zmienic na wzgledna ale ja lubie miec wsio
	//		na pulpicie jak tego uzywam czesto :P
	std::string path = "C:\\Users\\Acer\\Desktop\\";
	path += boost::lexical_cast<std::string>(time(0));
	path += ".txt";

	out->open(path, std::ios_base::app);

	if (out->is_open())
		output_ = out;
	else
		delete out;
	// TODO a jak isopen == false to co dalej z logerem?
}

Logger::Logger(const Logger &) {}

/**
 * Close file stream and delete handle
 */
Logger::~Logger() {
	try {
		dynamic_cast<std::ofstream&>(*output_).close();
		delete output_;

	} catch(std::exception &) {
		//was cout, do nothing
	}
}

/**
 * @return Logger instance
 */
Logger& Logger::getInstance() {
	static Logger instance;
	return instance;
}

/**
 * Logs message into file or if IOException
 * (in constructor) onto user stdout
 *
 * @param msg message to log
 */
void Logger::message(const char* msg) const {
	ioMutex_.lock();

	*output_<<" "<<count_++<<":\t"<<msg<<"\n";
	
	ioMutex_.unlock();
}

/**
 * Overloaded, do the same as other message
 *
 * @param msg message to log
 */
void Logger::message(const std::string &msg) const {
	message(msg.c_str());
}