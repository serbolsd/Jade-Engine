#include "jdLogger.h"


namespace jdEngineSDK {
		void 
		Logger::Log(const std::string& sMessage)	{
				std::cout << sMessage << std::endl;
				m_Logfile << "\n" << Time::CurrentDateTime() << ":\t";
				m_Logfile << sMessage << "\n";
		}

		void
  Logger::Log(const char* format, ...) {
				char* sMessage = NULL;
				int nLength = 0;
				va_list args;
				va_start(args, format);
				//  Return the number of characters in the string referenced the list of arguments.
				// _vscprintf doesn't count terminating '\0' (that's why +1)
				nLength = _vscprintf(format, args) + 1;
				sMessage = new char[nLength];
				vsprintf_s(sMessage, nLength, format, args);
				//vsprintf(sMessage, format, args);
				//ADD TIME HERE
				std::cout << sMessage << std::endl;
				m_Logfile << "\n" << Time::CurrentDateTime() << ":\t";
				m_Logfile << sMessage << "\n";
				va_end(args);

				delete[] sMessage;
  }

		Logger& 
		Logger::operator<<(const String& sMessage)	{
				//ADD TIME HERE
				m_Logfile << "\n" << g_time().CurrentDateTime() << ":\t";
				m_Logfile << sMessage << "\n";
				return *this;
		}

		JD_CORE_EXPORT 
		Logger& g_Logger()	{
				return Logger::instance();
		}
}