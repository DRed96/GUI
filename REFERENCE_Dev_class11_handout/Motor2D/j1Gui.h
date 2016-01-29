#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"

#define CURSOR_WIDTH 2

enum mouseEvents
{
	mouseEnter,
	mouseLeave,
	mouseClick,
	mouseUnknown
};

enum uiTypes
{
	labelT,
	imageT
};


class UI_element
{
public:
	virtual void draw(){ return; }
	virtual void eventReaction(mouseEvents){ return; }
	bool isColliding();
public:
	//Rectangle of screen position
	SDL_Rect pos_rect;
	j1Module* listener = NULL;
	
protected:
	uiTypes type;

};

class UI_image : public UI_element
{
public:
	UI_image();

	~UI_image();
	void draw();
	void eventReaction(mouseEvents);
public:
	const SDL_Texture * texture;
	SDL_Rect * draw_rect;
	//TOCHANGE rect types?
	SDL_Rect tex_rect;
	SDL_Rect hover_rect;
	SDL_Rect clicked_rect;
};

class UI_label : public UI_element
{
public:
	UI_label();
	void draw();
public:
	const SDL_Texture * texture;
	char* string;
};

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions
	UI_image* createImage(SDL_Rect , SDL_Rect);
	UI_label* createLabel(char*, int, int);
	
	void checkStates();
	//void recieveEvents(mouseEvents, uiTypes);
	
	const SDL_Texture* GetAtlas() const;

public:
	mouseEvents currentEvent;
private:
	p2List<UI_element*> uis;

	SDL_Texture* atlas;
	p2SString atlas_file_name;

	
	p2List_item<UI_element*>* currentUI;
};

#endif // __j1GUI_H__