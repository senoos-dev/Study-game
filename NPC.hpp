#ifndef NPC_HPP
#define NPC_HPP

#include <string>
#include <memory>

class DialogueTree;  // forward declaration

enum class NpcState {
    Friendly,
    Hostile
};

class NPC {
public:
    NPC(int id, const std::string& name, int health, int damage, NpcState state);
    ~NPC();  // Явно объявляем деструктор
    
    void setDialogue(std::unique_ptr<DialogueTree> dialogue);
    void becomeHostile();
    
    bool isHostile() const;
    int getHealth() const;
    int getDamage() const;
    const std::string& getName() const;
    int getId() const;
    
    void takeDamage(int amount);
    DialogueTree* getDialogue() const;

private:
    int m_id;
    std::string m_name;
    int m_health;
    int m_damage;
    NpcState m_state;
    std::unique_ptr<DialogueTree> m_dialogue;
};

#endif