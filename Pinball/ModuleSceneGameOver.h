#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"

class PhysBody;


class ModuleSceneGameOver : public Module
{
public:
	ModuleSceneGameOver(Application* app, bool start_enabled = true);
	~ModuleSceneGameOver();

	

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	void SceneChange();

private:
	SDL_Texture* game_over_tex;
	int font_score = -1;
	char score_text[10];

};

