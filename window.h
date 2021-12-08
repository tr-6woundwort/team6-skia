

#ifndef OPENGL_PRO_WINDOW_H
#define OPENGL_PRO_WINDOW_H

#include <game.h>
#include "calib/zhu.h"
#include <calib/checkerborad.h>
#include <calib/colormap.h>
#include <calib/zhang.h>
#include <SkBitmap.h>
#include <SkCanvas.h>
#include <SkPicture.h>
#include <SkTypeface.h>
#include <SkStream.h>
#include <SkCodec.h>
#include <SkAndroidCodec.h>
#include <SkImage.h>

class Window : public Game {
public:
    SpriteRenderer *spriteRender;
    sk_sp<SkTypeface> font;
    SkBitmap bmMaster;
    sk_sp<SkImage> image;
    SkCanvas *canvas;
    sk_sp<SkImage> imageServant;
    SkBitmap bmServant;

    cv::VideoCapture capture;

    Window(GLuint width, GLuint height);

    void init();

    void destory();

    void processInput();

    void update();

    void render(GLFWwindow *window);

    SkBitmap loadTexture2DToSkBitmap( std::string file);

    sk_sp<SkImage> loadTexture2DToSkImage(std::string file);

    void draw(SkCanvas *canvas, sk_sp<SkImage> image, int posX, int posY, int sizeX, int sizeY, uchar alpha = 0xff);

    void draw(SkCanvas *canvas, SkBitmap bitmap, int posX, int posY, int sizeX, int sizeY, uchar alpha = 0xff);

    void drawFront(SkCanvas *canvas, std::string text, int posX, int posY, float size, uint color, uchar alpha = 0xff);
};


#endif //OPENGL_PRO_WINDOW_H

