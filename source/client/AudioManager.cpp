#include "AudioManager.h"

#include "FileSystemHelper.h"
#include "Logger.h"

AudioManager::AudioManager() {
	LoadSoundEffects("./resources/audio/soundEffects/");
	m_BackgroundMusic.setLoop(true);
	m_BackgroundMusic.setVolume(50);	// As it's the background music, half its volume.
}

AudioManager &AudioManager::Instance() {
	static AudioManager s_AudioManager;

	return s_AudioManager;
}

bool AudioManager::OpenMusicFile(const std::string &p_PathToAudioFile, bool p_Play) {
	if (m_BackgroundMusic.openFromFile(p_PathToAudioFile)) {
		Log(Type::INFO) << "The music file was successfully opened. Music file: " << p_PathToAudioFile;
		if (p_Play)
			m_BackgroundMusic.play();
		return true;
	}
		
	Log(Type::FAULT) << "The music file couldn't be opened. Music file: " << p_PathToAudioFile;
	return false;
}

bool AudioManager::LoadSoundEffect(const std::string &p_Name, const std::string &p_PathToAudioFile) {
	sf::SoundBuffer buffer;
	if (buffer.loadFromFile(p_PathToAudioFile)) {
		m_SoundEffects.insert(std::pair<std::string, Audio>(p_Name, Audio(buffer)));
		Log(Type::INFO) << "The sound effect was successfully loaded. Sound effect file: " << p_PathToAudioFile;
		return true;
	}

	Log(Type::FAULT) << "The sound effect couldn't be loaded. Sound effect file: " << p_PathToAudioFile;
	return false;
}

bool AudioManager::LoadSoundEffect(const std::string &p_PathToAudioFile) {
	return LoadSoundEffect(FileSystemHelper::GetNameFromFile(p_PathToAudioFile), p_PathToAudioFile);
}

bool AudioManager::LoadSoundEffects(const std::string &p_PathToSoundEffectsFolder) {
	std::vector<FileInformation> audioFiles = FileSystemHelper::GetFilesInFolder(p_PathToSoundEffectsFolder);
	FileSystemHelper::RetainRemoveFilesWithExtensions(audioFiles, { ".wav", ".Wav", ".WAV", ".ogg", ".vorbis", ".flac" });

	bool allSuccessful = true;
	for (auto &audioFile : audioFiles) {
		bool success = LoadSoundEffect(audioFile.m_Location);

		if (!success)
			allSuccessful = false;
	}

	return allSuccessful;
}

bool AudioManager::PlaySoundEffect(const std::string &p_Name) {
	auto iter = m_SoundEffects.find(p_Name);
	if (iter == m_SoundEffects.end()) {
		Log(Type::INFO) << "Couldn't find a sound object with the name: " << p_Name;
		return false;
	}
	Log(Type::INFO) << "Playing the sound: " << p_Name;
	iter->second.m_Sound.setBuffer(iter->second.m_SoundBuffer);
	iter->second.m_Sound.play();

	return true;
}

const sf::SoundBuffer *AudioManager::GetSoundEffect(const std::string &p_Name) const {
	auto iter = m_SoundEffects.find(p_Name);
	if (iter != m_SoundEffects.end()) {
		Log(Type::INFO) << "Sound effect was found and returned: " << p_Name;
		return &iter->second.m_SoundBuffer;
	}

	Log(Type::FAULT) << "The sound effect couldn't be found: " << p_Name;
	return nullptr;
}

sf::Music &AudioManager::GetBackgroundMusic() {
	return m_BackgroundMusic;
}