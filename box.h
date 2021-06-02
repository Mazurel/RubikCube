#pragma once

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"

#include <map>
#include <array>
#include <string>

using namespace Ogre;

/// Enum that represents all possible faces that
/// some cube may have
enum RBOX_FACE {
    TOP = 0,
    BOTTOM = 1,
    LEFT = 2,
    RIGHT = 3,
    FRONT = 4,
    BACK = 5
};

/// Converts RBOX_FACE to its string representation
inline std::string to_string(const RBOX_FACE face) {
    switch (face) {
        case TOP: return "Top";
        case BOTTOM: return "Bottom";
        case LEFT: return "Left";
        case RIGHT: return "Right";
        case FRONT: return "Front";
        case BACK: return "Back";
        default: return "";
    }
}

/// Represents cube inside Rubiks cube
class RBox {
public: 
    /// This structure represents one face of the cube
    struct RFace {
        /// Pointer to the "mesh"
        MovableObject* plane;
        /// Color of the face
        String color;
    };

    /// Constructs the cube (without initializing any faces)
    /// @param mgr Pointer to SceneManager
    /// @param parent Parent of the cube
    /// @param x X coordinate of the cube
    /// @param y Y coordinate of the cube
    /// @param z Z coordinate of the cube
    /// @param size Size of the cube
    RBox(SceneManager* mgr, SceneNode* parent, double x, double y, double z, double size);

    /// Generates all materials required for the cube
    /// to adopt all avaible colors
    static void generateAllMaterials();

    /// Getter of the SceneNode pointer
    SceneNode* getNode() { return node; }

    /// Adds new face to the cube
    /// @param face Location of the face (FRONT, BACK, etc)
    /// @param color Color of the face 
    void addFace(const RBOX_FACE face, const String& color); 

    /// Sets or faces that were not initialized to black
    void fillFaces();

    /// Returns reference to the face
    RFace& getFace(const RBOX_FACE face);

    /// Return bounding box of some face. Useful for Raycasting
    const AxisAlignedBox& getBoundingboxFromFace(const RBOX_FACE face);

    /// Box face rotation. 
    /// Required in order to make data inside RBox usable
    void pitchFaces(bool alternative=false);
    void yawFaces(bool alternative=false);
    void rollFaces(bool alternative=false);
private:
    Real size;
    Real x, y, z;
    SceneNode* node;
    SceneManager* scnMgr;

    std::map<RBOX_FACE, RFace> faces;

    static void generateMaterial(String name, Vector3 colors);

    ManualObject* generateFace(ManualObject* o, const String& color, const RBOX_FACE face);

    void swapFaces(const RBOX_FACE f1, const RBOX_FACE f2);
};


