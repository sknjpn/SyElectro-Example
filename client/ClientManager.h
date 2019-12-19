#pragma once

class ClientManager
{
	static const unique_ptr<ClientManager>& GetInstance();

	TextWriter m_logger;

	void addLog(const String& text);

public:
	ClientManager()
		: m_logger(U"client_log.txt")
	{}

	static void Send(Serializer<MemoryWriter>& writer);
	static void OnRecv(Deserializer<ByteArray>& reader);

	static void Update();
};
