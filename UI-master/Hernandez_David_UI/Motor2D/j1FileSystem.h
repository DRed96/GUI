#ifndef __j1FILESYSTEM_H__
#define __j1FILESYSTEM_H__

#include "j1Module.h"
#include "j1Console.h"

struct SDL_RWops;

int close_sdl_rwops(SDL_RWops *rw);

class j1FileSystem : public j1Module
{
public:

	j1FileSystem();

	// Destructor
	virtual ~j1FileSystem();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Utility functions
	bool AddPath(const char* path_or_zip, const char* mount_point = NULL);
	bool AddDir(char* path);
	bool Exists(const char* file) const;
	bool IsDirectory(const char* file) const;
	const char* GetSaveDirectory() const
	{
		return "save/";
	}

	// Open for Read/Write
	unsigned int Load(const char* file, char** buffer) const;
	SDL_RWops* Load(const char* file) const;

	unsigned int Save(const char* file, const char* buffer, unsigned int size) const;

private:
#pragma region Commands
	struct Command_AddDir : public Command
	{
		Command_AddDir() : Command("mkdir", 1){}
		void Function(p2DynArray<p2SString>* args)
		{
			App->fs->AddDir(args->At(1)->GetString());
		}
	};
	Command_AddDir c_AddDir;
#pragma endregion

};

#endif // __j1FILESYSTEM_H__