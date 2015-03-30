#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <node.h>

#include <Windows.h>
#include <Psapi.h>

using namespace v8;
using namespace std;
 
wchar_t* ModuleGetWindowText(HWND hWnd)
{
	int bufferLen = GetWindowTextLengthW(hWnd) + 1;
	wchar_t *buffer = new wchar_t[bufferLen + 1];
	GetWindowTextW(hWnd, buffer, bufferLen + 1);
	return buffer;
}

wchar_t* MyProcessPath(HWND hWnd)
{
	LPDWORD lpdwProcID;
	DWORD pathLenght;
	wchar_t *buffer;

	DWORD hThread = GetWindowThreadProcessId(hWnd, lpdwProcID);
	HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, *lpdwProcID );
	if (hProcess == NULL) {
		cout << "Error opening process (" << *lpdwProcID << ")" << endl;
	}
	else
	{
		buffer = new wchar_t[256];
		pathLenght = GetProcessImageFileNameW(hProcess, buffer, 256);
		CloseHandle(hProcess);
		if (pathLenght) {
			return buffer;
		}
	}
}

void RunCallback(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);

	HWND topWindow = GetForegroundWindow();

	wchar_t* windowText = ModuleGetWindowText(topWindow);
	size_t len = wcslen(windowText);
	char *buffer = new char[len];
	wcstombs(buffer, windowText, len);

	Local<Function> cb = Local<Function>::Cast(args[0]);

	const unsigned argc = 1;
	Local<Value> argv[argc] = { String::NewFromUtf8(isolate, buffer, String::kNormalString, len)};

	cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
}

void init(Handle<Object> exports, Handle<Object> module) {
  NODE_SET_METHOD(module, "exports", RunCallback);
}

NODE_MODULE(addon, init)
