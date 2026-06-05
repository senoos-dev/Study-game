#ifndef DIALOGUE_TREE_HPP
#define DIALOGUE_TREE_HPP

#include <string>
#include <vector>
#include <memory>
#include "Character.hpp"
#include "NPC.hpp"

struct DialogueNode {
    std::string text;
    struct Option {
        std::string text;
        int nextNodeId;
        std::string effect;  // "give_key", "give_potion", "hostile", "heal", "open_location", "none"
        int effectValue;     // для give_potion: 1(малое) или 2(большое), для heal: количество HP
        std::string effectTarget; // для open_location: название локации
    };
    std::vector<Option> options;
};

class DialogueTree {
public:
    DialogueTree();
    void addNode(int id, const std::string& text);
    void addOption(int fromNode, const std::string& optionText, int toNode, 
                   const std::string& effect = "none", int effectValue = 0, 
                   const std::string& effectTarget = "");
    
    void start(int startNodeId, Character& player, NPC& npc);
    
private:
    std::vector<DialogueNode> m_nodes;
    void applyEffect(const std::string& effect, int value, const std::string& target, 
                     Character& player, NPC& npc);
};

#endif