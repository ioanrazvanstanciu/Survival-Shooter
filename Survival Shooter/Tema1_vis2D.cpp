#include "lab_m1/Tema1/Tema1_vis2D.h"

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


Tema1_Vis2D::Tema1_Vis2D()
{
}


Tema1_Vis2D::~Tema1_Vis2D()
{
}


void Tema1_Vis2D::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = 4;   // logic width
    logicSpace.height = 4;  // logic height

    translateX = 2;
    translateY = 2;

    gata_de_desenat = 0;

    inaintare_proiectil = 0;

    apasat = 0;

    translateX_proiectil = 0;
    translateY_proiectil = 0;

    timp_de_viata_proiectil = 0;

    determinare_valori_de_plecare = 0;

    stop_stanga = 0;

    stop_dreapta = 0;
    
    stop_jos = 0;
    
    stop_sus = 0;

    eliminare_primul_inamic = 0;

    evita_if_primul_inamic = 0;

    eliminare_al_doilea_inamic = 0;

    evita_if_al_doilea_inamic = 0;

    valoare_scor = 0;

    Mesh* obstacol = object2D::CreateSquare("obstacol", glm::vec3(0, 0, 0), 1.0f, glm::vec3(0.0f, 0.6f, 0.3f), true);
    AddMeshToList(obstacol);

    Mesh* cerc_mare = object2D::CreateCircle("cerc_mare", glm::vec3(0, 0, 0), 0.5f, glm::vec3(1.0f, 0.9f, 0.3f), true);
    AddMeshToList(cerc_mare);

    Mesh* cerc_mic = object2D::CreateCircle("cerc_mic", glm::vec3(0, 0, 0), 0.5f, glm::vec3(0.9f, 0.6f, 0.0f), true);
    AddMeshToList(cerc_mic);

    Mesh* inamic = object2D::CreateSquare("inamic", glm::vec3(0, 0, 0), 1.0f, glm::vec3(0.9f, 0.0f, 0.0f), true);
    AddMeshToList(inamic);

    Mesh* proiectil = object2D::CreateSquare("proiectil", glm::vec3(0, 0, 0), 1.0f, glm::vec3(0.0f, 0.0f, 0.2f), true);
    AddMeshToList(proiectil);

    Mesh* rama_scor = object2D::CreateSquare("rama_scor", glm::vec3(0, 0, 0), 1.0f, glm::vec3(0.0f, 0.0f, 0.2f), false);
    AddMeshToList(rama_scor);

    Mesh* punct_scor = object2D::CreateSquare("punct_scor", glm::vec3(0, 0, 0), 1.0f, glm::vec3(0.0f, 0.0f, 0.2f), true);
    AddMeshToList(punct_scor);
}


// 2D visualization matrix
glm::mat3 Tema1_Vis2D::VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    tx = viewSpace.x - sx * logicSpace.x;
    ty = viewSpace.y - sy * logicSpace.y;

    return glm::transpose(glm::mat3(
        sx, 0.0f, tx,
        0.0f, sy, ty,
        0.0f, 0.0f, 1.0f));
}


// Uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Tema1_Vis2D::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}


void Tema1_Vis2D::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}


void Tema1_Vis2D::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Tema1_Vis2D::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    gata_de_desenat += deltaTimeSeconds;

    if (apasat == 1) {
        inaintare_proiectil += deltaTimeSeconds * 1.5f;
    }

    if ((valoare_scor == 0.50f) || (valoare_scor == 1.00f) || (valoare_scor == 1.50f)) {
        eliminare_primul_inamic = 0;
        eliminare_al_doilea_inamic = 0;
        evita_if_primul_inamic = 0;
        evita_if_al_doilea_inamic = 0;
    }

    // The viewport is all the window

    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0.6f, 0.6f, 0.6f), true);

    // Compute uniform 2D visualization matrix
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
    
    DrawScene(visMatrix);
}


void Tema1_Vis2D::FrameEnd()
{
}


void Tema1_Vis2D::DrawScene(glm::mat3 visMatrix)
{
    // Scor
    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(-2.05, 6.40);
    modelMatrix *= transform2D::Scale(valoare_scor, 0.40f);
    RenderMesh2D(meshes["punct_scor"], shaders["VertexColor"], modelMatrix);
    
    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(-2.1, 6.35);
    modelMatrix *= transform2D::Scale(2.00f, 0.50f);
    RenderMesh2D(meshes["rama_scor"], shaders["VertexColor"], modelMatrix);
        
    // Proiectil

    if (apasat == 1) {
        if (determinare_valori_de_plecare == 0) {
            translateX_proiectil = translateX;
            translateY_proiectil = translateY;
            determinare_valori_de_plecare = 1;
        }
        if ((translateY_proiectil + 0.4f + inaintare_proiectil > 6.7f) &&
        (translateX_proiectil - 0.15f > -2.5f) && (translateX_proiectil - 0.15f < 7.5f))
        {
            apasat = 0;
            determinare_valori_de_plecare = 0;
            inaintare_proiectil = 0;
            timp_de_viata_proiectil = 0;
        } else {
            if (
                (translateY_proiectil + 0.4f + inaintare_proiectil > 0.75f) &&
                (translateX_proiectil - 0.15f > 2.50f) && (translateX_proiectil - 0.15f < 3.75f) &&
                (evita_if_primul_inamic == 0)
               )
            {
                apasat = 0;
                determinare_valori_de_plecare = 0;
                inaintare_proiectil = 0;
                timp_de_viata_proiectil = 0;
                eliminare_primul_inamic = 1;
                evita_if_primul_inamic = 1;
                valoare_scor += 0.25f;
            }
            else {
                if (
                    (translateY_proiectil + 0.4f + inaintare_proiectil > 5.25f) &&
                    (translateX_proiectil - 0.15f > 5.00f) && (translateX_proiectil - 0.15f < 7.25f) &&
                    (evita_if_al_doilea_inamic == 0)
                    )
                {
                    apasat = 0;
                    determinare_valori_de_plecare = 0;
                    inaintare_proiectil = 0;
                    timp_de_viata_proiectil = 0;
                    eliminare_al_doilea_inamic = 1;
                    evita_if_al_doilea_inamic = 1;
                    valoare_scor += 0.25f;
                }
                else {
                    modelMatrix = visMatrix;
                    modelMatrix *= transform2D::Translate(translateX_proiectil - 0.15f, translateY_proiectil + 0.4f + inaintare_proiectil);
                    modelMatrix *= transform2D::Scale(0.3f, 0.3f);
                    RenderMesh2D(meshes["proiectil"], shaders["VertexColor"], modelMatrix);
                }
            }
        }        
    }

    // Jucatorul

    if ((translateX - 0.3f > -2.05f) && (translateX + 0.3f < 7.30f)) {
        stop_stanga = 0;
        stop_dreapta = 0;

        modelMatrix = visMatrix;
        modelMatrix *= transform2D::Translate(translateX + 0.3f, translateY + 0.3f);
        modelMatrix *= transform2D::Scale(0.4f, 0.4f);
        RenderMesh2D(meshes["cerc_mic"], shaders["VertexColor"], modelMatrix);

        modelMatrix = visMatrix;
        modelMatrix *= transform2D::Translate(translateX - 0.3f, translateY + 0.3f);
        modelMatrix *= transform2D::Scale(0.4f, 0.4f);
        RenderMesh2D(meshes["cerc_mic"], shaders["VertexColor"], modelMatrix);

        modelMatrix = visMatrix;
        modelMatrix *= transform2D::Translate(translateX, translateY);
        modelMatrix *= transform2D::Scale(0.75f, 0.75f);
        RenderMesh2D(meshes["cerc_mare"], shaders["VertexColor"], modelMatrix);
    } else {
        if (translateX - 0.3f < -2.05f) {
            modelMatrix = visMatrix;
            modelMatrix *= transform2D::Translate(-1.75f + 0.3f, translateY + 0.3f);
            modelMatrix *= transform2D::Scale(0.4f, 0.4f);
            RenderMesh2D(meshes["cerc_mic"], shaders["VertexColor"], modelMatrix);

            modelMatrix = visMatrix;
            modelMatrix *= transform2D::Translate(-1.75f - 0.3f, translateY + 0.3f);
            modelMatrix *= transform2D::Scale(0.4f, 0.4f);
            RenderMesh2D(meshes["cerc_mic"], shaders["VertexColor"], modelMatrix);

            modelMatrix = visMatrix;
            modelMatrix *= transform2D::Translate(-1.75f, translateY);
            modelMatrix *= transform2D::Scale(0.75f, 0.75f);
            RenderMesh2D(meshes["cerc_mare"], shaders["VertexColor"], modelMatrix);

            stop_stanga = 1;
        } else {
            modelMatrix = visMatrix;
            modelMatrix *= transform2D::Translate(7.00f + 0.3f, translateY + 0.3f);
            modelMatrix *= transform2D::Scale(0.4f, 0.4f);
            RenderMesh2D(meshes["cerc_mic"], shaders["VertexColor"], modelMatrix);

            modelMatrix = visMatrix;
            modelMatrix *= transform2D::Translate(7.00f - 0.3f, translateY + 0.3f);
            modelMatrix *= transform2D::Scale(0.4f, 0.4f);
            RenderMesh2D(meshes["cerc_mic"], shaders["VertexColor"], modelMatrix);

            modelMatrix = visMatrix;
            modelMatrix *= transform2D::Translate(7.00f, translateY);
            modelMatrix *= transform2D::Scale(0.75f, 0.75f);
            RenderMesh2D(meshes["cerc_mare"], shaders["VertexColor"], modelMatrix);

            stop_dreapta = 1;
        }
    }

    if (translateY > -1.40f && (translateY + 0.3f < 6.80f)) {
        stop_jos = 0;
        stop_sus = 0;

        modelMatrix = visMatrix;
        modelMatrix *= transform2D::Translate(translateX + 0.3f, translateY + 0.3f);
        modelMatrix *= transform2D::Scale(0.4f, 0.4f);
        RenderMesh2D(meshes["cerc_mic"], shaders["VertexColor"], modelMatrix);

        modelMatrix = visMatrix;
        modelMatrix *= transform2D::Translate(translateX - 0.3f, translateY + 0.3f);
        modelMatrix *= transform2D::Scale(0.4f, 0.4f);
        RenderMesh2D(meshes["cerc_mic"], shaders["VertexColor"], modelMatrix);

        modelMatrix = visMatrix;
        modelMatrix *= transform2D::Translate(translateX, translateY);
        modelMatrix *= transform2D::Scale(0.75f, 0.75f);
        RenderMesh2D(meshes["cerc_mare"], shaders["VertexColor"], modelMatrix);
    } else {
        if (translateY < -1.40f) {
            modelMatrix = visMatrix;
            modelMatrix *= transform2D::Translate(translateX + 0.3f, -1.40f + 0.3f);
            modelMatrix *= transform2D::Scale(0.4f, 0.4f);
            RenderMesh2D(meshes["cerc_mic"], shaders["VertexColor"], modelMatrix);

            modelMatrix = visMatrix;
            modelMatrix *= transform2D::Translate(translateX - 0.3f, -1.40f + 0.3f);
            modelMatrix *= transform2D::Scale(0.4f, 0.4f);
            RenderMesh2D(meshes["cerc_mic"], shaders["VertexColor"], modelMatrix);

            modelMatrix = visMatrix;
            modelMatrix *= transform2D::Translate(translateX, -1.40f);
            modelMatrix *= transform2D::Scale(0.75f, 0.75f);
            RenderMesh2D(meshes["cerc_mare"], shaders["VertexColor"], modelMatrix);

            stop_jos = 1;
        } else {
            modelMatrix = visMatrix;
            modelMatrix *= transform2D::Translate(translateX + 0.3f, 6.50f + 0.3f);
            modelMatrix *= transform2D::Scale(0.4f, 0.4f);
            RenderMesh2D(meshes["cerc_mic"], shaders["VertexColor"], modelMatrix);

            modelMatrix = visMatrix;
            modelMatrix *= transform2D::Translate(translateX - 0.3f, 6.50f + 0.3f);
            modelMatrix *= transform2D::Scale(0.4f, 0.4f);
            RenderMesh2D(meshes["cerc_mic"], shaders["VertexColor"], modelMatrix);

            modelMatrix = visMatrix;
            modelMatrix *= transform2D::Translate(translateX, 6.50f);
            modelMatrix *= transform2D::Scale(0.75f, 0.75f);
            RenderMesh2D(meshes["cerc_mare"], shaders["VertexColor"], modelMatrix);

            stop_sus = 1;
        }
    }

    // Obstacolele
    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(-0.5, 3.6);
    modelMatrix *= transform2D::Scale(0.75f, 0.75f);
    RenderMesh2D(meshes["obstacol"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(1, 3.25);
    modelMatrix *= transform2D::Scale(1.5f, 1.5f);
    RenderMesh2D(meshes["obstacol"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(3.8, 2.75);
    modelMatrix *= transform2D::Scale(0.75f, 0.75f);
    RenderMesh2D(meshes["obstacol"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(0, 0);
    modelMatrix *= transform2D::Scale(0.5f, 1.8f);
    RenderMesh2D(meshes["obstacol"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(0, 0);
    modelMatrix *= transform2D::Scale(1.75f, 0.25f);
    RenderMesh2D(meshes["obstacol"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(4.25, 0.25);
    modelMatrix *= transform2D::Scale(0.75f, 0.75f);
    RenderMesh2D(meshes["obstacol"], shaders["VertexColor"], modelMatrix);

    // Margine harta

    // Sus
    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(7.75f, 7.0f);
    modelMatrix *= transform2D::Scale(-10.25f, 0.25f);
    RenderMesh2D(meshes["obstacol"], shaders["VertexColor"], modelMatrix);
    
    // Dreapta
    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(7.5f, 7.0f);
    modelMatrix *= transform2D::Scale(0.25f, -9.0f);
    RenderMesh2D(meshes["obstacol"], shaders["VertexColor"], modelMatrix);
    
    // Jos
    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(-2.5f, -2.0f);
    modelMatrix *= transform2D::Scale(10.0f, 0.25f);
    RenderMesh2D(meshes["obstacol"], shaders["VertexColor"], modelMatrix);

    // Stanga
    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(-2.5f, -2.0f);
    modelMatrix *= transform2D::Scale(0.25f, 9.0f);
    RenderMesh2D(meshes["obstacol"], shaders["VertexColor"], modelMatrix);

    if (gata_de_desenat > 5) {
        // Inamici
        if (eliminare_al_doilea_inamic == 0) {
            modelMatrix = visMatrix;
            modelMatrix *= transform2D::Translate(5.36, 5.85);
            modelMatrix *= transform2D::Scale(0.35f, 0.35f);
            RenderMesh2D(meshes["inamic"], shaders["VertexColor"], modelMatrix);

            modelMatrix = visMatrix;
            modelMatrix *= transform2D::Translate(5.86, 5.85);
            modelMatrix *= transform2D::Scale(0.35f, 0.35f);
            RenderMesh2D(meshes["inamic"], shaders["VertexColor"], modelMatrix);

            modelMatrix = visMatrix;
            modelMatrix *= transform2D::Translate(5.50, 5.50);
            modelMatrix *= transform2D::Scale(0.6f, 0.6f);
            RenderMesh2D(meshes["inamic"], shaders["VertexColor"], modelMatrix);
        }

        if (eliminare_primul_inamic == 0) {
            modelMatrix = visMatrix;
            modelMatrix *= transform2D::Translate(2.86, 1.35);
            modelMatrix *= transform2D::Scale(0.35f, 0.35f);
            RenderMesh2D(meshes["inamic"], shaders["VertexColor"], modelMatrix);

            modelMatrix = visMatrix;
            modelMatrix *= transform2D::Translate(3.36, 1.35);
            modelMatrix *= transform2D::Scale(0.35f, 0.35f);
            RenderMesh2D(meshes["inamic"], shaders["VertexColor"], modelMatrix);

            modelMatrix = visMatrix;
            modelMatrix *= transform2D::Translate(3, 1);
            modelMatrix *= transform2D::Scale(0.6f, 0.6f);
            RenderMesh2D(meshes["inamic"], shaders["VertexColor"], modelMatrix);
        }
    }
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1_Vis2D::OnInputUpdate(float deltaTime, int mods)
{
    if ((window->KeyHold(GLFW_KEY_W)) && (stop_sus == 0))
    {
        logicSpace.y += deltaTime * 2.0f;
        translateY += deltaTime * 2.0f;        
    }

    if ((window->KeyHold(GLFW_KEY_S)) && (stop_jos == 0))
    {
        logicSpace.y -= deltaTime * 2.0f;
        translateY -= deltaTime * 2.0f;        
    }

    if ((window->KeyHold(GLFW_KEY_A)) && (stop_stanga == 0))
    {
        logicSpace.x -= deltaTime * 2.0f;
        translateX -= deltaTime * 2.0f;        
    }

    if ((window->KeyHold(GLFW_KEY_D)) && (stop_dreapta == 0))
    {
        logicSpace.x += deltaTime * 2.0f;
        translateX += deltaTime * 2.0f;        
    }

    if (window->KeyHold(GLFW_KEY_Z))
    {
        logicSpace.x -= deltaTime;
        logicSpace.y -= deltaTime;
        logicSpace.width += deltaTime * 2.0f;
        logicSpace.height += deltaTime * 2.0f;        
    }

    if (window->KeyHold(GLFW_KEY_X))
    {
        logicSpace.x += deltaTime;
        logicSpace.y += deltaTime;
        logicSpace.width -= deltaTime * 2.0f;
        logicSpace.height -= deltaTime * 2.0f;        
    }
}


void Tema1_Vis2D::OnKeyPress(int key, int mods)
{
}


void Tema1_Vis2D::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1_Vis2D::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1_Vis2D::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (button == 1) {
        apasat = 1;
    }
}


void Tema1_Vis2D::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1_Vis2D::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}
