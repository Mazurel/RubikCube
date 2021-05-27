#pragma once

#include "OgreNode.h"

#include <vector>

using namespace Ogre;

// This class rotates by 90 degrees group of Nodes around some axis over time
class Rotator {
public:
    // Set group of Nodes and axis
    Rotator(std::vector<Node*> nodes, Vector3 axis);
    Rotator(const Rotator& copy);

    // Function that should be called every frame to make rotation happen
    bool update(Real dt);

private:
    // Time that each rotation takes
    const Real MaxDuration = 0.4;
    std::vector<Node*> nodes;
    Real duration = 0;
    Vector3 axis;
};
