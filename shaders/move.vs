#version 330

// Input vertex attributes
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
in vec4 vertexColor;

// Input uniform values
uniform mat4 mvp;

// Output vertex attributes (to fragment shader)
out vec2 fragTexCoord;
out vec4 fragColor;

// NOTE: Add here your custom variables
uniform float time;
// vec3 finalPosition;

float wrap(float value, float min, float max) {
  return mod((value - min), max - min) + min;
}

void main() {
  // Send vertex attributes to fragment shader
  fragTexCoord = vertexTexCoord + vec2(-time, -time);
  fragColor = vertexColor;
  //fragTexCoord = mod(fragTexCoord, 1.0);

  gl_Position = mvp*vec4(vertexPosition, 1.0);
}