#include "Server.h"

#include "Logger.h"

using namespace Packet;

Server::Server() {
	m_SocketSelector.clear();

	Log(MessageType::INFO) << "Dobble Server" << "\n" << "----------------" << "\n" << "Listening on port: " << s_m_PortToListenOn << "\n";
	// Listen on the selected port
	if (m_Listener.listen(s_m_PortToListenOn) != sf::Socket::Done)
		Log(MessageType::FAULT) << "Couldn't listen on port: " << s_m_PortToListenOn;

	Connect();
}

Server::~Server() {
	// Disconnect from every client.
}

void Server::Connect() {
	sf::Packet connectionPacket;
	Packet::SetPacketType(Packet::CONNECT, connectionPacket);

	// Wait for all of the clients to join.
	if (m_SocketSelector.wait()) {
		if (m_SocketSelector.isReady(m_Listener)) {
			std::shared_ptr<sf::TcpSocket> client = std::make_shared<sf::TcpSocket>();
			if (m_Listener.accept(*client) == sf::Socket::Done) {
				ClientID clientID = m_Clients.size();

				m_Clients.insert(std::pair(clientID, client));
				m_SocketSelector.add(*client);

				Log(MessageType::INFO) << "Client connected. Remote address: " << client->getRemoteAddress().toString() << " Their ID is: " << clientID;
			}
			else {
				Log(MessageType::FAULT) << "Occured during client connection. Skipping.";
			}
		}
		else {
			// The listerner socket isn't ready, check all of the other sockets. (Maybe someone wants to disconnect.)
			for (auto &client : m_Clients) {
				if (m_SocketSelector.isReady(*client.second)) {
					// The client has sent some data, it can be received.
					sf::Packet packet;
					if (client.second->receive(packet) == sf::Socket::Done) {
						if (GetPacketType(packet) == Packet::DISCONNECT) {
							Disconnect(client.first);														// CHECK
						}
					}
				}
			}
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

		// Disconnect from the client.
		client->second->disconnect();

		// Remove it from the socket selector.
		m_SocketSelector.remove(*client->second);

		// Check whether the socket it still in use (whether the client has actually disconnected).
		if (client->second->getRemoteAddress() == sf::IpAddress::None && status == sf::TcpSocket::Done) {		// (CHECK THE SECOND STATEMENT AFTER &&)
			Log(MessageType::INFO) << "Successfully disconnected from a client! Client ID: " << p_ClientID;
			return;
		}

		// Remove it from the list of sockets.
		m_Clients.erase(client);
	}

	Log(MessageType::FAULT) << "Failed to disconnected from a client! Client ID: " << p_ClientID;
}

void Server::Send(const ClientID &p_ClientID, sf::Packet &p_Packet) {
	auto client = m_Clients.find(p_ClientID);
	if (client != m_Clients.end()) {
		sf::Socket::Status status = client->second->send(p_Packet);

		if (status != sf::Socket::Done) 
			Log(MessageType::INFO) << "Successfully sent data to a client! Client ID: " << p_ClientID;
		else 
			Log(MessageType::FAULT) << "Failed to send data to a client! Client ID: " << p_ClientID;
	}
}

void Server::Send(sf::Packet &p_Packet) {
	for (auto &client : m_Clients) {
		sf::Socket::Status status = client.second->send(p_Packet);

		if (status != sf::TcpSocket::Done) 
			Log(MessageType::INFO) << "Successfully sent data to a client! Client ID: " << client.first;
		else
			Log(MessageType::FAULT) << "Failed to send data to a client! Client ID: " << client.first;
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

bool Server::GetReceivedData(std::map<ClientID&, sf::Packet&> &p_Packets) {
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

bool Server::IsRunning() {
	return m_Clients.size() > 0;
}