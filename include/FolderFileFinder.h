#pragma once

#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <filesystem>

class FolderFileFinder {
private:
	FolderFileFinder() {

	}

	~FolderFileFinder() {

	}

	struct PathLeafString {
		std::string operator()(const std::filesystem::directory_entry &p_Entry) const {
			return p_Entry.path().filename().string();
		}
	};

public:
	static std::vector<std::string> GetFilesInFolder(const std::string &p_FolderLocation, std::vector<std::string> p_ExtensionsOfFilesToKeepOrDiscard = std::vector<std::string>(), const bool &p_KeepExtensionType = true) {
		std::vector<std::string> files;

		std::filesystem::path pathLocation(p_FolderLocation);
		std::filesystem::directory_iterator start(pathLocation);
		std::filesystem::directory_iterator end;
		std::transform(start, end, std::back_inserter(files), PathLeafString());

		for (auto &file : files) {
			file.insert(0, p_FolderLocation + "/");
		}

		if (p_ExtensionsOfFilesToKeepOrDiscard.size() > 0) {
			std::vector<std::string> specificFiles;
			for (std::string &file : files) {
				size_t position = file.find_last_of(".");

				bool shouldKeep = !p_KeepExtensionType;
				for (std::string &extension : p_ExtensionsOfFilesToKeepOrDiscard) {
					if (file.substr(position) == extension) {
						shouldKeep = !shouldKeep;

						break;
					}
				}

				if (shouldKeep)
					specificFiles.push_back(file);
			}
			return specificFiles;
		}

		return files;
	}

	static std::vector<std::string> ExtractFileName(const std::vector<std::string> &p_Files) {
		std::vector<std::string> files;
		for (auto &file : p_Files) {
			size_t firstPos = file.find_last_of("/");
			size_t lastPos = file.find_last_of(".");

			files.push_back(file.substr(firstPos + 1, lastPos - firstPos - 1));
		}

		return files;
	}

	// Delete the copy and assignment operators.
	FolderFileFinder(FolderFileFinder const&) = delete; //!< Copy operator, deleted.
	FolderFileFinder& operator=(FolderFileFinder const&) = delete; //!< Assignment operator, deleted.
};