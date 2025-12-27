#include <Logging.hpp>

#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <fstream>
#include <map>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <thread>

#include <winioctl.h>
#include <shlwapi.h>

#include <ConsoleManager.hpp>

#pragma comment(lib, "Shlwapi.lib")

inline ConsoleManager* operator<<(ConsoleManager* console, const std::string& str)
{
	UInt32 numLines = str.length() / 500;
	for (UInt32 i = 0; i < numLines; i++)
		console->Print(str.substr(i * 500, 500).c_str(), nullptr);

	console->Print(str.substr(numLines * 500, str.length() - numLines * 500).c_str(), nullptr);

	return console;
};

class LoggerManager {
public:

	static LoggerManager& GetSingleton()
	{
		static LoggerManager instance; // Guaranteed to be destroyed and instantiated on first use
		return instance;
	}

	LoggerManager() : stop(false) {}

	~LoggerManager()
	{
		condition.notify_one();
		worker.join();
	}

	void play() { worker = std::thread([this] { processQueue(); }); }

	void addDestination(const std::string& id, const std::function<void(const std::string&, LogLevel)>& destFunc)
	{
		std::unique_lock<std::mutex> lock(destMutex);
		destinations[id] = destFunc;
	}
	void removeDestination(const std::string& id)
	{
		std::unique_lock<std::mutex> lock(destMutex);
		destinations.erase(id);
	}
	void log(const std::string& message, LogLevel level)
	{
		std::unique_lock<std::mutex> lock(mutex);
		logQueue.push({ message, level });
		lock.unlock();
		condition.notify_one();
	}

private:

	using Destination = std::function<void(const std::string&, LogLevel)>;

	std::map<std::string, Destination> destinations;
	std::mutex destMutex;

	std::queue<std::pair<std::string, LogLevel>> logQueue;
	std::mutex mutex;
	std::condition_variable condition;
	std::thread worker;
	bool stop;

	void processQueue()
	{
		while (!stop || !logQueue.empty())
		{
			std::unique_lock<std::mutex> lock(mutex);
			condition.wait(lock, [this] { return stop || !logQueue.empty(); });

			if (!logQueue.empty())
			{
				// Process the logging task
				auto logTask = logQueue.front();
				logQueue.pop();
				lock.unlock();

				// Broadcast log to destinations based on their log level
				std::unique_lock<std::mutex> destLock(destMutex);

				for (auto& [id, dest] : destinations)
					dest(logTask.first, logTask.second);
			}
		}
	}
};

namespace Logger
{
	void Play()
	{
		LoggerManager::GetSingleton().play();
	}

	void LogToManager(const std::string& message, LogLevel level)
	{
		LoggerManager::GetSingleton().log(message, level);
	}

	void AddDestinations(const std::filesystem::path& log, const std::string& prefix, UInt32 logLevel)
	{
		if (!exists(log.parent_path())) std::filesystem::create_directory(log.parent_path());

		LoggerManager::GetSingleton().addDestination("file", [log, logLevel](const std::string& msg, LogLevel level)
			{
				static std::fstream logFile(log, std::fstream::out | std::fstream::trunc);

				if (level & logLevel & LogLevel::LogFile)
					logFile << msg << '\n';

				if (level & LogLevel::LogFlush)
					logFile.flush();
			});

		LoggerManager::GetSingleton().addDestination("console", [prefix](const std::string& msg, LogLevel level)
			{
				if ((level & LogLevel::LogConsole))
					if (const auto console = ConsoleManager::GetSingleton())
						console << prefix + ": " + msg;
			});
	}

	char pathIn[MAX_PATH];
	char pathOut[MAX_PATH];

	void PrepareCopy(const char* in, const char* out) {
		strcpy_s(pathIn, in);
		strcpy_s(pathOut, out);
	}

	void Copy() {
		const char* in = pathIn;
		const char* out = pathOut;

		char lastmod[48];
		time_t now = time(nullptr);
		tm localTime;
		localtime_s(&localTime, &now);
		sprintf_s(lastmod, ".%04d-%02d-%02d-%02d-%02d-%02d",
			localTime.tm_year + 1900,
			localTime.tm_mon + 1,
			localTime.tm_mday,
			localTime.tm_hour,
			localTime.tm_min,
			localTime.tm_sec);

		if (!PathIsDirectory(out)) {
			CreateDirectory(out, NULL);

			// Enable NTFS compression
			HANDLE hDirectory = CreateFile(out, GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
			USHORT format = COMPRESSION_FORMAT_DEFAULT;
			DWORD bytesReturned;
			DeviceIoControl(hDirectory, FSCTL_SET_COMPRESSION, &format, sizeof(format), NULL, 0, &bytesReturned, NULL);
		}

		char newOut[MAX_PATH];
		char filename[MAX_PATH];
		char ext[16];
		_splitpath_s(in, NULL, 0, NULL, 0, filename, sizeof(filename), ext, sizeof(ext));
		sprintf_s(newOut, "%s\\%s%s%s", out, filename, lastmod, ext);
		{
			char sanitizedBufferIn[MAX_PATH] = {};
			char sanitizedBufferOut[MAX_PATH] = {};
			if (CopyFile(in, newOut, FALSE))
				_MESSAGE("\nCopied \"%s\" to \"%s\"", SanitizeString(in, sanitizedBufferIn, sizeof(sanitizedBufferIn)), SanitizeString(newOut, sanitizedBufferOut, sizeof(sanitizedBufferOut)));
			else
				_MESSAGE("\nFailed to copy \"%s\" to \"%s\"", SanitizeString(in, sanitizedBufferIn, sizeof(sanitizedBufferIn)), SanitizeString(newOut, sanitizedBufferOut, sizeof(sanitizedBufferOut)));
		}
	}
}