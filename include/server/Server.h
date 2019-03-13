#pragma once

#include <map>
#include <string>
#include <memory>

#include <SFML\Network.hpp>

#include "PacketTypes.h"

using ClientID = unsigned int;

class Server {
private:
	static const unsigned int s_m_PortToListenOn = 8787;
	static const unsigned int s_m_DefaultListeningCheckTime = 5;
	static unsigned int s_m_NewClientID;

	std::map<ClientID, std::shared_ptr<sf::TcpSocket>> m_Clients;
	sf::SocketSelector m_SocketSelector;
	sf::TcpListener m_Listener;
	sf::TcpListener m_LobbyListener;
	sf::Int32 m_GamePortNumber = -1;
	bool m_Listening;

public:
	Server();
	~Server();

	bool CheckForClientConnectionRequest(const sf::Time &p_WaitTime = sf::microseconds(s_m_DefaultListeningCheckTime));
	void WaitForClientsToConnect(int p_NumberOfClients = 1);
	void Disconnect(const ClientID &p_ClientID);
	void Disconnect();

	void Send(const ClientID &p_ClientID, sf::Packet &p_Packet);
	void Send(sf::Packet &p_Packet);	
	bool GetReceivedData(const ClientID &p_ClientID, sf::Packet &p_Packet);	
	bool GetReceivedData(std::map<ClientID, sf::Packet> &p_Packets);	

	bool GetListeningState() const;
	void SetListeningState(bool p_Listen);

	std::vector<ClientID> GetClientIDs();
	unsigned int GetNumberOfClients() const;

	bool IsRunning();
};