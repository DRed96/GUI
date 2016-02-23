#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Gui.h"
#include "j1Scene.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::PostAwake(pugi::xml_node& conf)
{
	LOG("Loading Scene");
	bool ret = true;
	//EXERCISE 3
		//GenerateUI(conf);

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->console->AddCVar("pathfinding_debug", &pathfindingDebugOn);
	App->map->Load("iso_walk.tmx");
	gotStart = false; gotEnd = false; gotPath = false;
	path = NULL;


	pathfindingDebugOn = false;

	endExecution = false;

	return true;
}


// Called each loop iteration
bool j1Scene::PreUpdate()
{


	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{

	if (moving)
	{
		if (moving->GetLastEvent() != UI_KEYBOARD_FOCUSED)
		{
			if (App->input->GetMouseButtonDown(1))
			{
				int x, y;
				App->input->GetMouseMotion(x, y);
				moving->localPosition.x += x; moving->localPosition.y += y;
			}
			else
			{
				moving = NULL;
			}
		}
	}

	
	// -------
	/*if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		endExecution = true;*/

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		App->render->camera.y += floor(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		App->render->camera.y -= floor(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		App->render->camera.x += floor(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		App->render->camera.x -= floor(200.0f * dt);

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		App->gui->debug = !App->gui->debug;

	/*if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
	{
		App->gui->KeyboardFocus();
	}*/

	App->map->debug = pathfindingDebugOn;
	if (pathfindingDebugOn)
	{
		if (App->input->GetMouseButtonDown(1) == KEY_REPEAT)
		{
			int x, y;
			App->input->GetMousePosition(x, y);
			x -= App->render->camera.x;
			y -= App->render->camera.y;
			App->map->SetWalkability(App->map->WorldToMap(x, y), false);
		}
		if (App->input->GetMouseButtonDown(3) == KEY_REPEAT)
		{
			int x, y;
			App->input->GetMousePosition(x, y);
			x -= App->render->camera.x;
			y -= App->render->camera.y;
			App->map->SetWalkability(App->map->WorldToMap(x, y), true);
		}
		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_REPEAT)
		{
			int x, y;
			App->input->GetMousePosition(x, y);
			x -= App->render->camera.x;
			y -= App->render->camera.y;
			iPoint point = App->map->WorldToMap(x, y);
			if (App->map->InBoundaries(point) && point != start)
			{
				start = point;
				gotStart = true;
				gotPath = false;
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_REPEAT)
		{

			int x, y;
			App->input->GetMousePosition(x, y);
			x -= App->render->camera.x;
			y -= App->render->camera.y;
			iPoint point = App->map->WorldToMap(x, y);
			if (App->map->InBoundaries(point) && point != end)
			{
				end = point;
				gotEnd = true;
				gotPath = false;
			}
		}

		if (!gotPath && gotStart && gotEnd)
		{
			/*p2DynArray<iPoint>* newWaypoints = App->pathfinding->FindPath(start, end);
			for (int n = 0; n < newWaypoints->Count(); n++)
			{
			path->PushBack(*newWaypoints->At(n));
			}*/
			//App->pathfinding->diagonalsAllowed = true;
			path = App->pathfinding->FindPath(start, end);
			gotPath = true;
		}
	}

	App->map->Draw();

	/*int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count(),
		map_coordinates.x, map_coordinates.y);

	App->win->SetTitle(title.GetString());*/
	if (pathfindingDebugOn)
	{
		int x, y;
		App->input->GetMousePosition(x, y);
		x -= App->render->camera.x;
		y -= App->render->camera.y;
		iPoint pos = App->map->WorldToMap(x, y);
		pos = App->map->MapToWorld(pos.x, pos.y);
		SDL_Rect picture; picture.x = 128; picture.y = 0; picture.h = picture.w = 64;
		App->render->Blit(App->map->data.tilesets[1]->texture, pos.x, pos.y, &picture);

		if (gotStart)
		{
			pos = App->map->MapToWorld(start.x, start.y);
			App->render->Blit(App->map->data.tilesets[1]->texture, pos.x, pos.y, &picture);
		}
		if (gotEnd)
		{
			pos = App->map->MapToWorld(end.x, end.y);
			App->render->Blit(App->map->data.tilesets[1]->texture, pos.x, pos.y, &picture);
		}
		if (path)
		{
			for (int n = 0; n < path->Count(); n++)
			{
				pos = App->map->MapToWorld(path->At(n)->x, path->At(n)->y);
				App->render->Blit(App->map->data.tilesets[1]->texture, pos.x, pos.y, &picture);
			}
		}
	}
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (endExecution)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void j1Scene::GUIEvent(UIElement* element, GUI_EVENTS event)
{
	if ((event == UI_MOUSE_DOWN || event == UI_KEYBOARD_FOCUSED) && element->movable)
		moving = element;

	if (event == UI_SLIDER_VALUE_CHANGED && element == slider)
	{
		//The progress bar is displaying the relationship between this two numbers
		total = 100;
		part = floor(slider->GetValor() * 100);
	}
}

void j1Scene::GenerateUI(pugi::xml_node& conf)
{
	//Creating UI
	pugi::xml_node label = conf.child("ui_label");
	while (label)
	{
		int posX = label.attribute("x").as_int();
		int posY = label.attribute("y").as_int();
		bool dragable = label.attribute("drag").as_bool();
		p2SString text = label.child_value();

		
		UILabel* UILabel = App->gui->CreateUILabel({ posX, posY, 0, 0 }, text.GetString(), UI_AlignLeft);
		UILabel->movable = dragable;
		UILabel->AddListener(this);
		label = label.next_sibling();
	}

	uint screenWidth, screenHeight;
	App->win->GetWindowSize(screenWidth, screenHeight);

	//Window
	UIImage* window = App->gui->CreateUIImage({ screenWidth / 2 - 292, screenHeight / 2 - 400, 483, 650 }, { 0, 512, 483, 512 }, { 30, 40, 423, 570 });
	window->movable = true;
	window->AddListener(this);

	//Minimize Button 1
	UICollapse* collapse = App->gui->CreateUICollapse({ 50, 80, 50, 50 }, window, { 871, 704, 38, 31 }, { 795, 535, 38, 31 });

	//Window Text
	UILabel* text = App->gui->CreateUILabel({ 241, 55, 0, 0 }, "Control Panel", "fonts/open_sans/OpenSans-Regular.ttf", 40, UI_AlignCenter);
	text->SetParent(window);

	//Slider
	slider = App->gui->CreateUISlider({ 79, 435, 0, 0 }, this, { 0, 11, 307, 11 }, { 805, 318, 26, 15 }, 5, 5, -2, { 5, -3, 297, 18 });
	slider->SetParent(window);
	//The slider is the first element to be focused when tab is pressed
	App->gui->keyboardFocuseable.add(slider);

	//Input
	_TTF_Font* inputTypo = App->font->Load("fonts/open_sans/OpenSans-Bold.ttf", 18);

	UIInputBox* InBox = App->gui->CreateUIInputBox({ 64, 450, 0, 0 }, { 485, 565, 355, 68 }, { 485, 633, 355, 68 }, "Your name", 35, UI_AlignLeft, inputTypo);
	InBox->SetParent(window);
	InBox->AddListener(this);
	App->gui->keyboardFocuseable.add(InBox);

	//Input2
	InBox = App->gui->CreateUIInputBox({ 64, 515, 0, 0 }, { 485, 565, 355, 68 }, { 485, 633, 355, 68 }, "Your password", 35, UI_AlignLeft, inputTypo);
	InBox->SetParent(window);
	InBox->AddListener(this);
	InBox->hideText = true;

	App->gui->keyboardFocuseable.add(InBox);
	
	//Window 2
	UIImage* window2 = App->gui->CreateUIImage({ 0, 0, 270, 350 }, { 0, 512, 483, 512 }, { 20,25,235,300});
	window2->movable = true;
	window2->AddListener(this);
	window2->SetParent(window);

	//Minimize Button 2
	collapse = App->gui->CreateUICollapse({ 115, 100, 0, 0 }, window2, { 871, 704, 38, 31 }, { 795, 535, 38, 31 });
	collapse->SetParent(window);

	//Button
	UIButton* button = App->gui->CreateUIButton({ 20, 50, 0, 0 }, { 0, 113, 229, 69 }, { 411, 169, 229, 69 }, { 642, 169, 229, 69 }, { 12, 10, 200, 47 });
	button->movable = true;
	text = App->gui->CreateUILabel({ 115, 18, 0, 0 }, "Shoot", "fonts/open_sans/OpenSans-Regular.ttf", 18, UI_AlignCenter);
	text->SetParent(button);
	button->SetParent(window2);
	button->AddListener(this);

	_TTF_Font* buttonFont = text->GetFont();

	App->gui->keyboardFocuseable.add(button);

	//Button2
	button = App->gui->CreateUIButton({ 20, 150, 0, 0 }, { 0, 113, 229, 69 }, { 411, 169, 229, 69 }, { 642, 169, 229, 69 }, { 12, 10, 200, 47 });
	button->movable = true;
	text = App->gui->CreateUILabel({ 115, 18, 0, 0 }, "Shields", UI_AlignCenter, buttonFont);
	text->SetParent(button);
	button->SetParent(window2);
	button->AddListener(this);

	App->gui->keyboardFocuseable.add(button);

	//Button 3
	button = App->gui->CreateUIButton({ 20, 250, 0, 0 }, { 0, 113, 229, 69 }, { 411, 169, 229, 69 }, { 642, 169, 229, 69 }, { 12, 10, 200, 47 });
	button->movable = true;
	text = App->gui->CreateUILabel({ 115, 18, 0, 0 }, "Warp", UI_AlignCenter, buttonFont);
	text->SetParent(button);
	button->SetParent(window2);
	button->AddListener(this);

	App->gui->keyboardFocuseable.add(button);

	//Loading bar
	UIImage* bar = App->gui->CreateUIImage(SDL_Rect{ screenWidth / 2 - 466, screenHeight - 90, 933, 112 }, SDL_Rect{ 0, 400, 933, 112 }, { 35,30,863, 50});
	bar->movable = true;
	bar->AddListener(this);
	SDL_Rect rects[100];
	for (int n = 0; n < 100; n++)
	{
		rects[n].x = 0;      rects[n].y = 275;
		rects[n].h = 40;     rects[n].w = n / 100.0f * 860;
	}
	App->gui->CreateUIAnimatedImage(SDL_Rect{ 37, 35, 0, 0 }, rects, 100, 10.0f)->SetParent(bar);
	text = App->gui->CreateUILabel(SDL_Rect{ 570, 35, 0, 0 }, "This bar is an animation", "fonts/open_sans/OpenSans-Italic.ttf", 14, UI_AlignLeft);
	text->SetParent(bar);
	_TTF_Font* loadingFont = text->GetFont();

	text = App->gui->CreateUILabel(SDL_Rect{ 570, 55, 0, 0 }, "There's a loop of 100 frames", UI_AlignLeft, loadingFont);
	text->SetParent(bar);
	App->gui->CreateUILabel(SDL_Rect{ 150, 45, 0, 0 }, "Loading...", UI_AlignLeft, loadingFont)->SetParent(bar);
	//Loading bar end
	

	//Loading bar 2
	bar = App->gui->CreateUIImage(SDL_Rect{ screenWidth / 2 - 466, -10, 933, 112 }, SDL_Rect{ 0, 400, 933, 112 }, { 35, 30, 863, 50 });
	bar->movable = true;
	bar->AddListener(this);

	App->gui->CreateUIProgressBar(SDL_Rect{ 37, 35, 0, 0 }, {0,275,860,40}, &total, &part)->SetParent(bar);
	text = App->gui->CreateUILabel(SDL_Rect{ 570, 35, 0, 0 }, "This bar is a progress bar", UI_AlignLeft, loadingFont);
	text->SetParent(bar);
	text = App->gui->CreateUILabel(SDL_Rect{ 570, 55, 0, 0 }, "Linked to the slider", UI_AlignLeft, loadingFont);
	text->SetParent(bar);
	loadingFont = text->GetFont();
	App->gui->CreateUILabel(SDL_Rect{ 150, 45, 0, 0 }, "Loading...", UI_AlignLeft, loadingFont)->SetParent(bar);
	//Loading bar end
}