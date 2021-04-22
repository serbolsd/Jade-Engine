#include "jdFile.h"

namespace jdEngineSDK {
  jdFile::jdFile() {
    openFile("jdFile");
  }

  jdFile::jdFile(String fileName, 
                 FILETYPE::E typeOfOpen, 
                 FILEEXTENTION::E fileExtention, 
                 String fileDirection) {
    openFile(fileName, typeOfOpen, fileExtention, fileDirection);
  }

  jdFile::~jdFile() {
    closeFile();
  }

  bool
  jdFile::openFile(String fileName, 
                   FILETYPE::E typeOfOpen, 
                   FILEEXTENTION::E fileExtention, 
                   String fileDirection) {

    if (m_file) {
      m_file.close();
    }

    m_fileType = typeOfOpen;
    m_fileName = fileName;
    m_fileDirection = fileDirection;
    String completeDirection = fileDirection + "/" + fileName;

    std::ios::openmode openmode;

    if (fileExtention == FILEEXTENTION::BIN) {
      openmode = std::ios::binary;
    }

    switch (typeOfOpen) {
    case jdEngineSDK::FILETYPE::WRITE:
      openmode |= std::ios::out;
      break;
    case jdEngineSDK::FILETYPE::READ:
      openmode |= std::ios::in;
      break;
    case jdEngineSDK::FILETYPE::WRITE_READ:
      openmode |= std::ios::out;
      openmode |= std::ios::in;
      break;
    default:
      break;
    }
    
    m_file.open(completeDirection.c_str(), openmode);
    return false;
  }
  
  void jdFile::saveInFile(char* data) {
    if (m_fileType == FILETYPE::WRITE || m_fileType == FILETYPE::WRITE_READ) {
      m_file << data;
    }
  }

  void 
  jdFile::readFile(char* data) {
    if (m_fileType == FILETYPE::READ || m_fileType == FILETYPE::WRITE_READ) {
      m_file >> data;
    }
  }

  void 
  jdFile::closeFile() {
    m_file.close();
  }
}