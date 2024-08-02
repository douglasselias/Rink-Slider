#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

void main() {
  vec4 texel = texture(texture0, fragTexCoord);   // Get texel color
  if(texel.r < 0.7
  && texel.g < 0.7 
  && texel.b < 0.7) {
    finalColor = vec4(.2, .2, .2, texel.a);
  } else {
    finalColor = vec4(0.3, 0.3, 0.3, texel.a);
  }
}