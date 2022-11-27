#ifndef CLASSES_H
#define CLASSES_H

#include <QtWidgets>

struct mySettings   {
    bool bDevMode;
    bool bEnglish;
};

struct fPoint   {
public:
    fPoint(float x, float y)    {
        X = x;
        Y = y;
    }
    float x() { return X; }
    float y() { return Y; }
private:
    float X, Y;
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
        if(X < 0) X += maxX;
        if(Y < 0) Y += maxY;
        if(X + Width  > maxX) X -= maxX;
        if(Y + Height > maxY) Y -= maxY;
    }
    void Draw(QPainter *pnt)    {
        if(EnableImage) {
            pnt->drawImage(X,Y,image);
        } else {
            pnt->setBrush(QBrush(Qt::yellow,Qt::SolidPattern));
            pnt->drawRect(X,Y,Width,Height);
        }
    }
    float x()   { return X;}
    float y()   { return Y;}
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

enum MenuActions    {
    None,
    StartGame,
    Exit
};

class myMenu    {
public:
    struct Punkt    {
        QString NameEng, NameRus;
        MenuActions Action;
        Punkt(QString name_eng, QString name_rus, MenuActions action) {
            NameEng = name_eng;
            NameRus = name_rus;
            Action = action;
        }
    };

    bool bEnglish;

    int Position;
    QVector<Punkt> Options;

    myMenu()    {
        Position = 0;
    }
    void Init(QVector<Punkt> Menu, bool english)  {
        Options.clear();
        Options = Menu;
        bEnglish = english;
    }
    void SetLanguage(bool english) { bEnglish = english; }
    QString getPositionName(int pos)    {
        if(Options.size() > 0)  {
            if(pos == Position) {
                if(bEnglish) return QString(" -> %1").arg(Options[pos].NameEng);
                else return QString(" -> %1").arg(Options[pos].NameRus);
            } else {
                if(bEnglish) return QString("    %1").arg(Options[pos].NameEng);
                else return QString("    %1").arg(Options[pos].NameRus);
            }
        }
        return "";
    }
    MenuActions getPositionAction(int pos)  {
        if(Options.size() > 0)  {
            return Options[pos].Action;
        }
        return MenuActions::None;
    }
    MenuActions getnAction()  {
        if(Options.size() > 0)  {
            return Options[Position].Action;
        }
        return MenuActions::None;
    }
    void goUp() { if(Position > 0) Position--; }
    void goDown() { if(Position + 1 < Options.size()) Position++; }

    QVector<QString> getMenu() {
        QVector<QString> menu;
        if(Options.size() == 0) {
            menu.push_back(QString("Empty menu"));
            return menu;
        }
        if(Options.size() <= 5) {
            for(int i = 0; i < Options.size();i++)  {
                menu.push_back(getPositionName(i));
            }
            return menu;
        }
        if(Position >= Options.size() - 3) {
            for(int i = Position + 3; i < Position - 5; i--)  {
                menu.push_front(getPositionName(i));
            }
            return menu;
        }
        return menu;
    }
};


#endif // CLASSES_H
