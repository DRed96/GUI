#include "GuiElements.h"
#include "j1App.h"
#include "j1Render.h"
//GUI Element-------------
void GuiElement::SetPos(int x, int y)
{
	posRect.x = x; posRect.y = y;
}

//Gui Image-------------
GuiImage::GuiImage( SDL_Rect & _section, SDL_Texture* _tex)
{
	texture = _tex;
	section = _section;

	posRect.w = section.w;
	posRect.h = section.h;
}

void GuiImage::Draw()
{
	App->render->Blit(texture, posRect.x, posRect.y, &section, 0.0f);
}