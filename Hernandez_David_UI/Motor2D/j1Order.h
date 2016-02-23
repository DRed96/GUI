#ifndef _ORDERS_H_
#define _ORDERS_H_

//This 2 includes need to be changed to the cpp and also the methods that use them
#include "j1App.h"
#include "p2Log.h"

#include "j1Module.h"

#include "p2DynArray.h"

class UIButton;

class Order
{
public:
	Order(){ button = NULL; }

	virtual void Function(){}

	//Setters & Getters
	void SetButton(UIButton& nButt){ button = &nButt; }

	const UIButton* getButton() const { return button; }

	

private:
	UIButton* button;

	
};

#pragma region Orders
//Orders
struct Gen_Zergling : public Order
{
	Gen_Zergling() :Order(){}
	void Function()
	{
		/*
		Maybe
		ordersMod->GenerateUnit(Zergling);
		*/
		//ordersMod->Generate_Zergling();
		LOG("Generate Zergling");
	}
};

struct Attack : public Order
{
	Attack() :Order(){}
	void Function()
	{
		/*
		Maybe
		ordersMod->GenerateUnit(Zergling);
		*/
		//ordersMod->Generate_Zergling();
		LOG("Attack!");
	}
};
#pragma endregion

class j1Orders : public j1Module
{
public:

	bool partial_solution = false;

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	void GUIEvent(UIElement* element, GUI_EVENTS event);

	//Add an order to the list and assign it a button
	void addOrder(Order&, UIButton*);

	Gen_Zergling o_genZergling;
	Attack o_attack;
private:
	
	p2DynArray<Order*> orders;

	
};


#endif // !_ORDERS_H_
