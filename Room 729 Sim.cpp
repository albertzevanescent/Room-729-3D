#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>

#include "Constants.h"
#include "Room729.h"
#include "RoomNumber.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
int setupWindow(GLFWwindow* window);
void renderWindow(GLFWwindow* window);
void update();
void rotateCameraStart(int type, bool cw);
void rotateCameraStop(int type, bool cw);
void rotateRoom(int type, int side);
void inputNum(int num);
void printDialogue(int num);
void enterFocus();

float angles[2] = {};
float targetAngles[2] = {};
int rotationTimer = -1;
bool inMotion = false;
int rotationType = -1;
bool rotationCW = true;
int rotationCube = 0;
Room729 testRoom729 = Room729(0, 0, 0);

bool skipAnim = false;
bool toggleDialogue = true;
bool inputRoom = false;
int enterFocusRoom = 0;
//find 3 then 24, goes to center
int main(void)
{
    if (!glfwInit())
    {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Room 729 Sim", NULL, NULL);
    if (setupWindow(window)) { return -1; }

    double limitFPS = 1.0 / 60.0;
    double lastTime = glfwGetTime(), timer = lastTime;
    double deltaTime = 0, nowTime = 0;
    int frames = 0, updates = 0;

    printDialogue(0);
    while (!glfwWindowShouldClose(window))
    {
        nowTime = glfwGetTime();
        deltaTime += (nowTime - lastTime) / limitFPS;
        lastTime = nowTime;

        while (deltaTime >= 1.0) {
            update();
            renderWindow(window);
            deltaTime = 0;
        }
    }
    glfwTerminate();

    return 0;
}

int setupWindow(GLFWwindow* window)
{
    glfwSetKeyCallback(window, keyCallback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glViewport((GLint)0, (GLint)0, (GLsizei)screenWidth, (GLsizei)screenHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, -500);

    return  0;
}

void renderWindow(GLFWwindow* window)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glRotatef(angles[0], 1, 0, 0);
    glRotatef(angles[1], 0, 1, 0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    testRoom729.draw();

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void update()
{
    testRoom729.update();

    if (!inMotion) { return; }

    if (!rotationCW && angles[rotationType] < targetAngles[rotationType]) {
        angles[rotationType] += (Constants::ROTATION_AMOUNT / 1) / Constants::ROTATION_TIME;
    }
    else if (rotationCW && angles[rotationType] > targetAngles[rotationType]) {
        angles[rotationType] -= (Constants::ROTATION_AMOUNT / 1) / Constants::ROTATION_TIME;
    }
    if (rotationTimer > 0) {rotationTimer--;}
    if (rotationTimer == 0)
    {
        rotateCameraStop(rotationType, rotationCW);
    }
}

void rotateCameraStart(int type, bool cw)
{
    inMotion = true;
    rotationTimer = Constants::ROTATION_TIME;
    rotationType = type;
    rotationCW = cw;
    targetAngles[type] += (cw ? -1 : 1) * (Constants::ROTATION_AMOUNT / 2);
}

void rotateCameraStop(int type, bool cw)
{
    inMotion = false;
    angles[type] = targetAngles[type];
    rotationType = -1;
    rotationCW = true;
}

void printDialogue(int num)
{
    if (!toggleDialogue) {return;}
    switch (num)
    {
    case 0:
        std::cout << "Instructions:" << std::endl;
        std::cout << "Arrow keys: Move camera" << std::endl;
        std::cout << "0: Select Room 729" << std::endl;
        std::cout << "1-9: Select Room 27" << std::endl;
        std::cout << "Q/A/Z: Rotate vertical" << std::endl;
        std::cout << "W/S/X: Rotate horizontal" << std::endl;
        std::cout << "E/D/C: Rotate face" << std::endl;
        std::cout << "F: Input room #" << std::endl;
        std::cout << "-------------------------" << std::endl;
        break;
    default:
        break;
    }
}

void rotateRoom(int type, int side)
{
    testRoom729.rotateStart(rotationCube, type, side, true, skipAnim);
}

void inputNum(int num)
{
    if (inputRoom)
    {
        enterFocusRoom = enterFocusRoom * 10 + num;
    }
    else
    {
        rotationCube = num;
    }
}

void enterFocus()
{
    if (!inputRoom) {return;}
    testRoom729.enterFocusRoom(enterFocusRoom);
    enterFocusRoom = 0;
    inputRoom = false;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_UP:
            rotateCameraStart(0, false);
            break;
        case GLFW_KEY_DOWN:
            rotateCameraStart(0, true);
            break;
        case GLFW_KEY_RIGHT:
            rotateCameraStart(1, true);
            break;
        case GLFW_KEY_LEFT:
            rotateCameraStart(1, false);
            break;
        case GLFW_KEY_Q:
            rotateRoom(0, 0);
            break;
        case GLFW_KEY_A:
            rotateRoom(0, 1);
            break;
        case GLFW_KEY_Z:
            rotateRoom(0, 2);
            break;
        case GLFW_KEY_W:
            rotateRoom(1, 0);
            break;
        case GLFW_KEY_S:
            rotateRoom(1, 1);
            break;
        case GLFW_KEY_X:
            rotateRoom(1, 2);
            break;
        case GLFW_KEY_E:
            rotateRoom(2, 0);
            break;
        case GLFW_KEY_D:
            rotateRoom(2, 1);
            break;
        case GLFW_KEY_C:
            rotateRoom(2, 2);
            break;
        case GLFW_KEY_F:
            inputRoom = true;
            break;
        case GLFW_KEY_0:
            inputNum(0);
            break;
        case GLFW_KEY_1:
            inputNum(1);
            break;
        case GLFW_KEY_2:
            inputNum(2);
            break;
        case GLFW_KEY_3:
            inputNum(3);
            break;
        case GLFW_KEY_4:
            inputNum(4);
            break;
        case GLFW_KEY_5:
            inputNum(5);
            break;
        case GLFW_KEY_6:
            inputNum(6);
            break;
        case GLFW_KEY_7:
            inputNum(7);
            break;
        case GLFW_KEY_8:
            inputNum(8);
            break;
        case GLFW_KEY_9:
            inputNum(9);
            break;
        case GLFW_KEY_ENTER:
            enterFocus();
            break;
        case GLFW_KEY_ESCAPE:
            std::exit(0);
            break;
        }
    }
}