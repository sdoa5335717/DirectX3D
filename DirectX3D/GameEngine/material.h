#ifndef MATERIAL_H
#define MATERIAL_H
struct stMaterial
{
	stMaterial()
	{
		emissiveR = emissiveG = emissiveB = emissiveA = 0;
		ambientR = ambientG = ambientB = ambientA = 1;
		diffuseA = diffuseG = diffuseB = diffuseR = 1;
		specularR = specularG = specularB = specularA = 0;
		power = 0;
	}

	float emissiveR, emissiveG, emissiveB, emissiveA;
	float ambientR, ambientG, ambientB, ambientA;
	float diffuseR, diffuseG, diffuseB, diffuseA;
	float specularR, specularG, specularB, specularA;

	float power;
};
#endif