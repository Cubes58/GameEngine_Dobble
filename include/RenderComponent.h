#pragma once

#include <memory>

#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Texture.hpp>

struct RenderComponent {
	std::shared_ptr<sf::Shape> m_Shape;
	sf::Texture m_Texture;
};