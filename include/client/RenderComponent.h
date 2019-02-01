#pragma once

#include <array>
#include <string>

#include "Component.h"
#include "Texture2D.h"

struct RenderComponent : public Component {
	std::array<Texture2D*, s_NUMBER_OF_CIRCLES_PER_CARD> m_SymbolTextureNames;

	RenderComponent() = default;
	RenderComponent(const std::array<Texture2D*, s_NUMBER_OF_CIRCLES_PER_CARD> &p_SymbolTextureNames)
		:  m_SymbolTextureNames(p_SymbolTextureNames) {}
};