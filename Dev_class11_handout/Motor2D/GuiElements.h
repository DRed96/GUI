#ifndef GUI_ELEMENTS_H
#define GUI_ELEMENTS_H

#include "SDL/include/SDL.h"

enum mouse_states
{
	mouse_enter,
	mouse_leave,
	mouse_click,
	mouse_unknown
};

enum gui_states
{
	gui_idle,
	gui_hover,
	gui_action
};

class GuiElement
{
public:
	GuiElement(){}
	~GuiElement(){}

	//Draws the guis
	virtual void Draw(){}

	//virtual void Update(){}
	virtual void onAction(mouse_states){}

	//Sets position of the posRect
	void SetPosition(int x, int y);
	//Sets the whole posRect
	void SetRect(int x, int y, int w, int h);
	
	SDL_Rect GetPosRect() const;

public:
	gui_states current_state;
private:
protected:
	//Position rect, also used for collsion
	SDL_Rect posRect;
};

class GuiImage : public GuiElement
{
public:
	GuiImage(SDL_Rect & section, SDL_Texture* tex);
	~GuiImage(){};

	void Draw();
private:
	SDL_Texture* texture;
	SDL_Rect section;
};

class GuiLabel : public GuiElement
{
public:
	GuiLabel(char * text);
	~GuiLabel(){};

	void Draw();
private:
	SDL_Texture* texture;
	//char* string;
};

class GuiButton : public GuiElement
{
public:

	GuiButton(SDL_Texture&, SDL_Rect &, SDL_Rect &, SDL_Rect &);
	~GuiButton(){}

	void Draw();

	void onAction(mouse_states);

private:

	
	//Texture rects
	SDL_Texture* texture;
	SDL_Rect* current_section;
	SDL_Rect idle;
	SDL_Rect hover;
	SDL_Rect action;
};
#endif // !GUI_ELEMENTS_H
