#version 330 core

in vec2 coord;
in vec4 vertexColor;
in float dashCoord;
uniform int lineStyle;  // 0 - сплошная, 1 - пунктир
flat in int pointMode; // 0 - нет, 1 - квадрат, 2 - круг
out vec4 finalColor;

void main() {
    if (pointMode == 0) {
        if (lineStyle == 1) {
            float dashLength = 0.2; // длина штриха
            if (mod(dashCoord, dashLength * 2.0) > dashLength) {
                discard;
            }
        }
    } else if (pointMode == 2) {
        float dist = length(coord - vec2(0.5));
        if (dist > 0.5) discard;
    }

    finalColor = vertexColor;
}

