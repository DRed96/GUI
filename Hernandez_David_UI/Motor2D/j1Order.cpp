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
	test->movable = true;
	UILabel* text = App->gui->CreateUILabel({ 115, 18, 0, 0 }, "Generate zergling", "fonts/open_sans/OpenSans-Regular.ttf", 18, UI_AlignCenter);
	text->SetParent(test);
	test->AddListener((j1Module*)App->scene);

	
	addOrder(o_genZergling,test);

	
	return true;
}

//Set a sistem of GUI events like in the scene