varying vec3 N;
varying vec3 v;
varying vec4 diffuse;


void main(void)
{

   v = vec3(gl_ModelViewMatrix * gl_Vertex);       
   N = normalize(gl_NormalMatrix * gl_Normal);
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   diffuse = gl_Color;
}