/*
 * Copyright (c) 2014 Roman Kuznetsov 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "logger.h"

#ifdef WIN32
    #define WIN32_LEAN_AND_MEAN 1
    #include <Windows.h>
	#include <stdarg.h>
	#include <stdio.h>
	#include <algorithm>
	#include <iostream>
#endif

namespace utils
{

unsigned char Logger::outputFlags = (unsigned char)Logger::OutputFlags::IDE_OUTPUT;

void Logger::toLog(const std::string& message)
{
#ifdef WIN32
	if ((outputFlags & (unsigned char)OutputFlags::IDE_OUTPUT) != 0)
	{
		OutputDebugStringA(message.c_str());
	}
	if ((outputFlags & (unsigned char)OutputFlags::CONSOLE) != 0)
	{
		std::cout << message;
	}
	if ((outputFlags & (unsigned char)OutputFlags::FILE) != 0)
	{
		//TODO: implement
	}
#endif
}

void Logger::toLog(const std::wstring& message)
{
#ifdef WIN32
	if ((outputFlags & (unsigned char)OutputFlags::IDE_OUTPUT) != 0)
	{
		OutputDebugStringW(message.c_str());
	}
	if ((outputFlags & (unsigned char)OutputFlags::CONSOLE) != 0)
	{
		std::wcout << message;
	}
	if ((outputFlags & (unsigned char)OutputFlags::FILE) != 0)
	{
		//TODO: implement
	}
#endif
}

void Logger::toLogWithFormat(const char* format, ...)
{
#ifdef WIN32
	std::string fmt(format);
	size_t n = std::count(fmt.begin(), fmt.end(), '%');
	if (n > 0)
	{
		static char buf[2048];
		va_list args;
		va_start(args, format);
		vsprintf_s(buf, format, args);
		va_end(args);

		toLog(buf);
	}
	else
	{
		toLog(format);
	}
#endif
}

void Logger::setOutputFlags(OutputFlags flag)
{
	outputFlags |= (unsigned int)flag;
}

void Logger::setOutputFlagsToDefault()
{
	outputFlags = (unsigned char)Logger::OutputFlags::IDE_OUTPUT;
}

}