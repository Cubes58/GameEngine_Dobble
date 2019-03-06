#pragma once

#include <string>

#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Window {
private:
	sf::RenderWindow m_Window;

	int m_Width;
	int m_Height;
	
	sf::Uint32 m_Style;
	std::string m_Title;

public:
	Window(sf::VideoMode p_Mode, const std::string &p_Title, const sf::Uint32 p_Style = sf::Style::Default);
	~Window() = default;

	bool IsOpen();
	void Close();
	void Display();

	bool SetIcon(const std::string &p_IconFileLocation);
	void SetScreenSize(int p_Width, int p_Height);

	sf::RenderWindow &GetWindow();
	const sf::Uint32 &GetStyle() const;

	int GetWidth() const;
	int GetHeight() const;
};