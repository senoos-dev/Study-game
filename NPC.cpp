#include "NPC.hpp"
#include "DialogueTree.hpp"

NPC::NPC(int id, const std::string& name, int health, int damage, NpcState state)
    : m_id(id), m_name(name), m_health(health), m_damage(damage), m_state(state) {}

NPC::~NPC() = default;  // Здесь DialogueTree уже полностью известен

void NPC::setDialogue(std::unique_ptr<DialogueTree> dialogue) {
    m_dialogue = std::move(dialogue);
}

void NPC::becomeHostile() {
    m_state = NpcState::Hostile;
}

bool NPC::isHostile() const {
    return m_state == NpcState::Hostile;
}

int NPC::getHealth() const {
    return m_health;
}

int NPC::getDamage() const {
    return m_damage;
}

const std::string& NPC::getName() const {
    return m_name;
}

int NPC::getId() const {
    return m_id;
}

void NPC::takeDamage(int amount) {
    m_health -= amount;
    if (m_health < 0) m_health = 0;
}

DialogueTree* NPC::getDialogue() const {
    return m_dialogue.get();
}