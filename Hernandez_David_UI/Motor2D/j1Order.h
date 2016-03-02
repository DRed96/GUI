#ifndef _ORDERS_H_
#define _ORDERS_H_

#define GRID_TOTAL 9

//This 2 includes need to be changed to the cpp and also the methods that use them

#include "j1Module.h"

#include "p2DynArray.h"

#include "p2Point.h"

#include "Orders Factory.h"

#include "SDL\include\SDL.h"




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
	void addOrder(Order&, UIButton2* = NULL);

	
private:
	Gen_probe o_genProbe_toss;
	Attack o_attack;

	p2DynArray<Order*> orders;
};

#pragma region Grid3x3

struct Grid_Coords
{
	Grid_Coords();
	//~Grid_Coords();

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
class Grid3x3
{
public:

	Grid3x3(Grid_Coords&);
	~Grid3x3();
	
	/*
	Declare an order and assign it a position into de 3x3 Grid using a texture path
	(Columns and rows go from 0 to 2)	
	-Idle: The rect that will normally be displayed
	-Click: The one that will appear when using the button
	-Index_col: Column of the button.
	-Index_row: Row of the button.
	-Icon: The image that will be parented to the button
	-Path: The path of the file where the button is if NULL will use atlas
	-ToRedender: Will set the bool active to the button and it's sons 
	- Width & Heigh: In case you want to resize the button
	RETURN: Returns a pointer to the created button so it can be edited NULL on error
	Order's button WILL be changed
	*/
	UIButton2* setOrder(const SDL_Rect & idle, const SDL_Rect & clicked, unsigned int row_index, unsigned int col_index, char* path = NULL, bool _toRender = false, unsigned int width = 0, unsigned int height = 0, SDL_Rect collider = { 0, 0, 0, 0 });
	/*
	Declare an order and assign it a position into de 3x3 Grid using
	(Columns and rows go from 0 to 2)
	-Idle: The rect that will normally be displayed
	-Click: The one that will appear when using the button
	-Index_col: Column of the button.
	-Index_row: Row of the button.
	-Tex: The texture that will use the button so we just load it once
	- Width & Heigh: In case you want to resize the button
	RETURN: Returns a pointer to the created button so it can be edited NULL on error
	Order's button WILL be changed
	*/
	UIButton2* setOrder(const SDL_Rect & idle, const SDL_Rect & clicked, unsigned int row_index, unsigned int col_index, SDL_Texture& tex, bool _toRender = false, unsigned int width = 0, unsigned int height = 0, SDL_Rect collider = { 0, 0, 0, 0 });

	/*
	Change the location of a pregenerated button into a grid, not good for testing
	*/
	UIButton2* setOrder(unsigned int row_index, unsigned int col_index, UIButton2 & button);

private:

	Grid_Coords* coords;

	UIButton2* buttons[GRID_TOTAL];
};

#pragma endregion
#endif // !_ORDERS_H_
