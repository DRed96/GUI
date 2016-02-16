#include "j1Console.h"

#include "j1Render.h"
#include "j1Input.h"
#include "j1Window.h"

uint j1Console::AddCommand(Command* commandToAdd, j1Module* listener)
{
	uint ret = commandsList.count();
	commandToAdd->SetListener(listener);
	commandsList.add(commandToAdd);
	return ret;
}

uint j1Console::AddCVar(const char* _name, float* reference, bool serialize)
{
	cVar* toAdd = new cVar(_name, reference, serialize);
	cVars.add(toAdd);
	return cVars.count() - 1;
}
uint j1Console::AddCVar(const char* _name, int* reference, bool serialize)
{
	cVar* toAdd = new cVar(_name, reference, serialize);
	cVars.add(toAdd);
	return cVars.count() - 1;
}
uint j1Console::AddCVar(const char* _name, char* reference, bool serialize)
{
	cVar* toAdd = new cVar(_name, reference, serialize);
	cVars.add(toAdd);
	return cVars.count() - 1;
}
uint j1Console::AddCVar(const char* _name, bool* reference, bool serialize)
{
	cVar* toAdd = new cVar(_name, reference, serialize);
	cVars.add(toAdd);
	return cVars.count() - 1;
}


void j1Console::ConsoleOutput(p2SString toAdd)
{
	uint y = output.Count() * LINE_SPACING;
	if (App->initialized)
	{
		char* text = toAdd.GetString();
		UILabel* newLine = App->gui->CreateUILabel({ 10, outputOffset + y, 0, 0 }, text, UI_AlignLeft, NULL, { 0, 0, 1, 1 });
		output.PushBack(newLine);
		newLine->layer = 1;
		newLine->SetActive(inputBox->GetActive());

		uint screenWidth, screenHeight;
		App->win->GetWindowSize(screenWidth, screenHeight);
		if (y > screenHeight / 2 - 60)
		{
			outputOffset -= LINE_SPACING;
			for (uint n = 0; n < output.Count(); n++)
			{
				output[n]->localPosition.y -= LINE_SPACING;
			}
		}
	}
}

void j1Console::Execute(p2SString input)
{
	input.ToLower();
	p2DynArray<p2SString>* toSend = new p2DynArray<p2SString>;
	//Cutting input into seperate pieces
	Cut(input, toSend);

	//Printing arguments on the console
	p2SString outputConsole;
	outputConsole += toSend->At(0)->GetString();
	for (int n = 1; n < toSend->Count(); n++)
	{
		outputConsole += " / ";
		outputConsole += toSend->At(n)->GetString();
	}
	LOG("%s", outputConsole.GetString());

	//Finding the command (if it exists)
	Command* command = FindCommand(*toSend->At(0), toSend->Count() - 1);

	if (command)
	{
		if (command->GetListener()->active)
		{
			command->Function(toSend);
		}
	}
	else
	{
		cVar* Var = FindCVar(*toSend->At(0));
		if (Var)
		{
			if (toSend->Count() == 1)
			{
				Var->Display();
			}
			else if (toSend->Count() == 2 || toSend->Count() == 3)
			{
				if (toSend->Count() == 3)
				{
					Var->serialize = std::stoi(toSend->At(2)->GetString());
				}
				Var->Set(*toSend->At(1));
			}
			else
			{
				LOG("Sent more than 1 argument for a variable");
			}
		}
		else
		{
			LOG("Unkown command");
		}
	}
	LOG(" ");

	delete toSend;
}

void j1Console::DisplayCommands()
{
	p2List_item<Command*>* item1 = commandsList.start;
	p2SString toPrint;
	LOG("List of commands:");
	while (item1)
	{
		toPrint += item1->data->GetName().GetString();
		toPrint += ", ";
		item1 = item1->next;
		if (toPrint.Length() > 60)
		{
			LOG("%s", toPrint.GetString());
			toPrint.Clear();
		}
	}
	LOG("%s", toPrint.GetString());
	toPrint.Clear();
	LOG(" ");
	LOG("List of variables:");
	p2List_item<cVar*>* item2 = cVars.start;
	while (item2)
	{
		switch (item2->data->GetType())
		{
		case e_float: { toPrint += "(float) "; break; }
		case e_int: { toPrint += "(int) "; break; }
		case e_string: { toPrint += "(string) "; break; }
		case e_bool: { toPrint += "(bool) "; break; }
		}
		toPrint += item2->data->GetName().GetString();
		toPrint += ", ";
		item2 = item2->next;
		if (toPrint.Length() > 60)
		{
			LOG("%s", toPrint.GetString());
			toPrint.Clear();
		}
	}
	LOG("%s", toPrint.GetString());
}


void j1Console::Cut(p2SString input, p2DynArray<p2SString>* output)
{
	//Cutting the recieved string into pieces, from space to space
	p2SString tmp = input;
	bool betweenComas = false;

	for (uint n = 0; n < input.Length(); n++)
	{
		if (betweenComas == false && input.GetString()[n] == ' ')
		{
			tmp = input;
			tmp.Cut(n);
			output->PushBack(tmp);
			input.Cut(0, n);
			n -= n;
		}

		if (input.GetString()[n] == '"')
		{
			betweenComas = !betweenComas;
			if (betweenComas)
			{
				input.Cut(0, 0);
				n--;
			}
			else
			{
				input.Cut(n, n);
				n--;
			}
		}

	}
	output->PushBack(input);
}

Command* j1Console::FindCommand(p2SString name, uint nArgs)
{
	Command* ret = NULL;
	//Looking for the command to execute
	p2List_item<Command*>* commands = commandsList.start;
	while (commands)
	{
		if (commands->data->GetName() == name)
		{
			ret = commands->data;
			break;
		}
		commands = commands->next;
	}

	if (ret)
	{
		if (nArgs != ret->GetNArgs())
		{
			LOG("For command %s, sent %i arguments, expecting %i.", name, nArgs, ret->GetNArgs());
			ret = NULL;
		}
	}
	return ret;
}

cVar* j1Console::FindCVar(p2SString name)
{
	cVar* ret = NULL;
	//Looking for the command to execute
	p2List_item<cVar*>* Vars = cVars.start;
	while (Vars)
	{
		if (Vars->data->GetName() == name)
		{
			ret = Vars->data;
			break;
		}
		Vars = Vars->next;
	}
	return ret;
}



j1Console::j1Console() : j1Module()
{
	name.create("console");
}

// Destructor
j1Console::~j1Console()
{}

// Called before render is available
bool j1Console::Awake(pugi::xml_node& conf)
{
	bool ret = true;

	//Commands
	AddCommand(&c_Quit, this);
	AddCommand(&c_Save, this);
	AddCommand(&c_Load, this);
	AddCommand(&c_List, this);
	AddCommand(&c_Update_Times, this);

	return ret;
}

// Called before the first frame
bool j1Console::Start()
{
	uint screenWidth, screenHeight;
	App->win->GetWindowSize(screenWidth, screenHeight);

	_TTF_Font* inputTypo = App->font->Load("fonts/open_sans/OpenSans-Bold.ttf", 15);

	consoleRect = App->gui->CreateUIRect({ 0, 0, screenWidth, screenHeight / 2 }, 1, 1, 1, 200);
	consoleRect->layer = 1;
	consoleRect->AddListener(this);

	inputRect = App->gui->CreateUIRect({ 0, screenHeight / 2 - 30, screenWidth, 30 }, 75, 75, 75);
	inputRect->layer = 2;

	inputBox = App->gui->CreateUIInputBox({ 20, screenHeight / 2 - 15, 0, 0 }, { 0, 0, 1, 1 }, { 0, 0, 1, 1 }, "Command", 35, UI_AlignLeft, inputTypo, { 0, -15, screenWidth, 30 });
	inputBox->layer = 2;
	inputBox->maxCharacters = 60;
	inputBox->AddListener(this);

//Deactivating all console elements on launch
		inputBox->SetActive(false);
		consoleRect->SetActive(false);
		inputRect->SetActive(false);
		for (uint n = 0; n < output.Count(); n++)
		{
			output[n]->SetActive(false);
		}
	
	
	return true;
}

// Update all guis
bool j1Console::PreUpdate()
{
	if (App->gui->focus == inputBox)
	{
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			Execute(inputBox->label.GetText());
			inputBox->Clear();
		}
	}
	return true;
}

// Called after all Updates
bool j1Console::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		bool state = !inputBox->GetActive();
		inputBox->SetActive(state);
		consoleRect->SetActive(state);
		inputRect->SetActive(state);
		for (uint n = 0; n < output.Count(); n++)
		{
			output[n]->SetActive(state);
		}
	}
	if (App->gui->focus == inputBox || App->gui->focus == consoleRect)
	{
		if (App->input->GetMouseButtonDown(1) == KEY_REPEAT)
		{
			int x, y;
			App->input->GetMouseMotion(x, y);

			outputOffset += y;
			uint screenWidth, screenHeight;
			App->win->GetWindowSize(screenWidth, screenHeight);
			for (uint n = 0; n < output.Count(); n++)
			{
				output[n]->localPosition.y += y;
				if (output[n]->GetActive() == true)
				{
					if (output[n]->localPosition.y > screenHeight / 2 - 60 || output[n]->localPosition.y < 0)
					{
						output[n]->SetActive(false);
					}
				}
				else
				{
					if (output[n]->localPosition.y < screenHeight / 2 - 60 && output[n]->localPosition.y > 0)
					{
						output[n]->SetActive(true);
					}
				}
			}
		}

	}

	return !closeGame;
}

// Called before quitting
bool j1Console::CleanUp()
{
	p2List_item<cVar*>* item = cVars.start;
	while (item)
	{
		RELEASE(item->data);
		item = item->next;
	}

	commandsList.clear();
	cVars.clear();


	return true;
}


void j1Console::GUIEvent(UIElement* element, GUI_EVENTS event)
{
	/*if (element == inputBox && event == UI_TEXT_CHANGED)
	{
		writtenCommand = inputBox->label.GetText();
	}*/
}

bool j1Console::SaveCVars(pugi::xml_node& Vars) const
{
	pugi::xml_node tmp;
	p2List_item<cVar*>* item = cVars.start;
	
	while (item)
	{
		switch (item->data->GetType())
		{
		case e_float:
		{
			float toAppend = 0.0f;
			item->data->Read(&toAppend);
			Vars.append_child(item->data->GetName().GetString()).append_attribute("value") = toAppend;
			break;
		}
		case e_int:
		{
			int toAppend = 0;
			item->data->Read(&toAppend);
			Vars.append_child(item->data->GetName().GetString()).append_attribute("value") = toAppend;
			break;
		}
		case e_string:
		{
			char* toAppend = "";
			item->data->Read(toAppend);
			Vars.append_child(item->data->GetName().GetString()).append_attribute("value") = toAppend;
			break;
		}
		case e_bool:
		{
			bool toAppend = true;
			item->data->Read(&toAppend);
			Vars.append_child(item->data->GetName().GetString()).append_attribute("value") = toAppend;
			break;
		}
		}
		item = item->next;
	}

	return true;
}
bool j1Console::LoadCVars(pugi::xml_node& conf)
{
	pugi::xml_node vars = conf.first_child();
	while (vars)
	{
		char* name = (char*)vars.name();
		p2SString toSend(name);
		cVar* cVarToSet = FindCVar(toSend);
		if (cVarToSet)
		{
			char* name2 = (char*)vars.attribute("value").as_string();
			p2SString toSend2(name2);
			cVarToSet->Set(toSend2);
		}

		vars = vars.next_sibling();
	}
	return true;
}