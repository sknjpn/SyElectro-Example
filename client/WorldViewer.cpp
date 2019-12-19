#include "WorldViewer.h"
#include "World.h"
#include "Object.h"
#include "Node.h"

#include "Command_RemoveObject.h"
#include "Command_RemoveWire.h"
#include "Command_SetObject.h"
#include "Command_SetWire.h"
#include "Command_Switch.h"
#include "Command_MoveObject.h"

#include "Object_Switch.h"
#include "ClientManager.h"

void WorldViewer::init()
{
}

void WorldViewer::update()
{
	Print << U"Tabでモード切替";

	RectF(getViewerSize()).draw(Color(0, 50, 0));

	// World処理
	if (g_world->getTargetFrame() > g_world->getCurrentFrame())
	{
		g_world->update();

		// 0.5s以上の遅延は消化する
		while (g_world->getTargetFrame() - g_world->getCurrentFrame() > 9)
			g_world->update();
	}

	// World描画
	g_world->draw();

	// コマンド処理
	{
		if (KeyTab.down()) g_cmdType = g_cmdType == 5 ? 0 : g_cmdType + 1;

		// オブジェクト削除コマンド
		if (g_cmdType == 0)
		{
			// 表示
			{
				static Font font(16);

				auto t = Transformer2D(Mat3x2::Translate(150, 0));

				font(U"オブジェクト削除").draw();
			}

			if (MouseL.down())
			{
				for (const auto& object : g_world->getObjects())
				{
					if (object->mouseover())
					{
						auto command = MakeShared<Command_RemoveObject>();
						command->m_objectID = object->m_id;

						Serializer<MemoryWriter> writer;
						writer << uint8(0);
						writer << command->cmdType();
						command->save(writer);
						ClientManager::Send(writer);

						break;
					}
				}
			}
		}

		// ワイヤー削除コマンド
		if (g_cmdType == 1)
		{
			// 表示
			{
				static Font font(16);

				auto t = Transformer2D(Mat3x2::Translate(150, 0));

				font(U"ワイヤー削除").draw();
				Print << U"この機能は未実装です";
			}
		}

		// オブジェクト配置コマンド
		if (g_cmdType == 2)
		{
			// 表示
			{
				static Font font(16);

				auto t = Transformer2D(Mat3x2::Translate(150, 0));

				font(U"オブジェクト配置").draw();
				Print << U"Spaceで部品切り替え";
			}

			static uint8 objType = 0;

			if (KeySpace.down()) objType = objType == 0 ? 3 : objType - 1;

			// セレクト
			{
				static Font font(16);

				auto t = Transformer2D(Mat3x2::Translate(300, 0));

				switch (objType)
				{
				case 0: font(U"電池").draw(); break;
				case 1: font(U"電球").draw(); break;
				case 2: font(U"リレー").draw(); break;
				case 3: font(U"スイッチ").draw(); break;
				}
			}

			if (MouseL.down())
			{
				auto command = MakeShared<Command_SetObject>();
				command->m_objType = objType;
				command->m_position = Cursor::PosF();

				Serializer<MemoryWriter> writer;
				writer << uint8(0);
				writer << command->cmdType();
				command->save(writer);
				ClientManager::Send(writer);
			}
		}

		// ワイヤー配置コマンド
		if (g_cmdType == 3)
		{
			// 表示
			{
				static Font font(16);

				auto t = Transformer2D(Mat3x2::Translate(150, 0));

				font(U"ワイヤー配置").draw();
			}

			static shared_ptr<Node> selectedNode;

			if (MouseL.down())
			{
				for (const auto& node : g_world->getNodes())
					if (node->getPosition().distanceFrom(Cursor::PosF()) < 5.0)
						selectedNode = node;
			}

			if (MouseL.up())
			{
				for (const auto& node : g_world->getNodes())
				{
					if (node->getPosition().distanceFrom(Cursor::PosF()) < 5.0 && node != selectedNode)
					{
						auto command = MakeShared<Command_SetWire>();
						command->m_nodeFrID = selectedNode->m_id;
						command->m_nodeToID = node->m_id;

						Serializer<MemoryWriter> writer;
						writer << uint8(0);
						writer << command->cmdType();
						command->save(writer);
						ClientManager::Send(writer);
					}
				}

				selectedNode = nullptr;
			}
		}

		// スイッチングコマンド
		if (g_cmdType == 4)
		{
			// 表示
			{
				static Font font(16);

				auto t = Transformer2D(Mat3x2::Translate(150, 0));

				font(U"スイッチング").draw();
			}

			if (MouseL.down())
			{
				for (const auto& object : g_world->getObjects())
				{
					if (object->mouseover() && dynamic_pointer_cast<Object_Switch>(object))
					{
						auto command = MakeShared<Command_Switch>();
						command->m_objectID = object->m_id;

						Serializer<MemoryWriter> writer;
						writer << uint8(0);
						writer << command->cmdType();
						command->save(writer);
						ClientManager::Send(writer);
					}
				}
			}
		}

		// オブジェクト移動モード
		if (g_cmdType == 5)
		{
			// 表示
			{
				static Font font(16);

				auto t = Transformer2D(Mat3x2::Translate(150, 0));

				font(U"オブジェクト移動").draw();
			}

			static shared_ptr<Object> selectedObject;
			static Vec2 selectedPosition;

			if (MouseL.down())
			{
				for (const auto& object : g_world->getObjects())
				{
					if (object->mouseover())
					{
						selectedObject = object;
						selectedPosition = object->m_position;
					}
				}
			}

			if (selectedObject) selectedPosition += Cursor::DeltaF();

			if (MouseL.up())
			{
				if (selectedObject)
				{
					auto command = MakeShared<Command_MoveObject>();
					command->m_objectID = selectedObject->m_id;
					command->m_position = selectedPosition;

					Serializer<MemoryWriter> writer;
					writer << uint8(0);
					writer << command->cmdType();
					command->save(writer);
					ClientManager::Send(writer);
				}

				selectedObject = nullptr;
			}
		}
	}

	// Command 生成
	if (MouseL.down())
	{
		// クリック場所の表示
		Circle(Cursor::PosF(), 8).draw(Palette::Red);
	}

	// 通信
	ClientManager::Update();
}
