#pragma once

#include "OgreNode.h"

#include <vector>

using namespace Ogre;

/// This class rotates by 90 degrees group of Nodes around some axis over time
class Rotator {
public:
    /// Constructor that takes Nodes to be rotated along axis
    /// @param nodes List of pointers of Nodes to be rotated
    /// @param axis Axis that nodes will be rotated along
    Rotator(std::vector<Node*> nodes, Vector3 axis);

    /// Simple copy contructor
    Rotator(const Rotator& copy);

    /// Function that should be called every frame to make rotation happen
    /// @param dt Time interval
    bool update(Real dt);

private:
    /// Time that each rotation takes
    const Real MaxDuration = 0.4;
    std::vector<Node*> nodes;
    Real duration = 0;
    Vector3 axis;
};
