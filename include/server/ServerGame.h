#pragma once

class ServerGame {
private:
	bool m_IsRunning;

public:
	ServerGame();
	~ServerGame();

	void Update(float p_DeltaTime);

	bool IsRunning() const;
};