/**
@file Server.h
@brief Manages the communication between the server and the players.
*/
#pragma once

#include <map>
#include <string>
#include <memory>

#include <SFML\Network.hpp>

#include "PacketTypes.h"

using ClientID = unsigned int;	//!< Setting ClientID to an unsigned int type.

/*! \class Server
	\brief  Manages the communication between the server and the players.
*/

class Server {
private:
	static const unsigned int s_m_PortToListenOn = 8787;	//!< The main listening port, for new clients.
	static const unsigned int s_m_DefaultListeningCheckTime = 5;	//!< The default amount of time to listen for client connection requests.
	static unsigned int s_m_NewClientID;	//!< The new client ID.

	std::map<ClientID, std::shared_ptr<sf::TcpSocket>> m_Clients;	//!< A map of client IDs and their relative TCP socket.
	sf::SocketSelector m_SocketSelector;	//!< A socket selector, to handle multiple sockets.
	sf::TcpListener m_Listener;	//!< A listener to listen for new clients, and incoming client data.
	sf::TcpListener m_LobbyListener;	//!< A lobby listener, used to help the main listener - when a client joins they're sent to this listener, to join a game lobby.
	sf::Int32 m_GamePortNumber = -1;	//!< The game active game's port number - randomly generated each time (keep it more secure).
	bool m_Listening;	//!< Manages whether the server is listening for new clients.

public:
	Server();	//!< Constructor.
	~Server();	//!< Destructor.

	/*!
		\brief Checks for incoming client connections requests.
		\param p_WaitTime The amount of time to wait, for incoming client connection requests.
		\return Returns true if a client has requested to connect, false otherwise.
	*/
	bool CheckForClientConnectionRequest(const sf::Time &p_WaitTime = sf::microseconds(s_m_DefaultListeningCheckTime));
	/*!
		\brief Waits for clients to connect.
		\param p_NumberOfClients The number of clients to wait for.
		\return Nothing.
	*/
	void WaitForClientsToConnect(int p_NumberOfClients = 1);
	/*!
		\brief Disconnects from a client.
		\param p_ClientID The client's ID to disconnect from.
		\return Nothing.
	*/
	void Disconnect(const ClientID &p_ClientID);
	/*!
		\brief Disconnects from every client.
		\return Nothing.
	*/
	void Disconnect();

	/*!
		\brief Sends a packet (data) to a client.
		\param p_ClientID The client's ID to send data to.
		\param p_Packet The packet to send to the client.
		\return Nothing.
	*/
	void Send(const ClientID &p_ClientID, sf::Packet &p_Packet);
	/*!
		\brief Send a packet to every client connected.
		\param p_Packet The packet to send to the clients
		\return Nothing.
	*/
	void Send(sf::Packet &p_Packet);	
	/*!
		\brief Checks if a packet has been received, from a client.
		\param p_ClientID The client who sent the packet.
		\param p_Packet The packet, if one has been received.
		\return Returns true if data has been received, otherwise false.
	*/
	bool GetReceivedData(const ClientID &p_ClientID, sf::Packet &p_Packet);	
	/*!
		\brief Checks if packets have been received.
		\param p_Packets A map of clients and packets, any data received from clients will be added.
		\return Returns true if a packet (data) was received, otherwise false.
	*/
	bool GetReceivedData(std::map<ClientID, sf::Packet> &p_Packets);	

	/*!
		\brief Gets whether the server is listening for new incoming clients.
		\return Returns true if the server is listening for incoming clients, otherwise false.
	*/
	bool GetListeningState() const;
	/*!
		\brief Sets whether the server should listen for new clients.
		\param p_Listen The listening state, to set.
		\return Nothing.
	*/
	void SetListeningState(bool p_Listen);

	/*!
		\brief Gets a vector of client IDs.
		\return Returns a vector of client IDs.
	*/
	std::vector<ClientID> GetClientIDs();
	/*!
		\brief Gets the number of clients, connected.
		\return Returns the number of clients, connected.
	*/
	unsigned int GetNumberOfClients() const;

	/*!
		\brief Gets whether the server instance is running.
		\return Returns the state of the server, true if it's running, false otherwise.
	*/
	bool IsRunning();
};