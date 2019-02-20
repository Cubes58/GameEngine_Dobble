#pragma once

#include <string>

#include <SFML/Network.hpp>

#include "PacketTypes.h"

class Client {
private:
	static const int s_m_PortNumber = 8787;	
	sf::IpAddress m_ServerIPAddress = "127.0.0.1";			
	sf::TcpSocket m_ServerSocket;			
	bool m_Connected;						

public:
	Client() = default;
	~Client();

	bool Connect(const sf::Time &p_ConnectWaitTimeOut = sf::Time::Zero);
	void Disconnect();

	bool Send(sf::Packet &p_Packet);
	bool ReceiveData(sf::Packet &p_Packet);

	bool IsConnected();
	void SetConnected(bool p_Connected);
};