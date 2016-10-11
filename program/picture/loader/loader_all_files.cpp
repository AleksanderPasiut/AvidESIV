#include "loader.h"

wchar_t* PathToCatalog(const wchar_t* url)
{
	int length = lstrlenW(url);
	wchar_t* ret = new wchar_t [length+2];
	memcpy(ret, url, length*sizeof(wchar_t));

	for (int i = length-1; i >= 0; i--)
		if (ret[i] == L'\\')
		{
			ret[i+1] = L'*';
			ret[i+2] = 0;
			break;
		}

	return ret;
}
wchar_t* FoundFileUrl(const wchar_t* path, const wchar_t* name)
{
	int path_length = lstrlenW(path);
	int name_length = lstrlenW(name);
	wchar_t* ret = new wchar_t [path_length+name_length+1];
	memcpy(ret, path, (path_length-1)*sizeof(wchar_t));
	memcpy(ret + (path_length-1), name, name_length*sizeof(wchar_t));
	ret[path_length+name_length-1] = 0;
	return ret;
}

void LOADER::LoadAllFiles(const wchar_t* url) noexcept
{
	wchar_t* path = PathToCatalog(url);

	WIN32_FIND_DATAW data;
	HANDLE handle = FindFirstFileW(path, &data);
	FindNextFileW(handle, &data);

	try
	{
		for (;;)
		{
			if (!FindNextFileW(handle, &data))
				break;

			LOADER_FILE* new_file = new LOADER_FILE;
			new_file->url = FoundFileUrl(path, data.cFileName);
			tab.push_back(new_file);
		}

		for (auto it = tab.begin(); it != tab.end(); it++)
			if (!lstrcmpW((*it)->url, url))
			{	
				current = it;
				(*current)->opened = true;
				break;
			}
	}
	catch(...)
	{
		for (auto it = tab.begin(); it != tab.end(); it++)
			delete (*it);

		tab.clear();
	}

	delete[] path;
}