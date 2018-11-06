/**
* @Author	Andrius Murauskas
* @date	16th March 2017
* @brief	Singleton class for logging 
*/
#include "Log.h"

#include "Utils.h"


const string Logger::m_sFileName = "Log " + Utils::CurrentDay() + ".txt";

Logger* Logger::m_pThis = NULL;
ofstream Logger::m_Logfile;

Logger::Logger() {};
Logger* Logger::GetLogger()
{
	if (m_pThis == NULL)
	{
		m_pThis = new Logger();
		m_Logfile.open(m_sFileName.c_str(), ios::out | ios::app);
	}
	return m_pThis;
}

void Logger::Log(const char *format, ...)
{
	char * sMessage = NULL;
	int nLength = 0;
	va_list  args;
	va_start(args, format);
	nLength = _vscprintf(format, args) + 1;
	sMessage = new char[nLength];
	vsprintf_s(sMessage, nLength, format, args);

	m_Logfile << Utils::CurrentTime() << ":\t";

	m_Logfile << sMessage << "\n";
	va_end(args);

	delete[] sMessage;
}

void Logger::Log(const string& sMessage)
{
	m_Logfile << Utils::CurrentTime() << ":\t";

	m_Logfile << sMessage << "\n";
}
Logger& Logger::operator<<(const string& sMessage)
{

	m_Logfile << "\n" << Utils::CurrentTime() << ":\t";

	m_Logfile << sMessage << "\n";
	return *this;
}
