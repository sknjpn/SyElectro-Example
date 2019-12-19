#include "Object_Battery.h"
#include "Node.h"

Object_Battery::Object_Battery()
{
	m_nodes.emplace_back(MakeShared<Node>(Vec2(48, 8)));
	m_nodes.emplace_back(MakeShared<Node>(Vec2(0, 8)));
}

void Object_Battery::update()
{
	m_nodes[0]->m_nowState = Node::State::High;
	m_nodes[1]->m_nowState = Node::State::Low;
}