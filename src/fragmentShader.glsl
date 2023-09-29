#version 330 core

// Valores interpolados del shader de vértices
in vec3 fragmentColor;

// Output data
out vec3 color;


void main()
{
    // Output color = red
    color = fragmentColor;

}

