#if defined(WIN32)
#  include "glut.h"
#  include "glext.h"
extern PFNGLMULTITEXCOORD2DVPROC glMultiTexCoord2dv;
#elif defined(__APPLE__) || defined(MACOSX)
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "box.h"

/*
** 箱の描画
*/
void box(double x, double y, double z)
{
  /* 頂点の座標値 */
  const GLdouble vertex[][4][3] = {
    {{ -x, -y, -z }, {  x, -y, -z }, {  x, -y,  z }, { -x, -y,  z }}, /* 下 */
    {{  x, -y, -z }, { -x, -y, -z }, { -x,  y, -z }, {  x,  y, -z }}, /* 裏 */
    {{  x, -y,  z }, {  x, -y, -z }, {  x,  y, -z }, {  x,  y,  z }}, /* 右 */
    {{ -x, -y,  z }, {  x, -y,  z }, {  x,  y,  z }, { -x,  y,  z }}, /* 前 */
    {{ -x, -y, -z }, { -x, -y,  z }, { -x,  y,  z }, { -x,  y, -z }}, /* 左 */
    {{ -x,  y,  z }, {  x,  y,  z }, {  x,  y, -z }, { -x,  y, -z }}, /* 上 */
  };
  
  /* 頂点のテクスチャ座標 */
  static const GLdouble texcoord[][4][2] = {
    {{ 0.0, 1.0 }, { 1.0, 1.0 }, { 1.0, 0.0 }, { 0.0, 0.0 }},
    {{ 0.0, 1.0 }, { 1.0, 1.0 }, { 1.0, 0.0 }, { 0.0, 0.0 }},
    {{ 0.0, 1.0 }, { 1.0, 1.0 }, { 1.0, 0.0 }, { 0.0, 0.0 }},
    {{ 0.0, 1.0 }, { 1.0, 1.0 }, { 1.0, 0.0 }, { 0.0, 0.0 }},
    {{ 0.0, 1.0 }, { 1.0, 1.0 }, { 1.0, 0.0 }, { 0.0, 0.0 }},
    {{ 0.0, 1.0 }, { 1.0, 1.0 }, { 1.0, 0.0 }, { 0.0, 0.0 }},
  };
  
  /* 面の法線ベクトル */
  static const GLdouble normal[][3] = {
    {  0.0, -1.0,  0.0 },
    {  0.0,  0.0, -1.0 },
    {  1.0,  0.0,  0.0 },
    {  0.0,  0.0,  1.0 },
    { -1.0,  0.0,  0.0 },
    {  0.0,  1.0,  0.0 },
  };
  
  int i, j;
  
  /* 四角形６枚で箱を描く */
  glBegin(GL_QUADS);
  for (j = 0; j < 6; ++j) {
    glNormal3dv(normal[j]);
    for (i = 0; i < 4; ++i) {
      /* テクスチャ座標の指定 */
      glMultiTexCoord2dv(GL_TEXTURE2, texcoord[j][i]);
      /* 対応する頂点座標の指定 */
      glVertex3dv(vertex[j][i]);
    }
  }
  glEnd();
}
