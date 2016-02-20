#ifndef _ORDERS_H_
#define _ORDERS_H_
#include "UI_Element.h"
#include "j1Module.h"

#include "p2DynArray.h"
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
	Order(orderTypes _type){ type = _type; ordersMod = App->orders; }

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
	j1Orders* ordersMod;
};

class j1Orders : public j1Module
{
public:

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	//Add an order to the list and assign it a button
	void addOrder(Order&, UIButton*);

private:
	p2DynArray<Order*> orders;
	Gen_Zergling o_genZergling;

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
#endif // !_ORDERS_H_
