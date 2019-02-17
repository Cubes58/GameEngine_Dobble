#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>
#include <assert.h>

#include "FileSystemHelper.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define LOG_FILES_FOLDER "./resources/logFiles"
#define DEFAULT_LOG_FILE  "logFile"

enum class MessageType : unsigned char {
	INFO = 0,
	WARNING,
	FAULT
};

class Log {
private:
	std::fstream m_FileStream;
	static std::vector<std::pair<std::string, std::string>> m_FileInformation;

	bool m_FileOpened = false;
	bool m_ShowMessageType = true;
	bool m_PrintToOutputFile = true;
	bool m_PrintToConsole = true;
	MessageType m_MessageType = MessageType::INFO;

	std::string GetLabel(const MessageType &p_Type) {
		std::string label("");

		switch (p_Type) {
		case MessageType::INFO:
			label = "INFO";
			break;
		case MessageType::WARNING:
			label = "WARNING";
			break;
		case MessageType::FAULT:
			label = "FAULT(ERROR)";
			break;
		default:
			label = "UNKNOWN";
			break;
		}
		return label;
	}

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
	Log() {

	}

	Log(const MessageType &p_Type, const std::string &p_FileName = DEFAULT_LOG_FILE) {
		m_MessageType = p_Type;

		if (m_PrintToOutputFile) {
			std::string fileLocation = GetFileFromName(p_FileName);
			m_FileStream.open(fileLocation, std::ios_base::app);
		}
		if (m_ShowMessageType)
			operator<< ("[" + GetLabel(p_Type) + "]: ");
	}

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

	static void Initialise(const std::string &p_FolderLocation = LOG_FILES_FOLDER) {
		std::vector<FileInformation> files = FileSystemHelper::GetFilesInFolder(p_FolderLocation);

		for (const auto &file : files) {
			m_FileInformation.emplace_back(std::make_pair(file.m_Location, file.m_Name));
		}
	}

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