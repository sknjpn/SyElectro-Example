#pragma once

#include "Command.h"

class Command_SetWire
	: public Command
{
public:
	uint16 m_nodeFrID;
	uint16 m_nodeToID;

public:
	void save(Serializer<MemoryWriter>& writer) override
	{
		Command::save(writer);

		writer << m_nodeFrID;
		writer << m_nodeToID;
	}

	void load(Deserializer<ByteArray>& reader) override
	{
		Command::load(reader);

		reader >> m_nodeFrID;
		reader >> m_nodeToID;
	}

	char cmdType() const override { return 3; }

	void action() override;
};

