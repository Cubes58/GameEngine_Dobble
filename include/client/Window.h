/**
@file Window.h
@brief A window managing class.
*/
#pragma once

#include <string>

#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

/*! \class Window
	\brief A class that stores the properties necessary for a a window, and has behaviour to manipulate it.
*/

class Window {
private:
	sf::RenderWindow m_Window;	//!< The SFML Render Window instance.

	int m_Width;	//!< The width of the window.
	int m_Height;	//!< The height of the window.
	
	sf::Uint32 m_Style;		//!< The window's style.
	std::string m_Title;	//!< The window's title.

public:
	/*!
		\brief Constructor.
		\param p_Mode The video mode of the SFML Render window.
		\param p_Title The title of the window.
		\param p_Style The style of the SFML Render window.
	*/
	Window(sf::VideoMode p_Mode, const std::string &p_Title, const sf::Uint32 p_Style = sf::Style::Default);
	~Window() = default;	//!< Destructor.

	/*!
		\brief Returns whether the window is open, or not.
		\return Returns true if the window is open, otherwise false.
	*/
	bool IsOpen();
	/*!
		\brief Closes the window.
		\return Nothing.
	*/
	void Close();
	/*!
		\brief Displays the rendered data, in the window.
		\return Nothing.
	*/
	void Display();

	/*!
		\brief Sets the window's icon.
		\param p_IconFileLocation The relative file path, to the file, containing the icon. 
		\return Returns true if the icon was successfully loaded/set.
	*/
	bool SetIcon(const std::string &p_IconFileLocation);
	/*!
		\brief Sets the window's screen size.
		\param p_Width The width of the window.
		\param p_Height The height of the window.
		\return Nothing.
	*/
	void SetScreenSize(int p_Width, int p_Height);

	/*!
		\brief Gets the exact SFML RenderWindow instance,
		\return Returns a reference to the SFML Render Window instance, used.
	*/
	sf::RenderWindow &GetWindow();
	/*!
		\brief Gets the SFML Render Window instance's style.
		\return Returns the SFML Render Window instance's style,
	*/
	const sf::Uint32 &GetStyle() const;

	/*!
		\brief Gets the windows width.
		\return Return the window's width.
	*/
	int GetWidth() const;
	/*!
		\brief Gets the window's height.
		\return Returns the window's height.
	*/
	int GetHeight() const;
};