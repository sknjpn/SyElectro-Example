#pragma once

#include "Command.h"

class Command_SetObject
	: public Command
{
public:
	Vec2 m_position;
	uint8 m_objType;

	void save(Serializer<MemoryWriter>& writer) override
	{
		Command::save(writer);

		writer << m_position;
		writer << m_objType;
	}

	void load(Deserializer<ByteArray>& reader) override
	{
		Command::load(reader);

		reader >> m_position;
		reader >> m_objType;
	}

	char cmdType() const override { return 2; }

	void action() override;
};
