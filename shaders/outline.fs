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
  vec4 texelColor = texture(texture0, fragTexCoord);
  if(texelColor.r < 0.7
  && texelColor.g < 0.7 
  && texelColor.b < 0.7) {
    /// @todo: is colDiffuse necessary?
    finalColor = vec4(.2, .2, .2, texelColor.a) * colDiffuse;
  } else {
    finalColor = vec4(0.3, 0.3, 0.3, texelColor.a) * colDiffuse;
  }
}