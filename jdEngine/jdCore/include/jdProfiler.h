/*****************************************************************************/
/**
 * @file    jdProfiler.h
 * @author  Sergio Juan Diaz Carmona
 * @date    27/01/2021
 * @brief   class for profiler data.
 * @note    All the time is in seconds
 *
 *
 * @bug     No known bugs.
 */
 /***************************************************************************/
#pragma once

#include "jdPrerequisitesCore.h"

#include <fstream>
#include <jdModulo.h>

using std::chrono::duration_cast;

namespace jdEngineSDK {
  class JD_CORE_EXPORT Profiler : public Module<Profiler> {

   public:
    /**
     * @brief default constructor
     */
    Profiler();

    /**
     * @brief constructor
     * @param seeInConsole is a bool to active writting in console
     */
    Profiler(bool seeInConsole);

    /**
     * @brief Function to start the counting
     */
    void 
    start();
    
    /**
     * @brief Function to stop and start again the counting
     */
    void 
    toc();

    /**
     * @brief Function to stop and start again the counting and write a message
     * @param message is the message to write
     */
    void 
    toc(std::string message);

    /**
     * @brief Function to stop the counting
     */
    void 
    stop();

    /**
     * @brief Function to stop the counting and write a message
     * @param message is the message to write
     */
    void 
    stop(std::string message);

    /**
     * @brief Function to write the profiler file in the temp profiler file of vcs.
     */
    void
    dump();

   private:
     /**
      * @brief is the var to save when start the couting
      */
    std::chrono::steady_clock::time_point m_begin;

    /**
     * @brief is the condition to write in console or not
     */
    bool m_seeInConsole = false;

    /**
     * @brief is the string to save the data
     */
    std::stringstream m_logfile;

    /**
    * @brief Log file name.
    **/
    String m_sFileName = "Prof.txt";

    /**
    *   Log file stream object.
    **/
    std::ofstream m_Logfile;
  };

  JD_CORE_EXPORT Profiler&
  g_Profiler();
}
