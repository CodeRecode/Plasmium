#pragma once
#include "Component.h"
#include "GameplayTypes.h"

namespace Plasmium {
    class CombatComponent : public Component<CombatComponent> {
    private:
        float healthMax;
        float currentHealth;

        float damage;
        FactionType faction = FactionType::Monster;

    public:
        CombatComponent(EntityId entityId, float health, float damage) :
            Component(entityId),
            healthMax(health),
            currentHealth(health),
            damage(damage)
        {}

        float GetDamage() const { return damage; }
        FactionType GetFaction() const { return faction; }
        float GetHealth() const { return currentHealth; }

        void DoDamage(float damage) { currentHealth -= damage; }
        void SetFaction(FactionType faction) { this->faction = faction; }
    };
}