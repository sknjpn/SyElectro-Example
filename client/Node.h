#pragma once

class Object;

class Node
{
public:
	enum struct State
	{
		None,
		High,
		Low,
	};

private:

	// Objectによって接続管理させる
	Array<weak_ptr<Node>> m_connecteds;

public:
	uint16 m_id;
	Vec2 m_localPosition;
	State m_nowState = State::None;
	State m_preState = State::None;
	weak_ptr<Object> m_object;

	Node(const Vec2& localPosition)
		: m_localPosition(localPosition)
	{

	}

	Vec2 getPosition() const;

	void draw();

	void flowToConnected();

	bool isConnected(const shared_ptr<Node>& to) const;
	void connect(const shared_ptr<Node>& to) { m_connecteds.emplace_back(to); }
	void disconnect(const shared_ptr<Node>& to) { m_connecteds.remove_if([to](const auto& n) { return n.lock() == to; }); }

	virtual void save(Serializer<MemoryWriter>& writer)
	{
		writer << m_id;
		writer << m_localPosition;
		writer << m_nowState;
		writer << m_preState;
	}

	virtual void load(Deserializer<ByteArray>& reader)
	{
		reader >> m_id;
		reader >> m_localPosition;
		reader >> m_nowState;
		reader >> m_preState;
	}
};
