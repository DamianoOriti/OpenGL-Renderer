#include "utility.h"

void extractDirectoryPath(std::string& filename)
{
	size_t length = filename.length();
	if (length == 0)
	{
		return;
	}

	char* path = new char[length + 1];

	size_t i;
	for (i = length - 1; i > 0; i--)
	{
		if (filename[i] == '/')
		{
			path[i + 1] = '\0';
			break;
		}
	}

	for (size_t j = 0; j <= i; j++)
	{
		path[j] = filename[j];
	}

	filename.clear();
	filename.assign(path);
}
