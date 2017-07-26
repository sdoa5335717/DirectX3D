#ifndef _UGP_STRUCTS_H_
#define _UGP_STRUCTS_H_


// GUI控件的顶点结构
struct stGUIVertex
{
	float x, y, z, rhw;
	unsigned long color;
	float tu, tv;
};

#endif