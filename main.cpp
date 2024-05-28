#include <iostream>
#include <vector>
#include <map>
#include <unordered_set>
#include <unordered_map>

struct Node {
    Node* parent;
    std::vector<Node*>* children;
    uint16_t Xs;
    uint16_t Os;
    int value;
};

std::unordered_map<uint32_t, Node*>* pSeen;

uint16_t moves[] = {
    2,
    4,
    8,
    16,
    32,
    64,
    128,
    256,
    512,
    1024,
    2048,
    4096,
    8192,
    16384,
    32768
};

int get_winner(Node* parent) {
    uint16_t checks[] = {
        // rows
        0xF000,
        0xF00,
        0xF0,
        0xF,

        // cols
        0x8888,
        0x4444,
        0x2222,
        0x1111,

        // diagonal
        0x8421,
        0x1248
    };
    for (uint16_t check : checks) {
        if ((check & parent->Xs) == check) {
            return 1;
        }
    }
    for (uint16_t check : checks) {
        if ((check & parent->Os) == check) {
            return 1;
        }
    }
    if (parent->Xs | parent->Os == 0b1111111111111111) {
        return -1;
    }
    return 0;
}
uint32_t calculate_hash(uint16_t Xs, uint16_t Os) {
    return ((uint32_t)Xs << 16 | (uint32_t)Os);
}
Node* make_move(Node* parent, uint16_t move, bool X) {
    if ((parent->Xs | move) == parent->Xs || (parent->Os | move) == parent->Os)
        return nullptr;

    uint16_t oldState = X ? parent->Xs : parent->Os;
    uint16_t newState = oldState | move;

    uint16_t newXs = X ? newState: parent->Xs;
    uint16_t newOs = !X ? newState: parent->Os;

    uint32_t board_state = calculate_hash(newXs, newOs);

    if (pSeen->count(board_state))
        return nullptr;

    Node* pNewNode = static_cast<Node*>(malloc(sizeof(Node)));
    pNewNode->children = new std::vector<Node*>();
    pNewNode->parent = parent;

    pNewNode->Xs = newXs;
    pNewNode->Os = newOs;

    pSeen->insert({board_state, pNewNode});
    return pNewNode;
}
int test_traversal(Node* node, int ctr) {
    for (Node* child : *node->children) {
        pSeen->find(calculate_hash(child->Xs, child->Os))->second;
        ctr += test_traversal(child, ctr);
    }
    return ctr + 1;
}
int minimax(Node* node, bool maxing) {
    int winner = get_winner(node);
    if (winner != 0) {
        node->value = winner;
    }
    uint32_t board_state = calculate_hash(node->Xs, node->Os);

    if (node->children->empty()) {
        node->value = minimax(pSeen->find(board_state)->second, maxing);
    }
    int minimax_val = 0;
    for (Node* child : *node->children) {
        int val = minimax(child, !maxing);
        if (maxing)
            minimax_val = std::max(minimax_val, val);
        else
            minimax_val = std::min(minimax_val, val);
    }
    node->value = minimax_val;
    return minimax_val;
}
void build_tree(Node* pParentNode) {
    if (get_winner(pParentNode) != 0) return;
    // All possible X moves
    for (uint16_t move : moves) {
        Node* pNewState = make_move(pParentNode, move, true);
        if (pNewState == nullptr)
            continue;

        pParentNode->children->push_back(pNewState);
        build_tree(pNewState);
    }
    // All possible O moves
    for (uint16_t move : moves) {
        Node* pNewState = make_move(pParentNode, move, false);
        if (pNewState == nullptr)
            continue;

        pParentNode->children->push_back(pNewState);
        build_tree(pNewState);
    }
}
int main() {
    Node* pRoot = static_cast<Node*>(malloc(sizeof(Node)));
    pRoot->children = new std::vector<Node*>();
    pRoot->Os = 0;
    pRoot->Xs = 0;

    pSeen = new std::unordered_map<uint32_t, Node*>();
    //pSeen->reserve(22012);
    build_tree(pRoot);
    //printf("Nodes: %i\n", nodes);

    minimax(pRoot, true);

    free(pRoot);
}
