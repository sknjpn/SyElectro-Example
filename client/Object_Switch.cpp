#include "Object_Switch.h"
#include "Node.h"

Object_Switch::Object_Switch()
{
	m_nodes.emplace_back(MakeShared<Node>(Vec2(12, 0)));
	m_nodes.emplace_back(MakeShared<Node>(Vec2(12, 32)));
}

void Object_Switch::update()
{
	if (m_isOn)
	{
		if (!m_nodes[0]->isConnected(m_nodes[1]))
		{
			m_nodes[0]->connect(m_nodes[1]);
			m_nodes[1]->connect(m_nodes[0]);
		}
	}
	else
	{
		if (m_nodes[0]->isConnected(m_nodes[1]))
		{
			m_nodes[0]->disconnect(m_nodes[1]);
			m_nodes[1]->disconnect(m_nodes[0]);
		}
	}
}