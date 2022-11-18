#include "widget.h"

void Widget::OnGame()   {

    OnTouch();
        // main app logic in timer period 15ms (edit in defines.h)

    /*
    if(myKey[BUTTON_UP])    game_ship->moveUp(MOVE_STEP);
    if(myKey[BUTTON_RIGHT]) game_ship->rotate(-ROTATE_STEP);
    if(myKey[BUTTON_DOWN])    game_ship->moveDown(MOVE_STEP);
    if(myKey[BUTTON_LEFT])    game_ship->rotate(ROTATE_STEP);
*/
    if(myKey[BUTTON_UP])    game_object[0].moveUp(MOVE_STEP);
    if(myKey[BUTTON_RIGHT]) game_object[0].rotate(-ROTATE_STEP);
    if(myKey[BUTTON_DOWN])  game_object[0].moveDown(MOVE_STEP);
    if(myKey[BUTTON_LEFT])  game_object[0].rotate(ROTATE_STEP);

    for(int i = 0; i < game_bullet.size(); i++) {
        game_bullet[i].Update();
    }



    repaint();
}

void Widget::addObject()
{
    QVector<QPoint> buff;
    buff.push_back(QPoint(0,-TILE_SIZE));
    buff.push_back(QPoint(-TILE_SIZE/2,TILE_SIZE/2));
    buff.push_back(QPoint(TILE_SIZE/2,TILE_SIZE/2));

    Convex game_ship(QPoint(game_object[0].center().x(), game_object[0].center().y()), buff, Qt::yellow);
    game_ship.setGameField(QRect(0,0,SCREEN_TILE_WIDTH*TILE_SIZE,SCREEN_TILE_HEIGHT*TILE_SIZE));
    //game_ship = new Convex(QPoint(Resolution.x()/2, Resolution.y()/2), buff, Qt::yellow);
    //game_ship->setGameField(QRect(0,0,SCREEN_TILE_WIDTH*TILE_SIZE,SCREEN_TILE_HEIGHT*TILE_SIZE));

    game_object.push_back(game_ship);
}
