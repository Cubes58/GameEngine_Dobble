/**
@file Logger.h
@brief Manages outputting debugging information to the console and/or files.
*/
#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>
#include <assert.h>

#include "FileSystemHelper.h"

#define ASSERT(x) if (!(x)) __debugbreak();	//!< Simple Assert function checker.
#define LOG_FILES_FOLDER "./resources/logFiles"	//!< Relative path to the log files folder, so they can be loaded in.
#define DEFAULT_LOG_FILE  "logFile"	//!< The default log file.

/**
	@file Logger.h
	@enum Type
	@brief The different types of logging information.
*/

enum class Type : unsigned char {
	INFO = 0,	//!< Outputting information.	
	WARNING,	//!< Outputting a warning.
	FAULT	//!< Outputting an error.
};

/*! \class Log
	\brief Manages outputting debugging information to the console and/or files.
*/

class Log {
private:
	std::fstream m_FileStream;	//!< The fileStream object to output information to a file.
	static std::vector<std::pair<std::string, std::string>> m_FileInformation;	//!< A vector of file names and their relative paths.
	Type m_MessageType = Type::INFO;	//!< The default message type, to output.

	bool m_FileOpened = false;	//!< Manages if a file has been opened.
	bool m_ShowMessageType = true;	//!< Manages whether debugging information outputted should display their message types.
	bool m_PrintToOutputFile = true;	//!< Manages whether the information should be outputted to a file.
	bool m_PrintToConsole = true;	//!< Manages whether the information should be outputted to a console.

	/*!
		\brief Get the name of a type.
		\param p_Type the type to get the name for.
		\return Returns the name of the type.
	*/
	std::string GetLabel(const Type &p_Type) {
		std::string label("");

		switch (p_Type) {
		case Type::INFO:
			label = "INFO";
			break;
		case Type::WARNING:
			label = "WARNING";
			break;
		case Type::FAULT:
			label = "FAULT(ERROR)";
			break;
		default:
			label = "UNKNOWN";
			break;
		}
		return label;
	}

	/*!
		\brief Gets a logging file's relative path, from its name.
		\param p_FileName The name of the file to get.
		\return Returns the relative path to the file, requested, if found, otherwise it returns empty.
	*/
	static std::string GetFileFromName(const std::string &p_FileName) {
		std::string fileLocation = "";
		for (auto &file : m_FileInformation) {
			if (file.second == p_FileName) {
				fileLocation = file.first;
				break;
			}
		}
		return fileLocation;
	}

public:
	/*!
		\brief Constructor.
	*/
	Log() = default;
	/*!
		\brief Constructor.
		\param p_Type The type of the message being outputted.
		\param p_FileName The name of the file to ouput the data to.
	*/
	Log(const Type &p_Type, const std::string &p_FileName = DEFAULT_LOG_FILE) {
		m_MessageType = p_Type;

		if (m_PrintToOutputFile) {
			std::string fileLocation = GetFileFromName(p_FileName);
			m_FileStream.open(fileLocation, std::ios_base::app);
		}
		if (m_ShowMessageType)
			operator<< ("[" + GetLabel(p_Type) + "]: ");
	}
	/*!
		\brief Destructor.
	*/
	~Log() {
		if (m_FileOpened) {
			if (m_PrintToConsole) {
				std::cout << std::endl;
			}
			if (m_PrintToOutputFile) {
				m_FileStream << "\n";
				m_FileStream.close();
			}
		}
		m_FileOpened = false;
	}

	/*!
		\brief Initializes the logger, by loading all the logging files, from a directory.
		\param p_FolderLocation The folder location of the logging files.
		\return Nothing.
	*/
	static void Initialise(const std::string &p_FolderLocation = LOG_FILES_FOLDER) {
		std::vector<FileInformation> files = FileSystemHelper::GetFilesInFolder(p_FolderLocation);

		for (const auto &file : files) {
			m_FileInformation.emplace_back(std::make_pair(file.m_Location, file.m_Name));
		}
	}

	/*!
		\brief Clears all of the logged data from the file passed in.
		\param p_FileName The file's name that will be cleared. Use "ALL" if every file loaded in should be cleared.
		\return
	*/
	static void Flush(const std::string &p_FileName) {
		for (auto &file : m_FileInformation) {
			if (file.second == p_FileName) {
				std::fstream fileStream(file.first, std::ofstream::out | std::ofstream::trunc);
				fileStream.close();

				break;
			}
			else if (p_FileName == "ALL") {
				std::fstream fileStream(file.first, std::ofstream::out | std::ofstream::trunc);
				fileStream.close();
			}
		}
	}

	/*!
		\brief Outputs messages.
		\tparam T the type of message being outputted.
		\param p_Message The message being outputted.
		\return Returns itself, so more messages can be outputted, from the same object.
	*/
	template<typename T>
	Log &operator<<(const T &p_Message) {
		if (m_PrintToConsole)
			std::cout << p_Message;

		m_FileOpened = true;
		if (m_PrintToOutputFile)
			m_FileStream << p_Message;

		return *this;
	}
};