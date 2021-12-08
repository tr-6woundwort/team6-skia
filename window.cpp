

#include "window.h"

Window::Window(GLuint width, GLuint height) : Game(width, height) {
    canvas = nullptr;
}

void Window::init() {
    StatusManager::renderMode = Render_2D;
    StatusManager::init();
    this->state = GAME_ACTIVE;

    ResourceManager::loadShader("sprite", ResourceManager::defaultVshader2D, ResourceManager::defaultFshader2D);

    spriteRender = new SpriteRenderer();
    spriteRender->init(ResourceManager::getShader("sprite"));
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::ortho(0.0f, static_cast<GLfloat>(this->width), static_cast<GLfloat>(this->height), 0.0f,
                            -1.0f, 1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));//move
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));//rotate
    model = glm::scale(model, glm::vec3(SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f)); //scale

    ResourceManager::getShader("sprite").setMatrix4("projection", projection);
    ResourceManager::getShader("sprite").setMatrix4("model", model);
    ResourceManager::getShader("sprite").setVector3f("spriteColor", glm::vec3(1.0, 1.0, 1.0));

    bmMaster.allocPixels(SkImageInfo::Make(1920, 720, kN32_SkColorType, kPremul_SkAlphaType));
    bmMaster.eraseColor(0xFF000000);
    canvas = new SkCanvas(bmMaster);

    //imageServant = loadTexture2DToSkImage("../res/colorMap.png");
    bmServant = loadTexture2DToSkBitmap("../res/colorMap.png");
}

void Window::destory() {
    Game::destroy();
}

void Window::processInput() {

}

SkBitmap Window::loadTexture2DToSkBitmap(std::string file) {
    clock_t start = clock();
    SkBitmap bitmap;
    SkString path(file.c_str());
    sk_sp<SkData> data = SkData::MakeFromFileName(path.c_str());
    if (!data) {
        printf("Missing file %s", path.c_str());
    }
    auto codec = SkAndroidCodec::MakeFromCodec(SkCodec::MakeFromData(std::move(data)));
    SkImageInfo info = codec->getInfo();
    bitmap.allocPixels(SkImageInfo::Make(info.width(), info.height(), kN32_SkColorType, kPremul_SkAlphaType));
    SkCodec::Result result = codec->getPixels(info, bitmap.getPixels(), bitmap.rowBytes());
    if (result == SkCodec::kSuccess) {
        printf("#Codec success\n");
    } else {
        printf("#Codec failure\n");
    }
    clock_t end = clock();
    int time_ms = (double) (end - start) / CLOCKS_PER_SEC * 1000;
    printf("#loadTexture2DToSkBitmap time: %d\n", time_ms);
    return bitmap;
}

sk_sp<SkImage> Window::loadTexture2DToSkImage(std::string file) {
    clock_t start = clock();
    sk_sp<SkData> data = SkData::MakeFromFileName(file.c_str());
    sk_sp<SkImage> image = SkImage::MakeFromEncoded(data);
    clock_t end = clock();
    int time_ms = (double) (end - start) / CLOCKS_PER_SEC * 1000;
    printf("#loadTexture2DToSkImage time: %d\n", time_ms);
    return image;
}

void Window::draw(SkCanvas *canvas, sk_sp<SkImage> image, int posX, int posY, int sizeX, int sizeY, uchar alpha) {
    SkRect rect;
    rect.setXYWH(posX, posY, posX + sizeX, posY + sizeY);
    SkPaint paint;
    paint.setAlpha(alpha);
    canvas->drawImageRect(image, rect, &paint);
}

void Window::draw(SkCanvas *canvas, SkBitmap bitmap, int posX, int posY, int sizeX, int sizeY, uchar alpha) {
    SkRect rect;
    rect.setXYWH(posX, posY, posX + sizeX, posY + sizeY);
    SkPaint paint;
    paint.setAlpha(alpha);
    canvas->drawBitmapRect(bitmap, rect, &paint);
}

void Window::drawFront(SkCanvas *canvas, std::string text, int posX, int posY, float size, uint color, uchar alpha) {
    SkPaint paint;
    paint.setARGB(uchar(color >> 24), uchar(color >> 16), uchar(color >> 8), (uchar)color);
    paint.setTypeface(SkTypeface::MakeFromFile("../res/fangsong.ttf"));
    paint.setTextSize(size);
    paint.setAlpha(alpha);
    std::string title(text.c_str());
    canvas->drawText(title.c_str(), title.length(), posX, posY, paint);
}

void Window::update() {
    canvas->clear(0x00000000);
    static int i = 0;

    //1.0 decode with bitmap
    draw(canvas, bmServant, 0, 0, 1920, 720, 0xff);
    drawFront(canvas, "abcd1234１２３４哈哈哈", 100, 50, 25, 0xffff0000, 0xff);
    ResourceManager::loadTexture2D("codec_data", (uchar *) bmMaster.getPixels(), bmMaster.width(), bmMaster.height(), 4);

    //2.0 decode with image
    //draw(canvas, imageServant, 0, 0, 1920, 720, 0xff);
    //drawFront(canvas, "123456789", 100, 50, 25, 0xffff0000, 0xff);
    //ResourceManager::loadTexture2D("codec_data", (uchar *) bmMaster.getPixels(), bmMaster.width(), bmMaster.height(), 4);
}

void Window::render(GLFWwindow *window) {
    if (this->state == GAME_ACTIVE) {
        spriteRender->drawSprite(ResourceManager::getTexture2D("codec_data"));
        usleep(1000 * 30);
    }
}

