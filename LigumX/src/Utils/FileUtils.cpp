#include "FileUtils.h"
#include "LXError.h"
#include "Logging.h"
//#include <filesystem>
#include <vector>
#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>

std::vector<LXString> FileUtils::GetAllFilesInDirectory(const char* path)
{
	std::vector<LXString> out;

	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	HANDLE hFind;

	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	//StringCchLength(path, MAX_PATH, &length_of_arg);
	StringCchCopy(szDir, MAX_PATH, path);
	StringCchCat(szDir, MAX_PATH, TEXT("*"));

	hFind = FindFirstFileA(szDir, &ffd);
	//if (hFind == INVALID_HANDLE_VALUE)
	//{
	//	printf("FindFirstFile failed (%d)\n", GetLastError());
	//	return;
	//}
	//else
	//{
	//	std::string outPath = std::string(FindFileData.cFileName);
	//	FindClose(hFind);
	//}


	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
		}
		else
		{
			//filesize.LowPart = ffd.nFileSizeLow;
			//filesize.HighPart = ffd.nFileSizeHigh;
			//_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
			std::string outPath = std::string(ffd.cFileName);
			out.push_back(outPath);
		}
	} while (FindNextFile(hFind, &ffd) != 0);
	FindClose(hFind);

	return out;

}

std::fstream FileUtils::OpenFile(const char* fileName)
{
	std::fstream file(fileName, std::fstream::in);
	if (file.is_open())
	{
	}
	else
	{
		lxAssert0();
	}

	return file;
}

void FileUtils::CloseFile(std::fstream& file)
{
	if (file.is_open())
	{
		file.close();
	}
	else
	{
		lxAssert0();
	}
}
