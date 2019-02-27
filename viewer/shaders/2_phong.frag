#version 410
#define M_PI  3.14159265358979323846

uniform float lightIntensity;
uniform bool blinnPhong;
uniform float shininess;
uniform float eta;
uniform sampler2D shadowMap;

in vec4 eyeVector;
in vec4 lightVector;
in vec4 vertColor;
in vec4 vertNormal;
in vec4 lightSpace;

out vec4 fragColor;

float F(float cos_theta){
    float Fs,Fp;
    float c_i;
    if ((pow(eta, 2) - 1 + pow(cos_theta, 2)) > 0) {
      c_i = sqrt(pow(eta, 2) - 1 + pow(cos_theta, 2));
      Fs = pow( (cos_theta - c_i)/(cos_theta + c_i) , 2);
      float eta_carre = eta * eta;
      Fp = pow( (eta_carre*cos_theta - c_i)/(eta_carre*cos_theta + c_i) , 2);
    }
    else {
      Fs = 1;
      Fp = 1;
    }
    return (Fs + Fp)/2;
}


float D(float cos_theta, float alpha){
  if (cos_theta > 0){
    float tan_2 = (1/ (cos_theta * cos_theta)) - 1;
    float num = alpha * alpha;
    float den = M_PI * pow(cos_theta, 4) * pow(alpha * alpha + tan_2, 2);
    return num/den;
  }
  else {
    return 0;
  }
}

float G1(float cos_theta, float alpha){
  float tan_2 = (1/ (cos_theta * cos_theta)) - 1;
  return 2.0/(1+sqrt(1+alpha*alpha*tan_2));
}

void main( void )
{
     // This is the place where there's work to be done

     //normalisation des vecteurs
     //  lightSpace = normalize(lightSpace);
     vec4 n = normalize(vertNormal);
     vec4 V = normalize(eyeVector);
     vec4 L = normalize(lightVector);
     vec4 C = normalize(vertColor);

     vec4 halfVector = normalize(eyeVector + lightVector);
     vec4 Ca, Cd, Cs;

     float cos_thetaD = dot(halfVector, L);
     float F = F(cos_thetaD);

     Ca = 0.1 * lightIntensity * C ;
     Cd = 0.5 * C * lightIntensity * max(0, dot(n, L));

     if (blinnPhong){
       Cs = F * C * lightIntensity * pow(max(0, dot(n, halfVector)), shininess);
     }

     else {
       float alpha = shininess/200;
       float cos_theta_i = dot(L,n);
       float cos_theta_o = dot(V,n);
       float cos_theta_h = dot(halfVector,n);
       float D = D(cos_theta_h,alpha);
       float G1i = G1(cos_theta_i,alpha);
       float G1o = G1(cos_theta_o,alpha);

       Cs = (C * lightIntensity * F * D * G1i * G1o) / (4*cos_theta_i*cos_theta_o);
     }

     fragColor = Ca + Cd + Cs;
}
