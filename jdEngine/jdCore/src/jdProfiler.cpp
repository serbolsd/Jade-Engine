#include "jdProfiler.h"

namespace jdEngineSDK {

  Profiler::Profiler() {
    m_seeInConsole = true;
    this->start();
  }

  Profiler::Profiler(bool seeInConsole) {
    m_seeInConsole = seeInConsole;
    this->start();
  }

  void Profiler::start() {
    m_begin = std::chrono::steady_clock::now();
  }

  void Profiler::stop(std::string s) {

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    if (m_seeInConsole) {
      std::cout << s.c_str() << 
                   " Time elapsed = " << 
                   duration_cast<std::chrono::microseconds>(end - m_begin).count() / 1e6f << 
                   std::endl;
    }

    m_logfile << s << "," << 
                 duration_cast<std::chrono::microseconds>(end - m_begin).count() / 1e6f << 
                 std::endl;
  }

  void Profiler::stop() {
    this->stop("");
  }

  void Profiler::toc() {
    this->toc("");
  }

  void Profiler::toc(std::string s) {
    this->stop(s);
    this->start();
  }

  void Profiler::dump() {
    std::ofstream myfile;
    m_Logfile.open(m_sFileName.c_str(), std::ios::out | std::ios::app);
    m_Logfile << m_logfile.rdbuf();
    m_Logfile.close();
    myfile.open("/tmp/Profiler.csv");
    myfile << m_logfile.rdbuf();
    myfile.close();
  }

  JD_CORE_EXPORT Profiler& g_Profiler() {
    return Profiler::instance();
  }
}