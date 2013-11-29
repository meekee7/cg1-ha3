////////////////////////////////////////////////////////////////////////////////////////////////////
// A PASSTHROUGH SHADER

// uniform ("global") variables
uniform vec4 color;

varying vec4 vertColor;


void main(){
	// Draw the fragment with given color
	gl_FragColor = color;
}
	
