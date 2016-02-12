#ifndef GUI_ELEMENTS_H
#define GUI_ELEMENTS_H

#include "SDL/include/SDL.h"

class GuiElement
{
public:
	GuiElement(){}
	~GuiElement(){}

	virtual void Draw(){}
	void SetPosition(int x, int y);
	void SetRect(int x, int y, int w, int h);
	
	SDL_Rect GetRect() const;

private:
protected:
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
	char* string;
};
#endif // !GUI_ELEMENTS_H
