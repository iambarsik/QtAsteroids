#ifndef CONVEX_H
#define CONVEX_H

#include <QPainter>
#include <QPoint>
#include <QVector>

class Convex
{
public:
    Convex(QPoint center, QVector<QPoint> body, QColor color);

    QVector<QPoint> getBody() { return m_body; }
    QVector<QPoint> getBodyMain() { return m_body_main; }
    QPoint center() { return m_center; }
    QPoint pointXY(int point) { return QPoint(m_body[point].x() + m_center.x(),m_body[point].y() + m_center.y()); }
    int size() { return m_body.size(); }
    void setGameField(QRect field) { m_game_field = field; }


    void Draw(QPainter *p);
    void moveUp(int step);
    void moveRight(int step);
    void moveDown(int step);
    void moveLeft(int step);
    void rotate(float angle);

    void update();

private:
    QPoint m_center;
    QVector<QPoint> m_body;
    QVector<QPoint> m_body_main;
    float m_angle;
    QColor m_color;
    QRect m_game_field;
};

#endif // CONVEX_H
