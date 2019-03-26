#include "Client.h"

#include <fstream>
#include <json/json.h>

#include "Logger.h"

Client::Client() {
	LoadServerInformation("resources/configuration/serverIPAddress.json");
}

Client::~Client() {
	if (IsConnected())
		Disconnect();
}

bool Client::Connect(const sf::Time &p_ConnectWaitTimeOut) {
	Log(Type::INFO) << "Attempting to connect to the server... " << "\nPort number: " << m_PortNumber << "\nIP Address: " << m_ServerIPAddress;
	if (m_ServerSocket.connect(m_ServerIPAddress, m_PortNumber, p_ConnectWaitTimeOut) != sf::Socket::Done) {
		Log(Type::FAULT) << "The server wasn't listening!";
		m_Connected = false;
		return false;
	}
	Log(Type::INFO) << "Connected to the server listener!";
	
	sf::Packet packet;
	if (ReceiveData(packet)) {
		if (Packet::GetPacketType(packet) == Packet::CONNECT) {
			// Get the new port number.
			packet >> m_PlayPortNumber;

			m_ServerSocket.disconnect();
			// Connect to the server, on this port.
			if (m_ServerSocket.connect(m_ServerIPAddress, m_PlayPortNumber, sf::Time::Zero) != sf::Socket::Done) {
				Log(Type::FAULT) << "Couldn't connect to the new server port, to play the game. Port number: " << m_PlayPortNumber;
				return false;
			}
			else {
				m_Connected = true;
				Log(Type::INFO) << "Connected to a game on the server (in lobby)!";
			}
		}
	}
	m_ServerSocket.setBlocking(false);	// Ensure the methods return immediately.

	return true;
}

void Client::Disconnect() {
	if (!m_Connected)
		return;

	Log(Type::INFO) << "Disconnecting, from the server.";
	sf::Packet packet;
	Packet::SetPacketType(Packet::PacketType::DISCONNECT, packet);
	if (m_ServerSocket.send(packet) == sf::TcpSocket::Done)
		Log(Type::INFO) << "The server has been notified!";

	m_Connected = false;
	m_ServerSocket.disconnect();

	Log(Type::INFO) << "Successfully disconnected, from the server.";
}

bool Client::Send(sf::Packet &p_Packet) {
	if (!m_Connected || m_ServerSocket.send(p_Packet) != sf::Socket::Done) {
		Log(Type::FAULT) << "Couldn't send packet, to the server." << "\n" << "Connection state: " << m_Connected;
		return false;
	}

	Log(Type::INFO) << "Packet being sent, to the server.";
	return true;
}

bool Client::ReceiveData(sf::Packet &p_Packet) {
	// Listen for data, from the server.
	if (m_ServerSocket.receive(p_Packet) != sf::TcpSocket::Done) {
		p_Packet.clear();
		return false; // No data to process, move on.
	}

	Log(Type::INFO) << "Packet being received, from the server.";
	return true;
}

bool Client::IsConnected() {
	return m_Connected;
}

void Client::SetConnected(bool p_Connected) {
	m_Connected = p_Connected;
}

bool Client::LoadServerInformation(const std::string &p_File) {
	// Load server information from a JSON file.
	std::fstream jsonData;
	Json::Value root;
	Json::Reader reader;

	try {
		jsonData.open(p_File.c_str());
		// Check for errors, when parsing the file data.
		if (!reader.parse(jsonData, root)) {
			Log(Type::FAULT) << "Failed to parse data from: " << p_File << reader.getFormattedErrorMessages() << "\n";
			throw std::invalid_argument("Couldn't parse file.");
		}

		if (jsonData.is_open())
			jsonData.close();
	}
	catch (const std::invalid_argument &p_Exception) {
		Log(Type::FAULT) << "INVALID_ARGUMENT: " << p_Exception.what() << "\n";
		if (jsonData.is_open())
			jsonData.close();
		return false;
	}
	catch (...) {
		// Catch all rune-time errors, and attempt to recover.
		Log(Type::FAULT) << "Something unexpected happened, while reading the server configuration file: " << p_File;
		if (jsonData.is_open())
			jsonData.close();
		return false;
	}
	const Json::Value serverInformationObjects = root["ServerInformation"];

	for (unsigned int i = 0; i < serverInformationObjects.size(); i++) {
		// Get the server's port number.
		const Json::Value portNode = serverInformationObjects[i]["portNumber"];
		if (portNode.type() != Json::nullValue) {
			int portNumber = portNode.asUInt();
			if (portNumber > 0 && portNumber < 65535)
				m_PortNumber = portNumber;
			else
				Log(Type::FAULT) << "Invalid port number: " << portNumber;
		}
		else
			Log(Type::WARNING) << "Couldn't find the port number, using the default." << m_PortNumber;

		const Json::Value ipAddressNode = serverInformationObjects[i]["serverIPAddress"];
		if (ipAddressNode.type() != Json::nullValue) {
			m_ServerIPAddress = ipAddressNode.asString();

			if (m_ServerIPAddress.toString() == "0.0.0.0") {
				Log(Type::FAULT) << "There was an issue reading in the server's IP address, from the file: " << p_File;
				Log(Type::FAULT) << "The IP address will be set to 127.0.0.1";
				m_ServerIPAddress = "127.0.0.1";
			}
		}
		else
			Log(Type::WARNING) << "Couldn't find an IP Address, for the server, using the default: " << m_ServerIPAddress;
	}

	return true;
}