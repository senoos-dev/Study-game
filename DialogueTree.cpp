#include "DialogueTree.hpp"

#include <iostream>

#include "Logger.hpp"

using namespace std;

DialogueTree::DialogueTree(vector<DialogueNode> nodes) : m_nodes(std::move(nodes)) {}

void DialogueTree::setNodes(vector<DialogueNode> nodes) {
    m_nodes = std::move(nodes);
}

DialogueTree DialogueTree::createHermitTree() {
    vector<DialogueNode> nodes;

    nodes.push_back(DialogueNode{
        "Старец: Кто идёт по моим залам?",
        {
            {"Я путник, ищу выход", 1, {}},
            {"Отдай сокровища!", 2, {}},
            {"Мне не о чем говорить", -1, {}},
        }});

    nodes.push_back(DialogueNode{
        "Старец: Умный ответ. Возьми зелье и слушай: код сейфа — «орк».",
        {
            {"Спасибо", -1, {DialogueEffectType::GiveItem, Item(10, 40, "Зелье старца"), "", 0}},
            {"А если я ошибусь в коде?", 3, {}},
        }});

    nodes.push_back(DialogueNode{
        "Старец: Ты груб. Теперь я не друг.",
        {
            {"Извините", -1, {}},
            {"Сражайся со мной!", -1, {DialogueEffectType::SetHostile, Item(0, 0, ""), "", 0}},
        }});

    nodes.push_back(DialogueNode{
        "Старец: Тогда открою тебе потайной проход... если пожалеешь соседей.",
        {
            {"Открывай проход", -1, {DialogueEffectType::OpenLocation, Item(0, 0, ""), "hall", 0}},
            {"Нет, я сам справлюсь", -1, {DialogueEffectType::HealPlayer, Item(0, 0, ""), "", 15}},
        }});

    return DialogueTree(nodes);
}

void DialogueTree::applyEffect(const DialogueEffect& effect,
                               Character& player,
                               NPC& npc,
                               Location* locationToOpen) {
    switch (effect.type) {
    case DialogueEffectType::SetHostile:
        npc.setMood(NpcMood::Hostile);
        Logger::log("Dialogue: NPC " + npc.getName() + " became hostile");
        break;
    case DialogueEffectType::GiveItem:
        player.add_to_inventory(effect.item);
        Logger::log("Dialogue: gave item " + effect.item.get_name());
        break;
    case DialogueEffectType::OpenLocation:
        if (locationToOpen != nullptr) {
            locationToOpen->setOpen(true);
            Logger::log("Dialogue: opened location " + locationToOpen->getId());
        } else {
            Logger::log("Dialogue: open location failed (null)");
        }
        break;
    case DialogueEffectType::HealPlayer:
        player.add_health(effect.amount);
        Logger::log("Dialogue: healed player for " + to_string(effect.amount));
        break;
    default:
        break;
    }
}

DialogueResult DialogueTree::run(int startNode,
                                 Character& player,
                                 NPC& npc,
                                 Location* locationToOpen,
                                 istream& in,
                                 ostream& out) {
    if (npc.isHostile()) {
        out << npc.getName() << " не хочет говорить.\n";
        return DialogueResult::StartedCombat;
    }

    int nodeIndex = startNode;
    while (true) {
        if (nodeIndex < 0 || nodeIndex >= (int)m_nodes.size()) {
            out << "Конец диалога.\n";
            return DialogueResult::Finished;
        }

        const DialogueNode& node = m_nodes[nodeIndex];
        out << "\n" << node.npcLine << "\n";
        for (int i = 0; i < (int)node.choices.size(); i++) {
            out << "  " << (i + 1) << ") " << node.choices[i].text << "\n";
        }
        out << "  0) Выйти из диалога\n";
        out << "Выбор: ";

        int choiceNum = -1;
        in >> choiceNum;
        if (!in) {
            return DialogueResult::Cancelled;
        }

        if (choiceNum == 0) {
            Logger::log("Dialogue cancelled with " + npc.getName());
            return DialogueResult::Cancelled;
        }

        int idx = choiceNum - 1;
        if (idx < 0 || idx >= (int)node.choices.size()) {
            out << "Неверный выбор.\n";
            continue;
        }

        const DialogueChoice& picked = node.choices[idx];
        if (picked.nextNode < 0) {
            applyEffect(picked.effect, player, npc, locationToOpen);
            if (npc.isHostile()) {
                return DialogueResult::StartedCombat;
            }
            return DialogueResult::Finished;
        }

        nodeIndex = picked.nextNode;
    }
}
