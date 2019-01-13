#pragma once

#include <vector>
#include <string>

#include "Component.h"

struct RenderComponent : public Component {
	std::string m_CardBackgroundTextureID;
	
	// Store eight texture names' for each of the symbols on the card.
	std::vector<std::string> m_SymbolTextureNames;

	// Generate and store 8 circles? (8 VBOs and a VAO to store them).

	RenderComponent() = default;
	RenderComponent(const std::vector<std::string> &p_SymbolTextureNames, const std::string &p_CardBackgroundTextureID)
		: m_SymbolTextureNames(p_SymbolTextureNames), m_CardBackgroundTextureID(p_CardBackgroundTextureID) {}
};