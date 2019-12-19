#pragma once
#include "stdafx.h"

class Node;

class Object
{
public:
	uint16 m_id;
	Vec2 m_position;
	Array<shared_ptr<Node>> m_nodes;

public:
	virtual ~Object() = default;

	static shared_ptr<Object> MakeObject(uint8 objType);
	static shared_ptr<Object> MakeObject(Deserializer<ByteArray>& reader);

	virtual uint8 type() = 0;

	virtual void update() = 0;

	virtual void draw() = 0;

	virtual bool mouseover() const = 0;

	virtual void save(Serializer<MemoryWriter>& writer)
	{
		writer << m_id;
		writer << m_position;
	}

	virtual void load(Deserializer<ByteArray>& reader)
	{
		reader >> m_id;
		reader >> m_position;
	}
};
