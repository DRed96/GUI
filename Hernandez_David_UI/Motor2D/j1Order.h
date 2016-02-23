#ifndef _ORDERS_H_
#define _ORDERS_H_

#define GRID_TOTAL 9

//This 2 includes need to be changed to the cpp and also the methods that use them
#include "j1App.h"
#include "p2Log.h"

#include "j1Module.h"

#include "p2DynArray.h"

#include "p2Point.h"

#include "SDL\include\SDL.h"
class UIButton;
class UIRect;

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
struct Gen_probe : public Order
{
	Gen_probe() :Order(){}
	void Function()
	{
		/*
		Maybe
		ordersMod->GenerateUnit(Zergling);
		*/
		//ordersMod->Generate_Zergling();
		LOG("Generate Probe");
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

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	void GUIEvent(UIElement* element, GUI_EVENTS event);

	/*
	Add an order to the list and assign it a button
	Recomendended to use with grid3x3 setOrder!!!!!
	*/
	void addOrder(Order&, UIButton* = NULL);

	
private:
	Gen_probe o_genProbe_toss;
	Attack o_attack;

	p2DynArray<Order*> orders;
};

#pragma region Grid3x3
class Grid3x3
{
public:

	Grid3x3();
	~Grid3x3();
	
	/*
	Declare an order and assign it a position into de 3x3 Grid
	(Columns and rows go from 0 to 2)	
	-Idle: The rect that will normally be displayed
	-Hover: Hover rect
	-Click: The one that will appear when using the button
	-Index_col: Column of the button.
	-Index_row: Row of the button.
	-Path: The path of the file where the button is if NULL will use atlas
	- Width & Heigh: In case you want to resize the button
	RETURN: Returns a pointer to the created button so it can be edited NULL on fail
	Order's button WILL be changed
	*/
	UIButton* setOrder(Order& assign, SDL_Rect& idle, SDL_Rect& hover, SDL_Rect& action, unsigned int index_col, unsigned int index_row, char* path = NULL, unsigned int width = 0, unsigned int height = 0, SDL_Rect collider = { 0, 0, 0, 0 } );

private:

	UIButton* buttons[GRID_TOTAL];

	//Invisible frame, parent of all the buttons
	UIRect*  frame;

	// Position of the first button
	p2Point<int> pos1;

	//Margin between buttons
	p2Point<unsigned int> margin;

	//Width and height of the buttons
	p2Point <unsigned int> measures;

	//Measures + margin sum
	p2Point <unsigned int> button_distance;
};

#pragma endregion
#endif // !_ORDERS_H_
