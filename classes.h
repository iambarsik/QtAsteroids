#ifndef CLASSES_H
#define CLASSES_H

#include <QtWidgets>

struct mySettings   {
    bool bDevMode;
    bool bEnglish;
};

class myObject    {
public:
    myObject() {
        X = 0;
        Y = 0;
        Width = 0;
        Height = 0;
        EnableImage = false;
    }
    QPoint point() { return QPoint(X,Y); }
    void setGeomerty(double x, double y, double w, double h)    {
        X = x;
        Y = y;
        Width = w;
        Height = h;
    }
    void setXY(int x, int y)    {
        X = x;
        Y = y;
    }
    void setImage(QImage image) {
        this->image = image; EnableImage = true;
        Width = image.width();
        Height = image.height();
    }
    void setField(double mx, double my) { maxX = mx, maxY = my; }
    void setV(double vx, double vy) { Vx = vx; Vy = vy; }
    void Update()   {
        X += Vx;
        Y += Vy;
        if(X < 0 || X + Width  > maxX) Vx = -Vx;
        if(Y < 0 || Y + Height > maxY) Vy = -Vy;
    }
    void Draw(QPainter *pnt)    {
        if(EnableImage) {
            pnt->drawImage(X,Y,image);
        } else {
            pnt->setBrush(QBrush(Qt::yellow,Qt::SolidPattern));
            pnt->drawRect(X,Y,Width,Height);
        }
    }
private:
    float X, Y, maxX, maxY, Width, Height , Vx, Vy;
    bool EnableImage;
    QImage image;
};

class TouchButton   {
public:
    QRect Geometry;
    QImage imgPressed, imgUnpressed;
    bool Value, oldValue, Enable, bImageEnabled;
    int Code;

    TouchButton(QRect geometry,int code)   {
        Geometry = geometry;
        Value = false;
        oldValue = false;
        Code = code;
        Enable = true;
        bImageEnabled = false;
    }

    void SetImage(QImage pressed, QImage unpressed) {
        imgPressed = pressed;
        imgUnpressed = unpressed;
        bImageEnabled = true;
    }

    void Draw(QPainter *pnt)    {
        if(bImageEnabled)   {
            if(Value)   {
                pnt->drawImage(Geometry.x(),Geometry.y(),imgPressed);
            } else {
                pnt->drawImage(Geometry.x(),Geometry.y(),imgUnpressed);
            }
        } else {
            if(Value)   {
                pnt->setPen(QPen(Qt::red));
                pnt->setBrush(QBrush(Qt::red));
                pnt->setBrush(Qt::SolidPattern);
            } else {
                pnt->setPen(QPen(Qt::white));
                pnt->setBrush(QBrush(Qt::white));
                pnt->setBrush(Qt::NoBrush);
            }
            pnt->drawEllipse(Geometry.x(),Geometry.y(),Geometry.width(),Geometry.height());
        }
    }
};


#endif // CLASSES_H
