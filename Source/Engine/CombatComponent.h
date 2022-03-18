#pragma once
#include "Component.h"

namespace Plasmium {
    class CombatComponent : public Component {
    private:
        float healthMax;
        float currentHealth;

        float damage;

    public:
        CombatComponent(const ComponentCreationArgs& args, float health, float damage) : 
            Component(args),
            healthMax(health),
            currentHealth(health),
            damage(damage)
        {}

        float GetHealth() const { return currentHealth; }
        float GetDamage() const { return damage; }

        void DoDamage(float damage) { currentHealth -= damage; }
    };
}