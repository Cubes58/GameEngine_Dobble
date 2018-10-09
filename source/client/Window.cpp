#include "Window.h"

#include "Logger.h"

#include "GLCore.hpp"

Window::Window(sf::VideoMode p_Mode, const std::string &p_Title, const sf::Uint32 p_Style) {
	sf::ContextSettings settings(0, 0, 0, 2, 4);
	settings.depthBits = 0;			// Disabled.
	settings.stencilBits = 0;		// Disabled.
	settings.antialiasingLevel = 4;	// If 4 doesn't work, then it'll try 2, if that doesn't work it'll set it to 0.	
	settings.attributeFlags = sf::ContextSettings::Attribute::Default;
	settings.majorVersion = 4;
	settings.minorVersion = 4;

	m_Width = p_Mode.width;
	m_Height = p_Mode.height;

	m_Window.create(p_Mode, p_Title, p_Style, settings);
	m_Window.setVerticalSyncEnabled(true);

	// Load OpenGL functions.
	gl::exts::LoadTest loaded = gl::sys::LoadFunctions();

	if (!loaded)
		Log(Type::FAULT) << "Unable to load GL functions.";

	Log(Type::INFO) << "OpenGL version: " << gl::GetString(gl::VERSION);
	Log(Type::INFO) << "Unable to load: " << loaded.GetNumMissing() << " functions.";
}

Window::~Window() {

}

bool Window::IsOpen() {
	return m_Window.isOpen();
}

void Window::Display() {
	m_Window.display();
}

sf::RenderWindow &Window::GetWindow() {
	return m_Window;
}

const sf::Uint32 &Window::GetStyle() {
	return m_Style;
}

int Window::GetWidth() {
	return m_Width;
}

int Window::GetHeight() {
	return m_Height;
}