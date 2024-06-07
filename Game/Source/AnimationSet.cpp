#include "AnimationSet.h"
#include "App.h"
#include "Textures.h"

#include "SDL/include/SDL.h"
#include "Log.h"

AnimationSet::AnimationSet()
{
}

AnimationSet::AnimationSet(const char* animPath)
{
	LoadAnimSet(animPath);
}

AnimationSet::~AnimationSet()
{

}

void AnimationSet::SetAnimation(SString name) {
	bool found = false;
	if (name != "") // Don't bother searching if string is empty
	for (size_t i = 0; i < animations.size() && !found; i++)
	{
		if (animations[i].name == name)
		{
			activeAnimation = i;
			GetCurrent().Reset();
		}
	}
	if (!found)
		LOG("Animation \"%s\" not found", name.GetString());
}

void AnimationSet::SetAnimation(int index)
{
	if (IN_RANGE(index, 0, animations.size() - 1))
	{
		activeAnimation = index;
		animations[activeAnimation].Reset();
	}
}

// Sets the animation to go back to after finishing the current animation. -1 to stay on current animation
void AnimationSet::SetDefaultAnimation(SString name)
{
	int id = GetAnimationId(name);
	if (id >= 0)
		defaultAnimation = id;
}

int AnimationSet::GetAnimationId(SString name)
{
	if (name != "") // Don't bother searching if string is empty
		for (size_t i = 0; i < animations.size(); i++)
		{
			if (animations[i].name == name)
			{
				return i;
			}
		}
	LOG("Animation \"%s\" not found", name.GetString());
	return -1;
}

// Loads a set of animations from an XML-formatted TexturePacker sprite sheet
void AnimationSet::LoadAnimSet(const char* animPath)
{
	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_file(animPath);
	if (result) {
		LOG("Animation file '%s' loaded", animPath);
	}
	else {
		LOG("Error parsing animation file '%s': %s", animPath, result.description());
		return;
	}
	pugi::xml_node rootNode = doc.child("TextureAtlas");

	texture = app->tex->LoadSP(rootNode.attribute("imagePath").as_string(), false);


	// TexturePacker does not have one list for each animation, nor does it store custom data along the spritesheet when using the XML export option.
	// This makes it required to manually touch up the animations with the parameters needed.
	// If no such parameters are provided at either the TextureAtlas node or the first sprite node of each animation, the defaults provided here are used instead
	float animSpeed = rootNode.attribute("speed").as_float(0.08f);
	bool animLoop = rootNode.attribute("loop").as_bool(false);
	bool animPingpong = rootNode.attribute("pingpong").as_bool(false);

	pugi::xml_node frameNode = rootNode.child("sprite");
	while (frameNode != NULL) {
		Animation a;

		//Gets the first sprite name in the sequence and stores it as the current animation name
		SString name = frameNode.attribute("n").as_string();
		std::vector<SString> nameComp = name.GetWords('-');
		a.name = nameComp[0];

		// If the specific animation parameters are not found on the first frame, use the animation set's default values
		a.speed = frameNode.attribute("speed").as_float(animSpeed);
		a.loop = frameNode.attribute("loop").as_bool(animLoop);
		a.pingpong = frameNode.attribute("pingpong").as_bool(animPingpong);

		// All consecutive sprites with the same name are part of the same animation
		while (frameNode != NULL && a.name == nameComp[0])
		{
			// Stores the sprite's coordinates and size in pixels
			SDL_Rect rect;
			rect.x = frameNode.attribute("x").as_int();
			rect.y = frameNode.attribute("y").as_int();
			rect.w = frameNode.attribute("w").as_int();
			rect.h = frameNode.attribute("h").as_int();

			//Stores the sprite pivot in pixels
			iPoint pivot;
			pivot.x = (frameNode.attribute("pX").as_float(0) * (float)rect.w);
			pivot.y = (frameNode.attribute("pY").as_float(0) * (float)rect.h);

			a.PushBack(rect, 4, pivot);

			// Loads next frame and gets its name
			frameNode = frameNode.next_sibling("sprite");

			name = frameNode.attribute("n").as_string();
			nameComp = name.GetWords('-');
		}

		animations.push_back(a);
	}

	// Animation format contained in this project's config.xml
	/*for (pugi::xml_node animNode = rootNode.child("animation"); animNode != NULL; animNode = animNode.next_sibling("animation"))
	{
		a.DeleteAnim();
		a.speed = animNode.attribute("speed").as_float(0.08f);
		a.loop = animNode.attribute("loop").as_bool(false);
		a.pingpong = animNode.attribute("pingpong").as_bool(false);


		for (pugi::xml_node frameNode = animNode.child("frame"); frameNode != NULL; frameNode = frameNode.next_sibling("frame"))
		{
			SDL_Rect rect;
			rect.x = frameNode.attribute("x").as_int();
			rect.y = frameNode.attribute("y").as_int();
			rect.w = frameNode.attribute("w").as_int();
			rect.h = frameNode.attribute("h").as_int();
			int duration = frameNode.attribute("duration").as_int(1);
			a.PushBack(rect, duration);
		}
		PushBack(a);
	}*/
}
