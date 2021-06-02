#pragma once

#include "box.h"
#include "rotator.h"

#include "OgreSceneManager.h"
#include "OgreSceneNode.h"

using namespace Ogre;

#include <array>
#include <memory>

#define RUBIK_SEGMENTS 3

/// Use unique pointer for memory safety
using RBoxPtr = std::unique_ptr<RBox>;

/// 3D Array - RubikData[x][y][z]
using RubikData = std::array<std::array<std::array<RBoxPtr, RUBIK_SEGMENTS>, RUBIK_SEGMENTS>, RUBIK_SEGMENTS>;

/// All rotations are two ways operation,
/// for example R has R'
///
/// This enum defines only one way rotations
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
    /// Default contructor, it takes SceneManager and creates rubik cube
    /// at the origin of the scene
    RubikCube(SceneManager *scnMgr);

    /// This function does two things.
    /// First of all it applies rotation to internal matrix of boxes. 
    /// For example for L rotation, where x = 0, rotation works as follows 
    /// <pre>
    ///  (OZ - Z axis, OY - Y axis, Numbers are some faces):\n
    ///\n
    ///        OZ ->                <- OY\n
    ///  OY   1  2  3   ROT_L      7  4  1  OZ\n
    ///  |    4  5  6  -------->   8  5  2   |\n
    ///  v    7  8  9  (not alt)   9  6  3   v    \n
    ///\n
    /// </pre>
    /// Above operation is applied with respect to proper rotation type.
    ///
    /// @param rotation Rotation to applied to the cube
    /// @param alternative If set to true, makes inverted rotation to the default one
    /// @returns Rotator object, that represents proper animation
    Rotator rotate(const RubiksRotation rotation, bool alternative = false);

    /// Gets reference to pointer that points to cube 
    /// at the location (x, y, z), according to RubikData type
    std::unique_ptr<RBox> &cubeAt(const int x, const int y, const int z);

    // Middle of the cube
    // WARNING: Everything may break when this variable will be changed
    const Vector3 Origin = Vector3(0, 0, 0);
private:
    /// "Radius" of each box
    const int BoxSize = 1;

    /// Pointer to rubik cube data
    std::unique_ptr<RubikData> data;

    /// Scene object that represents rubik cube 
    SceneNode *cube;
};
