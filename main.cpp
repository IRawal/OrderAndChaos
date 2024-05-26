#include <iostream>
#include <vector>
#include <map>
#include <unordered_set>

struct Node {
    Node* parent;
    std::vector<Node*>* children;
    int child_num;
    uint16_t Xs;
    uint16_t Os;
};

int nodes = 0;

std::unordered_set<uint16_t>* pSeen;

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

bool is_over(uint16_t state) {
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
        if ((check & state) == check) {
            return true;
        }
    }
    return false;
}
Node* make_move(Node* parent, uint16_t move, bool X) {

    uint16_t oldState = X ? parent->Xs : parent->Os;
    uint16_t newState = oldState | move;
    if (newState == oldState) {
        return nullptr;
    }

    uint32_t hash;

    if (pSeenXs->count(newState) > 0 && pSeenOs->count(newState) > 0)
        return nullptr;

    printf("%i\n", newState);
    Node* pNewNode = static_cast<Node*>(malloc(sizeof(Node)));
    pNewNode->children = new std::vector<Node*>();


    if (X) {
        pNewNode->Xs = newState;
        pNewNode->Os = parent->Os;
        pSeenXs->insert(newState);
    }
    else {
        pNewNode->Xs = parent->Xs;
        pNewNode->Os = newState;
        pSeenOs->insert(newState);
    }
    return pNewNode;
}
void build_tree(Node* pParentNode) {
    if (is_over(pParentNode->Xs) || is_over(pParentNode->Os)) return;
    nodes += 1;
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

    pSeenXs = new std::unordered_set<uint16_t>();
    pSeenOs = new std::unordered_set<uint16_t>();

    build_tree(pRoot);

    free(pRoot);
}
