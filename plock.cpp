// plock.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>


const char* _HEADER_HELP = "### Process Lock Version 0.1a                                         ###\n-------------------------------------------------------------------------\n### Usage:                                                            ###\n### plock.exe \"<FileLock>\" \"<ExecuteFile>\" \"<Arguments>\" <mode> ###\n### Mode:                                                             ###\n### s - single try to lock <default>                                  ###\n### r - try to lock recursive every 1 sec                             ###\n-------------------------------------------------------------------------\n### proxytype.blogspot.com                      2016                  ###\n";
const char* _HEADER_RECRUSIVE = "Enter Recrusive Mode...\n";

const char* _ERROR_UNABLE_START_PROCESS = "Unable To Run Process\n";
const char* _ERROR_EACCESS_DENIED = "Access Denied\n";
const char* _ERROR_OPERATION_ABORTED = "Operation Aborted\n";
const char* _ERROR_UNABLE_TO_LOCKFILE = "Unable To Lock File\n";

const char _MODE_TRY_LOCK_SINGLE = 's';
const char _MODE_TRY_LOCK_RECRUSIVE = 'r';

const short _MINIMUM_ARGS = 2;
const short _MAXIMUM_ARGS = 5;

const short _FILE_LOCK_ARG = 1;
const short _FILE_EXECUTE_ARG = 2;
const short _EXECUTE_ARG = 3;
const short _MODE_ARG = 4;

const short _ERROR_EXIT_CODE = -1;
const short _ERROR_CODE_ENOENT = 2;
const short _ERROR_CODE_UNABLE_PROCESS = 100;
const short _ERROR_CODE_UNABLE_TO_LOCKFILE = 101;

const short _NULLS_TERMINATOR = 2;

char* fileLock;
char* fileExecute;
char* args = "";
char recrusiveMode = 's';

void printHelp()
{
	printf("%s", _HEADER_HELP);
}

void printRecrusive()
{
	printf("%s", _HEADER_RECRUSIVE);
}

void printError(short err)
{
	if (err == EACCES)
	{
		printf("%s", _ERROR_EACCESS_DENIED);
	}
	else if (err == EPERM)
	{
		printf("%s", _ERROR_OPERATION_ABORTED);
	}
	else if (err == _ERROR_CODE_UNABLE_PROCESS) 
	{
		printf("%s", _ERROR_UNABLE_START_PROCESS);
	}
	else if (err == _ERROR_CODE_UNABLE_TO_LOCKFILE) 
	{
		printf("%s", _ERROR_UNABLE_TO_LOCKFILE);
	}
}

void executeProcess(FILE* f)
{
	char* newArgs;

	STARTUPINFO info;
	PROCESS_INFORMATION processInfo;

	ZeroMemory(&info, sizeof(info));
	ZeroMemory(&processInfo, sizeof(processInfo));

	size_t argsLen = strlen(fileExecute) + strlen(args) + _NULLS_TERMINATOR;
	newArgs = (char*)malloc(argsLen * sizeof(char));

	snprintf(newArgs, argsLen, "%s %s", fileExecute, args);

	if (CreateProcess(NULL, (TCHAR*)newArgs, NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo))
	{
		WaitForSingleObject(processInfo.hProcess, INFINITE);
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
	}
	else 
	{
		printError(_ERROR_CODE_UNABLE_PROCESS);
	}

	free(newArgs);
}

FILE* tryLock()
{
	FILE * f;
	errno_t err;

	err = fopen_s(&f, fileLock, "w");

	if (err != 0) 
	{
		printError((short)err);
		return NULL;
	}
	
	return f;
}


bool recrusiveTry() {

	FILE * f = tryLock();

	if (f != NULL)
	{
		//lock success
		executeProcess(f);
		fclose(f);
		return true;
	}

	if (recrusiveMode == _MODE_TRY_LOCK_RECRUSIVE)
	{
		Sleep(1000);
		printRecrusive();
		return recrusiveTry();
	}
	
	printError(_ERROR_CODE_UNABLE_TO_LOCKFILE);

	return false;

	

}

int main(int argc, char* argv[])
{

	if (argc <= _MINIMUM_ARGS) 
	{
		printHelp();
		return _ERROR_EXIT_CODE;
	}
	else if (argc > _MAXIMUM_ARGS) 
	{
		printHelp();
		return _ERROR_EXIT_CODE;
	}

	fileLock = argv[_FILE_LOCK_ARG];
	fileExecute = argv[_FILE_EXECUTE_ARG];

	if (argc >= _EXECUTE_ARG + 1) 
	{
		args = argv[_EXECUTE_ARG];
	}

	if (argc == _MODE_ARG + 1)
	{
		recrusiveMode = tolower(argv[_MODE_ARG][0]);
	}

	

	recrusiveTry();

	return 0;
}


