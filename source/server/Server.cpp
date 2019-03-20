#include "Server.h"

#include <climits>

#include "Randomiser.h"
#include "Logger.h"

unsigned int Server::s_m_NewClientID = 0;

Server::Server() {
	m_SocketSelector.clear();

	Log(Type::INFO) << "Dobble Server" << "\n" << "----------------" << "\n" << "Listening on port: " << s_m_PortToListenOn << "\n";
	// Listen on the selected port.
	if (m_Listener.listen(s_m_PortToListenOn) != sf::Socket::Done)
		Log(Type::FAULT) << "Couldn't listen on port: " << s_m_PortToListenOn;
	else {
		m_SocketSelector.add(m_Listener);
		m_Listening = true;
	}

	m_GamePortNumber = RandomiserInstance.GetUniformIntegerRandomNumber(8000, 10000);
	if (m_LobbyListener.listen(m_GamePortNumber) != sf::Socket::Done)
		Log(Type::FAULT) << "Couldn't listen on port: " << m_GamePortNumber;
	else {
		m_SocketSelector.add(m_LobbyListener);
		Log(Type::INFO) << "The game will be played on port: " << m_GamePortNumber;
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
				sf::Packet connectionPacket = Packet::SetPacketType(Packet::CONNECT);
				connectionPacket << m_GamePortNumber;
				client->send(connectionPacket);
			}
			else {
				Log(Type::FAULT) << "A problem occured during client connection. Skipping.";
			}
		}
		else if (m_SocketSelector.isReady(m_LobbyListener)) {
			std::shared_ptr<sf::TcpSocket> client = std::make_shared<sf::TcpSocket>();
			if (m_LobbyListener.accept(*client) == sf::Socket::Done) {
				// Acknowledge the connection.
				if (s_m_NewClientID >= UINT_MAX) {
					s_m_NewClientID = (UINT_MAX - UINT_MAX);
				}
				m_Clients.insert(std::pair(++s_m_NewClientID, client));
				m_SocketSelector.add(*client);

				Log(Type::INFO) << "Client connected. Remote address: " << client->getRemoteAddress().toString() << " Their ID is: " << s_m_NewClientID;
				return true;
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
						if (Packet::GetPacketType(packet) == Packet::DISCONNECT) {
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
	Log(Type::INFO) << "Waiting for " << p_NumberOfClients << " new clients.";

	int numberOfClientsAdded(0);
	int numberOfClientsBeforeWaiting = m_Clients.size();
	while (numberOfClientsAdded < p_NumberOfClients) {
		// Maximum time to wait, (use Time::Zero for infinity).
		if (CheckForClientConnectionRequest(sf::Time::Zero)) {
			++numberOfClientsAdded;
			Log(Type::INFO) << "Number of clients added: " << numberOfClientsAdded;

			// Check if a client has disconnected, while they were waiting.
			if (m_Clients.size() < numberOfClientsBeforeWaiting + p_NumberOfClients) {
				numberOfClientsAdded = (numberOfClientsBeforeWaiting + p_NumberOfClients) - m_Clients.size();
				Log(Type::INFO) << "Number of clients still needed: " << numberOfClientsAdded;
			}
		}
	}
}

void Server::Disconnect(const ClientID &p_ClientID) {
	Log(Type::INFO) << "Disconnecting from a client! Client ID: " << p_ClientID;
	
	auto client = m_Clients.find(p_ClientID);
	if (client != m_Clients.end()) {
		sf::Packet disconnectPacket;
		Packet::SetPacketType(Packet::DISCONNECT, disconnectPacket);

		// Tell the client the server is disconnecting, from them.
		client->second->send(disconnectPacket);

		// Order here is important, the client needs to be removed from the socket selector, before it's disconnected (strange behaviour occurs otherwise).
		// Remove it from the socket selector.
		m_SocketSelector.remove(*client->second);
		// Disconnect from the client.
		client->second->disconnect();

		// Check whether the socket it still in use (whether the client has actually disconnected).
		if (client->second->getRemoteAddress() == sf::IpAddress::None)
			Log(Type::INFO) << "Successfully disconnected from a client! Client ID: " << p_ClientID;

		// Remove it from the list of sockets.
		m_Clients.erase(client);
	}
	else 
		Log(Type::FAULT) << "Failed to disconnected from a client! Client ID: " << p_ClientID;
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
			Log(Type::INFO) << "Successfully sent data to a client! Client ID: " << p_ClientID;
		else 
			Log(Type::FAULT) << "Failed to send data to a client! Client ID: " << p_ClientID;
	}
}

void Server::Send(sf::Packet &p_Packet) {
	for (auto &client : m_Clients) {
		Send(client.first, p_Packet);
	}
}

bool Server::GetReceivedData(const ClientID &p_ClientID, sf::Packet &p_Packet) {
	auto client = m_Clients.find(p_ClientID);
	if (client != m_Clients.end() && m_SocketSelector.wait(sf::microseconds(1000))) {
		if (m_SocketSelector.isReady(*client->second)) {
			if (client->second->receive(p_Packet) == sf::TcpSocket::Done) {
				Log(Type::INFO) << "Data received from a client! Client ID: " << client->first;
				return true;
			}
		}
	}

	Log(Type::INFO) << "No data received, from client! Client ID: " << p_ClientID;
	p_Packet.clear();
	return false;
}

bool Server::GetReceivedData(std::map<ClientID, sf::Packet> &p_Packets) {
	p_Packets.clear();

	bool dataRecieved = false;
	if (m_SocketSelector.wait(sf::microseconds(1000))) {
		for (auto &client : m_Clients) {
			sf::Packet packet;
			if (m_SocketSelector.isReady(*client.second)) {
				if (client.second->receive(packet) == sf::TcpSocket::Done) {
					Log(Type::INFO) << "Data received from a client! Client ID: " << client.first;

					p_Packets.emplace(client.first, packet);
					dataRecieved = true;
				}
			}
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
	else {
		m_Listener.listen(sf::TcpListener::AnyPort);
	}
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