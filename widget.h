#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QtOpenGL>
#include <QPoint>
#include <QVector>

#include "defines.h"
#include "classes.h"
#include "convex.h"




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

    //Convex * game_ship;

    QVector <Convex> game_object;
    QVector <myObject> game_bullet;
    void addObject();

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
