#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

Mesh* object2D::CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);

    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateCircle(const std::string &name, glm::vec3 circleOrigin, float length, glm::vec3 color, bool fill)
{
    glm::vec3 origin = circleOrigin;

    std::vector<VertexFormat> vertices = {};
    std::vector<unsigned int> indices = {};

    for (int i = 0; i < 360; i++)
    {
        vertices.push_back(VertexFormat(origin + glm::vec3(cos(RADIANS(i)) * length, sin(RADIANS(i)) * length, 0), color));
        indices.push_back(i);
    }

    Mesh* circle = new Mesh(name);

    if (!fill) {
        circle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        circle->SetDrawMode(GL_TRIANGLE_FAN);
    }

    circle->InitFromData(vertices, indices);
    return circle;
}
