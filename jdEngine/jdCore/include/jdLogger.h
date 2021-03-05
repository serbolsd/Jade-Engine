/*****************************************************************************/
/**
 * @file    jdLogger.h
 * @author  Sergio Juan Diaz Carmona
 * @date    23/01/2021
 * @brief   class for logger data. 
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once

#include "jdPrerequisitesCore.h"

#include <fstream>
#include <jdModulo.h>
#include <jdTime.h>

namespace jdEngineSDK {
		class JD_CORE_EXPORT Logger : public Module<Logger>
		{
   public:
    /**
    *  @brief  Default constructor for the Logger class.
    */
    Logger() {
      m_Logfile.open(m_sFileName.c_str(), std::ios::out | std::ios::app);
    };

    /**
    *  @brief copy constructor for the Logger class.
    */
    Logger(const Logger&) {};

    ~Logger() {
      m_Logfile.close();
    }

    /**
    * @brief Logs a message
    * @param sMessage message to be logged.
    */
    void Log(const std::string& sMessage);

    /**
    * @brief Variable Length Logger function
    * @param format string for the message to be logged.
    */
    void Log(const char* format, ...);

    /**
    * @brief << overloaded function to Logs a message
    * @param sMessage message to be logged.
    */
    Logger& operator<<(const String& sMessage);

    /**
    * @brief assignment operator for the Logger class.
    */
    Logger& operator=(const Logger&) { return *this; };

   private:
    /**
    * @brief Log file name.
    **/
    String m_sFileName = "Log.txt";

    /**
    *   Singleton logger class object pointer.
    **/
    Logger* m_pThis = nullptr;

    /**
    *   Log file stream object.
    **/
    std::ofstream m_Logfile;
		};

  JD_CORE_EXPORT Logger&
  g_Logger();
}