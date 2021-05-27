#include "rubik.h"

RubikCube::RubikCube(SceneManager* scnMgr) : data(new RubikData()) {
    cube = scnMgr->getRootSceneNode()->createChildSceneNode();
    cube->setPosition(0, 0, 0);

    // Add proper colors on all the faces of buxes
    for (int x = 0; x < RUBIK_SEGMENTS; x++) {
        for (int y = 0; y < RUBIK_SEGMENTS; y++) {
            for (int z = 0; z < RUBIK_SEGMENTS; z++) {
                // TODO: Remove constant numbers
                data->data()[x][y][z] = std::make_unique<RBox>(
                    scnMgr, cube, x * 2 - 2, y * 2 - 2, z * 2 - 2, 1);

                if (z == 0)
                    data->data()[x][y][z]->addFace(BACK, "Orange");
                if (z == 2)
                    data->data()[x][y][z]->addFace(FRONT, "Red");
                if (x == 0)
                    data->data()[x][y][z]->addFace(LEFT, "Green");
                if (x == 2)
                    data->data()[x][y][z]->addFace(RIGHT, "Blue");
                if (y == 0)
                    data->data()[x][y][z]->addFace(BOTTOM, "Yellow");
                if (y == 2)
                    data->data()[x][y][z]->addFace(TOP, "White");
            }
        }
    }

    // Fill rest of the faces with black faces
    for (int x = 0; x < RUBIK_SEGMENTS; x++) {
        for (int y = 0; y < RUBIK_SEGMENTS; y++) {
            for (int z = 0; z < RUBIK_SEGMENTS; z++) {
                data->data()[x][y][z]->fillFaces();
            }
        }
    }
}

Rotator RubikCube::rotate(const RubiksRotation rotation, bool alternative) {
    std::unique_ptr<RubikData> newData = std::make_unique<RubikData>();
    std::vector<Node*> nodes;
    Vector3 dir;

    // This lambda function is a helper function for box rotation
    // What it does is it moves with respect to `alternative` variable
    // box from (sx, sy, sz) to (dx1, dy1, dz1) or (dx2, dy2, dz2).
    // It also returns porinter refrence to the box.
    auto newMoveSegment = [&newData, &nodes, alternative, this](int sx, int sy, int sz, int dx1, int dy1, int dz1, int dx2, int dy2, int dz2) -> std::unique_ptr<RBox>& {
        if (!alternative) {
            newData->data()[dx1][dy1][dz1].swap(this->data->data()[sx][sy][sz]);
            auto& rbox = newData->data()[dx1][dy1][dz1];
            nodes.push_back(rbox->getNode());
            return rbox;
        } else {
            newData->data()[dx2][dy2][dz2].swap(this->data->data()[sx][sy][sz]);
            auto& rbox = newData->data()[dx2][dy2][dz2];
            nodes.push_back(rbox->getNode());
            return rbox;
        }
    };

    auto copySegment = [&newData, &nodes, alternative, this](int x, int y, int z) {
        newData->data()[x][y][z].swap(this->data->data()[x][y][z]);
    };

    // TODO: Add rotations !
    for (int x = 0; x < RUBIK_SEGMENTS; x++) {
        for (int y = 0; y < RUBIK_SEGMENTS; y++) {
            for (int z = 0; z < RUBIK_SEGMENTS; z++) {
                // Makes current pointer point to nullptr and new one to proper
                // cube :)
                switch (rotation) {
                    case ROT_L:
                        if (x == 0) {
                            auto& rbox = newMoveSegment(
                                           /* Source:     */ x, y, z, 
                                           /* To (normal):*/ x, z, RUBIK_SEGMENTS - 1 - y,
                                           /* To (alt):   */ x, RUBIK_SEGMENTS - 1 - z, y);
                            rbox->pitchFaces(alternative);
                        } else {
                            copySegment(x, y, z);
                        }
                        break;
                    case ROT_R:
                        if (x == 2) {
                            auto& rbox = newMoveSegment(
                                           /* Source:     */ x, y, z, 
                                           /* To (normal):*/ x, RUBIK_SEGMENTS - 1 - z, y,
                                           /* To (alt):   */ x, z, RUBIK_SEGMENTS - 1 - y);

                            rbox->pitchFaces(!alternative);
                        } else {
                            copySegment(x, y, z);
                        }
                        break;
                    case ROT_U:
                        if (y == 2) {
                            auto& rbox = newMoveSegment(
                                           /* Source:     */ x, y, z, 
                                           /* To (normal):*/ z, y, RUBIK_SEGMENTS - 1 - x, 
                                           /* To (alt):   */ RUBIK_SEGMENTS - 1 - z, y, x);
                  
                            rbox->yawFaces(alternative);
                        } else {
                            copySegment(x, y, z);
                        }
                        break;
                    case ROT_D:
                        if (y == 0) {
                          auto& rbox = newMoveSegment(
                                        /* Source:     */ x, y, z, 
                                        /* To (normal):*/ RUBIK_SEGMENTS - 1 - z, y, x,
                                        /* To (alt):   */ z, y, RUBIK_SEGMENTS - 1 - x);

                            rbox->yawFaces(!alternative);
                        } else {
                            copySegment(x, y, z);
                        }
                        break;
                    case ROT_B:
                        if (z == 0) {
                            auto& rbox = newMoveSegment(
                                           /* Source:     */ x, y, z, 
                                           /* To (normal):*/ y, RUBIK_SEGMENTS - 1 - x, z,  
                                           /* To (alt):   */ RUBIK_SEGMENTS - 1 - y, x, z);

                            rbox->rollFaces(!alternative);
                        } else {
                            copySegment(x, y, z);
                        }
                        break;
                    case ROT_F:
                        if (z == 2) {
                            auto& rbox = newMoveSegment(
                                           /* Source:     */ x, y, z, 
                                           /* To (normal):*/ RUBIK_SEGMENTS - 1 - y, x, z, 
                                           /* To (alt):   */ y, RUBIK_SEGMENTS - 1 - x, z);

                            rbox->rollFaces(alternative);
                        } else {
                            copySegment(x, y, z);
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }

    // Swaps new data
    data.swap(newData);

    // Finds proper rotation axis
    switch (rotation) {
        case ROT_L:
            dir = -Vector3::UNIT_X;
            break;
        case ROT_R:
            dir = Vector3::UNIT_X;
            break;
        case ROT_U:
            dir = Vector3::UNIT_Y;
            break;
        case ROT_D:
            dir = -Vector3::UNIT_Y;
            break;
        case ROT_B:
            dir = -Vector3::UNIT_Z;
            break;
        case ROT_F:
            dir = Vector3::UNIT_Z;
            break;
        default:
            break;
    }

    return Rotator(nodes, (alternative) ? -dir : dir);
}

std::unique_ptr<RBox>& RubikCube::cubeAt(const int x, const int y, const int z) {
    return data->data()[x][y][z];
}

void RubikCube::nothing(int x) {

}
