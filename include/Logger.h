#pragma once

#include <iostream>
#include <fstream>
#include <string>

#define ASSERT(x) if (!(x)) __debugbreak();
// std::string function, std::string file, unsigned int line.
// MACROS: #x (turns the x function into a string), __FILE__ Get the file, __LINE__ to get the line.

enum class Type : unsigned char {
	DEBUG,
	INFO,
	WARNING,
	FAULT
};

struct Configure {
	Type m_Level = Type::INFO;
	bool m_HeadersEnabled = true;
	bool m_PrintToOutputFile = true;
	bool m_PrintToConsole = true;
};

class Log {
private:
	bool m_Opened = false;
	Type m_MessageLevel = Type::DEBUG;
	Configure m_ConfigureFile;
	std::fstream m_LogFile;

	std::string GetLabel(const Type &p_Type) {
		std::string label("");

		switch (p_Type) {
		case Type::DEBUG:
			label = "DEBUG";
			break;
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
public:
	Log() {

	}

	Log(const Type &p_Type, const std::string &p_FileName = "logfile.txt") {
		m_MessageLevel = p_Type;

#ifdef _DEBUG
		m_ConfigureFile.m_Level = Type::DEBUG;
#endif

		if (m_ConfigureFile.m_PrintToOutputFile)
			m_LogFile.open(p_FileName, std::ios_base::app);
		if (m_ConfigureFile.m_HeadersEnabled)
			operator<< ("[" + GetLabel(p_Type) + "] ");
	}

	~Log() {
		if (m_Opened) {
			if (m_ConfigureFile.m_PrintToConsole)
				std::cout << std::endl;
			if (m_ConfigureFile.m_PrintToOutputFile) {
				m_LogFile << "\n";
				m_LogFile.close();
			}
		}

		m_Opened = false;
	}

	static void Clear() {
		std::fstream logFile("logfile.txt", std::ios::out);
		logFile.close();
	}

	template<typename T>
	Log& operator<<(const T &p_Message) {
		if (m_MessageLevel >= m_ConfigureFile.m_Level) {
			if (m_ConfigureFile.m_PrintToConsole)
				std::cout << p_Message;

			m_Opened = true;
			if (m_ConfigureFile.m_PrintToOutputFile)
				m_LogFile << p_Message;
		}

		return *this;
	}
};