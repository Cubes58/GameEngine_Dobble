#include "Server.h"

#include <climits>

#include "Logger.h"

unsigned int Server::s_m_NewClientID = 0;

Server::Server() {
	m_SocketSelector.clear();

	Log(MessageType::INFO) << "Dobble Server" << "\n" << "----------------" << "\n" << "Listening on port: " << s_m_PortToListenOn << "\n";
	// Listen on the selected port.
	if (m_Listener.listen(s_m_PortToListenOn) != sf::Socket::Done)
		Log(MessageType::FAULT) << "Couldn't listen on port: " << s_m_PortToListenOn;
	else {
		m_SocketSelector.add(m_Listener);
		m_Listening = true;
	}
}

Server::~Server() {
	Disconnect();
}

bool Server::CheckForClientConnectionRequest(const sf::Time &p_WaitTime) {
	if (m_SocketSelector.wait(p_WaitTime)) {
		if (m_SocketSelector.isReady(m_Listener)) {
			std::shared_ptr<sf::TcpSocket> client = std::make_shared<sf::TcpSocket>();
			if (m_Listener.accept(*client) == sf::Socket::Done) {
				sf::Packet connectionPacket;
				Packet::SetPacketType(Packet::CONNECT, connectionPacket);

				if (s_m_NewClientID >= UINT_MAX)
					s_m_NewClientID = (UINT_MAX - UINT_MAX);
				ClientID clientID = s_m_NewClientID++;

				// Acknowledge the connection.
				if (client->send(connectionPacket) == sf::Socket::Done) {
					m_Clients.insert(std::pair(clientID, client));
					m_SocketSelector.add(*client);

					Log(MessageType::INFO) << "Client connected. Remote address: " << client->getRemoteAddress().toString() << " Their ID is: " << clientID;
					return true;
				}
			}
			else {
				Log(MessageType::FAULT) << "Occured during client connection. Skipping.";
			}
		}
		else {
			// The listerner socket isn't ready, check all of the other sockets. (Maybe someone wants to disconnect.)
			for (auto client = m_Clients.begin(), nextClient = client; client != m_Clients.end(); client = nextClient) {
				++nextClient;
				if (m_SocketSelector.isReady(*client->second)) {
					// The client has sent some data, it can be received.
					sf::Packet packet;
					if (client->second->receive(packet) == sf::Socket::Done) {
						if (Packet::GetPacketType(packet) == Packet::CONNECT) {
							// Maybe have the user's specify specific info, (number of cards in the deck - Could be a voting thing - user's joke, when they click on a certain game mode, etc...)
						}
						else if (Packet::GetPacketType(packet) == Packet::DISCONNECT) {
							Disconnect(client->first);
						}
					}
				}
			}
		}
	}

	return false;
}

void Server::WaitForClientsToConnect(int p_NumberOfClients) {
	unsigned int numberOfClientsAdded(0);
	while (numberOfClientsAdded < p_NumberOfClients) {
		// Maximum time to wait, (use Time::Zero for infinity).
		if (CheckForClientConnectionRequest(sf::Time::Zero)) {
			++numberOfClientsAdded;
		}
	}
}

void Server::Disconnect(const ClientID &p_ClientID) {
	Log(MessageType::INFO) << "Disconnecting from a client! Client ID: " << p_ClientID;
	
	auto client = m_Clients.find(p_ClientID);
	if (client != m_Clients.end()) {
		sf::Packet disconnectPacket;
		Packet::SetPacketType(Packet::DISCONNECT, disconnectPacket);

		// Tell the client the server is disconnecting, from them.
		sf::Socket::Status status = client->second->send(disconnectPacket);

		// Order here is important, the client needs to be removed from the socket selector, before it's disconnected (strange behaviour occurs otherwise).
		// Remove it from the socket selector.
		m_SocketSelector.remove(*client->second);
		// Disconnect from the client.
		client->second->disconnect();

		// Check whether the socket it still in use (whether the client has actually disconnected).
		if (client->second->getRemoteAddress() == sf::IpAddress::None)
			Log(MessageType::INFO) << "Successfully disconnected from a client! Client ID: " << p_ClientID;

		// Remove it from the list of sockets.
		m_Clients.erase(client);
	}
	else 
		Log(MessageType::FAULT) << "Failed to disconnected from a client! Client ID: " << p_ClientID;
}

void Server::Disconnect() {
	// Disconnect from every client, stored.
	for (auto iter = m_Clients.begin(), nextIter = iter; iter != m_Clients.end(); iter = nextIter) {
		++nextIter;
		Disconnect(iter->first);
	}
}

void Server::Send(const ClientID &p_ClientID, sf::Packet &p_Packet) {
	auto client = m_Clients.find(p_ClientID);
	if (client != m_Clients.end()) {
		if (client->second->send(p_Packet) == sf::Socket::Done)
			Log(MessageType::INFO) << "Successfully sent data to a client! Client ID: " << p_ClientID;
		else 
			Log(MessageType::FAULT) << "Failed to send data to a client! Client ID: " << p_ClientID;
	}
}

void Server::Send(sf::Packet &p_Packet) {
	for (auto &client : m_Clients) {
		Send(client.first, p_Packet);
	}
}

bool Server::GetReceivedData(const ClientID &p_ClientID, sf::Packet &p_Packet) {
	auto client = m_Clients.find(p_ClientID);
	if (client != m_Clients.end() && m_SocketSelector.wait()) {
		if (m_SocketSelector.isReady(*client->second)) {
			if (client->second->receive(p_Packet) == sf::TcpSocket::Done) {
				Log(MessageType::INFO) << "Data received from a client! Client ID: " << client->first;
				return true;
			}
		}
	}

	Log(MessageType::INFO) << "No data received, from client! Client ID: " << p_ClientID;
	p_Packet.clear();
	return false;
}

bool Server::GetReceivedData(std::map<ClientID, sf::Packet> &p_Packets) {
	p_Packets.clear();

	bool dataRecieved = false;
	for (auto &client : m_Clients) {
		sf::Packet packet;
		if (client.second->receive(packet) == sf::TcpSocket::Done) {
			Log(MessageType::INFO) << "Data received from a client! Client ID: " << client.first;

			p_Packets.emplace(client.first, packet);
			dataRecieved = true;
		}
	}

	return dataRecieved;
}

bool Server::GetListeningState() const {
	return m_Listening;
}

void Server::SetListeningState(bool p_Listen) {
	m_Listening = p_Listen;

	if (p_Listen)
		m_Listener.listen(s_m_PortToListenOn);
	else
		m_Listener.close();
}

std::vector<ClientID> Server::GetClientIDs() {
	std::vector<ClientID> clientIDs;
	for (const auto &client : m_Clients)
		clientIDs.emplace_back(client.first);

	return clientIDs;
}

unsigned int Server::GetNumberOfClients() const {
	return m_Clients.size();
}

bool Server::IsRunning() {
	return m_Clients.size() > 0;
}