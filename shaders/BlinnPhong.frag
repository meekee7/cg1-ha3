///////////////////////////////////////////////
// A PHONG FRAGMENT SHADER

// variables that are interpolated to the fragment shader
varying vec4 vertColor;

void main(){
	//TODO : IMPLEMENT THE PHONG SHADING MODEL

	// Set the color of the fragment
	gl_FragColor = vertColor;
}
	
