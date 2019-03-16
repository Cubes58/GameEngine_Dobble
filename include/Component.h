/**
@file Component.h
@brief A base class for components that's used so components can be stored in a large blocks of memory, together.
*/
#pragma once

/**
	\brief A based structure to represent a component.
*/

struct Component {
	Component() = default;	//!< Default constructor.
	virtual ~Component() = default;	//!< Default destructor.
};