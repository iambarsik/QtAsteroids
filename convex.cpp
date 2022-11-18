#include "convex.h"
#include <math.h>

Convex::Convex(QPoint center, QVector<QPoint> body, QColor color)
{
    m_angle = 0.0f;
    m_center = center;
    m_body = body;
    m_body_main = body;
    m_color = color;
}

void Convex::Draw(QPainter *p)
{
    p->setBrush(QBrush(m_color,Qt::NoBrush));
    p->setPen(QPen(m_color,3,Qt::SolidLine));
    p->drawLine(center(),pointXY(0));
    for(int i = 0; i < m_body.size() - 1; i++) {
        p->drawLine(pointXY(i),pointXY(i+1));
    }
    p->drawLine(pointXY(m_body.size()-1),pointXY(0));
}

void Convex::moveUp(int step)
{
    m_center.setX((float)m_center.x() - (float)step*sinf(m_angle));
    m_center.setY((float)m_center.y() - (float)step*cosf(m_angle));
    update();
}

void Convex::moveDown(int step)
{
    m_center.setX((float)m_center.x() + (float)step*sinf(m_angle));
    m_center.setY((float)m_center.y() + (float)step*cosf(m_angle));
    update();
}

void Convex::rotate(float angle)    {

    m_angle += angle;
    for(int i = 0; i < m_body.size();i++)  {
        float nx = m_body_main[i].x()*cosf(m_angle) + m_body_main[i].y()*sinf(m_angle);
        float ny = -m_body_main[i].x()*sinf(m_angle) + m_body_main[i].y()*cosf(m_angle);
        m_body[i].setX(nx);
        m_body[i].setY(ny);
    }
}

void Convex::update()
{
    if(m_center.x() < m_game_field.x()) m_center.setX(m_center.x() + m_game_field.width());
    if(m_center.x() > m_game_field.width()) m_center.setX(m_center.x() - m_game_field.width());
    if(m_center.y() < m_game_field.y()) m_center.setY(m_center.y() + m_game_field.height());
    if(m_center.y() > m_game_field.height()) m_center.setY(m_center.y() - m_game_field.height());
}


