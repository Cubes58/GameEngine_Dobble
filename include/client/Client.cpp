#include "Client.h"

#include "Logger.h"

using namespace Packet;

Client::Client() : m_Connected(Connect()) {
	// Connect to the server.
	m_ServerSocket.setBlocking(false);	// Ensure the methods return immediately.
}

Client::~Client() {
	if(m_Connected)
		Disconnect();			//CHECK
}

bool Client::Connect() {
	Log(MessageType::INFO) << "Attempting to connect to the server...";
	if (m_ServerSocket.connect(m_ServerIPAddress, m_PortNumber) != sf::Socket::Done) {
		Log(MessageType::FAULT) << "The server wasn't listening!";
		return false;
	}
	Log(MessageType::INFO) << "Connected to the server!";

	sf::Packet packet;
	ReceiveData(packet);

	return true;
}

void Client::Disconnect() {
	if (!m_Connected)
		return;

	sf::Packet packet;
	SetPacketType(PacketType::DISCONNECT, packet);
	sf::TcpSocket::Status socketStatus = m_ServerSocket.send(packet);

	m_Connected = false;
	m_ServerSocket.disconnect();

	if (socketStatus != sf::TcpSocket::Done)
		Log(MessageType::FAULT) << "Trying to disconnect, from the server!";
}

bool Client::Send(sf::Packet &p_Packet) {
	if (!m_Connected || m_ServerSocket.send(p_Packet) != sf::Socket::Done)
		return false;

	Log(MessageType::INFO) << "Packet being sent, to the server.";
	return true;
}

bool Client::ReceiveData(sf::Packet &p_Packet) {
	// Listen for data, from the server.
	if (m_ServerSocket.receive(p_Packet) != sf::TcpSocket::Done) {
		p_Packet.clear();
		return false; // No data to process, move on.
	}

	Log(MessageType::INFO) << "Packet being received, from the server.";
	return true;
}

bool Client::GetConnectionState() {
	return m_Connected;
}