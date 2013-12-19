///////////////////////////////////////////////
// A PHONG VERTEX SHADER
varying vec3 normal;
varying vec3 vertex;


void main() {
		normal = normalize(gl_NormalMatrix * gl_Normal); //transform normal into eye space
        vertex = vec3(gl_ModelViewMatrix * gl_Vertex); //set vertex position in eye space
        gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; //set correct position
}

