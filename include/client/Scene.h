/**
@file Scene.h
@brief Manages the scene data.
*/
#pragma once

#include <memory>
#include <string>

#include <glm/detail/type_vec3.hpp>
#include <SFML/Window/Event.hpp>

#include "Vector2D.h"
#include "GameState.h"
#include "Collision.h"

class Game;
class UserInterface;
class PostProcessor;
struct Text;

/*! \class Scene
	\brief  Manages the scene data.
*/

class Scene {
private:
	/*!
		* @brief The Game class can access all of the Scene's properties.
		* @relates Game
	*/
	friend Game;

protected:
	Collision m_Collision;	//!< A collision instance, used to check for collisions between objects in the scene, or the player's mouse and a button.
	Vector2Df m_ScreenSize;	//!< The window's size.
	GameState m_GameState;	//!< The state of the game.
	std::shared_ptr<UserInterface> m_UserInterface;	//!< A pointer to a user interface instance, which manages the user interface.
	std::shared_ptr<PostProcessor> m_PostProcessor;	//!< A pointer to a post processor instance, which manages post processing effects.

public:
	/*!
		\brief Constructor.
		\param p_ScreenSize The window's size.
		\param p_File The relative file path, to the user interface data.
	*/
	Scene(const Vector2Df &p_ScreenSize, const std::string &p_File);
	virtual ~Scene() = default;	//!< Destructor.

	/*!
		\brief Checks whether the game state has changed.
		\param p_GameState The game state, which will be changed, if the scene's game state has changed, to update where the player is, in the menu/game system.
		\return true if the game state has changed, false otherwise.
	*/
	virtual bool Change(GameState &p_GameState);
	/*!
		\brief Handles incoming input events.
		\param p_Event The input event to handle.
		\return Nothing.
	*/
	virtual void HandleInputEvent(sf::Event &p_Event);
	/*!
		\brief Updates the scene.
		\param p_DeltaTime The delta time between the current frame, and the previous one.
		\return Nothing.
	*/
	virtual void Update(float p_DeltaTime);
	/*!
		\brief Renders the scene.
		\return Nothing.
	*/
	virtual void Render();

	/*!
		\brief Renders text, to the sceen.
		\param p_Text The text to render to the screen.
		\param p_XPosition The X position of where the text should be rendered (between 0 and 1).
		\param p_YPosition The Y position of where the text should be rendered (between 0 and 1).
		\param p_Scale The scale of the text.
		\param p_Colour The colour of the text.
		\return Nothing.
	*/
	void RenderText(const std::string &p_Text, float p_XPosition, float p_YPosition, float p_Scale, const glm::vec3 &p_Colour);
	/*!
		\brief
		\param p_Text The text to render to the screen.
		\param p_Position The position of the text, on the screen (between 0 and 1).
		\param p_Scale The scale of the text.
		\param p_Colour The colour of the text.
		\return
	*/
	void RenderText(const std::string &p_Text, const Vector2Df &p_Position, float p_Scale, const glm::vec3 &p_Colour);

	/*!
		\brief Adds text to the user interface instance.
		\param p_Text The text instance to add to the user interface.
		\return Nothing.
	*/
	void AddText(std::shared_ptr<Text> p_Text);
	/*!
		\brief Sets the screen size.
		\param p_ScreenSize The new screen size.
		\return Nothing.
	*/
	void SetScreenSize(const Vector2Df &p_ScreenSize);
	/*!
		\brief Sets the scene's game state.
		\param p_GameState the scene's new gamestate.
		\return Nothing
	*/
	void SetGameState(const GameState &p_GameState);
};