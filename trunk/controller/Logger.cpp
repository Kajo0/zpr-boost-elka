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
	Logger::Logger(): count_(0), output_(&std::cout)
	{
		std::ofstream *out = new std::ofstream();
		
		boost::filesystem::path log_path = boost::filesystem::path(__FILE__).remove_filename().parent_path();
		log_path /= boost::lexical_cast<std::string>(time(0));
		log_path.replace_extension(".txt");

		out->open(log_path.string().c_str(), std::ios_base::app);

		if (out->is_open())
			output_ = out;
		else
			delete out;
	}

	Logger::Logger(const Logger &other) {}

	Logger::~Logger()
	{
		try
		{
			dynamic_cast<std::ofstream&>(*output_).close();
			delete output_;
		} catch(std::exception &) {} //was cout, do nothing
	}

	Logger& Logger::getInstance()
	{
		static Logger instance;
		return instance;
	}

	void Logger::message(const char *msg) const
	{
		boost::mutex::scoped_lock scoped_lock(ioMutex_);

		*output_<<" "<<count_++<<":\t"<<msg<<"\n";
	}

	void Logger::message(const std::string &msg) const
	{
		message(msg.c_str());
	}
}