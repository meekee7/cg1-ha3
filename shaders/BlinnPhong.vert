///////////////////////////////////////////////
// A PHONG VERTEX SHADER
//#version 150
varying vec3 normal;
varying vec3 vertex; //vertex

void main() {
		//normal = normalize(transpose(inverse(gl_ModelViewMatrix)) * gl_Normal);
        normal = normalize(gl_NormalMatrix * gl_Normal); //get normal, transform into eye space
        vertex = vec3(gl_ModelViewMatrix * gl_Vertex); // get vertex position in eye space
        gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; // setup correct position
}

