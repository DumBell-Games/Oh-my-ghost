#pragma once
#include <queue>
#include "Module.h"
#include "List.h"
#include "Timer.h"
#include "SDL/include/SDL_rect.h"

struct ReloadPreset {
	SString name;
	List<Module*> unload;
	List<Module*> load;
	//Fade out time in seconds
	float fadeOut;
	//Fade in time in seconds
	float fadeIn;

	ReloadPreset() : name("nameless"), fadeOut(0.0f), fadeIn(0.0f)
	{}

	ReloadPreset(const SString& _name, const float _fadeOut, float _fadeIn) : name(_name), fadeOut(_fadeOut), fadeIn(_fadeIn)
	{}

	bool AddReload(Module* m) {
		
		
		return AddUnload(m) && AddLoad(m);
	}
	bool AddUnload(Module* m) {
		bool ret = true;
		if (unload.Find(m) == -1)
			unload.Add(m);
		else ret = false;
		return ret;
	}
	bool AddLoad(Module* m) {
		bool ret = true;
		if (load.Find(m) == -1)
			load.Add(m);
		else ret = false;
		return ret;
	}
};

enum class ReloadStep {
    NONE,
    FADE_OUT,
    RELOAD,
    FADE_IN
};

class Reload : public Module
{
public:
    Reload(bool startEnabled = true);

    // Destructor
    virtual ~Reload();

    // Called before render is available
    bool Awake(pugi::xml_node config);

    // Called before the first frame
    bool Start();

    // Called each loop iteration
    bool PostUpdate();

    // Called before quitting
    bool CleanUp();

	// Adds transition to the queue
    bool QueueReload(SString presetName);

private:

    void FadeOut();

    void ReloadModules();

    void FadeIn();

public:

	std::queue<ReloadPreset*> queue;
    ReloadPreset* activePreset = nullptr;
    ReloadStep currentStep = ReloadStep::NONE;

    List<ReloadPreset*> presetList;

    List<Module*> moduleList;

    bool timerActive = false;
    Timer timer;

    float fadeRatio = 0.0f;

	SDL_Rect screenRect = {0,0,0,0};

};

