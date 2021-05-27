#include "rotator.h"

Rotator::Rotator(std::vector<Node*> nodes, Vector3 axis) :
    nodes(nodes), axis(axis) {}

Rotator::Rotator(const Rotator& copy) : nodes(copy.nodes), axis(copy.axis) {}

bool Rotator::update(Real dt) {
    Real timeLeft = MaxDuration - duration;
    if (timeLeft < 0) return false;

    duration += dt;
    if (duration > MaxDuration) {
        // Finish rotation
        for (auto& node: nodes) {
            node->rotate(axis, Degree(90.0 * (timeLeft / MaxDuration)), Node::TS_WORLD); 
        }

        return false;
    }

    for (auto& node: nodes) {
        node->rotate(axis, Degree(90.0 * (dt / MaxDuration)), Node::TS_WORLD); 
    }

    return true;
}

