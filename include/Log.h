#ifndef ENGINE_Logger_H
#define ENGINE_Logger_H
#include <fstream>
#include <iostream>
#include <cstdarg>
#include <string>

using namespace std;
#define LOGGER Logger::GetLogger()
/*
Singleton class - logger
*/
 class Logger
{
public:

	void Log(const std::string& myString);
	void Log(const char * format, ...);
	Logger& operator<<(const string& myString);
	static Logger* GetLogger();
private:
	/**
	Default constructor for the Logger class
	*/
	Logger();
	/**
	copy constructor
	*/
	Logger(const Logger&) {};

	Logger& operator= (const Logger&){return*this;};
	/**
	Log file name
	*/
	static const std::string m_sFileName;
	/*
	Singleton logger class object pointer
	*/
	static Logger* m_pThis;
	/*
	Log file stream object
	*/
	static ofstream m_Logfile;

};
#endif 
