#include "ipcserver.h"
#include <iostream>
#include "aura.h"

namespace Nickvision::Aura
{
	IPCServer::IPCServer()
		: m_id{ Aura::getActive().getAppInfo().getId() },
		m_running{ false }
	{
#ifdef _WIN32
		m_pipeName = "\\\\.\\pipe\\" + m_id;
		m_overlapped.hEvent = CreateEventA(nullptr, 1, 0, nullptr);
#endif
	}

	IPCServer::~IPCServer()
	{
		m_running = false;
#ifdef _WIN32
		SetEvent(m_overlapped.hEvent);
#endif
	}

	Events::Event<Events::ParamEventArgs<std::vector<std::string>>>& IPCServer::commandReceived()
	{
		return m_commandReceived;
	}

	bool IPCServer::communicate(const std::vector<std::string>& args)
	{
#ifdef _WIN32
		HANDLE pipe{ CreateFileA(m_pipeName.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr) };
		if (pipe != INVALID_HANDLE_VALUE)
		{
			if (args.empty())
			{
				return true;
			}
			std::string argc{ std::to_string(args.size()) };
			WriteFile(pipe, &argc, argc.size(), nullptr, nullptr);
			for (const std::string& arg : args)
			{
				WriteFile(pipe, arg.c_str(), arg.size(), nullptr, nullptr);
			}
			std::cout << "[AURA: " << m_id << "] Sent a command to the running instance." << std::endl;
			CloseHandle(pipe);
			return true;
		}
		else if(!m_running)
		{
			m_running = true;
			m_server = std::jthread(&IPCServer::runServer, this);
		}
#endif
		return false;
	}

	void IPCServer::runServer()
	{
#ifdef _WIN32
		std::vector<char> buffer(2048);
		HANDLE pipe{ CreateNamedPipeA(m_pipeName.c_str(), PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 1, 1024 * 16, 1024 * 16, NMPWAIT_USE_DEFAULT_WAIT, nullptr) };
		if (pipe == INVALID_HANDLE_VALUE)
		{
			m_running = false;
			return;
		}
		std::cout << "[AURA: " << m_id << "] Server started on this instance." << std::endl;
		while (m_running)
		{
			if (ConnectNamedPipe(pipe, &m_overlapped))
			{
				DWORD read;
				ReadFile(pipe, &buffer[0], sizeof(buffer) - 1, &read, nullptr);
				size_t argc{ std::stoull({ &buffer[0], read }) };
				std::vector<std::string> args(argc);
				for (int i = 0; i < argc; i++)
				{
					ReadFile(pipe, &buffer[0], sizeof(buffer) - 1, &read, nullptr);
					args[i] = { &buffer[0], read };
				}
				std::cout << "[AURA: " << m_id << "] Command received." << std::endl;
				m_commandReceived.invoke({ args });
			}
			DisconnectNamedPipe(pipe);
		}
		CloseHandle(pipe);
#endif
	}
} 