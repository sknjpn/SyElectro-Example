#pragma once
#include "stdafx.h"

class Node;

class Wire
{
public:
	uint16 m_id;
	shared_ptr<Node> m_fr;
	shared_ptr<Node> m_to;

	Wire() {}
	Wire(const shared_ptr<Node>& fr, const shared_ptr<Node>& to)
		: m_fr(fr)
		, m_to(to)
	{}

	void draw();

	virtual void save(Serializer<MemoryWriter>& writer);
	virtual void load(Deserializer<ByteArray>& reader);
};

