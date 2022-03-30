#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

using namespace std;
using namespace m1;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 100;

    cx = corner.x + squareSide / 2;
    cy = corner.y + squareSide / 2;

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    // Initialize angularStep
    angularStep = 0;

    //BONUS

    bonus_translateX = 0;
    bonus_translateY = 0;

    bonus_angularStep = 0;

    Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(square1);

    Mesh* square2 = object2D::CreateSquare("square2", corner, squareSide, glm::vec3(0, 1, 0));
    AddMeshToList(square2);

    Mesh* square3 = object2D::CreateSquare("square3", corner, squareSide, glm::vec3(0, 0, 1));
    AddMeshToList(square3);

    Mesh* square4 = object2D::CreateSquare("square4", corner, squareSide, glm::vec3(1, 1, 0), true);
    AddMeshToList(square4);
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    translateX += deltaTimeSeconds * 30.0f;
    translateY += deltaTimeSeconds * 30.0f * reverse_translate;
    scaleX += deltaTimeSeconds * 0.5f * reverse_scale;
    scaleY += deltaTimeSeconds * 0.5f * reverse_scale;
    angularStep += deltaTimeSeconds * 0.5f;

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(150, 250);
    modelMatrix *= transform2D::Translate(0, translateY);

    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(400, 250);
    modelMatrix *= transform2D::Translate(50, 50);
    modelMatrix *= transform2D::Rotate(angularStep);
    modelMatrix *= transform2D::Translate(-50, -50);

    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(650, 250);
    modelMatrix *= transform2D::Translate(50, 50);
    modelMatrix *= transform2D::Scale(scaleX, scaleY);
    modelMatrix *= transform2D::Translate(-50, -50);

    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);

    if (translateY > 250 || translateY < 0)
    {
        reverse_translate = -reverse_translate;
    }

    if (scaleX < 1.0 || scaleX > 2.5)
    {
        reverse_scale = -reverse_scale;
    }

    //BONUS

    bonus_translateX += deltaTimeSeconds * 50.0f * bonus_reverse_translateX;
    bonus_translateY += deltaTimeSeconds * 50.0f * bonus_reverse_translateY;
    bonus_angularStep += deltaTimeSeconds * 1.0f * bonus_reverse_angularStep;

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(0, 0);
    modelMatrix *= transform2D::Translate(bonus_translateX, bonus_translateY);
    modelMatrix *= transform2D::Translate(50, 50);
    modelMatrix *= transform2D::Rotate(-bonus_angularStep);
    modelMatrix *= transform2D::Translate(-50, -50);

    RenderMesh2D(meshes["square4"], shaders["VertexColor"], modelMatrix);

    if (bonus_translateX >= 1200 || bonus_translateX <= 0)
    {
        bonus_reverse_translateX = -bonus_reverse_translateX;
        bonus_reverse_angularStep = -bonus_reverse_angularStep;
    }

    if (bonus_translateY >= 100 || bonus_translateY <= 0)
    {
        bonus_reverse_translateY = -bonus_reverse_translateY;
    }
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
