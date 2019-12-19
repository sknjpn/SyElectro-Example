#pragma once

#include "Command.h"

class Command_RemoveWire
	: public Command
{
public:
	int16 m_wireID;

	void save(Serializer<MemoryWriter>& writer) override
	{
		Command::save(writer);

		writer << m_wireID;
	}

	void load(Deserializer<ByteArray>& reader) override
	{
		Command::load(reader);

		reader >> m_wireID;
	}

	char cmdType() const override { return 1; }

	void action() override;
};

