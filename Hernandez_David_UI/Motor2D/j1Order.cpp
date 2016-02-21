#include "j1Order.h"

#include "j1Gui.h"

void j1Orders::addOrder(Order& nOrder, UIButton* nButt)
{
	nOrder.SetButton(*nButt);
	orders.PushBack(&nOrder);
}


bool j1Orders::Awake(pugi::xml_node&)
{

	//Que passa amb els punters? Important

	UIButton* test = App->gui->CreateUIButton({ 20, 50, 0, 0 }, { 0, 113, 229, 69 }, { 411, 169, 229, 69 }, { 642, 169, 229, 69 }, { 12, 10, 200, 47 });
	test->movable = false;
	UILabel* text = App->gui->CreateUILabel({ 115, 18, 0, 0 }, "Generate zergling", "fonts/open_sans/OpenSans-Regular.ttf", 18, UI_AlignCenter);
	text->SetParent(test);
	
	test->AddListener(this);

	test->order = &o_genZergling;
	addOrder(o_genZergling,test);
	
	
	UIButton* test2;
	test2 = App->gui->CreateUIButton({ 20, 120, 0, 0 }, { 0, 113, 229, 69 }, { 411, 169, 229, 69 }, { 642, 169, 229, 69 }, { 12, 10, 200, 47 });
	
	text = App->gui->CreateUILabel({ 115, 18, 0, 0 }, "Attack!", "fonts/open_sans/OpenSans-Regular.ttf", 18, UI_AlignCenter);
	text->SetParent(test2);

	test2->AddListener(this);

	test->order = &o_attack;
	addOrder(o_attack, test2);
	
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