#include "box.h"

#include <array>
#include <tuple>

std::array<std::tuple<String, Vector3>, 7> MaterialColors = {
    std::tuple<String, Vector3>("Red", Vector3(255, 0, 0)),
    std::tuple<String, Vector3>("Blue", Vector3(0, 0, 255)),
    std::tuple<String, Vector3>("Green", Vector3(0, 255, 0)),
    std::tuple<String, Vector3>("White", Vector3(255, 255, 255)),
    std::tuple<String, Vector3>("Yellow", Vector3(255, 255, 0)),
    std::tuple<String, Vector3>("Orange", Vector3(255, 165, 0)),
    std::tuple<String, Vector3>("Black", Vector3(0, 0, 0)),
};

void RBox::generateAllMaterials() {
    for (const std::tuple<String, Vector3>& color : MaterialColors) {
        RBox::generateMaterial(std::get<0>(color), std::get<1>(color));
    }
}

void RBox::generateMaterial(String name, Vector3 color) {
    // Create the texture
    TexturePtr texture = TextureManager::getSingleton().createManual(
        name + "RubikTexture",  // name
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        TEX_TYPE_2D,   // type
        256, 256,      // width & height
        0,             // number of mipmaps
        PF_BYTE_BGRA,  // pixel format
        TU_DEFAULT);   // usage; should be TU_DYNAMIC_WRITE_ONLY_DISCARDABLE for

    // Get the pixel buffer
    HardwarePixelBufferSharedPtr pixelBuffer = texture->getBuffer();

    // Lock the pixel buffer and get a pixel box
    pixelBuffer->lock(
        HardwareBuffer::HBL_NORMAL);  // for best performance use HBL_DISCARD!
    const PixelBox& pixelBox = pixelBuffer->getCurrentLock();

    uint8* pDest = static_cast<uint8*>(pixelBox.data);

    // Fill in some pixel data.
    for (size_t j = 0; j < 256; j++) {
        for (size_t i = 0; i < 256; i++) {
            if (i < 20 || j < 20 || i > 235 || j > 235) {
                *pDest++ = 0;    // B
                *pDest++ = 0;    // G
                *pDest++ = 0;    // R
                *pDest++ = 255;  // A
            } else {
                *pDest++ = color[2];  // B
                *pDest++ = color[1];  // G
                *pDest++ = color[0];  // R
                *pDest++ = 255;       // A
            }
        }

        pDest += pixelBox.getRowSkip() *
                 Ogre::PixelUtil::getNumElemBytes(pixelBox.format);
    }

    // Unlock the pixel buffer
    pixelBuffer->unlock();

    auto material = Ogre::MaterialManager::getSingleton().create(
        name + "RubikMaterial",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    material->setShininess(90);

    material->getTechnique(0)->getPass(0)->createTextureUnitState(
        name + "RubikTexture");
    material->getTechnique(0)->getPass(0)->setAmbient(0.6,0.6,0.6);
    material->getTechnique(0)->getPass(0)->setEmissive(0.3,0.3,0.3);
    material->getTechnique(0)->getPass(0)->setSelfIllumination(0.3,0.3,0.3);
    material->compile();
}

ManualObject* RBox::generateFace(ManualObject* o, const String& color,
                                 const RBOX_FACE face) {
    o->begin(color + "RubikMaterial", RenderOperation::OT_TRIANGLE_LIST);

    switch (face) {
        case RBOX_FACE::BACK:
            o->position(x + size, y - size, z - size);
            o->normal(0, 0, -1);
            o->textureCoord(0, 1);

            o->position(x - size, y - size, z - size);
            o->normal(0, 0, -1);
            o->textureCoord(1, 1);

            o->position(x - size, y + size, z - size);
            o->normal(0, 0, -1);
            o->textureCoord(1, 0);

            o->position(x + size, y + size, z - size);
            o->normal(0, 0, -1);
            o->textureCoord(0, 0);
            break;
        case RBOX_FACE::FRONT:
            o->position(x - size, y - size, z + size);
            o->normal(0, 0, 1);
            o->textureCoord(0, 1);

            o->position(x + size, y - size, z + size);
            o->normal(0, 0, 1);
            o->textureCoord(1, 1);

            o->position(x + size, y + size, z + size);
            o->normal(0, 0, 1);
            o->textureCoord(1, 0);

            o->position(x - size, y + size, z + size);
            o->normal(0, 0, 1);
            o->textureCoord(0, 0);
            break;
        case RBOX_FACE::LEFT:
            o->position(x - size, y - size, z - size);
            o->normal(-1, 0, 0);
            o->textureCoord(1, 1);

            o->position(x - size, y - size, z + size);
            o->normal(-1, 0, 0);
            o->textureCoord(0, 1);

            o->position(x - size, y + size, z + size);
            o->normal(-1, 0, 0);
            o->textureCoord(0, 0);

            o->position(x - size, y + size, z - size);
            o->normal(-1, 0, 0);
            o->textureCoord(1, 0);
            break;
        case RBOX_FACE::RIGHT:
            o->position(x + size, y + size, z - size);
            o->normal(1, 0, 0);
            o->textureCoord(0, 1);

            o->position(x + size, y + size, z + size);
            o->normal(1, 0, 0);
            o->textureCoord(1, 1);

            o->position(x + size, y - size, z + size);
            o->normal(1, 0, 0);
            o->textureCoord(1, 0);

            o->position(x + size, y - size, z - size);
            o->normal(1, 0, 0);
            o->textureCoord(0, 0);

        case RBOX_FACE::TOP:
            o->position(x + size, y + size, z + size);
            o->normal(0, 1, 0);
            o->textureCoord(0, 1);

            o->position(x + size, y + size, z - size);
            o->normal(0, 1, 0);
            o->textureCoord(1, 1);

            o->position(x - size, y + size, z - size);
            o->normal(0, 1, 0);
            o->textureCoord(1, 0);

            o->position(x - size, y + size, z + size);
            o->normal(0, 1, 0);
            o->textureCoord(0, 0);
            break;
        case RBOX_FACE::BOTTOM:
            o->position(x - size, y - size, z + size);
            o->normal(0, -1, 0);
            o->textureCoord(0, 1);

            o->position(x - size, y - size, z - size);
            o->normal(0, -1, 0);
            o->textureCoord(1, 1);

            o->position(x + size, y - size, z - size);
            o->normal(0, -1, 0);
            o->textureCoord(1, 0);

            o->position(x + size, y - size, z + size);
            o->normal(0, -1, 0);
            o->textureCoord(0, 0);
            break;
    }

    o->quad(0, 1, 2, 3);
    o->end();
    return o;
}

RBox::RBox(SceneManager* mgr, SceneNode* parent, const double x, const double y,
           const double z, const double size)
    : size(size), x(x), y(y), z(z), scnMgr(mgr) {
    this->node = parent->createChildSceneNode();
    this->node->setPosition(0, 0, 0);
}

void RBox::addFace(const RBOX_FACE face, const String& color) {
    if (this->faces.find(face) != this->faces.end()) throw std::exception();

    MovableObject* plane = this->generateFace(scnMgr->createManualObject(), color, face);

    this->node->attachObject(plane);
    this->faces[face] = { plane, color };
}

std::array<RBOX_FACE, 6> AllRBoxFaces = { TOP, BOTTOM, LEFT, RIGHT, FRONT, BACK };

void RBox::fillFaces() {
    for (const auto face: AllRBoxFaces) {
        if (this->faces.find(face) != this->faces.end()) continue;

        this->addFace(face, "Black");
    }
}

RBox::RFace& RBox::getFace(const RBOX_FACE face) {
    return faces[face];
}

const AxisAlignedBox& RBox::getBoundingboxFromFace(const RBOX_FACE face) {
    return faces[face].plane->getWorldBoundingBox();
}


void RBox::swapFaces(const RBOX_FACE f1, const RBOX_FACE f2) {
    auto faceCopy = faces[f1];
    faces[f1] = faces[f2];
    faces[f2] = faceCopy;
}

void RBox::pitchFaces(bool alternative) {
    if (!alternative) {
        swapFaces(TOP, BACK);
        swapFaces(TOP, BOTTOM);
        swapFaces(TOP, FRONT);
    } else {
        swapFaces(TOP, FRONT);
        swapFaces(TOP, BOTTOM);
        swapFaces(TOP, BACK);
    }
}

void RBox::yawFaces(bool alternative) {
    if (!alternative) {
        swapFaces(FRONT, RIGHT);
        swapFaces(FRONT, BACK);
        swapFaces(FRONT, LEFT);
        } else {
        swapFaces(FRONT, LEFT);
        swapFaces(FRONT, BACK);
        swapFaces(FRONT, RIGHT);
    }
}

void RBox::rollFaces(bool alternative) {
    if (!alternative) {
        swapFaces(TOP, LEFT);
        swapFaces(TOP, BOTTOM);
        swapFaces(TOP, RIGHT);
    } else {
        swapFaces(TOP, RIGHT);
        swapFaces(TOP, BOTTOM);
        swapFaces(TOP, LEFT);
    }
}
