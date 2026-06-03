#ifndef DIALOGUE_TREE_HPP
#define DIALOGUE_TREE_HPP

#include <iostream>
#include <string>
#include <vector>

#include "Character.hpp"
#include "Location.hpp"
#include "NPC.hpp"

using namespace std;

enum class DialogueEffectType {
    None,
    SetHostile,
    GiveItem,
    OpenLocation,
    HealPlayer
};

struct DialogueEffect {
    DialogueEffectType type = DialogueEffectType::None;
    int itemId = 0;
    std::string itemName = "";
    int amount = 0;
};

struct DialogueChoice {
    string text;
    int nextNode = -1; // -1 => конечный узел, применить effect
    DialogueEffect effect;
};

struct DialogueNode {
    string npcLine;
    vector<DialogueChoice> choices;
};

enum class DialogueResult {
    Finished,
    StartedCombat,
    Cancelled
};

class DialogueTree {
public:
    DialogueTree() = default;
    explicit DialogueTree(vector<DialogueNode> nodes);

    void setNodes(vector<DialogueNode> nodes);
    static DialogueTree createHermitTree();

    DialogueResult run(int startNode,
                       Character& player,
                       NPC& npc,
                       Location* locationToOpen,
                       istream& in = cin,
                       ostream& out = cout);

private:
    void applyEffect(const DialogueEffect& effect,
                     Character& player,
                     NPC& npc,
                     Location* locationToOpen);

    vector<DialogueNode> m_nodes;
};

#endif
