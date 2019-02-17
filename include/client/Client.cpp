#include "Client.h"

#include "Logger.h"

Client::Client() : m_Connected(Connect()) {

}

Client::~Client() {
	if(m_Connected)
		Disconnect();
}

bool Client::Connect() {
	Log(MessageType::INFO) << "Attempting to connect to the server... " << "\nPort number: " << m_PortNumber << "\nIP Address: " << m_ServerIPAddress;
	if (m_ServerSocket.connect(m_ServerIPAddress, m_PortNumber) != sf::Socket::Done) {
		Log(MessageType::FAULT) << "The server wasn't listening!";
		return false;
	}
	Log(MessageType::INFO) << "Connected to the server!";
	
	m_ServerSocket.setBlocking(false);	// Ensure the methods return immediately.
	return true;
}

void Client::Disconnect() {
	if (!m_Connected)
		return;

	Log(MessageType::INFO) << "Disconnecting, from the server.";
	sf::Packet packet;
	Packet::SetPacketType(Packet::PacketType::DISCONNECT, packet);
	if (m_ServerSocket.send(packet) == sf::TcpSocket::Done)
		Log(MessageType::INFO) << "The server has been notified!";

	m_Connected = false;
	m_ServerSocket.disconnect();

	Log(MessageType::INFO) << "Successfully disconnected, from the server.";
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

bool Client::IsConnected() {
	return m_Connected;
}