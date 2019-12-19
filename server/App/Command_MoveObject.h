#pragma once

#include "Command.h"

class Command_MoveObject
	: public Command
{
public:
	int16	m_objectID;
	Vec2	m_position;

	void save(Serializer<MemoryWriter>& writer) override
	{
		Command::save(writer);

		writer << m_objectID;
		writer << m_position;
	}

	void load(Deserializer<ByteArray>& reader) override
	{
		Command::load(reader);

		reader >> m_objectID;
		reader >> m_position;
	}

	char cmdType() const override { return 5; }

	void action() override;
};
