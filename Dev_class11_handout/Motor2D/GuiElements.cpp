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
GuiButtons::GuiButtons(SDL_Texture& _tex, SDL_Rect & _idle, SDL_Rect * _hover, SDL_Rect * _action)
{
	texture = &_tex;
	idle = _idle;

	//This allows the easy creation of buttons without hover/action
	if (_hover != NULL)
	{
		hover = *_hover;
	}
	else
	{
		hover = idle;
	}

	if (_action != NULL)
	{
		action = *_action;
	}
	else
	{
		action = idle;
	}

	posRect.w = _idle.w;
	posRect.h = _idle.h;
}

void GuiButtons::onAction(mouse_states _state)
{
	switch (_state)
	{
	case mouse_enter:
		current_state = gui_hover;
		break;

	case mouse_click:
		current_state = gui_action;
		break;

	case mouse_leave:
	default:
		current_state = gui_idle;
		break;
	}
}

void GuiButtons::Draw()
{
	//The used seciton will be idle by default
	SDL_Rect used_section = idle;

	switch (current_state)
	{
	case gui_hover:
		used_section = hover;
		break;
	case gui_action:
		used_section = action;
		break;
	}
	App->render->Blit(texture, posRect.x, posRect.y, &used_section, 0.0f);
}
//-------------