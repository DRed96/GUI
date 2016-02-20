#ifndef _ORDERS_H_
#define _ORDERS_H_

//This 2 includes need to be changed to the cpp and also the methods that use them
#include "j1App.h"
#include "p2Log.h"

#include "j1Module.h"

#include "p2DynArray.h"

class UIButton;
enum orderTypes
{
	attack,
	gen_zergling
};

enum unitTypes
{
	zergling
};


class Order
{
public:
	Order(orderTypes _type){ type = _type; /*ordersMod = App->orders;*/ button = NULL; }

	virtual void Function(){}

	//Setters & Getters
	void SetButton(UIButton& nButt){ button = &nButt; }
	void SetType(const orderTypes& nType) { type = nType; }

	orderTypes getType()const { return type; }
	const UIButton* getButton() const { return button; }

private:
	orderTypes type;
	//The button is asigned later
	UIButton* button;
protected:
	//Little optimization, maybe
	//j1Orders* ordersMod;
};

#pragma region Orders
//Orders
struct Gen_Zergling : public Order
{
	Gen_Zergling() :Order(gen_zergling){}
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
#pragma endregion

class j1Orders : public j1Module
{
public:

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	//Add an order to the list and assign it a button
	void addOrder(Order&, UIButton*);

	//void GUIEvent(UIElement* element, GUI_EVENTS event);

private:
	p2DynArray<Order*> orders;

	Gen_Zergling o_genZergling;

};


#endif // !_ORDERS_H_
