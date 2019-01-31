#pragma once

#include "Component.h"

struct InputComponent : public Component {
	unsigned int m_SymbolHashID; // Hashed value of the symbol's name, to compare.
	unsigned int m_Key;
};