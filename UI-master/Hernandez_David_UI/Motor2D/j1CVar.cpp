#include "j1Console.h"



cVar::cVar(const char* _name, float* _reference, bool _serialize) : name(_name), type(e_float), serialize(_serialize)
{
	reference.floatRef = _reference;
	value.floatVar = *_reference;
}
cVar::cVar(const char* _name, int* _reference, bool _serialize) : name(_name), type(e_int), serialize(_serialize)
{
	reference.intRef = _reference;
	value.intVar = *_reference;
}
cVar::cVar(const char* _name, char* _reference, bool _serialize) : name(_name), type(e_string), serialize(_serialize)
{
	reference.stringRef = _reference;
	value.stringVar = _reference;
}
cVar::cVar(const char* _name, bool* _reference, bool _serialize) : name(_name), type(e_bool), serialize(_serialize)
{
	reference.boolRef = _reference;
	value.boolVar = *_reference;
}


void cVar::LinkCommand(Command* toLink)
{
	linkedFunction = toLink;
}


void cVar::Set(float _value)
	{
		if (type == e_float)
		{
			*reference.floatRef = _value;

			LOG("Set cVar %s to %f.", name.GetString(), _value);
			if (serialize)
			{
				value.floatVar = _value;
				LOG("Will be serialized");
			}
			if (linkedFunction)
			{
				linkedFunction->Function(NULL);
			}
		}
	}
void cVar::Set(int _value)
	{
		if (type == e_int)
		{
			*reference.intRef = _value;

			LOG("Set cVar %s to %i.", name.GetString(), _value);
			if (serialize)
			{
				LOG("Will be serialized");
				value.intVar = _value;
			}
			if (linkedFunction)
			{
				linkedFunction->Function(NULL);
			}
		}
	}
void cVar::Set(char* _value)
	{
		if (type == e_string)
		{
			reference.stringRef = _value;

			LOG("Set cVar %s to %s.", name.GetString(), _value);
			if (serialize)
			{
				LOG("Will be serialized");
				value.stringVar = _value;
			}
			if (linkedFunction)
			{
				linkedFunction->Function(NULL);
			}
		}
	}
void cVar::Set(bool _value)
	{
		if (type == e_bool)
		{
			
			*reference.boolRef = _value;

			LOG("Set cVar %s to %i.", name.GetString(), _value);
			if (serialize)
			{
				LOG("Will be serialized");
				value.boolVar = _value;
			}
			if (linkedFunction)
			{
				linkedFunction->Function(NULL);
			}
		}
	}
void cVar::Set(p2SString data)
	{
		switch (type)
		{
		case e_float:
		{
			Set(std::stof(data.GetString()));
			break;
		}
		case e_int:
		{
			Set(std::stoi(data.GetString()));
			break;
		}
		case e_string:
		{
			Set(data.GetString());
			break;
		}
		case e_bool:
		{
			bool toSend = std::stoi(data.GetString());
			Set(toSend);
			break;
		}
		}
	}


cVarTypes cVar::GetType() { return type; }
p2SString cVar::GetName() { return name; }


void cVar::Read(void* ret, cVarTypes expectedData)
	{
		if (expectedData == type)
		{
			ret = &value;
		}
		else
		{
			//LOG("Incorrect data type request to %s", name->GetString());
		}
	}
bool cVar::Read(float* output) { if (type == e_float){ *output = value.floatVar; return true; } return false; }
bool cVar::Read(int* output) { if (type == e_int){ *output = value.intVar; return true; } return false; }
bool cVar::Read(char* output) { if (type == e_string){ output = value.stringVar; return true; } return false; }
bool cVar::Read(bool* output) { if (type == e_bool){ *output = value.boolVar; return true; } return false; }
void* cVar::ForceRead() { return &value; }

void cVar::Display()
{
	switch (type)
	{
	case e_float:
		LOG("Float value: %f", value.floatVar);
		break;
	case e_int:
		LOG("Int value: %i", value.intVar);
		break;
	case e_string:
		LOG("String value: %s", value.stringVar);
		break;
	case e_bool:
		LOG("Bool value: %b", value.boolVar);
		break;
	default:
		break;
	}
}