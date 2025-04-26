#version 330 core

layout(lines) in;
layout(triangle_strip, max_vertices = 10) out;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform float lineWidth;  // Толщина линии
uniform float pointSize;  // Размер вершины
uniform int drawPoints;   // Режим отрисовки вершин: 0 — не рисовать, 1 — квадрат, 2 — круг

uniform vec4 lineColor;   // Цвет линий
uniform vec4 pointColor;  // Цвет вершин

out vec2 coord;           // Координаты для фрагментного шейдера
flat out int pointMode;   // Способ отрисовки вершин
out float dashCoord;      // Координата для пунктивных линий
flat out int isPoint;     // Флаг: 0 — линия, 1 — вершина
out vec4 vertexColor;     // Цвет вершины/линии

void main(void) {
    vec3 p1 = gl_in[0].gl_Position.xyz;
    vec3 p2 = gl_in[1].gl_Position.xyz;

    vec3 direction = normalize(p2 - p1);
    vec3 offset = normalize(cross(direction, vec3(0.0, 0.0, 1.0))) * lineWidth * 0.5;

    if (lineWidth > 0.0) { // Формирование прямоугльников вместо линий между вершинами
        isPoint = 0;
        vertexColor = lineColor;

        gl_Position = projectionMatrix * viewMatrix * vec4(p1 + offset, 1.0);
        coord = vec2(0.0, 0.0);
        dashCoord = 0.0;
        pointMode = 0;
        EmitVertex();

        gl_Position = projectionMatrix * viewMatrix * vec4(p1 - offset, 1.0);
        coord = vec2(0.0, 1.0);
        dashCoord = 0.0;
        pointMode = 0;
        EmitVertex();

        gl_Position = projectionMatrix * viewMatrix * vec4(p2 + offset, 1.0);
        coord = vec2(1.0, 0.0);
        dashCoord = 1.0;
        pointMode = 0;
        EmitVertex();

        gl_Position = projectionMatrix * viewMatrix * vec4(p2 - offset, 1.0);
        coord = vec2(1.0, 1.0);
        dashCoord = 1.0;
        pointMode = 0;
        EmitVertex();

        EndPrimitive();
    }

    if (drawPoints != 0 && pointSize > 0.0) { // Формирование вершин
        isPoint = 1;
        vertexColor = pointColor;

        vec4 center1 = projectionMatrix * viewMatrix * vec4(p1, 1.0);
        vec4 center2 = projectionMatrix * viewMatrix * vec4(p2, 1.0);

        vec2 size = vec2(pointSize);

        // Вершина 1
        gl_Position = center1 + vec4(-size.x, -size.y, 0.0, 0.0);
        coord = vec2(0.0, 0.0);
        pointMode = drawPoints;
        dashCoord = 0.0;
        EmitVertex();

        gl_Position = center1 + vec4(size.x, -size.y, 0.0, 0.0);
        coord = vec2(1.0, 0.0);
        pointMode = drawPoints;
        dashCoord = 0.0;
        EmitVertex();

        gl_Position = center1 + vec4(-size.x, size.y, 0.0, 0.0);
        coord = vec2(0.0, 1.0);
        pointMode = drawPoints;
        dashCoord = 0.0;
        EmitVertex();

        gl_Position = center1 + vec4(size.x, size.y, 0.0, 0.0);
        coord = vec2(1.0, 1.0);
        pointMode = drawPoints;
        dashCoord = 0.0;
        EmitVertex();

        EndPrimitive();

        // Вершина 2
        gl_Position = center2 + vec4(-size.x, -size.y, 0.0, 0.0);
        coord = vec2(0.0, 0.0);
        pointMode = drawPoints;
        dashCoord = 1.0;
        EmitVertex();

        gl_Position = center2 + vec4(size.x, -size.y, 0.0, 0.0);
        coord = vec2(1.0, 0.0);
        pointMode = drawPoints;
        dashCoord = 1.0;
        EmitVertex();

        gl_Position = center2 + vec4(-size.x, size.y, 0.0, 0.0);
        coord = vec2(0.0, 1.0);
        pointMode = drawPoints;
        dashCoord = 1.0;
        EmitVertex();

        gl_Position = center2 + vec4(size.x, size.y, 0.0, 0.0);
        coord = vec2(1.0, 1.0);
        pointMode = drawPoints;
        dashCoord = 1.0;
        EmitVertex();

        EndPrimitive();
    }
}
