#pragma once

#include <string>

#include <SFML/Network.hpp>

#include "PacketTypes.h"

using PlayerIdentifier = char;

class Client {
private:
	static const int m_PortNumber = 8787;	
	sf::IpAddress m_ServerIPAddress = "127.0.0.1";			
	sf::TcpSocket m_ServerSocket;			
	bool m_Connected;						

public:
	Client();
	~Client();

	bool Connect();
	void Disconnect();

	bool Send(sf::Packet &p_Packet);
	bool ReceiveData(sf::Packet &p_Packet);
	bool GetConnectionState();
};