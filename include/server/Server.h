#pragma once

#include <map>
#include <functional>
#include <string>
#include <memory>

#include <SFML\Network.hpp>

#include "PacketTypes.h"

using ClientID = unsigned int;

static constexpr unsigned int s_NUMBER_OF_PLAYERS_PER_GAME = 2;

class Server {
private:
	static const int s_m_PortToListenOn = 8787;
	std::map<ClientID, std::shared_ptr<sf::TcpSocket>> m_Clients;
	sf::SocketSelector m_SocketSelector;
	sf::TcpListener m_Listener;
	std::hash<std::string> m_StringHasher;

public:
	Server();
	~Server();

	void Connect();	
	void Disconnect(const ClientID &p_ClientID);

	void Send(const ClientID &p_ClientID, sf::Packet &p_Packet);
	void Send(sf::Packet &p_Packet);	
	bool GetReceivedData(const ClientID &p_ClientID, sf::Packet &p_Packet);	
	bool GetReceivedData(std::map<ClientID&, sf::Packet&> &p_Packets);	

	bool IsRunning();	
};