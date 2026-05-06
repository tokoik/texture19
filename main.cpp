#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#if defined(WIN32)
//#  pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#  include "glut.h"
#  include "glext.h"
PFNGLACTIVETEXTUREPROC glActiveTexture;
PFNGLMULTITEXCOORD2DVPROC glMultiTexCoord2dv;
#elif defined(__APPLE__) || defined(MACOSX)
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

/* トラックボール処理用関数の宣言 */
#include "trackball.h"

/* 箱を描く関数の宣言 */
#include "box.h"

/*
** 光源
*/
static const GLfloat lightpos[] = { 0.0, 0.0, 1.0, 0.0 }; /* 位置　　　 */
static const GLfloat lightcol[] = { 1.0, 1.0, 1.0, 1.0 }; /* 直接光強度 */
static const GLfloat lightamb[] = { 0.1, 0.1, 0.1, 1.0 }; /* 環境光強度 */

/*
** テクスチャ
*/
#define TEXWIDTH  256                      /* テクスチャの幅　　　 */
#define TEXHEIGHT 256                      /* テクスチャの高さ　　 */
static const char texture1[] = "dot.raw";  /* テクスチャファイル名 */

/*
** 初期化
*/
static void init(void)
{
  /* テクスチャの読み込みに使う配列 */
  GLubyte texture[TEXHEIGHT * TEXWIDTH * 4];
  FILE *fp;
  
  /* テクスチャの境界色 */
  static const GLfloat border[] = { 0.0, 0.0, 0.0, 0.0 };
  
  /* テクスチャ画像はワード単位に詰め込まれている */
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  
  /* 各テクスチャユニット用にテクスチャオブジェクトを作る */
  GLuint texname[3];
  glGenTextures(3, texname);
  
#if defined(WIN32)
  glActiveTexture =
    (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
  glMultiTexCoord2dv =
    (PFNGLMULTITEXCOORD2DVPROC)wglGetProcAddress("glMultiTexCoord2dv");
#endif
  
  /* 裏面の放物面テクスチャのマッピングに使うテクスチャユニット */
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texname[0]);
  
  /* テクスチャ画像の読み込み */
  if ((fp = fopen("paraboloid1.raw", "rb")) != NULL) {
    fread(texture, sizeof texture, 1, fp);
    fclose(fp);
  }
  
  /* テクスチャの割り当て */
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXWIDTH, TEXHEIGHT, 0,
    GL_RGBA, GL_UNSIGNED_BYTE, texture);
  
  /* テクスチャを拡大・縮小する方法の指定 */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  
  /* テクスチャの繰り返し方法の指定 */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  
  /* テクスチャ環境 */
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  
  /* 反射ベクトルをテクスチャ座標として使う */
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
  glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

  /* 裏面のマッピングに使うテクスチャ変換行列の設定 */
  glMatrixMode(GL_TEXTURE);
  static const GLdouble mat1[] = {
     1.0,  0.0,  0.0,  0.0,
     0.0,  1.0,  0.0,  0.0,
    -1.0, -1.0,  0.0, -2.0,
     1.0,  1.0,  0.0,  2.0,
  };
  glLoadMatrixd(mat1);
  glMatrixMode(GL_MODELVIEW);
  
  /* 表面の放物面テクスチャのマッピングに使うテクスチャユニット */
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texname[1]);

  /* テクスチャ画像の読み込み */
  if ((fp = fopen("paraboloid2.raw", "rb")) != NULL) {
    fread(texture, sizeof texture, 1, fp);
    fclose(fp);
  }
  
  /* テクスチャの割り当て */
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXWIDTH, TEXHEIGHT, 0,
    GL_RGBA, GL_UNSIGNED_BYTE, texture);
  
  /* テクスチャを拡大・縮小する方法の指定 */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  
  /* テクスチャの繰り返し方法の指定 */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  
  /* テクスチャの境界色を黒にする */
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
  
  /* テクスチャ環境 */
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  
  /* 反射ベクトルをテクスチャ座標として使う */
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
  glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
  
  /* 表面のマッピングに使うテクスチャ変換行列の設定 */
  glMatrixMode(GL_TEXTURE);
  static const GLdouble mat2[] = {
     1.0,  0.0,  0.0,  0.0,
     0.0,  1.0,  0.0,  0.0,
     1.0,  1.0,  0.0,  2.0,
     1.0,  1.0,  0.0,  2.0,
  };
  glLoadMatrixd(mat2);
  glMatrixMode(GL_MODELVIEW);

  /* 拡散色のマッピングに用いるテクスチャユニット */
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, texname[2]);
  
  /* テクスチャ画像の読み込み */
  if ((fp = fopen(texture1, "rb")) != NULL) {
    fread(texture, sizeof texture, 1, fp);
    fclose(fp);
  }
  
  /* テクスチャの割り当て */
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXWIDTH, TEXHEIGHT, 0,
    GL_RGBA, GL_UNSIGNED_BYTE, texture);
  
  /* テクスチャを拡大・縮小する方法の指定 */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  
  /* テクスチャの繰り返し方法の指定 */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  
  /* テクスチャ環境 */
//  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
  glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE);
  static const GLfloat blend[] = { 1.0, 1.0, 1.0, 0.5 };
  glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, blend);
  
  /* 初期設定 */
  glClearColor(0.3, 0.3, 1.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  
  /* 光源の初期設定 */
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcol);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol);
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb);
}

/*
** シーンの描画
*/
static void scene(void)
{
  static const GLfloat color[] = { 1.0f, 1.0f, 1.0f, 1.0f };  /* 材質 (色) */
  
  /* 材質の設定 */
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  
  /* 表面の放物面テクスチャのマッピング開始 */
  glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
  glEnable(GL_TEXTURE_GEN_R);

  /* 表面の放物面テクスチャのマッピング開始 */
  glActiveTexture(GL_TEXTURE1);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
  glEnable(GL_TEXTURE_GEN_R);
  
#if 1
  /* テクスチャマッピング開始 */
  glActiveTexture(GL_TEXTURE2);
  glEnable(GL_TEXTURE_2D);
#endif
  
  /* トラックボール処理による回転 */
  glMultMatrixd(trackballRotation());
  
  /* 箱を描く */
#if 1
  box(1.0, 1.0, 1.0);
#else
  glutSolidTeapot(1.2);
  //glutSolidSphere(1.5, 32, 64);
  //glutSolidTorus(0.5, 1.0, 32, 64);
#endif
  
#if 1
  /* テクスチャマッピング終了 */
  glDisable(GL_TEXTURE_2D);
#endif
  
  /* 表面の放物面テクスチャのマッピング終了 */
  glActiveTexture(GL_TEXTURE1);
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
  glDisable(GL_TEXTURE_GEN_R);
  glDisable(GL_TEXTURE_2D);
  
  /* 裏面の放物面テクスチャのマッピング終了 */
  glActiveTexture(GL_TEXTURE0);
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
  glDisable(GL_TEXTURE_GEN_R);
  glDisable(GL_TEXTURE_2D);
}


/****************************
** GLUT のコールバック関数 **
****************************/

static void display(void)
{
  /* 画面クリア */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  /* モデルビュー変換行列の設定 */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  /* 光源の位置を設定 */
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
  
  /* 視点の移動（物体の方を奥に移動）*/
  glTranslated(0.0, 0.0, -5.0);
  
  /* シーンの描画 */
  scene();
  
  /* ダブルバッファリング */
  glutSwapBuffers();
}

static void resize(int w, int h)
{
  /* トラックボールする範囲 */
  trackballRegion(w, h);
  
  /* ウィンドウ全体をビューポートにする */
  glViewport(0, 0, w, h);
  
  /* 透視変換行列の指定 */
  glMatrixMode(GL_PROJECTION);
  
  /* 透視変換行列の初期化 */
  glLoadIdentity();
  gluPerspective(40.0, (double)w / (double)h, 0.1, 10.0);
}

static void idle(void)
{
  /* 画面の描き替え */
  glutPostRedisplay();
}

static void mouse(int button, int state, int x, int y)
{
  switch (button) {
  case GLUT_LEFT_BUTTON:
    switch (state) {
    case GLUT_DOWN:
      /* トラックボール開始 */
      trackballStart(x, y);
      glutIdleFunc(idle);
      break;
    case GLUT_UP:
      /* トラックボール停止 */
      trackballStop(x, y);
      glutIdleFunc(0);
      break;
    default:
      break;
    }
    break;
    default:
      break;
  }
}

static void motion(int x, int y)
{
  /* トラックボール移動 */
  trackballMotion(x, y);
}

static void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 'q':
  case 'Q':
  case '\033':
    /* ESC か q か Q をタイプしたら終了 */
    exit(0);
  default:
    break;
  }
}

/*
** メインプログラム
*/
int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(keyboard);
  init();
  glutMainLoop();
  return 0;
}
