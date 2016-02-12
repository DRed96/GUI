#ifndef GUI_ELEMENTS
#define GUI_ELEMENTS

#include "SDL/include/SDL.h"

class GuiElement
{
public:
	GuiElement(){}
	~GuiElement(){}

	virtual void Draw(){}
	void SetPos(int x, int y);
	//GetRect

//private:
//protected:
	SDL_Rect posRect;
};

class GuiImage : public GuiElement
{
public:
	GuiImage(SDL_Rect & section, SDL_Texture* tex);
	~GuiImage(){};

	void Draw();
//private:
	SDL_Texture* texture;
	SDL_Rect section;
};
#endif // !GUI_ELEMENTS
