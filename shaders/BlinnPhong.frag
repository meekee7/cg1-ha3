///////////////////////////////////////////////
// A PHONG FRAGMENT SHADER

varying vec3 normal; // surface nomal in eye coords
varying vec3 vertex; // vertex in eye coords

void main() {
        vec3 light = normalize(vec3(gl_LightSource[0].position) - vertex);
        vec3 eye = normalize(-vertex); //use -vertex because vertex is in eye space (0,0,0); direction from surface to eye is needed
        vec3 halfv = normalize(eye + light); //half vector //always add ambient light
        vec4 colour = gl_FrontMaterial.ambient * gl_LightSource[0].ambient + gl_LightModel.ambient * gl_FrontMaterial.ambient;
        float ndotl = dot(normal,light);
        if (ndotl > 0.0) { //check light direction, add diffuse/specular light only if reflection is possible
                colour += gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse * ndotl; //add diffuse light
                float ndoth = dot(normal,halfv); 
                if (ndoth >0.0) //Add specular light when necessary
                        colour += gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(ndoth, gl_FrontMaterial.shininess);
        }
        gl_FragColor = colour; //set current pixel colour
}

