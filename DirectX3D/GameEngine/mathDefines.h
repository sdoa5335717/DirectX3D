#ifndef _UGP_MATH_DEF_H_
#define _UGP_MATH_DEF_H_


#define UGP_FRONT    0
#define UGP_BACK     1
#define UGP_ON_PLANE 2
#define UGP_CLIPPED  3
#define UGP_CULLED   4
#define UGP_VISIBLE  5

#ifndef PI
#define PI 3.14159265358979323846
#endif

// 将度转换成弧度
#define GET_RADIANS(degree) (float)(degree / 180.0f * PI)

#endif
