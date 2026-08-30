#pragma once

#include "events/Event.h"
#include "math/Vector3D.h"

class EnemyDiedEvent : public Event{
public:
    EnemyDiedEvent(const Vector3D& position, int scoreValue = 10)
    : m_Position(position), m_ScoreValue(scoreValue) {}

    const Vector3D& GetPosition() const{ return m_Position; }
    int GetScoreValue() const{ return m_ScoreValue; }

    static EventType GetStaticType(){ return EventType::EnemyDied; }
    EventType GetEventType() const override{ return EventType::EnemyDied; }

private:
    Vector3D m_Position;
    int m_ScoreValue;
};
