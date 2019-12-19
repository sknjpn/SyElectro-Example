#pragma once

#include "Command.h"

class Command_RemoveObject
	: public Command
{
public:
	int16 m_objectID;

	void save(Serializer<MemoryWriter>& writer) override
	{
		Command::save(writer);

		writer << m_objectID;
	}

	void load(Deserializer<ByteArray>& reader) override
	{
		Command::load(reader);

		reader >> m_objectID;
	}

	char cmdType() const override { return 0; }

	void action() override;
};
