#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QtOpenGL>
#include <QPoint>
#include <QVector>
#include <QThread>
#include <QSound>

#include "defines.h"
#include "classes.h"
#include "convex.h"

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QGLWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

        // engine variables
        // ================================================================================
    QRect GameField;
    QRect Resolution;
    bool bRotateScreen;
    bool bStretchHorizontal;
    float fImageX, fImageY;
    float fImageZoom;

    mySettings Settings;

    bool myKey[BUTTON_COUNT];
    float dX,dY;

    QImage DEV_TILE_IMAGE;

    TouchButton *TouchKeys[BUTTON_COUNT];
    QList <QTouchEvent::TouchPoint> listKeys;

    QString sSymbols;
    QImage imgSymbol_medium[136];
    QImage imgSymbol_small[136];
    QImage imgSymbol_micro[136];
    QImage imgSymbol_big[136];
    QImage FONT_IMAGE;

    QTimer *GameLoop;
    long long iForGame, iForAnimation;
    int dev_player;


    // ==================================== user data =================================

    QSound * sound_shoot;
    QSound * sound_explosive;
    QSound * sound_damage;
    QSound * sound_bomb;

    myMenu game_main_menu;
    myMenu game_language_menu;
    myMenu game_pause_menu;
    myMenu game_dead_menu;

    GameScreens game_screen, game_state;
    int game_results[5];

    bool game_play;
    bool game_flash;
    int game_hp;
    int game_score;
    int game_spawn_timer;

    int AsteroidTimer;
    QVector <Convex> game_object;
    QVector <myObject> game_bullet;

    void addAsteroid(enum ConvexType type, int X, int Y);
    void addBullet(int num_player);
    bool collideAsteroidAndBullet(myObject bullet, Convex Asteroid);
    void CheckCollide();
    void setButton(int Code, bool enable);

    bool dev;

        // engine methods
    // ================================================================================
    void SetStartValues();
    void OnKeys(int Key, bool press);
    void OnTouch();
    void OnDrawText(int x, int y, int size, QString text_eng, QString text_rus, QPainter *pnt);
    void setColor(QColor brush, QColor pen, Qt::BrushStyle style, QPainter *qScr);
    int randomBetween(int low, int high) { return (qrand() % ((high + 1) - low) + low); }
        // ================================================================================

        // ============================= QTs methods() ===================================

    void paintEvent(QPaintEvent *event)override;
    void keyPressEvent(QKeyEvent *event)override;
    void keyReleaseEvent(QKeyEvent *event)override;

    void gameover();
    void saveresults();
    void loadresults();

private slots:
    void OnGame();
    void OnAnimation();
    void GamePlayFunction();

protected:
    bool event(QEvent *event) override;

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
