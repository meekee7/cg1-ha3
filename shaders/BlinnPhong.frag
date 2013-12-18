///////////////////////////////////////////////
// A PHONG FRAGMENT SHADER
//#version 150

varying vec3 normal; // surface nomal in eye coords
varying vec3 vertex; // vertex in eye coords

void main() {
        vec3 light = normalize(vec3(gl_LightSource[0].position) - vertex);
        vec3 eye = normalize(-vertex); // use -V because V is in eye space (0,0,0); we need direction from surface to eye
        vec3 halfv = normalize(eye + light); //half vector // always add ambient light
        vec4 color = gl_FrontMaterial.ambient * gl_LightSource[0].ambient + gl_LightModel.ambient * gl_FrontMaterial.ambient;
        float ndotl = dot(normal,light);
        if (ndotl > 0.0) { // check light direction and add diffuse/specular light only if reflection is possible
                color += gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse * ndotl; // add diffuse light
                float ndoth = dot(normal,halfv); // add specular light
                if (dot(normal,halfv) >0.0)
                        color += gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(ndoth, gl_FrontMaterial.shininess);
        }
        gl_FragColor = color; // set current pixel color
}

