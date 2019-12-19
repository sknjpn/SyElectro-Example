#include "ClientManager.h"
#include "Command.h"
#include "World.h"

const unique_ptr<ClientManager>& ClientManager::GetInstance()
{
	static unique_ptr<ClientManager> instance = MakeUnique<ClientManager>();

	return instance;
}

void ClientManager::addLog(const String& text)
{
	m_logger.write(DateTime::Now().format(U"Client[yyyy/M/d HH:mm:ss]") + text + U"\n");
}

void ClientManager::Send(Serializer<MemoryWriter>& writer)
{
	MemoryWriter mw;
	uint16 size = (uint16)writer.getWriter().size();
	mw.write(&size, sizeof(uint16));
	mw.write(writer.getWriter().data(), size);
	g_client->send(mw.data(), mw.size());

	// LOG
	GetInstance()->addLog(Format(U"SEND dataSize=", size));
}

void ClientManager::OnRecv(Deserializer<ByteArray>& reader)
{
	uint8 recvType;
	reader >> recvType;

	// Command
	if (recvType == 0)
	{
		auto command = Command::MakeCommand(reader);

		g_world->addCommand(command);
	}

	// World
	if (recvType == 1)
	{
		g_world->load(reader);
	}

	// Timing
	if (recvType == 2)
	{
		uint64 targetFrame;
		reader >> targetFrame;
		g_world->setTargetFrame(targetFrame);
	}
}

void ClientManager::Update()
{
	uint16 dataSize = 0;
	while (g_client->lookahead<uint16>(dataSize) && g_client->available() >= (sizeof(dataSize) + dataSize))
	{
		g_client->skip(sizeof(dataSize));

		Array<Byte> bytes(dataSize);

		g_client->read(bytes.data(), bytes.size());

		// LOG
		GetInstance()->addLog(Format(U"RECV dataSize=", dataSize));

		Deserializer<ByteArray> reader(std::move(bytes));

		OnRecv(reader);
	}
}
