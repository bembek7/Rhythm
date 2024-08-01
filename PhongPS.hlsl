float Attenuate(uniform float attenuationConst, uniform float attenuationLin, uniform float attenuationQuad, const in float distanceToL)
{
    return 1.0f / (attenuationConst + attenuationLin * distanceToL + attenuationQuad * (distanceToL * distanceToL));
}

float3 Diffuse(
    uniform float3 diffuseColor,
    uniform float diffuseIntensity,
    const in float attenuation,
    const in float3 viewDirToL,
    const in float3 viewNormal)
{
    return diffuseColor * diffuseIntensity * attenuation * max(0.0f, dot(viewDirToL, viewNormal));
}

float3 Speculate(
    const in float3 specularColor,
    uniform float specularIntensity,
    const in float3 viewNormal,
    const in float3 viewToLight,
    const in float3 viewPos,
    const in float attenuation,
    const in float specularPower)
{
    const float3 w = viewNormal * dot(viewToLight, viewNormal);
    const float3 reflected = normalize(w * 2.0f - viewToLight);

    const float3 viewCamToPos = normalize(viewPos);

    return attenuation * specularColor * specularIntensity * pow(max(0.0f, dot(-reflected, viewCamToPos)), specularPower);
}

struct LightVectorData
{
    float3 vectorToLight;
    float3 directionToLight;
    float distanceToLight;
};

float4 main(float3 viewPos : POSITION, float3 viewNormal : NORMAL) : SV_TARGET
{
    const float3 diffuseColor = float3(1.0f, 1.0f, 1.0f);
    const float diffuseIntensity = 1.f;
    const float specularIntensity = 0.8f;
    const float specularPower = 0.2f;
    const float3 ambient = float3(0.05f, 0.05f, 0.05f);
    
    const float3 lightPos = float3(0.f, 0.f, 0.f);
    
    LightVectorData lightVector;
    lightVector.vectorToLight = lightPos - viewPos;
    lightVector.distanceToLight = length(lightVector.vectorToLight);
    lightVector.directionToLight = lightVector.vectorToLight / lightVector.distanceToLight;
    
    viewNormal = normalize(viewNormal);
    
    const float attenuation = Attenuate(1.0f, 0.045f, 0.0075f, lightVector.distanceToLight);
	
    const float3 diffuse = Diffuse(diffuseColor, diffuseIntensity, attenuation, lightVector.directionToLight, viewNormal);
	
    const float3 specular = Speculate(diffuseColor, diffuseIntensity * specularIntensity, viewNormal, lightVector.vectorToLight, viewPos, attenuation, specularPower);
	
    return float4(saturate((diffuse + ambient) * float3(1.0f, 1.0f, 1.0f) + specular), 1.0f);
}