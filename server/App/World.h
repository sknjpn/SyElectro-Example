#pragma once
#include "stdafx.h"

class Wire;
class Node;
class Object;
class Command;

class World
{
	// id
	uint16 m_nextObjectID;
	uint16 m_nextWireID;
	uint16 m_nextNodeID;
	uint16 m_nextCommandID;

	uint64	m_currentFrame = 0;
	uint64	m_targetFrame = 0;
	Array<shared_ptr<Node>> m_nodes;
	Array<shared_ptr<Wire>> m_wires;
	Array<shared_ptr<Object>> m_objects;
	Array<shared_ptr<Command>> m_commands;
public:
	void update();
	void draw();

	void save(Serializer<MemoryWriter>& writer);
	void load(Deserializer<ByteArray>& reader);

	uint64 getCurrentFrame() const { return m_currentFrame; }
	uint64 getTargetFrame() const { return m_targetFrame; }
	void setTargetFrame(uint64 targetFrame) { m_targetFrame = targetFrame; }

	// void addNode(const shared_ptr<Node>& node);
	void addWire(const shared_ptr<Wire>& wire);
	void addObject(const shared_ptr<Object>& object);
	void addCommand(const shared_ptr<Command>& command);
	const Array<shared_ptr<Node>> getNodes() const { return m_nodes; }
	const Array<shared_ptr<Wire>> getWires() const { return m_wires; }
	const Array<shared_ptr<Object>> getObjects() const { return m_objects; }
	const Array<shared_ptr<Command>> getCommands() const { return m_commands; }

	const shared_ptr<Object>& getObject(uint16 id) const;
	const shared_ptr<Node>& getNode(uint16 id) const;
	const shared_ptr<Wire>& getWire(uint16 id) const;

	void removeObject(uint16 id);
	void removeWire(uint16 id);
};

extern unique_ptr<World> g_world;
