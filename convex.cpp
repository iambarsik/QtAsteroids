#include "convex.h"
#include <math.h>

Convex::Convex(QPoint center, QVector<QPoint> body, QColor color)   {
    m_angle = 0.0f;
    m_center = center;
    m_center_prev = center;
    m_body = body;
    m_body_main = body;
    m_color = color;
    m_radius = abs(m_body[0].y());
    m_mass = 1;
    Vx = 0;
    Vy = 0;
    Va = 0.05f;
    m_need_destroy = false;
}

void Convex::Draw(QPainter *p)  {
    p->setBrush(QBrush(m_color,Qt::SolidPattern));
    p->setPen(QPen(m_color,3,Qt::SolidLine));
    for(int i = 0; i < m_body.size() - 1; i++) {
        p->drawLine(pointXY(i),pointXY(i+1));
    }
    p->drawLine(pointXY(m_body.size()-1),pointXY(0));
}

void Convex::moveUp(float step)     {
    Vx -= (float)step*sinf(m_angle);
    Vy -= (float)step*cosf(m_angle);
}

void Convex::moveDown(float step)   {
    Vx += (float)step*sinf(m_angle);
    Vy += (float)step*cosf(m_angle);
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

bool Convex::collidePoint(QPoint p)    {
    bool collided = false;
    float x = p.x();
    float y = p.y();
    for(int i = 1; i < getBody().size();i++)   {
        float x1 = pointXY(i).x();
        float y1 = pointXY(i).y();
        float x2 = pointXY(i-1).x();
        float y2 = pointXY(i-1).y();
        if(((y1 <= y && y < y2) || (y2 <= y && y < y1)) &&
            (x > (x2 - x1)*(y - y1)/(y2 - y1) + x1))
        {
            collided = !collided;
        }
    }
    if(collided)    {
        if(x >= center().x() - radius() && x <= center().x() + radius() &&
           y >= center().y() - radius() && y <= center().y() + radius())    {

        } else {
            collided = false;
        }
    }
    return collided;
}

bool Convex::collideConvex(Convex c)    {
    float x1 = m_center.x();
    float y1 = m_center.y();
    float r1 = m_radius;
    float x2 = c.center().x();
    float y2 = c.center().y();
    float r2 = c.radius();
    float distance = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
    if(distance <= (r1 + r2))
        return true;
    return false;
}

bool Convex::collideConvex(QPoint p)    {
    float x1 = m_center.x();
    float y1 = m_center.y();
    float r1 = m_radius;
    float x2 = p.x();
    float y2 = p.y();
    float distance = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
    if(distance <= (r1))
        return true;
    return false;
}

void Convex::fly()  {
    m_center_prev = m_center;
    if(m_type != ConvexType::Player)  {
        m_center.setX(m_center.x() + Vx);
        m_center.setY(m_center.y() + Vy);
        rotate(Va);
        update();
    } else {
        m_center.setX(m_center.x() + Vx);
        m_center.setY(m_center.y() + Vy);
        update();
    }
}

void Convex::update()   {
    if(isnan(Vx)) Vx = 0;
    if(isnan(Vy)) Vy = 0;
    if(isnan(m_center.x())) m_center.setX(0);
    if(isnan(m_center.y())) m_center.setY(0);
    if(m_center.x() < m_game_field.x()) m_center.setX(m_center.x() + m_game_field.width());
    if(m_center.x() > m_game_field.width() - 1) m_center.setX(m_center.x() - m_game_field.width());
    if(m_center.y() < m_game_field.y()) m_center.setY(m_center.y() + m_game_field.height());
    if(m_center.y() > m_game_field.height() - 1) m_center.setY(m_center.y() - m_game_field.height());
}
