#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1Gui.h"
#include "j1Console.h"

struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool PostAwake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void GUIEvent(UIElement* element, GUI_EVENTS event);

	void GenerateUI(pugi::xml_node& conf);


private:
	bool endExecution = false;

	//Moving UI element
	UIElement* moving = NULL;

	//Progress bar debug
	int total = 845;
	int part = 0;
	UISlider* slider;

	//Pathfinding
private:
	iPoint start;
	iPoint end;
	p2DynArray<iPoint>* path;

	bool gotStart;
	bool gotEnd;
	bool gotPath;

	bool pathfindingDebugOn;
};

#endif // __j1SCENE_H__