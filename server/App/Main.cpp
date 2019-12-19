#include "World.h"
#include "Command.h"
#include <unistd.h>

unique_ptr<TCPServer> g_server;

void Main()
{
	g_server = MakeUnique<TCPServer>();
	g_world = MakeUnique<World>();

	g_server->startAcceptMulti(31400);

	Array<size_t> users;

	while (true)
	{
		usleep(16000);
		//ClearPrint();
		//Window::SetTitle(U"SyElectro Server numUsers=", g_server->num_sessions());

		Array<shared_ptr<Command>> newCommands;
		for (const auto& sessionID : g_server->getSessionIDs())
		{
			// 新規ユーザー判定
			if (!users.count(sessionID))
			{
				users.emplace_back(sessionID);

				// Worldデータ送信
				{
					Serializer<MemoryWriter> writer;

					uint8 recvType = 1;
					writer << recvType;

					g_world->save(writer);

					MemoryWriter mw;
					uint16 size = (uint16)writer.getWriter().size();
					mw.write(&size, sizeof(uint16));
					mw.write(writer.getWriter().data(), writer.getWriter().size());
					g_server->send(mw.data(), mw.size(), sessionID);
					//Print << U"SEND user=" << sessionID << U" size=" << mw.size();
				}
			}

			// ユーザーからのデータ取得
			uint16 dataSize = 0;
			while (g_server->lookahead<uint16>(dataSize, sessionID) && (g_server->available() >= (sizeof(dataSize) + dataSize), sessionID))
			{
				//Print << U"RECV user=" << sessionID << U" size=" << (sizeof(dataSize) + dataSize);

				g_server->skip(sizeof(dataSize), sessionID);

				Array<Byte> bytes(dataSize);

				g_server->read(bytes.data(), bytes.size(), sessionID);

				Deserializer<ByteArray> reader(std::move(bytes));

				uint8 recvType;
				reader >> recvType;

				// Command
				if (recvType == 0) newCommands.emplace_back(Command::MakeCommand(reader));

				// World(全ての情報を受け取る)
				/*if (recvType == 1)
				{
					Print << U"World";

					//g_world->load(reader);
				}*/
			}
		}

		// 新規コマンド処理
		for (const auto& command : newCommands)
		{
			// コマンド登録
			g_world->addCommand(command);
			command->m_timing = g_world->getCurrentFrame();

			// 全てのユーザーに送信
			for (const auto& sessionID : g_server->getSessionIDs())
			{
				Serializer<MemoryWriter> writer;

				uint8 recvType = 0;
				writer << recvType;

				writer << command->cmdType();
				command->save(writer);

				MemoryWriter mw;
				uint16 size = (uint16)writer.getWriter().size();
				mw.write(&size, sizeof(uint16));
				mw.write(writer.getWriter().data(), writer.getWriter().size());
				g_server->send(mw.data(), mw.size(), sessionID);
				//Print << U"SEND user=" << sessionID << U" size=" << mw.size();
			}
		}

		// ターゲットフレーム
		if (g_world->getCurrentFrame() % 6 == 0)
		{
			// 全てのユーザーに送信
			for (const auto& sessionID : g_server->getSessionIDs())
			{
				Serializer<MemoryWriter> writer;

				uint8 recvType = 2;
				writer << recvType;
				writer << g_world->getCurrentFrame();

				MemoryWriter mw;
				uint16 size = (uint16)writer.getWriter().size();
				mw.write(&size, sizeof(uint16));
				mw.write(writer.getWriter().data(), writer.getWriter().size());
				g_server->send(mw.data(), mw.size(), sessionID);
				//Print << U"SEND user=" << sessionID << U" size=" << mw.size();
			}
		}

		// アップデート
		g_world->update();
		g_world->setTargetFrame(g_world->getCurrentFrame());
		//g_world->draw();
	}

	g_server.reset();
	g_world.reset();
}
