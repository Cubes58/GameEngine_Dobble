/**
@file AudioManager.h
@brief Manages the game's audio.
*/
#pragma once

#include <map>

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>

#define AudioManagerInstance AudioManager::Instance()	//!< Direct access to the AudioManager singleton instance.

/*! \class AudioManager
	\brief Manages the game's audio.
*/

class AudioManager {
private:
	/*! 
		\brief A simple structure, to store a sound buffer object, and a sound object that's tied to the sound buffer.
	*/
	struct Audio {
		sf::SoundBuffer m_SoundBuffer;	//!< The audio data.
		sf::Sound m_Sound;	//!< The sound object, used to play the sound buffer.

		/*!
			\brief Constructor.
		*/
		Audio() = default;
		/*!
			\brief Constructor.
			\param p_SoundBuffer The sound buffer object, that stores the sound effect to be played.
		*/
		Audio(sf::SoundBuffer p_SoundBuffer) : m_SoundBuffer(p_SoundBuffer) {  }
	};
	std::map<std::string, Audio> m_SoundEffects;	//!< Stores the sound buffer, and the sound object they're bound to. The name of the sound effect is its key.
	sf::Music m_BackgroundMusic;	//!< Manages the game's (background) music.

	AudioManager();	//!< Constructor.
	~AudioManager() = default;	//!< Destructor.

public:
	/*!
		\brief Gets the AudioManager instance.
		\return Returns the Audio Manager instance.
	*/
	static AudioManager &Instance();

	/*!
		\brief Starts streaming an audio file.
		\param p_PathToAudioFile The audio file to start streaming.
		\param p_Play Checks whether the music should play, or not, when opened.
		\return Returns true if the audio file could be streamed, otherwise false.
	*/
	bool OpenMusicFile(const std::string &p_PathToAudioFile, bool p_Play = true);

	/*!
		\brief Loads a sound effect buffer, into memory.
		\param p_Name The name given to the sound buffer.
		\param p_PathToAudioFile The relative path to the sound buffer.
		\return Returns true if the sound effects file could be loaded, otherwise false.
	*/
	bool LoadSoundEffect(const std::string &p_Name, const std::string &p_PathToAudioFile);
	/*!
		\brief Loads a sound effect buffer, into memory.
		\param p_PathToAudioFile The relative path to the sound buffer.
		\return Returns true if the sound effects file could be loaded, otherwise false.
	*/
	bool LoadSoundEffect(const std::string &p_PathToAudioFile);

	/*!
		\brief Loads sounds effects, into memory, from a folder.
		\param p_PathToSoundEffectsFolder The relative path to the sound effects.
		\return Returns true if all of the sound effects could be loaded, otherwise false.
	*/
	bool LoadSoundEffects(const std::string &p_PathToSoundEffectsFolder);
	/*!
		\brief Play a requested sounds effect.
		\param p_Name The name of the sound effect.
		\return Returns true if the sound effect could be found and played, otherwise false.
	*/
	bool PlaySoundEffect(const std::string &p_Name);

	/*!
		\brief Gets a sound effect, to be used.
		\param p_Name The name of the sound buffer to get.
		\return Returns a const pointer to the sound effect, if found, otherwise it returns a nullptr.
	*/
	const sf::SoundBuffer *GetSoundEffect(const std::string &p_Name) const;
	/*!
		\brief Gets the music object.
		\return Returns the game's Music object.
	*/
	sf::Music &GetBackgroundMusic();

	// Delete the copy and assignment operators.
	AudioManager(AudioManager const&) = delete;	//!< Copy constructor deleted.
	AudioManager& operator=(AudioManager const&) = delete;	//!< Assignment operator deleted.
};