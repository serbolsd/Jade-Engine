#pragma once
#include "jdRTTIPrerequisites.h"

namespace jdEngineSDK {

		namespace FILEEXTENTION { 
				enum E	{
						BIN = 0,
						TXT,
						MAX_NUM_TYPE
				};
		}

		namespace FILETYPE {
				enum E {
						WRITE = 0,
						READ,
						WRITE_READ,
						MAX_NUM_TYPE
				};
		}

		class JD_UTILITY_EXPORT jdFile	{
		public:
				jdFile();
				jdFile(String fileName, 
						     FILETYPE::E typeOfOpen = FILETYPE::WRITE_READ,
						     FILEEXTENTION::E fileExtention = FILEEXTENTION::BIN,
						     String fileDirection = "");
				~jdFile();

				bool
				openFile(String fileName, 
						       FILETYPE::E typeOfOpen = FILETYPE::WRITE_READ,
       						FILEEXTENTION::E fileExtention = FILEEXTENTION::BIN,
						       String fileDirection = "");

				void
				saveInFile(char* data);

				void
				readFile(char* data);

				void
				closeFile();
		private:
				FILEEXTENTION::E m_fileExtention;
				FILETYPE::E m_fileType = FILETYPE::WRITE_READ;
				String m_fileName;
				String m_fileDirection;
				std::fstream m_file;
		};
}