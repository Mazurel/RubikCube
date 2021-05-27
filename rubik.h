#pragma once

#include "box.h"
#include "rotator.h"

#include "OgreSceneManager.h"
#include "OgreSceneNode.h"

using namespace Ogre;

#include <array>
#include <memory>

#define RUBIK_SEGMENTS 3

// Use unique pointer for memory safety
using RBoxPtr = std::unique_ptr<RBox>;

// 3D Array - RubikData[x][y][z]
using RubikData = std::array<std::array<std::array<RBoxPtr, RUBIK_SEGMENTS>, RUBIK_SEGMENTS>, RUBIK_SEGMENTS>;

// All rotations are two ways operation,
// for example R has R'
enum RubiksRotation {
    ROT_F,
    ROT_R,
    ROT_U,
    ROT_B,
    ROT_L,
    ROT_D
};

class RubikCube {
public:
    // Default contructor
    RubikCube(SceneManager *scnMgr);

    // This function does two things.
    // First of all it applies rotation to internal matrix of boxes. 
    // For example for L rotation, where x = 0, rotation works as follows 
    //  (OZ - Z axis, OY - Y axis, Numbers are some faces):
    //
    //        OZ ->                <- OY
    //  OY   1  2  3   ROT_L      7  4  1  OZ
    //  |    4  5  6  -------->   8  5  2   |
    //  ∨    7  8  9  (not alt)   9  6  3   ∨    
    //
    // Above operation is applied with respect to proper rotation type.
    //
    // After above step, function returns Rotator object which rotates
    // selected part of entities. It's implementation relies on internal
    // rotation function of the OGRE library.
    Rotator rotate(const RubiksRotation rotation, bool alternative = false);

    std::unique_ptr<RBox> &cubeAt(const int x, const int y, const int z);

    void nothing(int x);

    // Middle of the cube
    // WARNING: Everything may break when this variable will be changed
    const Vector3 Origin = Vector3(0, 0, 0);
private:
    // "Radius" of each box
    const int BoxSize = 1;

    std::unique_ptr<RubikData> data;
    SceneNode *cube;
};
