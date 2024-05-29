#pragma once
#include "Animation.h"
#include <PugiXml/src/pugixml.hpp>

// Objeto de transicion entre animaciones, admite cualquier cosa que se pueda llamar como una funcion sin parametros que devuelva un booleano (un "functor")
// NOTE: No se eestá usando en ninguna parte del proyecto al momento de escribir este comentario
template <typename Condition>
struct AnimationTransition
{
	bool CheckCondition()
	{
		return condition();
	}
	Condition condition;

};
	template <typename tdata>
	struct TransitionHelper {
		static bool AGreaterThanB(const tdata& a, const tdata& b) { return a > b; }
		static bool AEqualOrGreaterThanB(const tdata& a, const tdata& b) { return a >= b; }
		static bool AEqualsB(const tdata& a, const tdata& b) { return a == b; }
	};

class AnimationSet
{
public:
	int activeAnimation = 0;
	std::vector<Animation> animations;
	std::shared_ptr<SDL_Texture> texture;

public:
	AnimationSet();

	AnimationSet(const char* animPath);

	~AnimationSet();

	void PushBack(const Animation& anim) {
		animations.push_back(anim);
	}

	void SetAnimation(SString name);

	void SetAnimation(int index)
	{
		if (index >= 0 && index < animations.size())
		{
			activeAnimation = index;
			animations[activeAnimation].Reset();
		}
	}
	Animation& GetCurrent() { return animations[activeAnimation]; }
	SDL_Texture* GetTextureRawPointer() { return texture.get(); }

	void Update()
	{
		GetCurrent().Update();
	}

	void Render(const iPoint& position, bool useCamera = true) {
		GetCurrent().Render(GetTextureRawPointer(), position, useCamera);
	}



	void LoadAnimSet(const char* animPath);

};