#include <optional>
#include <iostream>
#include <stack>

// Ogre includes
#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreCameraMan.h"
#include "OgreInput.h"
#include "OgreTrays.h"

// My includes
#include "box.h"
#include "rubik.h"

using namespace Ogre;

class RubikApp : public OgreBites::ApplicationContext,
                 public OgreBites::InputListener,
                 public OgreBites::TrayListener {
public:
    RubikApp();
    void setup() override;
    bool keyPressed(const OgreBites::KeyboardEvent& evt) override;
    bool keyReleased(const OgreBites::KeyboardEvent& evt) override;

    bool mouseMoved(const OgreBites::MouseMotionEvent& evt) override;
    bool mousePressed(const OgreBites::MouseButtonEvent& evt) override;
    bool mouseReleased(const OgreBites::MouseButtonEvent& evt) override;
    bool mouseWheelRolled(const OgreBites::MouseWheelEvent& evt) override;

    bool frameStarted(const Ogre::FrameEvent& evt) override;

    void buttonHit(OgreBites::Button* b) override;
private:
    void generateSolidMaterial(String name, Vector3 color);
    RBOX_FACE getCurrentFace();

    bool isCubeRotating();
    void updateRotation(float dt);
    void rotateCube(const RubiksRotation rotation, bool alternative, bool saveMove=true);

    Ogre::SceneNode* cam;
    Ogre::SceneNode* lightNode;
    Ogre::SceneNode* sky;

    OgreBites::CameraMan* cameraMan;
    OgreBites::TrayManager* mTrayMgr;

    std::optional<RubikCube> cube;
    std::optional<Rotator> rotator;

    std::stack<std::tuple<RubiksRotation, bool>> rotations;

    bool alt = false;
};

RubikApp::RubikApp() : OgreBites::ApplicationContext("Kostka Rubika") {}

bool RubikApp::isCubeRotating() {
    return rotator.has_value() && rotator->update(0);
}

void RubikApp::updateRotation(float dt) {
    if (rotator.has_value()) rotator->update(dt);
}

void RubikApp::rotateCube(const RubiksRotation rotation, bool alternative, bool saveMove) {
    if (!isCubeRotating()) {
        rotator.emplace(cube->rotate(rotation, alternative));
        // Safe rotation type
        if (saveMove)
            rotations.push(std::make_tuple(rotation, alternative));
    }
}

void RubikApp::buttonHit(OgreBites::Button *b) {
    if (b->getName() == "ROTL") rotateCube(ROT_L, false);
    if (b->getName() == "ROTLP") rotateCube(ROT_L, true);
    if (b->getName() == "ROTR") rotateCube(ROT_R, false);
    if (b->getName() == "ROTRP") rotateCube(ROT_R, true);
    if (b->getName() == "ROTF") rotateCube(ROT_F, false);
    if (b->getName() == "ROTFP") rotateCube(ROT_F, true);
    if (b->getName() == "ROTB") rotateCube(ROT_B, false);
    if (b->getName() == "ROTBP") rotateCube(ROT_B, true);
    if (b->getName() == "ROTU") rotateCube(ROT_U, false);
    if (b->getName() == "ROTUP") rotateCube(ROT_U, true);
    if (b->getName() == "ROTD") rotateCube(ROT_D, false);
    if (b->getName() == "ROTDP") rotateCube(ROT_D, true);
    if (b->getName() == "UNDO" && !isCubeRotating() && rotations.size() > 0){
        auto[rotType, alternative] = rotations.top();
        rotations.pop();
        rotateCube(rotType, !alternative, false);
    }
}

bool RubikApp::keyReleased(const OgreBites::KeyboardEvent& evt) {
    if (evt.keysym.sym == OgreBites::SDLK_LSHIFT) alt = !alt;

    return true;
}

bool RubikApp::keyPressed(const OgreBites::KeyboardEvent& evt) {
    if (evt.keysym.sym == OgreBites::SDLK_ESCAPE) {
        getRoot()->queueEndRendering();
    }

    if (evt.keysym.sym == OgreBites::SDLK_LSHIFT) alt = !alt;

    if (rotator.has_value() && !rotator->update(0) || !rotator.has_value()) {
        if ((char)evt.keysym.sym == 'l') {
            rotateCube(ROT_L, alt);
        }
        if ((char)evt.keysym.sym == 'r') {
            rotateCube(ROT_R, alt);
        }
        if ((char)evt.keysym.sym == 'u') {
            rotateCube(ROT_U, alt);
        }
        if ((char)evt.keysym.sym == 'd') {
            rotateCube(ROT_D, alt);
        }
        if ((char)evt.keysym.sym == 'f') {
            rotateCube(ROT_F, alt);
        }
        if ((char)evt.keysym.sym == 'b') {
            rotateCube(ROT_B, alt);
        }
    }

    return true;
}

bool RubikApp::mouseReleased(const OgreBites::MouseButtonEvent &evt) {
    OgreBites::InputListener::mouseReleased(evt);
    cameraMan->mouseReleased(evt);

    return true;
}

bool RubikApp::mousePressed(const OgreBites::MouseButtonEvent &evt) {
    OgreBites::InputListener::mousePressed(evt);

    // Disable targer modification
    if (evt.button != OgreBites::BUTTON_RIGHT)
        cameraMan->mousePressed(evt);

    return true;
}

bool RubikApp::mouseMoved(const OgreBites::MouseMotionEvent &evt) {
    OgreBites::InputListener::mouseMoved(evt);
    cameraMan->mouseMoved(evt);

    sky->resetToInitialState();
    sky->setDirection(-cameraMan->getCamera()->getPosition().normalisedCopy(), Ogre::Node::TS_WORLD);

    return true;
}

bool RubikApp::mouseWheelRolled(const OgreBites::MouseWheelEvent &evt) {
    auto cameraPos = cam->getPosition();
    auto newPos = cameraPos * (evt.y == 1 ? 0.95 : 1.05);

    // Set max distance from cube to 50
    if (newPos.length() > 50 || newPos.length() < 15) return true;
    cam->setPosition(newPos);

    return true;
}

void RubikApp::setup(void) {
    // do not forget to call the base first
    OgreBites::ApplicationContext::setup();

    // register for input events
    addInputListener(this);

    // get a pointer to the already created root
    Ogre::Root* root = getRoot();
    Ogre::SceneManager* scnMgr = root->createSceneManager();

    // register our scene with the RTSS
    Ogre::RTShader::ShaderGenerator* shadergen =
        Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);

    // without light we would just get a black screen
    Ogre::Light* light = scnMgr->createLight("MainLight");
    lightNode =
        scnMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 20, 35);
    lightNode->attachObject(light);

    // also need to tell where we are
    cam = scnMgr->getRootSceneNode()->createChildSceneNode();

    // create the camera
    Ogre::Camera* camera = scnMgr->createCamera("myCam");
    camera->setNearClipDistance(1);  // specific to this sample
    camera->setFarClipDistance(0);
    camera->setAutoAspectRatio(true);
    cam->attachObject(camera);

    cameraMan = new OgreBites::CameraMan(cam);
    cameraMan->setStyle(OgreBites::CS_ORBIT);
    
    cam->setPosition(0, 0, 20);
    cam->lookAt(Vector3(0, 0, 0), Node::TS_WORLD);

    auto p = Plane(camera->getRealPosition().normalisedCopy(), Vector3(0,0,0));
    auto pMesh = MeshManager::getSingleton().createPlane("skyMesh", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, p, 80, 60);

    auto pE = scnMgr->createEntity("skyboxPlane", pMesh);
    pE->setMaterialName("customSkybox");

    sky = scnMgr->createSceneNode();
    sky->setDirection(-camera->getRealPosition(), Ogre::Node::TS_WORLD);
    sky->setFixedYawAxis(true);
    sky->attachObject(pE);
    scnMgr->getRootSceneNode()->addChild(sky);
    
    // and tell it to render into the main window
    getRenderWindow()->addViewport(camera);

    RBox::generateAllMaterials();

    cube.emplace(scnMgr);

    // Add overlay system so that TrayManager will work
    scnMgr->addRenderQueueListener(getOverlaySystem());

    mTrayMgr = new OgreBites::TrayManager("Rubik controls", this->getRenderWindow(), this);
    mTrayMgr->hideCursor();

    addInputListener(mTrayMgr);

    mTrayMgr->createLabel(OgreBites::TL_TOP, "CURRENT_FACE", "Front", 80);

    mTrayMgr->createButton(OgreBites::TL_BOTTOMLEFT, "ROTL", "L ");
    mTrayMgr->createButton(OgreBites::TL_BOTTOMLEFT, "ROTLP", "L'");
    mTrayMgr->createButton(OgreBites::TL_BOTTOMLEFT, "ROTR", "R ");
    mTrayMgr->createButton(OgreBites::TL_BOTTOMLEFT, "ROTRP", "R'");
    mTrayMgr->createButton(OgreBites::TL_BOTTOMLEFT, "ROTF", "F ");
    mTrayMgr->createButton(OgreBites::TL_BOTTOMLEFT, "ROTFP", "F'");
    mTrayMgr->createButton(OgreBites::TL_BOTTOMLEFT, "ROTB", "B ");
    mTrayMgr->createButton(OgreBites::TL_BOTTOMLEFT, "ROTBP", "B'");
    mTrayMgr->createButton(OgreBites::TL_BOTTOMLEFT, "ROTU", "U ");
    mTrayMgr->createButton(OgreBites::TL_BOTTOMLEFT, "ROTUP", "U'");
    mTrayMgr->createButton(OgreBites::TL_BOTTOMLEFT, "ROTD", "D ");
    mTrayMgr->createButton(OgreBites::TL_BOTTOMLEFT, "ROTDP", "D'");

    mTrayMgr->createButton(OgreBites::TL_BOTTOMRIGHT, "UNDO", "Cofnij");
}


bool RubikApp::frameStarted(const Ogre::FrameEvent& evt) {
    // Run all event
    auto camPosition = cam->getPosition();    
    lightNode->setPosition(camPosition * 3); 

    updateRotation(evt.timeSinceLastFrame);

    auto ray = Ray(camPosition, (-camPosition));

    bool b = false;

    RBOX_FACE f = FRONT;
    float v = INFINITY;

    auto checkTrace = [&f, &v, &ray](RBOX_FACE face, std::unique_ptr<RBox>& box) {
        auto result = ray.intersects(box->getBoundingboxFromFace(face));
        if (result.first && result.second < v) {
            f = face;
            v = result.second;
        }
    };

    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            for (int z = 0; z < 3; z++) {
                auto& box = cube->cubeAt(x, y, z);

                if (x == 0) checkTrace(LEFT, box);
                if (x == 2) checkTrace(RIGHT, box);
                if (y == 0) checkTrace(BOTTOM, box);    
                if (y == 2) checkTrace(TOP, box);    
                if (z == 0) checkTrace(BACK, box);    
                if (z == 2) checkTrace(FRONT, box);    
            }
        }
    }

    if (v != INFINITY) {
        (dynamic_cast<OgreBites::Label*>(mTrayMgr->getWidget("CURRENT_FACE")))->setCaption(to_string(f));
    }

    pollEvents();

    return true;
}

int main(int argc, char* argv[]) {
    RubikApp app;
    app.initApp();
    app.getRoot()->startRendering();
    app.closeApp();
    return 0;
}
