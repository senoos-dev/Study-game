#include "DialogueTree.hpp"
#include <iostream>
#include "Location.hpp"
#include "HealthPotion.hpp"

using namespace std;

DialogueTree::DialogueTree() {
    m_nodes.resize(10); // запас
}

void DialogueTree::addNode(int id, const string& text) {
    if (id >= (int)m_nodes.size()) m_nodes.resize(id + 5);
    m_nodes[id].text = text;
}

void DialogueTree::addOption(int fromNode, const string& optionText, int toNode,
                              const string& effect, int effectValue, const string& effectTarget) {
    if (fromNode >= (int)m_nodes.size()) m_nodes.resize(fromNode + 5);
    DialogueNode::Option opt = {optionText, toNode, effect, effectValue, effectTarget};
    m_nodes[fromNode].options.push_back(opt);
}

void DialogueTree::applyEffect(const string& effect, int value, const string& target,
                                Character& player, NPC& npc) {
    if (effect == "give_key") {
        player.add_to_inventory(make_unique<Item>(10, 0, "Ключ"));
        cout << "[!] Ты получил КЛЮЧ!\n";
    } else if (effect == "give_potion") {
        if (value == 1) {
            player.add_to_inventory(make_unique<HealthPotion>(1, "Малое зелье", 30));
            cout << "[!] Ты получил МАЛОЕ ЗЕЛЬЕ!\n";
        } else {
            player.add_to_inventory(make_unique<HealthPotion>(2, "Большое зелье", 60));
            cout << "[!] Ты получил БОЛЬШОЕ ЗЕЛЬЕ!\n";
        }
    } else if (effect == "hostile") {
        npc.becomeHostile();
        cout << "[!] NPC стал ВРАЖДЕБНЫМ!\n";
    } else if (effect == "heal") {
        player.add_health(value);
        cout << "[!] Ты восстановил " << value << " HP!\n";
    } else if (effect == "open_location") {
        cout << "[!] " << target << " открыта!\n";
        // Здесь нужна логика открытия локации по имени
    }
}

void DialogueTree::start(int startNodeId, Character& player, NPC& npc) {
    int currentNode = startNodeId;
    
    while (currentNode != -1) {
        if (currentNode >= (int)m_nodes.size()) break;
        
        const auto& node = m_nodes[currentNode];
        cout << "\n" << node.text << "\n";
        
        if (node.options.empty()) {
            cout << "\n[Диалог окончен]\n";
            break;
        }
        
        for (size_t i = 0; i < node.options.size(); i++) {
            cout << "  " << (i + 1) << ") " << node.options[i].text << "\n";
        }
        cout << "  0) Выйти из диалога\n";
        cout << "Выбор: ";
        
        int choice;
        cin >> choice;
        
        if (choice == 0) {
            cout << "\n[Ты прервал разговор]\n";
            break;
        }
        
        if (choice < 1 || choice > (int)node.options.size()) {
            cout << "Неверный выбор!\n";
            continue;
        }
        
        const auto& opt = node.options[choice - 1];
        applyEffect(opt.effect, opt.effectValue, opt.effectTarget, player, npc);
        
        if (opt.effect == "hostile") {
            cout << "\n[Начался бой!]\n";
            break;
        }
        
        currentNode = opt.nextNodeId;
    }
}