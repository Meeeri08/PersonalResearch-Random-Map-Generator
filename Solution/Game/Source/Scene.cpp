#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "RandomMapGenerator.h"

#include "Defs.h"
#include "Log.h"


Scene::Scene() : Module()
{
	name.create("scene");
}

// Destructor
Scene::~Scene()
{
}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	waterTex = app->tex->Load("Assets/textures/water.png");
	sandTex = app->tex->Load("Assets/textures/sand.png");
	grassTex = app->tex->Load("Assets/textures/grass.png");
	forestTex = app->tex->Load("Assets/textures/forest.png");

	app->random_map->generateNoise(app->random_map->generateSeed());
	
	return true;

}

// Called each loop iteration
bool Scene::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += 20;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= 20;

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += 20;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= 20;

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		app->random_map->generateNoise(app->random_map->generateSeed());

	app->map->DrawRandomMap();


	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	static char title[256];
	sprintf_s(title, 256, "Random Map Generator ");
	app->win->SetTitle(title);

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
