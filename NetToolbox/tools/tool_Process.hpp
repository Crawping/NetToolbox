﻿////////////////////////////////////////////////////////////////////////////////
//
// Class Name:  tool_Process
// Description: 进程工具类
// Class URI:   https://github.com/fawdlstty/NetToolbox
// Author:      Fawdlstty
// Author URI:  https://www.fawdlstty.com/
// License:     此文件单独授权 以MIT方式开源共享
// Last Update: Dec 19, 2018
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __TOOL_PROCESS_HPP__
#define __TOOL_PROCESS_HPP__

#include <map>
#include <tuple>
#include <TlHelp32.h>
#include <shellapi.h>
#include <Psapi.h>

#include "tool_String.hpp"
#include "tool_Utils.hpp"



class tool_Process {
public:
	// 获取进程列表
	static std::map<DWORD, string_t> get_processes () {
		HANDLE hSnapShot = ::CreateToolhelp32Snapshot (TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32 pe32 { sizeof (PROCESSENTRY32) };
		std::map<DWORD, string_t> m;
		if (::Process32First (hSnapShot, &pe32)) {
			do {
				m[pe32.th32ProcessID] = pe32.szExeFile;
			} while (::Process32Next (hSnapShot, &pe32));
		}
		::CloseHandle (hSnapShot);
		return m;
	}

	// 打开notepad之类的命令或者传入http://xxx打开一个网页
	static void shell_exec (string_t url) {
		::ShellExecute (NULL, _T ("open"), url.c_str (), nullptr, nullptr, SW_SHOW);
	}

	// 判断进程是否存在
	static bool process_exist (string_t file) {
		HANDLE hSnapShot = ::CreateToolhelp32Snapshot (TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32 pe32 { sizeof (PROCESSENTRY32) };
		std::map<DWORD, string_t> m;
		if (::Process32First (hSnapShot, &pe32)) {
			do {
				string_t _path = pe32.szExeFile;
				tool_StringT::replace (_path, _T ('/'), _T ('\\'));
				size_t p = _path.rfind (_T ('\\'));
				if (_path.substr (p + 1) == file) {
					::CloseHandle (hSnapShot);
					return true;
				}
			} while (::Process32Next (hSnapShot, &pe32));
		}
		::CloseHandle (hSnapShot);
		return false;
	}

	// 创建进程，进程名必须包括路径的全称
	static bool create_process (string_view_t file) {
		STARTUPINFO si = { sizeof (STARTUPINFO) };
		PROCESS_INFORMATION pi = { 0 };
		bool bRet = !!::CreateProcess (file.data (), nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi);
		if (bRet) {
			CloseHandle (pi.hThread);
			CloseHandle (pi.hProcess);
		} else {
			string_t err_info = tool_StringT::format (_T ("创建进程失败，%s"), tool_Utils::get_error_info (::GetLastError ()).c_str ());
			::MessageBox (NULL, err_info.c_str (), _T ("提示"), MB_ICONHAND);
		}
		return bRet;
	}

	// 杀掉某进程
	static bool kill (DWORD pid) {
		HANDLE hProcess = ::OpenProcess (PROCESS_TERMINATE, FALSE, pid);
		if (!hProcess)
			return false;
		bool bRet = !!::TerminateProcess (hProcess, 0);
		::CloseHandle (hProcess);
		return bRet;
	}

	// 获取某个进程的所在文件夹
	static std::tuple<DWORD, string_t> get_path (DWORD pid) {
		HANDLE hProcess = ::OpenProcess (PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
		if (!hProcess) {
			DWORD d = ::GetLastError ();
			return { d, _T ("") };
		}
		HMODULE hModule = NULL;
		DWORD need;
		if (!::EnumProcessModulesEx (hProcess, &hModule, sizeof (HMODULE), &need, LIST_MODULES_ALL)) {
			DWORD d = ::GetLastError ();
			::CloseHandle (hProcess);
			return { d, _T ("") };
		}
		TCHAR tBuf[2048] = { 0 };
		if (!::K32GetModuleFileNameExW/*GetModuleFileNameEx*/ (hProcess, hModule, tBuf, sizeof (tBuf) / sizeof (TCHAR))) {
			DWORD d = ::GetLastError ();
			::CloseHandle (hProcess);
			return { d, _T ("") };
		}
		::CloseHandle (hProcess);
		return { 0, tBuf };
	}
};

#endif //__TOOL_PROCESS_HPP__
