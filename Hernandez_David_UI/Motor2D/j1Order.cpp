#include "j1Order.h"

#include "j1Gui.h"

#pragma region Orders
void j1Orders::addOrder(Order& nOrder, UIButton* nButt)
{
	if (nButt != NULL)
		nOrder.SetButton(*nButt);
	orders.PushBack(&nOrder);
}

bool j1Orders::Awake(pugi::xml_node&)
{

	Grid3x3 panel;

	addOrder(o_genProbe_toss);
	addOrder(o_attack);

	panel.setOrder(o_genProbe_toss, SDL_Rect{ 468, 102, 32, 32 }, SDL_Rect{ 467, 102, 32, 32 }, SDL_Rect{ 466, 102, 32, 32 }, 0, 0, "graphics/cmdicons.png");
	panel.setOrder(o_attack, SDL_Rect{ 252, 442, 32, 32 }, SDL_Rect{ 252, 443, 32, 32 }, SDL_Rect{ 252, 441, 32, 32 }, 1, 0, "graphics/cmdicons.png");

	/*UIButton* test = App->gui->CreateUIButton({ 20, 50, 0, 0 }, { 0, 113, 229, 69 }, { 411, 169, 229, 69 }, { 642, 169, 229, 69 }, { 12, 10, 200, 47 });
	test->movable = false;
	
	test->AddListener(this);

	test->order = &o_genZergling;
	
	
	
	UIButton* test2;
	test2 = App->gui->CreateUIButton({ 0, 0, 0, 0 }, { 0, 113, 229, 69 }, { 411, 169, 229, 69 }, { 642, 169, 229, 69 }, { 12, 10, 200, 47 });

	test2->AddListener(this);

	test->order = &o_attack;*/
	
	
		//App->gui->panel.Initialize();
	return true;
}

/*
If mouse is pressed iterate all the orders untill element is equal to an orders' button
*/
void  j1Orders::GUIEvent(UIElement* element, GUI_EVENTS event)
{
	if (event == UI_MOUSE_DOWN)
	{
		for (unsigned int i = 0; i < orders.Count(); i++)
		{
			if (orders[i]->getButton() == element)
			{
				orders[i]->Function();
				break;
			}
		}		
	}
}
#pragma endregion

//Grid-------------
#pragma region Grid3x3

Grid3x3::Grid3x3()
{
	//Frame definition!
	frame = App->gui->CreateUIRect({ 496, 355, 135, 118 }, 0, 0, 0, 0);

	//Forma de guardar les coordenades dels 9 rects
	//{506,358} {552,358} {598,358}
	//{506,398} {552,398} {598,398}
	//{506,438} {552,438} {598,438}

	//Button W 32
	//Buttons H 32
	//X entre butons 4 Tileset
	//Y entre butons 3 Tileset

	//X entre butons 15 UI
	//Y entre butons 8 UI

	//1st Button pos {10,3}

	measures.x = 32;
	measures.y = 32;

	pos1 = { 10, 3 };

	margin = { 15, 8 };

	button_distance.x = measures.x + margin.x;
	button_distance.y = measures.y + margin.y;

	/*
	int x = pos1.x;
	int y = pos1.y;

	
	for (unsigned int i2 = 0, y = pos1.y; i2 < 3; i2++) // Y
	{
		for (unsigned int i3 = 0, x = pos1.x; i3 < 3; i3++) // X
		{
		unsigned int i = (i2 + i3);
			/*buttons[i] = App->gui->CreateUIButton({ x, y, 0, 0 }, "graphics/cmdicons.png", { 468, 102, 32, 32 }, { 468, 102, 32, 32 }, { 468, 102, 32, 32 });
			buttons[i]->SetParent(frame);
			buttons[i]->AddListener((j1Module*)App->orders);
			buttons[i]->order = &App->orders->o_genZergling; *
			x += (measures.x + margin.x);
		}
	y += (measures.y + margin.y);
	}
	*/
}

bool Grid3x3::setOrder(Order& assign, SDL_Rect& idle, SDL_Rect& hover, SDL_Rect& action, unsigned int index_col, unsigned int index_row, char* path, unsigned int width, unsigned int height, SDL_Rect collider)
{
	bool ret = true;
	if (index_col > 2 && index_row > 2)
	{
		LOG("Invalid row/column arguments");
		ret = false;
	}
	else
	{
		unsigned int result = index_col + index_row;

		unsigned int pX = pos1.x + (button_distance.x *index_col);
		unsigned int pY = pos1.y + (button_distance.y *index_row);

		UIButton* created = App->gui->CreateUIButton({ pX, pY, width, height }, path, idle, hover, action, collider);
		created->SetParent(frame);
		if (created == NULL)
		{
			LOG("Error at button creation");
			ret = false;
		}
		else
		{
			buttons[result] = created;
			assign.SetButton(*created);
		}
	}

	return ret;
}

Grid3x3::~Grid3x3()
{
	//Just in case despite most likely unnecessary
	for (unsigned int i = GRID_TOTAL; i < GRID_TOTAL; i++)
	{
		RELEASE(buttons[i]);
	}
}
#pragma endregion