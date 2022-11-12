#pragma once
#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "Singleton.h"
#include "DateTime.h"
#include "Format.h"

enum class ELogLevel
{
	Info,
	Debug,
	Warn,
	Error,
	Fatal,
};

extern class Logger* GLogger;

class Logger : public ISingleton<Logger>
{
	friend class ISingleton<Logger>;
private:
	bool mExitFlag;
	bool mConsoleLog;
	int mFlushDurationMilliSec;
	std::string basePath;
	std::string programName;

	std::ofstream mOutFile;
	std::stringstream mBuffer;

	std::condition_variable mCV;
	std::thread mWorker;
	std::mutex  mSync;

	ELogLevel mLogLevel;
private:
	Logger();

public:
	~Logger();

	static void initialize();

	void setConsolelogger(bool bConsolelogger) { mConsoleLog = bConsolelogger; }

	void setloggerLevel(ELogLevel eloggerlevel) { mLogLevel = eloggerlevel; }

	void setProgramName(const char* name) { programName = name; }

	void out(ELogLevel level, std::thread::id thread_id, int line, const char* function, const char* fmt, ...);
	
private:
	void write(const std::string& log);

	void flush();
};


#define LOG_FATAL(fmt, ...) GLogger->out(ELogLevel::Fatal, std::this_thread::get_id(), __LINE__, __FUNCTION__, fmt, __VA_ARGS__)
#define LOG_ERROR(fmt, ...) GLogger->out(ELogLevel::Error, std::this_thread::get_id(), __LINE__, __FUNCTION__, fmt, __VA_ARGS__)
#define LOG_WARN(fmt, ...) GLogger->out(ELogLevel::Warn, std::this_thread::get_id(), __LINE__, __FUNCTION__, fmt, __VA_ARGS__)
#define LOG_DEBUG(fmt, ...) GLogger->out(ELogLevel::Debug, std::this_thread::get_id(), __LINE__, __FUNCTION__, fmt, __VA_ARGS__)
#define LOG_INFO(fmt, ...) GLogger->out(ELogLevel::Info, std::this_thread::get_id(), __LINE__, __FUNCTION__, fmt, __VA_ARGS__)

#endif