#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "GuiElements.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());
	current_mouse = mouse_unknown;
	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

bool j1Gui::Update(float dt)
{
	/*
	-Iterate the guis
	-Checks hover if the mouse is not already inside
	-If it detects new hover, state is mouse_enter
	-If enter it sends 1 event of mouse enter
	*/
	p2List_item<GuiElement*>*it = guis.start;

	bool hover = false;
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
	{
		char debug = '\0';
	}

	bool inside = false;

	switch (current_mouse)
	{

	case mouse_click:
	case mouse_enter:
	{
		j1KeyState mouse = App->input->GetMouseButtonDown(SDL_BUTTON_LEFT);

		
		if (mouse == KEY_DOWN || mouse == KEY_REPEAT)
		{
			if (current_mouse != mouse_click)
			{
				current_mouse = mouse_click;
				currentGui->onAction(current_mouse);
				LOG("Mouse click!");
			}
		}
		else if (current_mouse == mouse_click)
		{
			current_mouse = mouse_enter;
			currentGui->onAction(current_mouse);
			LOG("Mouse unclick!");
		}

		if (checkMouseHover(currentGui) == false)
		{
			current_mouse = mouse_leave;
			currentGui->onAction(current_mouse);

			currentGui = NULL;
			LOG("Mouse left!");
		}
	}
		break;

	case mouse_leave:
	case mouse_unknown:
	default:
		while (it != NULL && !hover)
		{
			if (checkMouseHover(it->data) == true)
			{
				current_mouse = mouse_enter;
				it->data->onAction(current_mouse);
				hover = true;

				currentGui = it->data;
				LOG("Mouse inside!");
			}
			it = it->next;
		}
		break;
	}
	/*
	while (it != NULL && current_mouse != mouse_enter)
	{
		if (checkMouseHover(it->data) == true)
		{
			current_mouse = mouse_enter;
			it->data->onAction(mouse_enter);

			currentGui = it->data;
			LOG("Mouse inside!");
		}
		it = it->next;
	}

	if (current_mouse == mouse_enter)
	{
		j1KeyState mouse = App->input->GetMouseButtonDown(SDL_BUTTON_LEFT);

		if (mouse == KEY_DOWN)
		{
			//current_mouse = mouse_click;
			currentGui->onAction(mouse_click);
			LOG("Mouse click!");
		}
	//	else
   //		currentGui->onAction(mouse_enter);

		if (checkMouseHover(currentGui) == false)
		{
			current_mouse = mouse_leave;
			currentGui->onAction(mouse_leave);

			currentGui = NULL;
			LOG("Mouse left!");
		}
	}
	*/
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	p2List_item<GuiElement*>*it = guis.start;
	while (it)
	{
		it->data->Draw();
		it = it->next;
	}
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	guis.clear();
	return true;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------

void j1Gui::createImage(SDL_Rect& _section, SDL_Texture* _tex,  int x,  int y)
{
	SDL_Texture* used_tex = _tex;
	if (_tex == NULL)
	{
		used_tex = atlas;
	}

	GuiElement* ret = new GuiImage(_section, used_tex);
	ret->SetPosition(x, y);

	guis.add(ret);

}

void j1Gui::createLabel(char * _string,  int x,  int y)
{
	if (_string != NULL)
	{
		GuiElement* ret = new GuiLabel(_string);
		ret->SetPosition(x, y);

		guis.add(ret);
	}
}

void j1Gui::createButton(SDL_Rect & _idle, SDL_Rect & _hover, SDL_Rect &_active , SDL_Texture* _tex, int x, int y)
{
	//Use atlas as default
	SDL_Texture* used_tex = _tex;
	if (_tex == NULL)
	{
		used_tex = atlas;
	}

	//What happens if you use sdl texture reference?
	GuiElement* ret = new GuiButton(*used_tex, _idle, _hover, _active);
	ret->SetPosition(x, y);

	guis.add(ret);
}

bool j1Gui::checkMouseHover(GuiElement* element)
{

	bool inside = false;
	
	int m_x, m_y;
	App->input->GetMousePosition(m_x, m_y);

	SDL_Rect rect = element->GetPosRect();

	//Check collsion
	if (m_x > rect.x && m_x < (rect.x + rect.w))
	{
		if (m_y > rect.y && m_y < (rect.y + rect.h))
		{
			inside = true;
		}
	}

	return inside;
}

/*	LOG("Mouse Enter");
			inside = true;
			it->data->onAction(mouse_enter);
			if (App->input->GetMouseButtonDown(KEY_DOWN) == KEY_DOWN || App->input->GetMouseButtonDown(KEY_REPEAT) == KEY_REPEAT)
			{
				LOG("Mouse Clicked");
					it->data->onAction(mouse_click);
					}
	if (!inside && it->data->current_state == (gui_hover || gui_action)) //Aquest operador boolea no esta ben posat
	{
		LOG("Mouse Left");
		it->data->onAction(mouse_leave);
	}
					*/