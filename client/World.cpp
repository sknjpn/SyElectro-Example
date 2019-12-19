#include "World.h"
#include "Object.h"
#include "Node.h"
#include "Wire.h"
#include "Command.h"

uint8 g_cmdType = 0;

unique_ptr<World> g_world;

void World::save(Serializer<MemoryWriter>& writer)
{
	// ID
	{
		writer << m_nextObjectID;
		writer << m_nextWireID;
		writer << m_nextNodeID;
		writer << m_nextCommandID;
	}

	// Frame
	{
		writer << m_currentFrame;
	}

	// Object
	{
		uint16 size = (uint16)m_objects.size();
		writer << size;

		for (const auto& object : m_objects)
		{
			writer << object->type();
			object->save(writer);
		}
	}

	// Node (Objectから計算できるのでSizeは送らない)
	for (const auto& node : m_nodes)
	{
		node->save(writer);
	}

	// Wire
	{
		uint16 size = (uint16)m_wires.size();
		writer << size;

		for (const auto& wire : m_wires)
		{
			wire->save(writer);
		}
	}

	// Command
	{

		uint16 size = (uint16)m_commands.size();
		writer << size;

		for (const auto& command : m_commands)
		{
			writer << command->cmdType();
			command->save(writer);
		}
	}
}

void World::load(Deserializer<ByteArray>& reader)
{
	// reset
	m_objects.clear();
	m_nodes.clear();
	m_wires.clear();
	m_commands.clear();

	// ID
	{
		reader >> m_nextObjectID;
		reader >> m_nextWireID;
		reader >> m_nextNodeID;
		reader >> m_nextCommandID;
	}

	// Frame
	{
		reader >> m_currentFrame;
		m_targetFrame = m_currentFrame;
	}

	// Object
	{
		uint16 size;
		reader >> size;

		m_objects.resize(size);
		for (auto& object : m_objects)
		{
			// Object 追加
			object = Object::MakeObject(reader);

			// Node 追加
			for (const auto& node : object->m_nodes)
				m_nodes.emplace_back(node);
		}
	}

	// Node
	for (const auto& node : m_nodes)
		node->load(reader);

	// Wire
	{
		uint16 size;
		reader >> size;

		m_wires.resize(size);
		for (auto& wire : m_wires)
		{
			wire = MakeShared<Wire>();
			wire->load(reader);
		}
	}

	// Command
	{

		uint16 size;
		reader >> size;

		m_commands.resize(size);
		for (auto& commmand : m_commands)
		{
			commmand = Command::MakeCommand(reader);
		}
	}
}

void World::addWire(const shared_ptr<Wire>& wire)
{
	m_wires.emplace_back(wire);
	wire->m_id = m_nextWireID++;
	if (m_nextWireID == UINT16_MAX) m_nextWireID = 0;
}

void World::addObject(const shared_ptr<Object>& object)
{
	// Object 追加
	m_objects.emplace_back(object);
	object->m_id = m_nextObjectID++;
	if (m_nextObjectID == UINT16_MAX) m_nextObjectID = 0;

	// Node 追加
	for (const auto& node : object->m_nodes)
	{
		m_nodes.emplace_back(node);
		node->m_id = m_nextNodeID++;
		if (m_nextNodeID == UINT16_MAX) m_nextNodeID = 0;
	}
}

void World::addCommand(const shared_ptr<Command>& command)
{
	m_commands.emplace_back(command);
	command->m_id = m_nextCommandID++;
	if (m_nextCommandID == UINT16_MAX) m_nextCommandID = 0;
}

const shared_ptr<Object>& World::getObject(uint16 id) const
{
	for (const auto& object : m_objects)
		if (object->m_id == id) return object;

	throw Error(Format(U"存在しないObjectID", id));
}

const shared_ptr<Node>& World::getNode(uint16 id) const
{
	for (const auto& node : m_nodes)
		if (node->m_id == id) return node;

	throw Error(Format(U"存在しないNodeID", id));
}

const shared_ptr<Wire>& World::getWire(uint16 id) const
{
	for (const auto& wire : m_wires)
		if (wire->m_id == id) return wire;

	throw Error(Format(U"存在しないWireID", id));
}

void World::removeObject(uint16 id)
{
	m_wires.remove_if([id](const auto& wire) { return wire->m_fr->m_object.lock()->m_id == id || wire->m_to->m_object.lock()->m_id == id; });
	m_nodes.remove_if([id](const auto& node) { return node->m_object.lock()->m_id == id; });
	m_objects.remove_if([id](const auto& object) { return object->m_id == id; });
}

void World::removeWire(uint16 id)
{
	m_wires.remove_if([id](const auto& wire) { return wire->m_id == id; });
}

void World::update()
{
	// Nodeのリセット
	for (const auto& node : m_nodes) node->m_nowState = Node::State::None;

	// Object更新
	for (const auto& object : m_objects)
	{
		object->update();
	}

	// Node情報の伝搬
	{
		for (const auto& node : m_nodes)
			node->flowToConnected();

		for (;;)
		{
			bool flag = true;

			for (const auto& wire : m_wires)
			{
				if (wire->m_fr->m_nowState != Node::State::None && wire->m_to->m_nowState == Node::State::None)
				{
					wire->m_to->m_nowState = wire->m_fr->m_nowState;
					wire->m_to->flowToConnected();

					flag = false;
				}

				if (wire->m_fr->m_nowState == Node::State::None && wire->m_to->m_nowState != Node::State::None)
				{
					wire->m_fr->m_nowState = wire->m_to->m_nowState;
					wire->m_fr->flowToConnected();

					flag = false;
				}
			}

			if (flag) break;
		}
	}

	// Nodeの状態記録
	for (const auto& node : m_nodes)
		node->m_preState = node->m_nowState;

	// Command処理
	for (const auto& command : m_commands)
		if (m_currentFrame >= command->m_timing)
			command->action();

	m_commands.remove_if([this](const auto& c) { return m_currentFrame >= c->m_timing; });

	++m_currentFrame;
}

void World::draw()
{
	switch (g_cmdType)
	{
	case 0:
	case 2:
	case 4:
	case 5:
		for (const auto& wire : m_wires) wire->draw();
		for (const auto& object : m_objects) object->draw();
		break;

	case 1:
	case 3:
		for (const auto& object : m_objects) object->draw();
		for (const auto& node : m_nodes) node->draw();
		for (const auto& wire : m_wires) wire->draw();
		break;
	}

}
