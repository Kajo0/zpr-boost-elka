#ifndef LOGGER_HPP
#define LOGGER_HPP

/**
 * Singleton logger to write essential
 * information about simulation into file
 * or standard output
 */
class Logger {
	/** Message counter */
	mutable int count_;
	/** Output log stream */
	std::ostream* output_;
	/** Input Output control mutex */
	mutable boost::mutex ioMutex_;

public:
	static Logger& getInstance();
	void message(const char*) const;
	void message(const std::string&) const;

private:
	Logger();
	Logger(const Logger &);
	~Logger();
};

#endif