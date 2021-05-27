#pragma once

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"

#include <map>
#include <array>
#include <string>

using namespace Ogre;


enum RBOX_FACE {
    TOP = 0,
    BOTTOM = 1,
    LEFT = 2,
    RIGHT = 3,
    FRONT = 4,
    BACK = 5
};

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

class RBox {
public: 
    struct RFace {
        MovableObject* plane;
        String color;
    };

    RBox(SceneManager* mgr, SceneNode* parent, double x, double y, double z, double size);

    static void generateAllMaterials();
    SceneNode* getNode() { return node; }

    // Add new colored face to the cube
    void addFace(const RBOX_FACE face, const String& color); 
    void fillFaces();

    RFace& getFace(const RBOX_FACE face);
    const AxisAlignedBox& getBoundingboxFromFace(const RBOX_FACE face);

    // Box face rotation. 
    // Required in order to make data inside RBox usable
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


