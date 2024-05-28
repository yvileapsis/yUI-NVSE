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

#include <ConsoleManager.hpp>

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

	std::vector<std::pair<std::filesystem::path, std::filesystem::path>> copyQueue;

	void PrepareCopy(const std::filesystem::path& in, const std::filesystem::path& out)
	{
		copyQueue.push_back({ in, out });
	}

	// not on separate thread which is weird, had to add a flushing crutch
	void Copy()
	{
		for (const auto& [in, out] : copyQueue)
		{
			Log(LogLevel::LogFlush) << "";

			const auto lastmod = std::format(".{0:%F}-{0:%H}-{0:%M}-{0:%S}", floor<std::chrono::seconds>(std::chrono::time_point(std::chrono::system_clock::now())));

			if (!exists(out.parent_path())) {
				std::filesystem::create_directory(out.parent_path());

				// Enable NTFS compression
				HANDLE hDirectory = CreateFile(out.parent_path().string().c_str(), GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
				USHORT format = COMPRESSION_FORMAT_DEFAULT;
				DWORD bytesReturned;
				DeviceIoControl(hDirectory, FSCTL_SET_COMPRESSION, &format, sizeof(format), NULL, 0, &bytesReturned, NULL);
			}

			auto newOut = out.parent_path();
			newOut /= out.stem();
			newOut += lastmod;
			newOut += out.extension();

			try {
				std::filesystem::copy_file(in, newOut);
				Log() << "Copied " << in << " to " << newOut;
			}
			catch (std::filesystem::filesystem_error& e) {
				Log() << "Could not copy sandbox/abc: " << e.what() << '\n';
			}
		}
	}
}