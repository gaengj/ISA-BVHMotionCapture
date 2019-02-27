#version 410
#define M_PI 3.14159265358979323846

uniform mat4 mat_inverse;
uniform mat4 persp_inverse;
uniform sampler2D envMap;
uniform vec3 center;
uniform float radius;

uniform bool transparent;
uniform float shininess;
uniform float eta;

in vec4 position;

out vec4 fragColor;

float F(float cos_theta){
    float Fs,Fp;
    float c_i;
    float test = eta * eta - 1 + cos_theta * cos_theta;
    float eta_carre = eta * eta;
    if (test > 0) {
      c_i = sqrt(test);
      Fs = ((cos_theta - c_i) * (cos_theta - c_i)) / ((cos_theta + c_i) * (cos_theta + c_i));
      Fp = ((eta_carre*cos_theta - c_i) * (eta_carre*cos_theta - c_i)) / ((eta_carre*cos_theta + c_i) * (eta_carre*cos_theta + c_i));
    }
    else {
      Fs = 1;
      Fp = 1;
    }
    return (Fs + Fp)/2;
}

vec4 getColorFromEnvironment(in vec3 direction)
{
    // TODO
    vec2 coordsphere;
    coordsphere.y = (asin(direction.z) + (M_PI/2) )/ M_PI;
    coordsphere.x = (atan(direction.y,direction.x) + M_PI)/ (2*M_PI);
    vec4 color = texture2D(envMap, coordsphere);
    return color;
}



bool raySphereIntersect(in vec3 start, in vec3 direction, out vec3 newPoint) {
    vec3 CP;
    CP = start - center;
    // CP.w = 0.0;
    float delta = pow(dot(direction, CP),2) - dot(CP,CP) + radius*radius;
    if (delta > 0) {
      float x1 = -dot(direction, CP) - sqrt(delta);
      float x2 = -dot(direction, CP) + sqrt(delta);
      float lambda;
      if (x2 < 0.001){
        return false;
      }
      else if (x1 < 0.001) {
        lambda = x2;
      }
      else {
        lambda = x1;
      }
      newPoint = start + lambda * direction ;
      return true;
    }
    else return false;
}

void main(void)
{
    // Step 1: I need pixel coordinates. Division by w?
    vec4 worldPos = position;
    worldPos.z = 1; // near clipping plane
    worldPos = persp_inverse * worldPos;
    worldPos /= worldPos.w;
    worldPos.w = 0;
    worldPos = normalize(worldPos);
    // Step 2: ray direction:
    vec3 u = normalize((mat_inverse * worldPos).xyz);
    vec3 eye = (mat_inverse * vec4(0, 0, 0, 1)).xyz;

    // TODO

    bool res;
    vec3 old_inter, new_inter;
    vec3 reflechi,refracte,normal,incident;
    vec4 resultColor = vec4(0,0,0,1);
    float cos_theta, old_fresnel, new_fresnel;

    if (transparent){

      res = raySphereIntersect(eye,u,old_inter);
      if (res){
        normal = normalize(old_inter - center);
        reflechi = normalize(reflect(u, normal));
        refracte = normalize(refract(u, normal, 1.0/eta));

        int nb = 2; //nb rayons refracte
        cos_theta = dot(-u, normal);
        old_fresnel = F(cos_theta);
        resultColor = old_fresnel*getColorFromEnvironment(reflechi);
        old_fresnel = 1 - old_fresnel;
        incident = refracte;

        for(int i = 0; i< nb-1; i++){
          res = raySphereIntersect(old_inter,incident,new_inter);
          if (res){
            normal = normalize(center - new_inter);
            reflechi = normalize(reflect(incident, normal));
            refracte = normalize(refract(incident, normal, eta));
            cos_theta = dot(-incident, normal);
            new_fresnel = F(cos_theta);
            // resultColor += new_fresnel*old_fresnel*getColorFromEnvironment(reflechi) + old_fresnel*(1-new_fresnel)*getColorFromEnvironment(refracte);
            if (new_fresnel < 1){
              resultColor += old_fresnel*(1-new_fresnel)*getColorFromEnvironment(refracte);
            }
            old_fresnel = old_fresnel*new_fresnel;
            incident = reflechi;
          }

        }
      }
      else {
        resultColor = getColorFromEnvironment(u);
      }
    }

    else {
      res = raySphereIntersect(eye,u,old_inter);
      if (res){
        normal = normalize(old_inter - center);
        reflechi = normalize(reflect(u, normal));
        cos_theta = dot(-u, normal);
        old_fresnel = F(cos_theta);
        resultColor = old_fresnel*getColorFromEnvironment(reflechi);
      }
      else {
        resultColor = getColorFromEnvironment(u);
      }
    }

    // faire boucle pour tous les rayons, utiliser coeff fresnel
    // resultColor = getColorFromEnvironment(reflected1) + getColorFromEnvironment(reflected2);
    fragColor = resultColor;
}
