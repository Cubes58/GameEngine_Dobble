#include "Client.h"

#include "Logger.h"

Client::~Client() {
	if(m_Connected)
		Disconnect();
}

bool Client::Connect(const sf::Time &p_ConnectWaitTimeOut) {
	Log(Type::INFO) << "Attempting to connect to the server... " << "\nPort number: " << s_m_PortNumber << "\nIP Address: " << m_ServerIPAddress;
	if (m_ServerSocket.connect(m_ServerIPAddress, s_m_PortNumber, p_ConnectWaitTimeOut) != sf::Socket::Done) {
		Log(Type::FAULT) << "The server wasn't listening!";
		m_Connected = false;
		return false;
	}
	m_Connected = true;
	Log(Type::INFO) << "Connected to the server!";
	
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
	if (!m_Connected || m_ServerSocket.send(p_Packet) != sf::Socket::Done)
		return false;

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
