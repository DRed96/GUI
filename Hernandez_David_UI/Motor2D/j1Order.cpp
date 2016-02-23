#include "j1Order.h"

#include "j1Gui.h"

void j1Orders::addOrder(Order& nOrder, UIButton* nButt)
{
	nOrder.SetButton(*nButt);
	orders.PushBack(&nOrder);
}

bool j1Orders::Awake(pugi::xml_node&)
{

	UIButton* test = App->gui->CreateUIButton({ 20, 50, 0, 0 }, { 0, 113, 229, 69 }, { 411, 169, 229, 69 }, { 642, 169, 229, 69 }, { 12, 10, 200, 47 });
	test->movable = false;
	
	test->AddListener(this);

	test->order = &o_genZergling;
	addOrder(o_genZergling,test);
	
	
	UIButton* test2;
	test2 = App->gui->CreateUIButton({ 0, 0, 0, 0 }, { 0, 113, 229, 69 }, { 411, 169, 229, 69 }, { 642, 169, 229, 69 }, { 12, 10, 200, 47 });

	test2->AddListener(this);

	test->order = &o_attack;
	addOrder(o_attack, test2);
	


	UIButton* grid1[GRID_TOTAL];
	grid1[0] = test;
	grid1[1] = test2;

	App->gui->panel.Initialize(grid1);
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