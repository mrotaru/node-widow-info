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
	wchar_t *buffer = new wchar_t[bufferLen];
	GetWindowTextW(hWnd, buffer, bufferLen);
	return buffer;
}

wchar_t* MyProcessPath(HWND hWnd)
{
	//cout << "finding process path for HWND: " << hWnd << endl;
	DWORD dwProcID;
	DWORD pathLenght;

	DWORD hThread = GetWindowThreadProcessId(hWnd, &dwProcID);
	HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, dwProcID );
	if (hProcess == NULL) {
		cout << "Error opening process (" << dwProcID << ")" << endl;
	}
	else
	{
		wchar_t *buffer = new wchar_t[256];
		pathLenght = GetProcessImageFileNameW(hProcess, buffer, 256);
		CloseHandle(hProcess);
		if (pathLenght) {
			return buffer;
		}
		else {
			return 0;
		}
	}
}

void RunCallback(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);

	HWND topWindow = GetForegroundWindow();

	wchar_t* windowText = ModuleGetWindowText(topWindow);
	size_t len = wcslen(windowText); // will be GetWindowTextLength-1
	char *buffer = new char[len];
	wcstombs(buffer, windowText, len+1);

	wchar_t* processPath = MyProcessPath(topWindow);
	size_t procPathLen = wcslen(processPath);
	char *pathBuffer = new char[procPathLen];
	wcstombs(pathBuffer, processPath, procPathLen);

	Local<Function> cb = Local<Function>::Cast(args[0]);

	Local<Value> argv[2] = { String::NewFromUtf8(isolate, buffer, String::kNormalString, len), String::NewFromUtf8(isolate, pathBuffer, String::kNormalString, procPathLen)};

	cb->Call(isolate->GetCurrentContext()->Global(), 2, argv);
}

void init(Handle<Object> exports, Handle<Object> module) {
  NODE_SET_METHOD(module, "exports", RunCallback);
}

NODE_MODULE(addon, init)
