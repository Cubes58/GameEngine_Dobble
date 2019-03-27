#include "Window.h"

#include "Logger.h"

#include "GLCore.hpp"

Window::Window(sf::VideoMode p_Mode, const std::string &p_Title, const sf::Uint32 p_Style) : m_Style(p_Style) {
	sf::ContextSettings settings(8, 0, 4, 4, 4);
	settings.depthBits = 8;			
	settings.stencilBits = 0;		// Disabled.
	settings.antialiasingLevel = 4;	// If 4 doesn't work, then it'll try 2, if that doesn't work it'll set it to 0.	
	settings.attributeFlags = sf::ContextSettings::Attribute::Default;
	settings.majorVersion = 4;
	settings.minorVersion = 4;

	m_Width = p_Mode.width;
	m_Height = p_Mode.height;

	m_Window.create(p_Mode, p_Title, m_Style, settings);
	m_Window.setVerticalSyncEnabled(true);
	m_Window.setActive(true);
	m_Window.setFramerateLimit(0);	// Disable the frame rate limit.

	// Load OpenGL functions.
	gl::exts::LoadTest loaded = gl::sys::LoadFunctions();
	if (!loaded)
		Log(Type::FAULT) << "Unable to load GL functions.";

	Log(Type::INFO) << "OpenGL version: " << gl::GetString(gl::VERSION);
	Log(Type::WARNING) << "Unable to load: " << loaded.GetNumMissing() << " functions.";

	gl::Viewport(0, 0, GetWidth(), GetHeight());
	gl::Enable(gl::CULL_FACE);

	gl::Enable(gl::BLEND);
	gl::BlendFunc(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);

	gl::Disable(gl::DEPTH_TEST);
	gl::DepthFunc(gl::LESS);

	SetIcon("resources/images/dobbleIcon.png");
}

bool Window::IsOpen() {
	return m_Window.isOpen();
}

void Window::Close() {
	m_Window.close();
}

void Window::Display() {
	m_Window.display();
}

bool Window::SetIcon(const std::string &p_IconFileLocation) {
	sf::Image image;
	if (image.loadFromFile(p_IconFileLocation)) {
		m_Window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
		return true;
	}
	return false;
}

void Window::SetScreenSize(int p_Width, int p_Height) {
	m_Width = p_Width;
	m_Height = p_Height;

	m_Window.setSize(sf::Vector2u((unsigned int)m_Width, (unsigned int)m_Height));
}

sf::RenderWindow &Window::GetWindow() {
	return m_Window;
}

const sf::Uint32 &Window::GetStyle() const {
	return m_Style;
}

int Window::GetWidth() const {
	return m_Width;
}

int Window::GetHeight() const {
	return m_Height;
}