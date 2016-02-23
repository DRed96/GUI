#include "j1Gui.h"
#include "j1Input.h"


// --------------- GUI MODULE --------------------------------------------------------


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

	atlas_file_name = conf.child("atlas").attribute("file").as_string();
	
	atlas = App->tex->Load(atlas_file_name.GetString());

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	focus = NULL;
	panel.Initialize();
	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

// Called after all Updates
bool j1Gui::Update(float dt)
{
	App->render->useCamera = false;

	p2List_item<UIElement*>* item = UIElements.start;
	//Item update
	for (int n = 0; n <= N_GUI_LAYERS; n++)
	{
		item = UIElements.start;
		while (item)
		{
			if (item->data->layer == n)
			{
				item->data->Update(dt);
			}
			item = item->next;
		}
	}

	//Input update, focus management
	if (focus != NULL)
	{
		if (focus->GetActive() == false)
		{
			focus = NULL;
		}
		UIElement* focused = focus;
		item = UIElements.end;
		//Input for elements above the focus
		while (item && focus == focused)
		{
			item->data->InputManager();
			item = item->prev;
			if (item && focus == item->data)
			{
				focus->InputManager();
				item = item->prev;
				break;
			}
		}
		//If the mouse isn't inside the focused object, check the ones below
		while (item && focus)
		{
			if (focus->GetLastEvent() == UI_MOUSE_EXIT)
			{
				item->data->InputManager();
			}
			else
			{
				item->data->ForceLastEvent(UI_MOUSE_EXIT);
			}
			item = item->prev;
		}
	}
	//If there's no focus, check all elements until finding one
	else
	{
		item = UIElements.end;
		while (item && focus == NULL)
		{
			item->data->InputManager();
			item = item->prev;
		}
	}
	//Necessary for Input_Box
	if (focus)
	{
		if (focus->GetType() != UI_InputBox)
		{
			App->input->DisableTextInput();
		}
	}

	App->render->useCamera = true;
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	p2List_item<UIElement*>* item = UIElements.start;
	while (item)
	{
		p2List_item<UIElement*>* nextItem = item->next;
		RELEASE(item->data);
		item = nextItem;
	}
	UIElements.clear();
	return true;
}

// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

void j1Gui::KeyboardFocus()
{
	if (keyboardFocuseable.count() > 0)
	{
		p2List_item<UIElement*>* toFocus = NULL;

		int index = keyboardFocuseable.find(focus);
		if (index != -1)
		{
			while (toFocus == NULL || toFocus->data->GetActive() == false)
			{
				toFocus = keyboardFocuseable.At(index + 1);
			}
			if (toFocus == NULL)
			{
				toFocus = keyboardFocuseable.start;
			}
		}
		else
		{
			toFocus = keyboardFocuseable.start;
			while (toFocus->data->GetActive() == false)
			{
				toFocus = toFocus->next;
			}
		}
		if (focus)
		{
			focus->ForceLastEvent(UI_NONE);
		}
		focus = toFocus->data;
		focus->ForceLastEvent(UI_KEYBOARD_FOCUSED);
	}
}

// Load Game State
bool j1Gui::Load(pugi::xml_node& data)
{
	pugi::xml_node positions = data.child("UI_Element_Positions");
	pugi::xml_node element = positions.child("UI_Element");
	p2List_item<UIElement*>* item = UIElements.start;
	while (element)
	{
		item = UIElements.start;
		int ID = element.attribute("ID").as_int();
		while (item && item->data->GetId() != ID)
		{
			item = item->next;
		}
		if (item && item->data->movable)
		{
			item->data->localPosition.x = element.attribute("x").as_int();
			item->data->localPosition.y = element.attribute("y").as_int();
		}

		element = element.next_sibling();
	}

	return true;
}

// Save Game State
bool j1Gui::Save(pugi::xml_node& data) const
{
	pugi::xml_node positions = data.append_child("UI_Element_Positions");
	pugi::xml_node element;
	p2List_item<UIElement*>* item = UIElements.start;
	while (item)
	{
		if (item->data->movable)
		{
			element = positions.append_child("UI_Element");
			element.append_attribute("ID") = item->data->GetId();
			element.append_attribute("x") = item->data->localPosition.x;
			element.append_attribute("y") = item->data->localPosition.y;
		}
		item = item->next;
	}

	return true;
}

//Grid-------------

void Grid3x3::Initialize()
{
	//with and height
	frame = App->gui->CreateUIRect({ 496, 355, 135, 118 }, 0, 0, 0, 0);
	//Forma de guardar les coordenades dels 9 rects
	//{506,358} {552,358} {598,358}
	//{506,398} {552,398} {598,398}
	//{506,438} {552,438} {598,438}

	//Button W 33
	//Buttons H 33
	//X entre butons 15
	//Y entre butons 8
	//1st Button pos {10,3}

	measures.x = 33;
	measures.y = 33;

	p2Point<int> pos1 = { 10, 3 };

	int x = pos1.x;
	int y = pos1.y;

	//Buttons.count

	
	
	for (unsigned int i2 = 0, y = pos1.y; i2 < 3; i2++) // Y
	{
		for (unsigned int i3 = 0, x = pos1.x; i3 < 3; i3++) // X
		{
			buttons[(i2 + i3)] = App->gui->CreateUIButton({ x, y, 0, 0 }, "graphics/cmdicons.png", { 468, 102, 32, 32 }, { 468, 102, 32, 32 }, { 468, 102, 32, 32 });
			buttons[(i2 + i3)]->SetParent(frame);
			x += (measures.x + 15);
		}
		y += (measures.y + 8);
	}
			
}

Grid3x3::~Grid3x3()
{

}