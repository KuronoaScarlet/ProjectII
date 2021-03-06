#include"AssetsManager.h"
#include"App.h"
#include "Log.h"
#include"External/PhysFS/include/physfs.h"

AssetsManager::AssetsManager()
{
	//TODO 1: initialize the library (1 line)
	PHYSFS_init(nullptr);
	PHYSFS_mount("Assets.zip", NULL, 0);
}
AssetsManager::~AssetsManager()
{
	//TODO 2: Deinitialize the library (1 line)
	PHYSFS_deinit();
}
void AssetsManager::Init()
{
	//TODO 3: add the search path (1 line)
}
bool AssetsManager::DeleteBuffer() {
	delete[] buffer;
	return true;
}
char* AssetsManager::GetLastBuffer() {
	return buffer;
}
int AssetsManager::MakeLoad(const char* fileName)
{
	int ret = 0;
	//TODO 5: get the information buffer, and its size with the functions and variables of the library (5 line ~)
	PHYSFS_file* data_file = PHYSFS_openRead(fileName);
	LOG("%s",PHYSFS_getLastError());

	if (data_file != nullptr)
	{
		int file_lenght = PHYSFS_fileLength(data_file);
		buffer = new char[(int)file_lenght];
		int readed = PHYSFS_read(data_file, buffer, 1, (int)file_lenght);
		if (readed != file_lenght)
		{
			delete[] buffer;
		}
		ret = readed;
		PHYSFS_close(data_file);
	}


	return ret;
}
SDL_RWops* AssetsManager::Load(const char* fileName)
{
	//TODO 4: call the MakeLoad functionand get the RWops structure to load the data(3 line)
	

	int size = MakeLoad(fileName);
	SDL_RWops* pointer = SDL_RWFromConstMem(buffer, size);
	return pointer;
}
