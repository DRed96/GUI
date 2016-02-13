#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"

#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes
class GuiElement;

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

	// Called each loop iteration
	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions
	void createImage(SDL_Rect& section, SDL_Texture* = NULL,  int x = 0,  int y = 0);
	void createLabel(char * _string,  int x= 0,  int y = 0);
	void createButton(SDL_Rect &, SDL_Rect &, SDL_Rect &, SDL_Texture* = NULL,  int x = 0,  int y = 0);
	//Const, how?

	//Checks mouse hover
	bool checkMouseHover();

	const SDL_Texture* GetAtlas() const;
private:
	SDL_Texture* atlas;
	p2SString atlas_file_name;

	p2List<GuiElement*> guis;

	
};

#endif // __j1GUI_H__