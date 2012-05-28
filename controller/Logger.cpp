#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/filesystem.hpp>
#include "Logger.hpp"

namespace zpr
{
	/**
	 * Private constructor to make only one
	 * instance of logger - Singleton
	 *
	 * Creates file named as ${actual timestamp}.txt
	 * if succeeded write log there, otherwise write onto std::cout
	 */
	Logger::Logger(): count_(0), output_(&std::cout) {

		std::ofstream *out = new std::ofstream();
		
		boost::filesystem::path log_path("C:/Users/Acer/Desktop");//__FILE__);
		log_path /= boost::lexical_cast<std::string>(time(0));
		log_path.replace_extension(".txt");

		out->open(log_path.c_str(), std::ios_base::app);

		if (out->is_open())
			output_ = out;
		else
			delete out;
	}

	Logger::Logger(const Logger &other) {}

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
	void Logger::message(const char *msg) const {
		boost::mutex::scoped_lock scoped_lock(ioMutex_);

		*output_<<" "<<count_++<<":\t"<<msg<<"\n";
	}

	/**
	 * Overloaded, do the same as other message
	 *
	 * @param msg message to log
	 */
	void Logger::message(const std::string &msg) const {
		message(msg.c_str());
	}
}