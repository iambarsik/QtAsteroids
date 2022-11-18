#include "widget.h"

void Widget::SetStartValues()   {
    Settings.bDevMode = true;
    Settings.bEnglish = true;

    QVector<QPoint> buff;
    buff.push_back(QPoint(0,-TILE_SIZE));
    buff.push_back(QPoint(-TILE_SIZE/2,TILE_SIZE/2));
    buff.push_back(QPoint(TILE_SIZE/2,TILE_SIZE/2));

    Convex game_ship(QPoint(Resolution.x()/2, Resolution.y()/2), buff, Qt::yellow);
    game_ship.setGameField(QRect(0,0,SCREEN_TILE_WIDTH*TILE_SIZE,SCREEN_TILE_HEIGHT*TILE_SIZE));
    //game_ship = new Convex(QPoint(Resolution.x()/2, Resolution.y()/2), buff, Qt::yellow);
    //game_ship->setGameField(QRect(0,0,SCREEN_TILE_WIDTH*TILE_SIZE,SCREEN_TILE_HEIGHT*TILE_SIZE));

    game_object.push_back(game_ship);
}



bool Widget::event(QEvent *event)   {
    switch (event->type())  {
        case QEvent::TouchBegin:
        case QEvent::TouchUpdate:
        case QEvent::TouchEnd:  {
            QTouchEvent* pte = static_cast<QTouchEvent*>(event);
            listKeys = pte->touchPoints();
            update();
        } break;
        default: return QWidget::event(event);
    }
    return true;
}

void Widget::OnKeys(int Key, bool press)    {

    switch(Key) {
        case Qt::Key_Up:
        case Qt::Key_W: {
            myKey[BUTTON_UP] = press;
        } break;
        case Qt::Key_Right:
        case Qt::Key_D: {
            myKey[BUTTON_RIGHT] = press;
        } break;
        case Qt::Key_Down:
        case Qt::Key_S: {
            myKey[BUTTON_DOWN] = press;
        } break;
        case Qt::Key_Left:
        case Qt::Key_A: {
            myKey[BUTTON_LEFT] = press;
        } break;
        case Qt::Key_Space: {
            //if(press) addObject();
            if(press)   {
                myObject bullet;
                bullet.setXY(game_object[0].center().x(),game_object[0].center().y());
                bullet.setField(SCREEN_TILE_WIDTH*TILE_SIZE,SCREEN_TILE_HEIGHT*TILE_SIZE);
                bullet.setV(10,10);
                game_bullet.push_back(bullet);
            }
        } break;
        case Qt::Key_Return:
        case Qt::Key_J: {
            myKey[BUTTON_ACT1] = press;
        } break;
        case Qt::Key_K: {
            myKey[BUTTON_ACT2] = press;
        } break;
        case Qt::Key_U: {
            myKey[BUTTON_ACT3] = press;
        } break;
        case Qt::Key_I: {
            myKey[BUTTON_ACT4] = press;
        } break;
        case Qt::Key_Q: {
            myKey[BUTTON_ACT5] = press;
        } break;
        case Qt::Key_E: {
            myKey[BUTTON_ACT6] = press;
        } break;
        case Qt::Key_H: {
            myKey[BUTTON_ACT7] = press;
        } break;
        case Qt::Key_Y: {
            myKey[BUTTON_ACT8] = press;
        } break;
        case Qt::Key_P: {

        } break;
        case Qt::Key_L: {

        } break;
        case Qt::Key_Escape: {
           exit(0);
        } break;
    }
}

void Widget::OnTouch()  {
    for(int i = 0; i < BUTTON_COUNT; i++) {
        TouchKeys[i]->Value = false;
    }
    int tpx;
    int tpy;
    float tpdX;
    float tpdY;
    foreach (QTouchEvent::TouchPoint tp, listKeys)  {
        if(bRotateScreen == false)  {
            tpx = tp.pos().x();
            tpy = tp.pos().y();
            tpdX = dX;
            tpdY = dY;
        } else {
            tpx = tp.pos().y();
            tpy = (this->width() - tp.pos().x());
            tpdX = dY;
            tpdY = dX;
        }
        switch (tp.state()) {
            case Qt::TouchPointReleased:
                for(int i = 0; i < BUTTON_COUNT; i++) {
                    if(
                      (tpx > TouchKeys[i]->Geometry.x()*tpdX && tpx < (TouchKeys[i]->Geometry.x() + TouchKeys[i]->Geometry.width())*tpdX)&&
                      (tpy > TouchKeys[i]->Geometry.y()*tpdY && tpy < (TouchKeys[i]->Geometry.y() + TouchKeys[i]->Geometry.height())*tpdY)
                      ) {
                            TouchKeys[i]->Value = false;
                    }
                }
            break;
            case Qt::TouchPointStationary:
            case Qt::TouchPointPressed:
            case Qt::TouchPointMoved:
                for(int i = 0; i < BUTTON_COUNT; i++) {
                    if(
                      (tpx > TouchKeys[i]->Geometry.x()*tpdX && tpx < (TouchKeys[i]->Geometry.x() + TouchKeys[i]->Geometry.width())*tpdX)&&
                      (tpy > TouchKeys[i]->Geometry.y()*tpdY && tpy < (TouchKeys[i]->Geometry.y() + TouchKeys[i]->Geometry.height())*tpdY)
                      ) {
                            TouchKeys[i]->Value = true;
                    }
                }
            break;
        }
    }
    for(int i = 0; i < BUTTON_COUNT; i++) {
        if(TouchKeys[i]->oldValue != TouchKeys[i]->Value)    {
            OnKeys(TouchKeys[i]->Code,TouchKeys[i]->Value);
            TouchKeys[i]->oldValue = TouchKeys[i]->Value;
        }
    }
}

void Widget::OnDrawText(int x, int y, int size, QString text_eng, QString text_rus, QPainter *pnt)  {
    QString text;
    if(Settings.bEnglish == true)   {
        text = text_eng;
    } else {
        text = text_rus;
    }
    switch(size)    {
        case FONT_SIZE_MICRO:
            for(int cnt = 0; cnt < text.size(); cnt++)    {
                for(int i = 0; i < sSymbols.size(); i++)    {
                    if(text.at(cnt) == sSymbols.at(i))    {
                        pnt->drawImage(x + (cnt)*imgSymbol_micro[i].width(),y,imgSymbol_micro[i]);
                    }
                }
            }
        break;
        case FONT_SIZE_SMALL:
            for(int cnt = 0; cnt < text.size(); cnt++)    {
                for(int i = 0; i < sSymbols.size(); i++)    {
                    if(text.at(cnt) == sSymbols.at(i))    {
                        pnt->drawImage(x + (cnt)*imgSymbol_small[i].width(),y,imgSymbol_small[i]);
                    }
                }
            }
        break;
        case FONT_SIZE_MEDIUM:
            for(int cnt = 0; cnt < text.size(); cnt++)    {
                for(int i = 0; i < sSymbols.size(); i++)    {
                    if(text.at(cnt) == sSymbols.at(i))    {
                        pnt->drawImage(x + (cnt)*imgSymbol_medium[i].width(),y,imgSymbol_medium[i]);
                    }
                }
            }
        break;
        case FONT_SIZE_LARGE:
            for(int cnt = 0; cnt < text.size(); cnt++)    {
                for(int i = 0; i < sSymbols.size(); i++)    {
                    if(text.at(cnt) == sSymbols.at(i))    {
                        pnt->drawImage(x + (cnt)*imgSymbol_big[i].width(),y,imgSymbol_big[i]);
                    }
                }
            }
        break;
    }
}

void Widget::setColor(QColor brush, QColor pen, Qt::BrushStyle style, QPainter *qScr) {
    qScr->setBrush(QBrush(brush, style));
    qScr->setPen(QPen(pen));
}

void Widget::keyPressEvent(QKeyEvent *event)    {
    if(!event->isAutoRepeat())
        OnKeys(event->key(),true);
    QWidget::keyPressEvent(event);
}

void Widget::keyReleaseEvent(QKeyEvent *event)  {
    if(!event->isAutoRepeat())
        OnKeys(event->key(),false);
    QWidget::keyReleaseEvent(event);
}
