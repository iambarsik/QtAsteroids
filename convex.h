#ifndef CONVEX_H
#define CONVEX_H

#include <QPainter>
#include <QPoint>
#include <QVector>

#include "classes.h"

enum ConvexType {
    Player,
    AsteroidBig,
    AsteroidMedium,
    AsteroidSmall,
    AsteroidHP,
    AsteroidBomb
};

class Convex
{
public:
    Convex(QPoint center, QVector<QPoint> body, QColor color);

    QVector<QPoint> getBody() { return m_body; }
    QVector<QPoint> getBodyMain() { return m_body_main; }
    QPoint center() { return m_center; }
    QPoint pointXY(int point) { return QPoint(m_body[point].x() + m_center.x(),m_body[point].y() + m_center.y()); }
    int size() { return m_body.size(); }
    float angle()   { return m_angle; }
    float radius()  { return m_radius; }
    float vx()  { return Vx; }
    float vy()  { return Vy; }
    float mass(){ return m_mass; }
    ConvexType type()   { return m_type; }

    void reverseRotate()    { Va = -Va; }
    void setGameField(QRect field) { m_game_field = field; }
    void setType(ConvexType type) { m_type = type; }
    void setV(float vx, float vy) { Vx = vx; Vy = vy; }
    void setVx(float vx)    { Vx = vx; }
    void setVy(float vy)    { Vy = vy; }
    void setVReverse() { Vx = -Vx; Vy = -Vy; }
    void setRadius(float R)    { m_radius = R; }

    void setColor(QColor color) { m_color = color; }
    void setID(int id)  { m_id = id; }

    void restore()  { m_center = m_center_prev; }
    void correctX(float value) { m_center.setX(m_center.x() + value); }
    void correctY(float value) { m_center.setY(m_center.y() + value); }
    void destroy() { m_need_destroy = true; }

    void Draw(QPainter *p);
    void moveUp(float step);
    void moveRight(int step);
    void moveDown(float step);
    void moveLeft(int step);
    void rotate(float angle);

    bool collidePoint(QPoint p);
    bool collideConvex(Convex c);
    bool collideConvex(QPoint p);
    bool isDestroyed()    { return m_need_destroy; }

    int id() { return m_id; }

    void fly();
    void update();

private:
    QPoint m_center;
    QPoint m_center_prev;
    QVector<QPoint> m_body;
    QVector<QPoint> m_body_main;
    float m_angle;
    float Vx, Vy, Va;
    float m_mass;
    int m_id;
    bool m_need_destroy;

    float m_radius;

    QColor m_color;
    QRect m_game_field;
    ConvexType m_type;

};

#endif // CONVEX_H
