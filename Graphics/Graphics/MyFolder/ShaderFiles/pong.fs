#version 330 core

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D tex0;
uniform sampler2D tex1;

uniform int drawIndex;

void main()
{
    if(drawIndex == 0) // left bat
    {
        FragColor = texture(tex0, texCoord);
    }
    else if(drawIndex == 1) // right bat
    {
        FragColor = texture(tex1, texCoord);
    }
    else if(drawIndex == 2) // ball
    {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
}