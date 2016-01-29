#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"

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
	currentEvent = mouseUnknown;
	currentUI = NULL;
	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	checkStates();
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	p2List_item<UI_element*>* tmp = uis.start;
	while (tmp)
	{
		tmp->data->draw();
		tmp = tmp->next;
	}
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	p2List_item<UI_element*>* tmp = uis.start;
	while (tmp)
	{
		RELEASE(tmp->data);
		tmp = tmp->next;
	}
	return true;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------

UI_image* j1Gui::createImage(SDL_Rect pos, SDL_Rect rtex)
{
	UI_image* ret = new UI_image;
	ret->pos_rect = pos;
	ret->tex_rect = rtex;
	ret->pos_rect.h = rtex.h;
	ret->pos_rect.w = rtex.w;
	ret->texture = App->gui->GetAtlas();

	uis.add(ret);
	return ret;
}

UI_label* j1Gui::createLabel(char* str, int _x, int _y)
{
	UI_label* ret = new UI_label();

	ret->pos_rect.x = _x;
	ret->pos_rect.y = _y;
	ret->string = str;

	ret->texture = App->font->Print(str);
	App->font->CalcSize(str, ret->pos_rect.w, ret->pos_rect.h);

	uis.add(ret);
	return ret;
}
void j1Gui::checkStates()
{
	
	p2List_item<UI_element*>* tmp = uis.start;

	switch (currentEvent)
	{
	case mouseLeave:
		currentUI = NULL;
	case mouseUnknown:
	default:
		while (tmp)
		{
			if (tmp->data->isColliding())
			{
				currentEvent = mouseEnter;
				tmp->data->listener->recieveEvents(currentEvent, tmp->data);
				currentUI = tmp;
			}
			tmp = tmp->next;
		}
		break;
	case mouseEnter:
		if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
		{
			char debug = '\0';
		}
		if (currentUI->data->isColliding() == false)
		{
			currentEvent = mouseLeave;
			tmp->data->listener->recieveEvents(currentEvent, currentUI->data);
		}
		break;
	}
	
}

// UI Element	--------------------
bool UI_element::isColliding()
{
	bool ret = false;
	int mx, my;
	App->input->GetMousePosition(mx, my);

	if (mx > pos_rect.x && mx < (pos_rect.x + pos_rect.w) &&
		my > pos_rect.y && my < (pos_rect.y + pos_rect.h))
	{
		ret = true;
	}
	return ret;
}
// UI Image		--------------------
UI_image::UI_image()
{
	draw_rect = &tex_rect;
	
	type = imageT; 
	listener = (j1Module*)App->scene; 
}

UI_image::~UI_image()
{}
void UI_image::draw()
{	
	App->render->Blit((SDL_Texture*)texture, pos_rect.x, pos_rect.y, draw_rect, 0);
}
void UI_image::eventReaction(mouseEvents _m)
{
	switch (_m)
	{
	case mouseEnter:
		draw_rect = &hover_rect;
		break;
	default:
		draw_rect = &tex_rect;
		break;
	}
}
// UI Label		--------------------
UI_label::UI_label()
{ 
	type = labelT; 
	listener = (j1Module*)App->scene; 
}
void UI_label::draw()
{
	App->render->Blit((SDL_Texture*)texture, pos_rect.x, pos_rect.y, NULL, 0);
}