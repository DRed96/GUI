#include "GuiElements.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Fonts.h"
//GUI Element-------------
void GuiElement::SetPosition(int x, int y)
{
	posRect.x = x; posRect.y = y;
}
void GuiElement::SetRect(int x, int y, int w, int h)
{
	posRect.x = x; 	posRect.y = y; 	posRect.w = w; 	posRect.h = h;
}

SDL_Rect GuiElement::GetRect() const
{
	return posRect;
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

//GuiLabel-------------

GuiLabel::GuiLabel(char * text)
{
	string = text;
	texture = App->font->Print(text);
	

	App->font->CalcSize(text, posRect.w, posRect.h);
}

void GuiLabel::Draw()
{
	App->render->Blit(texture, posRect.x, posRect.y, NULL, 0.0f);
}