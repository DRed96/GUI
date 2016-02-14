#include "GuiElements.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Input.h"
//GUI Element-------------
void GuiElement::SetPosition(int x, int y)
{
	posRect.x = x; posRect.y = y;
}
void GuiElement::SetRect(int x, int y, int w, int h)
{
	posRect.x = x; 	posRect.y = y; 	posRect.w = w; 	posRect.h = h;
}

SDL_Rect GuiElement::GetPosRect() const
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
	//string = text;
	texture = App->font->Print(text);
	
	App->font->CalcSize(text, posRect.w, posRect.h);
}

void GuiLabel::Draw()
{
	App->render->Blit(texture, posRect.x, posRect.y, NULL, 0.0f);
}

//GuiButton-------------
GuiButton::GuiButton(SDL_Texture& _tex, SDL_Rect & _idle, SDL_Rect & _hover, SDL_Rect & _action)
{
	texture = &_tex;
	idle = _idle;
	hover = _hover;
	action = _action;
	
	current_section = &idle;

	posRect.w = _idle.w;
	posRect.h = _idle.h;
}

void GuiButton::onAction(mouse_states _state)
{
	switch (_state)
	{
	case mouse_enter:
		current_section = &hover;
		break;

	case mouse_click:
		current_section = &action;
		break;

	case mouse_leave:
	default:
		current_section = &idle;
		break;
	}
}

void GuiButton::Draw()
{
	//The used seciton will be idle by default
	App->render->Blit(texture, posRect.x, posRect.y, current_section, 0.0f);
}
//-------------