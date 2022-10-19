#pragma once 
#include "OFS_BinarySerialization.h"
#include "OFS_ScriptSettings.h"
#include "Funscript.h"
#include "OFS_ScriptSimulator.h"

#include <vector>

#include "SDL_mutex.h"

#define OFS_PROJECT_EXT ".ofsp"

enum OFS_Project_Version : int32_t
{
	One = 1,
	FloatingPointTimestamps = 2,
	PlayerSettingsChange = 3,
};

class OFS_Project
{
	bool FindMedia(const std::string& funscriptPath) noexcept;
	void LoadScripts(const std::string& funscriptPath) noexcept;
	bool ImportFunscript(const std::string& path) noexcept;
	void LoadedSuccessful() noexcept;
public:
	static constexpr const char* Extension = OFS_PROJECT_EXT;

	bool Valid = false;
	bool Loaded = false;
	std::string LoadingError;

	struct ProjSettings
	{
		// this timer tracks how long the project has been 'actively' worked on in seconds
		float activeTimer = 0.f;
		// when this is true
		// the user gets nudged to enter metadata
		bool NudgeMetadata = true;
		static ScriptSimulator::SimulatorSettings* Simulator;

		template<typename S>
		void serialize(S& s)
		{
			s.ext(*this, bitsery::ext::Growable{},
				[](S& s, ProjSettings& o) {
					s.boolValue(o.NudgeMetadata);
					FUN_ASSERT(o.Simulator, "Simulator not hooked up.");
					s.object(*o.Simulator);
					s.value4b(o.activeTimer);
				});
		}
	} ProjectSettings;

	std::string LastPath;
	OFS_ScriptSettings Settings;
	Funscript::Metadata Metadata;
	std::vector<std::shared_ptr<Funscript>> Funscripts;
	std::string MediaPath;

	SDL_mutex* ProjectMut = nullptr;
	ByteBuffer ProjectBuffer;

	OFS_Project() noexcept;
	~OFS_Project() noexcept;

	void Clear() noexcept;
	bool Load(const std::string& path) noexcept;

	void Save(bool clearUnsavedChanges) noexcept { Save(LastPath, clearUnsavedChanges); }
	void Save(const std::string& path, bool clearUnsavedChanges) noexcept;

	void AddFunscript(const std::string& path) noexcept;
	void RemoveFunscript(int idx) noexcept;

	bool Import(const std::string& path) noexcept;

	void ExportFunscript(const std::string& outputPath, int idx) noexcept;
	void ExportFunscripts(const std::string& outputPath) noexcept;
	void ExportFunscripts() noexcept;

	void ExportClips(const std::string& outputDirectory) noexcept;
	bool HasUnsavedEdits() noexcept;

	void ShowProjectWindow(bool* open) noexcept;

	void Update(float delta, bool idleMode) noexcept;

	template<typename S>
	void serialize(S& s)
	{
		s.ext(*this, bitsery::ext::Growable{},
			[](S& s, OFS_Project& o) {
				auto CurrentVersion = OFS_Project_Version::PlayerSettingsChange;
				s.value4b(CurrentVersion);
				switch(CurrentVersion)
				{
					case One:
					o.LoadingError = "Project not compatible.\n";
						"Last compatible version was 1.2.0.";
					case FloatingPointTimestamps:
					o.LoadingError = "Project not compatible.\n"
						"Last compatible version was 2.0.0.";
					o.Valid = false;
						return;
					case PlayerSettingsChange:
						break;
				}
				s.text1b(o.MediaPath, o.MediaPath.max_size());
			    s.object(o.Settings);
				s.container(o.Funscripts, 100, 
					[](S& s, std::shared_ptr<Funscript>& script) {
					s.ext(script, bitsery::ext::StdSmartPtr{});
				});
				s.object(o.Metadata);
				s.object(o.ProjectSettings);
				o.Valid = true;
			});
	}
};
