#include "pch.h"

#define private public
#define protected public

#include <memory>
#include <future>
#include <thread>

#include "EntityManager.h"
#include "Randomiser.h"

#include "Server.h"
#include "Client.h"

#include "ServerGame.h"
#include "Deck.h"

TEST(ServerGameTest, Initialize) {
	std::shared_ptr<ServerGame> serverGame;

	// Create instance of serverGame, but it'll hold the thread until the minimum number of clients join,
	// so it needs to be put in a separate thread.
	std::thread serverGameThread([&]() { serverGame = std::make_shared<ServerGame>(); });

	std::vector<std::shared_ptr<Client>> clients;
	clients.resize(MINIMUM_NUMBER_OF_PLAYERS);

	bool success = true;
	for (auto &client : clients) {
		client = std::make_shared<Client>();
		bool couldConnect = client->Connect(sf::seconds(2));

		if (!couldConnect)
			success = false;
	}
	// Once the clients have connected join the thread.
	serverGameThread.join();

	EXPECT_TRUE(serverGame->m_IsRunning && success);
}

TEST(ServerGameTest, Update) {
	std::shared_ptr<ServerGame> serverGame;

	// Create the necessary objects.
	std::thread serverGameThread([&]() { serverGame = std::make_shared<ServerGame>(); });

	std::vector<std::shared_ptr<Client>> clients;
	clients.resize(MINIMUM_NUMBER_OF_PLAYERS);

	bool ableToConnect = true;
	for (auto &client : clients) {
		client = std::make_shared<Client>();
		bool couldConnect = client->Connect(sf::seconds(2));

		if (!couldConnect)
			ableToConnect = false;
	}
	serverGameThread.join();

	float updateTime = 1.0f / 60.0f;
	EXPECT_NO_THROW(serverGame->Update(updateTime));

	EXPECT_TRUE(serverGame->m_RoundLength == updateTime);
}

TEST(ServerGameTest, IsRunning) {
	std::shared_ptr<ServerGame> serverGame;

	// Create the necessary objects.
	std::thread serverGameThread([&]() { serverGame = std::make_shared<ServerGame>(); });

	std::vector<std::shared_ptr<Client>> clients;
	clients.resize(MINIMUM_NUMBER_OF_PLAYERS);

	bool ableToConnect = true;
	for (auto &client : clients) {
		client = std::make_shared<Client>();
		bool couldConnect = client->Connect(sf::seconds(2));

		if (!couldConnect)
			ableToConnect = false;
	}
	serverGameThread.join();

	EXPECT_TRUE(serverGame->IsRunning() == serverGame->m_IsRunning);
}

TEST(RandomiserTest, UniformRealRandomNumberGenerated) {
	float min = 0.5f;
	float max = 10.0f;

	float realRandomNumber = RandomiserInstance.GetUniformRealRandomNumber(min, max);

	EXPECT_TRUE((realRandomNumber >= min) && (realRandomNumber <= max));
}

TEST(RandomiserTest, UniformIntegerRandomNumberGenerated) {
	int min = 5;
	int max = 10;

	int integerRandomNumber = RandomiserInstance.GetUniformIntegerRandomNumber(min, max);

	EXPECT_TRUE((integerRandomNumber >= min) && (integerRandomNumber <= max));
}

TEST(RandomiserTest, NormalRandomNumberGenerated) {
	float min = 0.5f;
	float max = 10.0f;

	float integerRandomNumber = RandomiserInstance.GetNormalRandomNumber(min, max);

	EXPECT_TRUE((integerRandomNumber >= min) && (integerRandomNumber <= max));
}

TEST(CollisionTest, CircleCircle) {
	Collision collision;

	Vector2Df positionOne(100.0f, 100.0f);
	float radiusOne = 10.0f;

	Vector2Df positionTwo(100.0f, 100.0f);
	float radiusTwo = 5.0f;

	EXPECT_TRUE(collision(positionOne, radiusOne, positionTwo, radiusTwo));
}

TEST(CollisionTest, CirclePoint) {
	Collision collision;

	Vector2Df positionOne(100.0f, 100.0f);
	float radiusOne = 10.0f;
	Vector2Df positionTwo(100.0f, 100.0f);

	EXPECT_TRUE(collision(positionOne, radiusOne, positionTwo));
}

TEST(CollisionTest, AABBPoint) {
	Collision collision;

	Vector2Df positionOne(100.0f, 100.0f);
	Vector2Df sizeOne(10.0f, 10.0f);
	Vector2Df positionTwo(100.0f, 100.0f);

	EXPECT_TRUE(collision(positionOne, sizeOne, positionTwo));
}

TEST(CollisionTest, IsCircleInCircle) {
	Collision collision;

	Vector2D positionOne(100.0f, 100.0f);
	float radiusOne = 10.0f;

	Vector2D positionTwo(100.0f, 100.0f);
	float radiusTwo = 5.0f;

	EXPECT_TRUE(collision.IsCircleInCircle(positionOne, radiusOne, positionTwo, radiusTwo));
}

TEST(ServerTest, Initialize) {
	std::shared_ptr<Server> server;

	EXPECT_NO_THROW(server = std::make_shared<Server>());
}

TEST(ServerTest, CheckForClientConnectionRequest) {
	std::shared_ptr<Server> server = std::make_shared<Server>();
	std::shared_ptr<Client> client = std::make_shared<Client>();

	// Try connecting to the server (wait as long as it takes).
	std::thread clientThread([&]() { client->Connect(sf::Time::Zero); });

	// The request should be there, as soon as it's checked for, as the client is waiting, but just incase it'll wait a maximum of a second, before moving on.
	// The client will connect to the "global" port, and ask the server for a lobby.
	// Once the client requests a game, by joining the main port it'll be sent a different port, which will have a game being played on it,
	// this is where the client is considered a player, and is added as an actual client, opposed to being a temporary.
	bool success = server->CheckForClientConnectionRequest(sf::seconds(1));
	if (success)
		success = server->CheckForClientConnectionRequest(sf::seconds(1));

	clientThread.join();

	EXPECT_TRUE(success && server->m_Clients.size() > 0);
}

TEST(ServerTest, WaitForClientsToConnect) {
	std::shared_ptr<Server> server = std::make_shared<Server>();
	std::shared_ptr<Client> client = std::make_shared<Client>();

	std::thread serverThread([&]() { server->WaitForClientsToConnect(); });

	bool success = client->Connect(sf::seconds(1));
	serverThread.join();

	EXPECT_TRUE(success && server->m_Clients.size() > 0);
}

TEST(ServerTest, Disconnect) {
	std::shared_ptr<Server> server = std::make_shared<Server>();
	std::shared_ptr<Client> client = std::make_shared<Client>();

	std::thread clientThread([&]() { client->Connect(sf::Time::Zero); });
	bool clientRequestedConnection = server->CheckForClientConnectionRequest(sf::seconds(1));
	bool couldConnect = false;;
	if (clientRequestedConnection)
		couldConnect = server->CheckForClientConnectionRequest(sf::seconds(1));

	clientThread.join();

	bool success = false;
	if (couldConnect && server->m_Clients.size() > 0)
		success = true;

	server->Disconnect();
	if (server->m_Clients.size() > 0)
		success = false;

	EXPECT_TRUE(success);
}

TEST(ServerTest, Send) {
	std::shared_ptr<Server> server = std::make_shared<Server>();
	std::shared_ptr<Client> client = std::make_shared<Client>();

	std::thread clientThread([&]() { client->Connect(sf::Time::Zero); });
	bool clientRequestedConnection = server->CheckForClientConnectionRequest(sf::seconds(1));

	bool couldConnect = false;;
	if (clientRequestedConnection)
		couldConnect = server->CheckForClientConnectionRequest(sf::seconds(1));

	clientThread.join();

	bool dataToAddToPacket = true;
	sf::Packet packet;
	packet << dataToAddToPacket;
	server->Send(packet);

	sf::Packet receivedDatePacket;
	client->ReceiveData(receivedDatePacket);

	bool dataFromPacket;
	receivedDatePacket >> dataFromPacket;

	EXPECT_TRUE(couldConnect && dataToAddToPacket == dataFromPacket);
}

TEST(ServerTest, GetReceivedData) {
	std::shared_ptr<Server> server = std::make_shared<Server>();
	std::shared_ptr<Client> client = std::make_shared<Client>();

	std::thread clientThread([&]() { client->Connect(sf::Time::Zero); });
	bool clientRequestedConnection = server->CheckForClientConnectionRequest(sf::seconds(1));

	bool couldConnect = false;;
	if (clientRequestedConnection)
		couldConnect = server->CheckForClientConnectionRequest(sf::seconds(1));

	clientThread.join();

	bool dataToAddToPacket = true;
	sf::Packet packet;
	packet << dataToAddToPacket;
	client->Send(packet);

	// Get any packets from the clients.
	bool dataFromPacket = false;

	std::map<ClientID, sf::Packet> incomingData;
	if (server->GetReceivedData(incomingData)) {
		for (auto &clientData : incomingData) {
			clientData.second >> dataFromPacket;
		}
	}

	EXPECT_TRUE(couldConnect && dataToAddToPacket == dataFromPacket);
}

TEST(ServerTest, GetListeningState) {
	std::shared_ptr<Server> server = std::make_shared<Server>();

	EXPECT_TRUE(server->m_Listening == server->GetListeningState());
}

TEST(ServerTest, SetListeningState) {
	std::shared_ptr<Server> server = std::make_shared<Server>();

	bool newListeningState = false;
	server->SetListeningState(newListeningState);

	EXPECT_TRUE(server->m_Listening == newListeningState);
}

TEST(ServerTest, GetClientIDs) {
	std::shared_ptr<Server> server = std::make_shared<Server>();
	std::shared_ptr<Client> client = std::make_shared<Client>();

	std::thread clientThread([&]() { client->Connect(sf::Time::Zero); });
	bool clientRequestedConnection = server->CheckForClientConnectionRequest(sf::seconds(1));

	bool couldConnect = false;;
	if (clientRequestedConnection)
		couldConnect = server->CheckForClientConnectionRequest(sf::seconds(1));

	clientThread.join();

	EXPECT_TRUE(server->GetClientIDs().size() == server->m_Clients.size());
}

TEST(ServerTest, GetNumberOfClients) {
	std::shared_ptr<Server> server = std::make_shared<Server>();
	std::shared_ptr<Client> client = std::make_shared<Client>();

	std::thread clientThread([&]() { client->Connect(sf::Time::Zero); });
	bool clientRequestedConnection = server->CheckForClientConnectionRequest(sf::seconds(1));

	bool couldConnect = false;;
	if (clientRequestedConnection)
		couldConnect = server->CheckForClientConnectionRequest(sf::seconds(1));

	clientThread.join();

	EXPECT_TRUE(server->GetNumberOfClients() == server->m_Clients.size());
}

TEST(ServerTest, IsRunning) {
	std::shared_ptr<Server> server = std::make_shared<Server>();

	// If there's clients connected to the server then it's considered running.
	EXPECT_TRUE((server->m_Clients.size() > 0) == server->IsRunning());
}

TEST(DeckTest, GenerateCards) {
	Deck deck;
	unsigned int numberOfSymbolsPerCard = 8;
	deck.GenerateCards(Vector2Df(0.0f, 0.0f), 250.0f, numberOfSymbolsPerCard);

	bool singleSymbolMatchPerCardPair = true;
	for (unsigned int i = 0; i < deck.m_CardIDs.size(); i++) {
		for (unsigned int j = 0; j < deck.m_CardIDs.size(); j++) {
			if (i == j)
				continue;

			RenderComponent renderComponent = *EntityManagerInstance.GetComponent<RenderComponent>(deck.m_CardIDs[i]);
			RenderComponent otherRenderComponent = *EntityManagerInstance.GetComponent<RenderComponent>(deck.m_CardIDs[j]);

			int numberOfMatchingSymbols = 0;
			for (unsigned int k = 0; k < renderComponent.m_SymbolTextureIDs.size(); k++) {
				for (unsigned int l = 0; l < otherRenderComponent.m_SymbolTextureIDs.size(); l++) {
					if (renderComponent.m_SymbolTextureIDs[k] == otherRenderComponent.m_SymbolTextureIDs[l]) {
						numberOfMatchingSymbols++;
					}
				}
			}

			// Two because both cards will have that symbol - every symbol is looped over twice.
			// Anymore than two then there's more than a single matching symbol.
			// Any less than two then there's no matching symbol.
			if (numberOfMatchingSymbols > 2 || numberOfMatchingSymbols < 2) {
				singleSymbolMatchPerCardPair = false;
			}
		}
	}

	EXPECT_TRUE(singleSymbolMatchPerCardPair);
}

TEST(DeckTest, Shuffle) {
	Deck deck;
	unsigned int numberOfSymbolsPerCard = 8;
	deck.GenerateCards(Vector2Df(0.0f, 0.0f), 250.0f, numberOfSymbolsPerCard);

	std::vector<unsigned int> startingOrderOfCardIDs = deck.m_CardIDs;
	deck.Shuffle();
	std::vector<unsigned int> shuffledOrderOfCardIDs = deck.m_CardIDs;

	bool success = true;
	unsigned int numberOfCardIDsInSamePosition = 0;
	const unsigned int maxNumberOfCardIDsInSamePositionToConsiderFailure = 23;
	for (unsigned int i = 0; i < deck.m_CardIDs.size(); ++i) {
		if (startingOrderOfCardIDs[i] == shuffledOrderOfCardIDs[i]) 
			++numberOfCardIDsInSamePosition;
	}

	if (numberOfCardIDsInSamePosition >= maxNumberOfCardIDsInSamePositionToConsiderFailure)
		success = false;


	EXPECT_TRUE(success);
}

TEST(DeckTest, HasMatchingSymbol) {
	std::shared_ptr<RenderComponent> renderComponent = std::make_shared<RenderComponent>();

	unsigned int numberOfSymbolsPerCard = 8;
	for (unsigned int i = 0; i < numberOfSymbolsPerCard; ++i) {
		renderComponent->m_SymbolTextureIDs.emplace_back(i);
	}
	unsigned int symbolIDToMatch = --numberOfSymbolsPerCard;

	EXPECT_TRUE(Deck::HasMatchingSymbol(renderComponent, symbolIDToMatch));
}

TEST(DeckTest, IsDeckEmpty) {
	Deck deck;
	unsigned int numberOfSymbolsPerCard = 8;
	deck.GenerateCards(Vector2Df(0.0f, 0.0f), 250.0f, numberOfSymbolsPerCard);

	bool success = true;

	if (deck.m_CardIDs.size() > 0 && deck.IsDeckEmpty())
		success = false;

	// Clear the deck, so empty should return true.
	deck.m_CardIDs.clear();

	if (!deck.IsDeckEmpty())
		success = false;

	EXPECT_TRUE(success);
}

TEST(DeckTest, NumberOfRemainingCards) {
	Deck deck;
	unsigned int numberOfSymbolsPerCard = 8;
	deck.GenerateCards(Vector2Df(0.0f, 0.0f), 250.0f, numberOfSymbolsPerCard);

	EXPECT_TRUE(deck.NumberOfRemainingCards() == deck.m_CardIDs.size());
}

TEST(DeckTest, GetCardIDFromTop) {
	Deck deck;
	unsigned int numberOfSymbolsPerCard = 8;
	deck.GenerateCards(Vector2Df(0.0f, 0.0f), 250.0f, numberOfSymbolsPerCard);

	// Get the last card from the deck.
	unsigned int topCardID = *(--deck.m_CardIDs.end());

	EXPECT_TRUE(topCardID == deck.GetCardIDFromTop());
}

TEST(DeckTest, GetCardIDs) {
	Deck deck;
	unsigned int numberOfSymbolsPerCard = 8;
	deck.GenerateCards(Vector2Df(0.0f, 0.0f), 250.0f, numberOfSymbolsPerCard);

	EXPECT_TRUE(deck.m_CardIDs.size() == deck.GetCardIDs().size());
}

int main(int ac, char *av[]) {
	testing::InitGoogleTest(&ac, av);
	return RUN_ALL_TESTS();
}