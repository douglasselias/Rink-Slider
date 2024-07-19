shader_type canvas_item;

uniform float cutoff: hint_range(0.0, 1.0, 0.01);
uniform float smooth_size: hint_range(0.0, 1.0, 0.01);
uniform sampler2D mask: hint_albedo;

void fragment() {
	float value = texture(mask, UV).r; // Get 'grayscale' channel
	float alpha = smoothstep(cutoff, cutoff + smooth_size, value * (1.0 - smooth_size)+ smooth_size);
	COLOR = vec4(COLOR.rgb, alpha);
}