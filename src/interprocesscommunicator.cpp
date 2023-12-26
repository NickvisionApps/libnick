#include "interprocesscommunicator.h"
#include <stdexcept>
#include "aura.h"

namespace Nickvision::Aura
{
	InterProcessCommunicator::InterProcessCommunicator()
		: m_id{ Aura::getActive().getAppInfo().getId() },
		m_running{ false }
	{
#ifdef _WIN32
		m_pipeName = "\\\\.\\pipe\\" + m_id;
		m_serverPipe = nullptr;
		WIN32_FIND_DATAA fd;
		HANDLE find{ FindFirstFileA(m_pipeName.c_str(), &fd) };
		if (find == INVALID_HANDLE_VALUE) //no server exists
		{
			m_serverPipe = CreateNamedPipeA(m_pipeName.c_str(), PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 1, 2048, 2048, NMPWAIT_USE_DEFAULT_WAIT, nullptr);
			if (m_serverPipe == INVALID_HANDLE_VALUE)
			{
				throw std::runtime_error("Unable to start IPC server.");
			}
			m_running = true;
			FindClose(find);
		}
#endif
		if (m_running)
		{
			m_server = std::jthread(&InterProcessCommunicator::runServer, this);
		}
	}

	InterProcessCommunicator::~InterProcessCommunicator()
	{
		m_running = false;
#ifdef _WIN32
		if (m_serverPipe)
		{
			CancelSynchronousIo(m_serverPipe);
			CloseHandle(m_serverPipe);
		}
#endif
	}

	Events::Event<Events::ParamEventArgs<std::vector<std::string>>>& InterProcessCommunicator::commandReceived()
	{
		return m_commandReceived;
	}

	bool InterProcessCommunicator::isServer() const
	{
		return m_running;
	}

	bool InterProcessCommunicator::isClient() const
	{
		return !m_running;
	}

	bool InterProcessCommunicator::communicate(const std::vector<std::string>& args)
	{
		if (m_running) //server
		{
			m_commandReceived.invoke({ args });
			return true;
		}
#ifdef _WIN32
		if (args.empty())
		{
			return true;
		}
		HANDLE clientPipe{ CreateFileA(m_pipeName.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr) };
		if (clientPipe == INVALID_HANDLE_VALUE)
		{
			return false;
		}
		char success{ 0 };
		std::string argc{ std::to_string(args.size()) };
		WriteFile(clientPipe, argc.c_str(), argc.size(), nullptr, nullptr);
		ReadFile(clientPipe, &success, sizeof(char), nullptr, nullptr);
		if (success != 1)
		{
			return false;
		}
		for (const std::string& arg : args)
		{
			success = 0;
			WriteFile(clientPipe, arg.c_str(), arg.size(), nullptr, nullptr);
			ReadFile(clientPipe, &success, sizeof(char), nullptr, nullptr);
			if (success != 1)
			{
				return false;
			}
		}
		CloseHandle(clientPipe);
		return true;
#endif
		return false;
	}

	void InterProcessCommunicator::runServer()
	{
#ifdef _WIN32
		std::vector<char> buffer(2048);
		char success{ 1 };
		while (m_running)
		{
			if (ConnectNamedPipe(m_serverPipe, nullptr))
			{
				DWORD read;
				ReadFile(m_serverPipe, &buffer[0], buffer.size(), &read, nullptr);
				size_t argc{ std::stoull({ &buffer[0], read }) };
				std::vector<std::string> args(argc);
				WriteFile(m_serverPipe, &success, sizeof(char), nullptr, nullptr);
				for (int i = 0; i < argc; i++)
				{
					ReadFile(m_serverPipe, &buffer[0], buffer.size(), &read, nullptr);
					WriteFile(m_serverPipe, &success, sizeof(char), nullptr, nullptr);
					args[i] = { &buffer[0], read };
				}
				m_commandReceived.invoke({ args });
				DisconnectNamedPipe(m_serverPipe);
			}
		}
#endif
	}
} 