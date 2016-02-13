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

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

bool j1Gui::Update(float dt)
{
	checkMouseHover();
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

void j1Gui::createImage(SDL_Rect& _section, SDL_Texture* _tex, const int x, const int y)
{
	if (_tex == NULL)
	{
		_tex = atlas;
	}

	GuiElement* ret = new GuiImage(_section, _tex);
	ret->SetPosition(x, y);

	guis.add(ret);

}

void j1Gui::createLabel(char * _string, const int x, const int y)
{
	if (_string != NULL)
	{
		GuiElement* ret = new GuiLabel(_string);
		ret->SetPosition(x, y);

		guis.add(ret);
	}
}

bool j1Gui::checkMouseHover()
{
	p2List_item<GuiElement*>*it = guis.start;

	bool inside = false;

	while (it != NULL)
	{
		int m_x, m_y;
		App->input->GetMousePosition(m_x, m_y);

		SDL_Rect rect = it->data->GetPosRect();

		//Check collsion
		if (m_x > rect.x && m_x < (rect.x + rect.w))
		{
			if (m_y > rect.y && m_y < (rect.y + rect.h))
			{
				inside = true;
				it->data->onAction(mouse_enter);
			}
		}
		if (!inside && it->data->current_state == (gui_hover || gui_action))
		{
			it->data->onAction(mouse_leave);
		}
		it = it->next;
	}
	return false;
}