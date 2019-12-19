#pragma once
#include "stdafx.h"

class Command
{
public:
	uint16 m_id;
	uint64 m_timing = 0;	// 実行タイミング

	virtual ~Command() = default;

	virtual void save(Serializer<MemoryWriter>& writer)
	{
		writer << m_id;
		writer << m_timing;
	}

	virtual void load(Deserializer<ByteArray>& reader)
	{
		reader >> m_id;
		reader >> m_timing;
	}

	static shared_ptr<Command> MakeCommand(Deserializer<ByteArray>& reader);

	virtual char cmdType() const = 0;

	virtual void action() = 0;

	void send();
};
