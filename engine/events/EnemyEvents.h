#pragma once

#include "events/Event.h"
#include "math/Vector3D.h"

/**
 * @brief Event dispatched when an enemy dies.
 *
 * Contains the enemy's position at death and the score value awarded.
 *
 * @note This event can be used by UI systems to update score or by audio
 *       systems to play death sounds.
 * @see EnemySystem
 */
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
