///////////////////////////////////////////////
// A PHONG VERTEX SHADER


// variables that are interpolated to the fragment shader
varying vec4 vertColor;

void main(){
	vertColor = gl_Vertex;
	// 	TODO : APPLY THE CORECT TRANSFORMATIONS
	gl_Position = gl_Vertex;
}

