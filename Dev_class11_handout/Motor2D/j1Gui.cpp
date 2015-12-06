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

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
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

UI_image* j1Gui::createImage(SDL_Rect pos, SDL_Rect texture)
{
	UI_image* ret = new UI_image;
	ret->pos_rect = pos;
	ret->tex_rect = texture;
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
// UI Image		--------------------
void UI_image::draw()
{
	App->render->Blit((SDL_Texture*)texture, pos_rect.x, pos_rect.y, &tex_rect, 0);
}
// UI Label		--------------------
void UI_label::draw()
{
	App->render->Blit((SDL_Texture*)texture, pos_rect.x, pos_rect.y, NULL, 0);
}