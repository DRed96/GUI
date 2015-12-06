#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"

#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes
class UI_element
{
public:

	virtual void draw(){ return; }
public:
	//Rectangle of screen position
	SDL_Rect pos_rect;

};

class UI_image : public UI_element
{
public:
	void draw();
public:
	const SDL_Texture * texture;
	SDL_Rect tex_rect;
};

class UI_label : public UI_element
{
public:
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
	UI_image* createImage(SDL_Rect rect, SDL_Rect rect2);
	UI_label* createLabel(char*, int , int );

	//TODO destruir-ho tot una mica
	const SDL_Texture* GetAtlas() const;

private:
	p2List<UI_element*> uis;
	SDL_Texture* atlas;
	p2SString atlas_file_name;
};

#endif // __j1GUI_H__