/**
@file Client.h
@brief Manages the communication between the player and the server.
*/
#pragma once

#include <string>

#include <SFML/Network.hpp>

#include "PacketTypes.h"

/*! \class Client
	\brief  Manages the communication between the player and the server.
*/

class Client {
private:
	static const int s_m_PortNumber = 8787;	//!< The port number to connect to the server.
	sf::IpAddress m_ServerIPAddress = "127.0.0.1"; //!< The server's IP address.
	sf::TcpSocket m_ServerSocket;	//!< The socket (connection) between the client and the server, used to communicate.
	sf::Int32 m_PlayPortNumber = -1;	//!< The port number the game is being played on (the server sends information about the game details).
	bool m_Connected;	//!< Manages if the client is connected to the server.

public:
	Client() = default;	//!< Constructor.
	~Client();	//!< Destructor.

	/*!
		\brief Attempts to connect to a server.
		\param p_ConnectWaitTimeOut The time it waits, while trying to connect to the server, before it times out.
		\return Returns true if all of the shaders were successfully loaded, otherwise false.
	*/
	bool Connect(const sf::Time &p_ConnectWaitTimeOut = sf::Time::Zero);
	/*!
		\brief Disconnects from the server.
		\return Nothing.
	*/
	void Disconnect();

	/*!
		\brief Sends a packet (data) to the server.
		\param p_Packet The packet to send to the server.
		\return Return true if the packet sent successfully, false otherwise.
	*/
	bool Send(sf::Packet &p_Packet);
	/*!
		\brief Checks to see if a packet (data) has been received the server.
		\param p_Packet The packet to set as the one received from the server.
		\return Return true if a packet has been received, false otherwise.
	*/
	bool ReceiveData(sf::Packet &p_Packet);

	/*!
		\brief Returns the connection state of the client and the server.
		\return Returns true if the client is connected, otherwise false.
	*/
	bool IsConnected();
	/*!
		\brief Sets the connection state between the client and the server.
		\param p_Connected The new state to set as the connected state.
		\return Nothing.
	*/
	void SetConnected(bool p_Connected);
};