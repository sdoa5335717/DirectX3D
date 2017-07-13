#ifndef LIGHT_H
#define LIGHT_H
struct stLight
{
	stLight()
	{
		type = 0;
		posX = 0, posY=0, posZ=0;
		dirX = 0, dirY=0, dirZ=0;

		ambientR = ambientG = ambientB = ambientA = 1;
		diffuseR = diffuseG = diffuseB = diffuseA = 1;
		specularR = specularG = specularB = specularA = 0;

		range = 0;
		falloff = 0;
		attenuation0 = 0;
		attenuation1 = 0;
		attenuation2 = 0;

		theta = 0;
		phi = 0;
	}

	int type;
	float posX, posY, posZ;
	float dirX, dirY, dirZ;
	float ambientR, ambientG, ambientB, ambientA;
	float diffuseR, diffuseG, diffuseB, diffuseA;
	float specularR, specularG, specularB, specularA;
	float range;
	float falloff;
	float attenuation0,attenuation1,attenuation2;

	float theta;
	float phi;
};
#endif