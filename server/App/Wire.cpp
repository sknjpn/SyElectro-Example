#include "Wire.h"
#include "World.h"
#include "Node.h"

void Wire::draw()
{
	Line(m_fr->getPosition(), m_to->getPosition()).draw(2.0, Palette::Red);
}

void Wire::save(Serializer<MemoryWriter>& writer)
{
	writer << m_id;
	writer << m_fr->m_id;
	writer << m_to->m_id;
}

void Wire::load(Deserializer<ByteArray>& reader)
{
	reader >> m_id;

	uint16 frId;
	uint16 toId;

	reader >> frId;
	reader >> toId;

	m_fr = g_world->getNode(frId);
	m_to = g_world->getNode(toId);
}
