#pragma once

#include <array>
#include <string>

#include "Component.h"
#include "Texture2D.h"

struct RenderComponent : public Component {
	std::array<Texture2D*, s_NUMBER_OF_SYMBOLS_ON_CARD + 1> m_SymbolTextureNames;

	RenderComponent() = default;
	RenderComponent(const std::array<Texture2D*, s_NUMBER_OF_SYMBOLS_ON_CARD + 1> &p_SymbolTextureNames)
		:  m_SymbolTextureNames(p_SymbolTextureNames) {}
};