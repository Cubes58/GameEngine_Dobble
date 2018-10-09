#pragma once

#include <string>

#include <SFML\Window.hpp>
#include <SFML\Graphics\RenderWindow.hpp>

class Window {
private:
	sf::RenderWindow m_Window;

	int m_Width;
	int m_Height;
	
	sf::Uint32 m_Style;
	std::string m_Title;
public:
	Window(sf::VideoMode p_Mode, const std::string &p_Title, const sf::Uint32 p_Style = sf::Style::Default);
	~Window();

	bool IsOpen();
	void Display();

	sf::RenderWindow &GetWindow();
	const sf::Uint32 &GetStyle();

	int GetWidth();
	int GetHeight();
};