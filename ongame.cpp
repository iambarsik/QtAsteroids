#include "widget.h"

void Widget::OnGame()   {

    OnTouch();
        // main app logic in timer period 15ms (edit in defines.h)

    if(game_play)   {
        AsteroidTimer++;
        if(AsteroidTimer >= game_spawn_timer)    {
            if(game_object.size() < 30)
                addAsteroid(ConvexType::AsteroidBig,0,randomBetween(0,SCREEN_TILE_HEIGHT*TILE_SIZE));
            AsteroidTimer = 0;
        }

        if(myKey[BUTTON_UP])    game_object[0].moveUp(SHIP_V);
        if(myKey[BUTTON_RIGHT]) game_object[0].rotate(-ROTATE_STEP);
        if(myKey[BUTTON_DOWN])  game_object[0].moveDown(SHIP_V);
        if(myKey[BUTTON_LEFT])  game_object[0].rotate(ROTATE_STEP);

        for(int i = 0; i < game_object.size(); i++) {
            game_object[i].fly();
            game_object[i].setID(i);
        }

        dev = false;
        for(int i = 0; i < game_object.size(); i++) {
            for(int o = 0; o < game_object.size(); o++) {
                if(game_object[i].id() == game_object[o].id())
                    continue;
                bool collided = game_object[i].collideConvex(game_object[o]);
                if(collided)    {

                    //=========== true collisions of balls, but unexpected bugs ===============
    /*
                    float distance = sqrtf((game_object[i].center().x() - game_object[o].center().x())*(game_object[i].center().x() - game_object[o].center().x()) +
                                           (game_object[i].center().y() - game_object[o].center().y())*(game_object[i].center().y() - game_object[o].center().y())
                                           );
                    float nx = (game_object[o].center().x() - game_object[i].center().x()) / distance;
                    float ny = (game_object[o].center().y() - game_object[i].center().y()) / distance;

                    float tx = -ny;
                    float ty = nx;

                    float dpt1 = game_object[i].vx()*tx + game_object[i].vy()*ty;
                    float dpt2 = game_object[o].vx()*tx + game_object[o].vy()*ty;

                    float dpn1 = game_object[i].vx()*nx + game_object[i].vy()*ny;
                    float dpn2 = game_object[o].vx()*nx + game_object[o].vy()*ny;

                    float m1 = (dpn1*(game_object[i].mass() - game_object[o].mass()) + game_object[o].mass()*dpn2)/(game_object[i].mass() + game_object[o].mass());
                    float m2 = (dpn2*(game_object[o].mass() - game_object[i].mass()) + game_object[i].mass()*dpn1)/(game_object[o].mass() + game_object[i].mass());

                    game_object[i].setV(tx*dpt1 + nx*m1,ty*dpt1 + ny*m1);
                    game_object[o].setV(tx*dpt2 + nx*m2,ty*dpt2 + ny*m2);
    */
                    //=========================================================++++++++++++++++

                        // swap velocity of objects on colliding
                    float vx1 = game_object[i].vx();
                    float vy1 = game_object[i].vy();
                    float vx2 = game_object[o].vx();
                    float vy2 = game_object[o].vy();
                    game_object[i].setV(vx2,vy2);
                    game_object[o].setV(vx1,vy1);
                    game_object[i].restore();
                    game_object[o].restore();

                        // move objectcs (if one of them is located inside another)
                    if(game_object[i].center().x() < game_object[o].center().x())   {
                        game_object[i].correctX(-COLLIDE_CORRECTOR);
                        game_object[o].correctX(COLLIDE_CORRECTOR);
                    } else {
                        game_object[i].correctX(COLLIDE_CORRECTOR);
                        game_object[o].correctX(-COLLIDE_CORRECTOR);
                    }
                    if(game_object[i].center().y() < game_object[o].center().y())   {
                        game_object[i].correctY(-COLLIDE_CORRECTOR);
                        game_object[o].correctY(COLLIDE_CORRECTOR);
                    } else {
                        game_object[i].correctY(COLLIDE_CORRECTOR);
                        game_object[o].correctY(-COLLIDE_CORRECTOR);
                    }

                    if(i == 0) {
                        sound_damage->play();
                        game_hp-=5;
                        if(game_hp <= 0)    {
                            gameover();
                        }
                        game_object[i].setColor(Qt::red);
                    } else {
                        game_object[o].reverseRotate();
                    }
                    dev = true;
                }
            }
        }

        for(int i = 0; i < game_bullet.size(); i++) {
            game_bullet[i].Update();
            for(int o = 0; o < game_object.size(); o++) {
                if(game_object[o].collideConvex(game_bullet[i].point())) {
                    if(o == 0)  {
                        game_hp-=5;
                        sound_damage->play();
                        if(game_hp <= 0)    {
                            gameover();
                        }
                        game_object[o].setColor(Qt::red);
                        myObject *delete_bullet = &game_bullet[i];
                        game_bullet.erase(delete_bullet);
                        break;
                    } else {
                        switch(game_object[o].type())   {
                            case ConvexType::AsteroidBig: {
                                addAsteroid(ConvexType::AsteroidMedium, game_object[o].center().x() - 5,game_object[o].center().y() - 5);
                                addAsteroid(ConvexType::AsteroidMedium, game_object[o].center().x(),game_object[o].center().y());
                                addAsteroid(ConvexType::AsteroidMedium, game_object[o].center().x() + 5,game_object[o].center().y() + 5);
                                game_score++;
                                int r = randomBetween(0,100);
                                if(r > 80)
                                    addAsteroid(ConvexType::AsteroidHP, game_object[o].center().x() + 5,game_object[o].center().y() + 5);
                            } break;
                            case ConvexType::AsteroidMedium: {
                                addAsteroid(ConvexType::AsteroidSmall, game_object[o].center().x() - 5,game_object[o].center().y() - 5);
                                addAsteroid(ConvexType::AsteroidSmall, game_object[o].center().x(),game_object[o].center().y());
                                //addAsteroid(ConvexType::AsteroidSmall, game_object[o].center().x() + 5,game_object[o].center().y() + 5);
                                game_score++;
                                int r = randomBetween(0,100);
                                if(r > 80)
                                    addAsteroid(ConvexType::AsteroidHP, game_object[o].center().x() + 5,game_object[o].center().y() + 5);
                            } break;
                            case ConvexType::AsteroidSmall: {
                                game_score++;
                                int r = randomBetween(0,100);
                                if(r > 80)
                                    addAsteroid(ConvexType::AsteroidHP, game_object[o].center().x() + 5,game_object[o].center().y() + 5);
                                if(r < 10)
                                    addAsteroid(ConvexType::AsteroidBomb, game_object[o].center().x() + 5,game_object[o].center().y() + 5);
                            } break;
                            case ConvexType::AsteroidHP: {
                                game_hp += 7;
                                if(game_hp > MAX_HP) game_hp = MAX_HP;
                            } break;
                            case ConvexType::AsteroidBomb: {
                                game_flash = true;
                                sound_bomb->play();
                                game_object[o].setRadius(TILE_SIZE*5);
                                for(int bb = 0; bb < game_object.size(); bb++) {
                                    if(game_object[o].collideConvex(game_object[bb].center()))    {
                                        if(bb == 0) {
                                            game_hp -= 15;
                                            if(game_hp <= 0)    {
                                                gameover();
                                                break;
                                            }
                                        } else {
                                            game_object[bb].destroy();
                                        }
                                    }
                                }
                                game_object[o].setRadius(TILE_SIZE*10);
                                for(int bb = 0; bb < game_object.size(); bb++) {
                                    if(game_object[o].collideConvex(game_object[bb].center()))    {
                                        if(game_object[o].center().x() < game_object[bb].center().x())
                                            game_object[bb].setVx(randomBetween(MOVE_STEP,MOVE_STEP*2));
                                        else
                                            game_object[bb].setVx(randomBetween(-MOVE_STEP*2,-MOVE_STEP));
                                        if(game_object[o].center().y() < game_object[bb].center().y())
                                            game_object[bb].setVy(randomBetween(MOVE_STEP,MOVE_STEP*2));
                                        else
                                            game_object[bb].setVy(randomBetween(-MOVE_STEP*2,-MOVE_STEP));
                                    }
                                }
                            } break;
                            default:

                            break;
                        }
                        game_object[o].destroy();
                        myObject *delete_bullet = &game_bullet[i];
                        game_bullet.erase(delete_bullet);
                        sound_explosive->play();
                        break;
                    }
                }
            }
        }
    }
    for(int i = 0; i < game_object.size(); i++) {
        if(game_object[i].isDestroyed())    {
            Convex *delete_object = &game_object[i];
            game_object.erase(delete_object);
        }
    }
    repaint();
}

void Widget::addAsteroid(enum ConvexType type, int X, int Y)    {
    QVector<QPoint> buff;
    QColor color;
    switch(type)    {
        case ConvexType::AsteroidBig: {
            int value = randomBetween(0,100);
            if(value < 33)  {
                buff.push_back(QPoint(0.01*TILE_SIZE, -1.79*TILE_SIZE ));
                buff.push_back(QPoint(0.65*TILE_SIZE, -1.47*TILE_SIZE ));
                buff.push_back(QPoint(1.01*TILE_SIZE, -1.17*TILE_SIZE ));
                buff.push_back(QPoint(1.22*TILE_SIZE, -0.81*TILE_SIZE ));
                buff.push_back(QPoint(0.88*TILE_SIZE, -0.55*TILE_SIZE ));
                buff.push_back(QPoint(1.07*TILE_SIZE, -0.23*TILE_SIZE ));
                buff.push_back(QPoint(1.22*TILE_SIZE, -0.14*TILE_SIZE ));
                buff.push_back(QPoint(1.39*TILE_SIZE, 0.26*TILE_SIZE  ));
                buff.push_back(QPoint(1.67*TILE_SIZE, 0.68*TILE_SIZE  ));
                buff.push_back(QPoint(1.72*TILE_SIZE, 1.07*TILE_SIZE  ));
                buff.push_back(QPoint(1.28*TILE_SIZE, 1.52*TILE_SIZE  ));
                buff.push_back(QPoint(0.94*TILE_SIZE, 1.79*TILE_SIZE  ));
                buff.push_back(QPoint(0.48*TILE_SIZE, 1.87*TILE_SIZE  ));
                buff.push_back(QPoint(-0.53*TILE_SIZE, 1.79*TILE_SIZE ));
                buff.push_back(QPoint(-1.46*TILE_SIZE, 1.55*TILE_SIZE ));
                buff.push_back(QPoint(-1.5*TILE_SIZE, 1.12*TILE_SIZE  ));
                buff.push_back(QPoint(-1.35*TILE_SIZE, 0.81*TILE_SIZE ));
                buff.push_back(QPoint(-1.3*TILE_SIZE, 0.34*TILE_SIZE  ));
                buff.push_back(QPoint(-1.63*TILE_SIZE, -0.11*TILE_SIZE));
                buff.push_back(QPoint(-1.79*TILE_SIZE, -0.55*TILE_SIZE));
                buff.push_back(QPoint(-1.6*TILE_SIZE, -1.18*TILE_SIZE ));
                buff.push_back(QPoint(-1.31*TILE_SIZE, -1.34*TILE_SIZE));
                buff.push_back(QPoint(-1*TILE_SIZE, -1.57*TILE_SIZE   ));
                buff.push_back(QPoint(-1.01*TILE_SIZE, -1.87*TILE_SIZE));
                buff.push_back(QPoint(-0.72*TILE_SIZE, -1.92*TILE_SIZE));
            } else if(value < 66){
                buff.push_back(QPoint(0*TILE_SIZE, -1.81*TILE_SIZE    ));
                buff.push_back(QPoint(0.66*TILE_SIZE, -1.59*TILE_SIZE ));
                buff.push_back(QPoint(1.16*TILE_SIZE, -1.55*TILE_SIZE ));
                buff.push_back(QPoint(1.6*TILE_SIZE, -1.43*TILE_SIZE  ));
                buff.push_back(QPoint(1.71*TILE_SIZE, -1.12*TILE_SIZE ));
                buff.push_back(QPoint(1.86*TILE_SIZE, -0.69*TILE_SIZE ));
                buff.push_back(QPoint(1.75*TILE_SIZE, -0.23*TILE_SIZE ));
                buff.push_back(QPoint(1.44*TILE_SIZE, 0.46*TILE_SIZE  ));
                buff.push_back(QPoint(0.9*TILE_SIZE, 1.15*TILE_SIZE   ));
                buff.push_back(QPoint(0.13*TILE_SIZE, 1.6*TILE_SIZE   ));
                buff.push_back(QPoint(-0.69*TILE_SIZE, 1.69*TILE_SIZE ));
                buff.push_back(QPoint(-1.56*TILE_SIZE, 1.69*TILE_SIZE ));
                buff.push_back(QPoint(-1.75*TILE_SIZE, 1.57*TILE_SIZE ));
                buff.push_back(QPoint(-1.79*TILE_SIZE, 1.08*TILE_SIZE ));
                buff.push_back(QPoint(-1.51*TILE_SIZE, 0.61*TILE_SIZE ));
                buff.push_back(QPoint(-1.2*TILE_SIZE, 0.08*TILE_SIZE  ));
                buff.push_back(QPoint(-1.27*TILE_SIZE, -0.52*TILE_SIZE));
                buff.push_back(QPoint(-1.65*TILE_SIZE, -1.08*TILE_SIZE));
                buff.push_back(QPoint(-1.8*TILE_SIZE, -1.52*TILE_SIZE ));
                buff.push_back(QPoint(-1.65*TILE_SIZE, -1.76*TILE_SIZE));
            } else {
                buff.push_back(QPoint(0*TILE_SIZE, -1.97*TILE_SIZE    ));
                buff.push_back(QPoint(0.65*TILE_SIZE, -1.7*TILE_SIZE  ));
                buff.push_back(QPoint(0.57*TILE_SIZE, -1.16*TILE_SIZE ));
                buff.push_back(QPoint(0.4*TILE_SIZE, -0.68*TILE_SIZE  ));
                buff.push_back(QPoint(0.78*TILE_SIZE, -0.07*TILE_SIZE ));
                buff.push_back(QPoint(1.51*TILE_SIZE, 0.01*TILE_SIZE  ));
                buff.push_back(QPoint(1.93*TILE_SIZE, 0.27*TILE_SIZE  ));
                buff.push_back(QPoint(1.9*TILE_SIZE, 0.83*TILE_SIZE   ));
                buff.push_back(QPoint(1.67*TILE_SIZE, 1.65*TILE_SIZE  ));
                buff.push_back(QPoint(1.36*TILE_SIZE, 1.91*TILE_SIZE  ));
                buff.push_back(QPoint(0.14*TILE_SIZE, 1.73*TILE_SIZE  ));
                buff.push_back(QPoint(-1.47*TILE_SIZE, 1.38*TILE_SIZE ));
                buff.push_back(QPoint(-1.61*TILE_SIZE, 0.75*TILE_SIZE ));
                buff.push_back(QPoint(-1.58*TILE_SIZE, 0.22*TILE_SIZE ));
                buff.push_back(QPoint(-1.87*TILE_SIZE, -0.27*TILE_SIZE));
                buff.push_back(QPoint(-1.66*TILE_SIZE, -1.08*TILE_SIZE));
                buff.push_back(QPoint(-1.38*TILE_SIZE, -1.48*TILE_SIZE));
            }
            color = Qt::white;
        } break;
        case ConvexType::AsteroidMedium:  {
            int value = randomBetween(0,100);
            if(value < 33)  {
                buff.push_back(QPoint(0.02*TILE_SIZE, -1.3*TILE_SIZE  ));
                buff.push_back(QPoint(0.17*TILE_SIZE, -1.17*TILE_SIZE ));
                buff.push_back(QPoint(0.38*TILE_SIZE, -1*TILE_SIZE    ));
                buff.push_back(QPoint(0.73*TILE_SIZE, -0.6*TILE_SIZE  ));
                buff.push_back(QPoint(1.67*TILE_SIZE, -0.42*TILE_SIZE ));
                buff.push_back(QPoint(1.78*TILE_SIZE, -0.26*TILE_SIZE ));
                buff.push_back(QPoint(1.55*TILE_SIZE, 0.04*TILE_SIZE  ));
                buff.push_back(QPoint(0.78*TILE_SIZE, 0.4*TILE_SIZE   ));
                buff.push_back(QPoint(0.16*TILE_SIZE, 0.5*TILE_SIZE   ));
                buff.push_back(QPoint(-0.08*TILE_SIZE, 0.23*TILE_SIZE ));
                buff.push_back(QPoint(-0.33*TILE_SIZE, 0.02*TILE_SIZE ));
                buff.push_back(QPoint(-1.08*TILE_SIZE, -0.28*TILE_SIZE));
                buff.push_back(QPoint(-1.12*TILE_SIZE, -0.54*TILE_SIZE));
                buff.push_back(QPoint(-0.92*TILE_SIZE, -0.6*TILE_SIZE ));
                buff.push_back(QPoint(-0.61*TILE_SIZE, -0.85*TILE_SIZE));
                buff.push_back(QPoint(-0.39*TILE_SIZE, -1.21*TILE_SIZE));
            } else if(value < 66)   {
                buff.push_back(QPoint(0.02*TILE_SIZE, -0.98*TILE_SIZE ));
                buff.push_back(QPoint(0.23*TILE_SIZE, -0.8*TILE_SIZE  ));
                buff.push_back(QPoint(0.25*TILE_SIZE, -0.53*TILE_SIZE ));
                buff.push_back(QPoint(0.63*TILE_SIZE, -0.57*TILE_SIZE ));
                buff.push_back(QPoint(0.88*TILE_SIZE, -0.68*TILE_SIZE ));
                buff.push_back(QPoint(0.86*TILE_SIZE, -0.45*TILE_SIZE ));
                buff.push_back(QPoint(0.78*TILE_SIZE, -0.2*TILE_SIZE  ));
                buff.push_back(QPoint(0.91*TILE_SIZE, 0.08*TILE_SIZE  ));
                buff.push_back(QPoint(0.94*TILE_SIZE, 0.36*TILE_SIZE  ));
                buff.push_back(QPoint(0.46*TILE_SIZE, 0.79*TILE_SIZE  ));
                buff.push_back(QPoint(-0.07*TILE_SIZE, 0.92*TILE_SIZE ));
                buff.push_back(QPoint(-0.02*TILE_SIZE, 0.65*TILE_SIZE ));
                buff.push_back(QPoint(-0.27*TILE_SIZE, 0.46*TILE_SIZE ));
                buff.push_back(QPoint(-0.51*TILE_SIZE, 0.13*TILE_SIZE ));
                buff.push_back(QPoint(-0.7*TILE_SIZE, -0.23*TILE_SIZE ));
                buff.push_back(QPoint(-0.51*TILE_SIZE, -0.5*TILE_SIZE ));
                buff.push_back(QPoint(-0.23*TILE_SIZE, -0.77*TILE_SIZE));
            } else {
                buff.push_back(QPoint(0*TILE_SIZE, -1.01*TILE_SIZE    ));
                buff.push_back(QPoint(0.35*TILE_SIZE, -0.84*TILE_SIZE ));
                buff.push_back(QPoint(0.61*TILE_SIZE, -0.69*TILE_SIZE ));
                buff.push_back(QPoint(0.58*TILE_SIZE, -0.57*TILE_SIZE ));
                buff.push_back(QPoint(0.61*TILE_SIZE, -0.41*TILE_SIZE ));
                buff.push_back(QPoint(0.83*TILE_SIZE, -0.28*TILE_SIZE ));
                buff.push_back(QPoint(1.33*TILE_SIZE, 0.1*TILE_SIZE   ));
                buff.push_back(QPoint(1.2*TILE_SIZE, 0.32*TILE_SIZE   ));
                buff.push_back(QPoint(1.03*TILE_SIZE, 0.39*TILE_SIZE  ));
                buff.push_back(QPoint(0.89*TILE_SIZE, 0.64*TILE_SIZE  ));
                buff.push_back(QPoint(0.76*TILE_SIZE, 0.93*TILE_SIZE  ));
                buff.push_back(QPoint(0.41*TILE_SIZE, 0.98*TILE_SIZE  ));
                buff.push_back(QPoint(0.34*TILE_SIZE, 0.84*TILE_SIZE  ));
                buff.push_back(QPoint(0.11*TILE_SIZE, 0.71*TILE_SIZE  ));
                buff.push_back(QPoint(-0.3*TILE_SIZE, 0.72*TILE_SIZE  ));
                buff.push_back(QPoint(-0.68*TILE_SIZE, 0.61*TILE_SIZE ));
                buff.push_back(QPoint(-0.63*TILE_SIZE, 0.33*TILE_SIZE ));
                buff.push_back(QPoint(-0.3*TILE_SIZE, 0.22*TILE_SIZE  ));
                buff.push_back(QPoint(-0.28*TILE_SIZE, -0.14*TILE_SIZE));
                buff.push_back(QPoint(-0.33*TILE_SIZE, -0.34*TILE_SIZE));
                buff.push_back(QPoint(-0.29*TILE_SIZE, -0.56*TILE_SIZE));
            }
            color = Qt::white;
        } break;
        case ConvexType::AsteroidSmall:   {
            int value = randomBetween(0,100);
            if(value < 33)  {
                buff.push_back(QPoint(-0.01*TILE_SIZE, -0.48*TILE_SIZE));
                buff.push_back(QPoint(0.23*TILE_SIZE, -0.42*TILE_SIZE ));
                buff.push_back(QPoint(0.69*TILE_SIZE, -0.45*TILE_SIZE ));
                buff.push_back(QPoint(0.66*TILE_SIZE, -0.26*TILE_SIZE ));
                buff.push_back(QPoint(0.41*TILE_SIZE, 0.08*TILE_SIZE  ));
                buff.push_back(QPoint(0.1*TILE_SIZE, 0.36*TILE_SIZE   ));
                buff.push_back(QPoint(-0.17*TILE_SIZE, 0.51*TILE_SIZE ));
                buff.push_back(QPoint(-0.46*TILE_SIZE, 0.46*TILE_SIZE ));
                buff.push_back(QPoint(-0.32*TILE_SIZE, 0.28*TILE_SIZE ));
                buff.push_back(QPoint(-0.23*TILE_SIZE, -0.08*TILE_SIZE));
                buff.push_back(QPoint(-0.27*TILE_SIZE, -0.28*TILE_SIZE));
            } else if(value < 66) {
                buff.push_back(QPoint(-0.2*TILE_SIZE, -0.46*TILE_SIZE));
                buff.push_back(QPoint(0.25*TILE_SIZE, -0.19*TILE_SIZE));
                buff.push_back(QPoint(0.34*TILE_SIZE, 0.08*TILE_SIZE ));
                buff.push_back(QPoint(0.48*TILE_SIZE, 0.25*TILE_SIZE ));
                buff.push_back(QPoint(0.46*TILE_SIZE, 0.42*TILE_SIZE ));
                buff.push_back(QPoint(0.08*TILE_SIZE, 0.55*TILE_SIZE ));
                buff.push_back(QPoint(-0.39*TILE_SIZE, 0.6*TILE_SIZE ));
                buff.push_back(QPoint(-0.56*TILE_SIZE, 0.4*TILE_SIZE ));
                buff.push_back(QPoint(-0.45*TILE_SIZE, 0.11*TILE_SIZE));
            } else {
                buff.push_back(QPoint(-0.01*TILE_SIZE, -0.65*TILE_SIZE));
                buff.push_back(QPoint(0.32*TILE_SIZE, -0.57*TILE_SIZE ));
                buff.push_back(QPoint(0.4*TILE_SIZE, -0.45*TILE_SIZE  ));
                buff.push_back(QPoint(0.68*TILE_SIZE, -0.34*TILE_SIZE ));
                buff.push_back(QPoint(0.84*TILE_SIZE, -0.05*TILE_SIZE ));
                buff.push_back(QPoint(0.79*TILE_SIZE, 0.1*TILE_SIZE   ));
                buff.push_back(QPoint(0.53*TILE_SIZE, 0.22*TILE_SIZE  ));
                buff.push_back(QPoint(0.13*TILE_SIZE, 0.72*TILE_SIZE  ));
                buff.push_back(QPoint(-0.02*TILE_SIZE, 0.93*TILE_SIZE ));
                buff.push_back(QPoint(-0.11*TILE_SIZE, 0.9*TILE_SIZE  ));
                buff.push_back(QPoint(-0.17*TILE_SIZE, 0.64*TILE_SIZE ));
                buff.push_back(QPoint(-0.38*TILE_SIZE, 0.27*TILE_SIZE ));
                buff.push_back(QPoint(-0.36*TILE_SIZE, -0.07*TILE_SIZE));
                buff.push_back(QPoint(-0.29*TILE_SIZE, -0.34*TILE_SIZE));
                buff.push_back(QPoint(-0.14*TILE_SIZE, -0.66*TILE_SIZE));
            }
            color = Qt::white;
        } break;
        case ConvexType::AsteroidHP:   {
            buff.push_back(QPoint(0*TILE_SIZE, -0.67*TILE_SIZE    ));
            buff.push_back(QPoint(0.23*TILE_SIZE, -0.3*TILE_SIZE  ));
            buff.push_back(QPoint(0.69*TILE_SIZE, -0.33*TILE_SIZE ));
            buff.push_back(QPoint(0.33*TILE_SIZE, -0.01*TILE_SIZE ));
            buff.push_back(QPoint(0.65*TILE_SIZE, 0.38*TILE_SIZE  ));
            buff.push_back(QPoint(0.27*TILE_SIZE, 0.38*TILE_SIZE  ));
            buff.push_back(QPoint(0*TILE_SIZE, 0.75*TILE_SIZE     ));
            buff.push_back(QPoint(-0.16*TILE_SIZE, 0.4*TILE_SIZE  ));
            buff.push_back(QPoint(-0.6*TILE_SIZE, 0.39*TILE_SIZE  ));
            buff.push_back(QPoint(-0.4*TILE_SIZE, 0*TILE_SIZE     ));
            buff.push_back(QPoint(-0.63*TILE_SIZE, -0.32*TILE_SIZE));
            buff.push_back(QPoint(-0.23*TILE_SIZE, -0.32*TILE_SIZE));
            color = Qt::green;
        } break;
        case ConvexType::AsteroidBomb:   {
            buff.push_back(QPoint(0*TILE_SIZE, -0.52*TILE_SIZE    ));
            buff.push_back(QPoint(0.2*TILE_SIZE, -0.48*TILE_SIZE  ));
            buff.push_back(QPoint(0.4*TILE_SIZE, -0.4*TILE_SIZE   ));
            buff.push_back(QPoint(0.59*TILE_SIZE, -0.2*TILE_SIZE  ));
            buff.push_back(QPoint(0.65*TILE_SIZE, 0.01*TILE_SIZE  ));
            buff.push_back(QPoint(0.66*TILE_SIZE, 0.27*TILE_SIZE  ));
            buff.push_back(QPoint(0.55*TILE_SIZE, 0.42*TILE_SIZE  ));
            buff.push_back(QPoint(0.34*TILE_SIZE, 0.57*TILE_SIZE  ));
            buff.push_back(QPoint(0.14*TILE_SIZE, 0.69*TILE_SIZE  ));
            buff.push_back(QPoint(-0.08*TILE_SIZE, 0.69*TILE_SIZE ));
            buff.push_back(QPoint(-0.3*TILE_SIZE, 0.65*TILE_SIZE  ));
            buff.push_back(QPoint(-0.47*TILE_SIZE, 0.47*TILE_SIZE ));
            buff.push_back(QPoint(-0.58*TILE_SIZE, 0.36*TILE_SIZE ));
            buff.push_back(QPoint(-0.63*TILE_SIZE, 0.23*TILE_SIZE ));
            buff.push_back(QPoint(-0.66*TILE_SIZE, 0.02*TILE_SIZE ));
            buff.push_back(QPoint(-0.64*TILE_SIZE, -0.07*TILE_SIZE));
            buff.push_back(QPoint(-0.81*TILE_SIZE, -0.3*TILE_SIZE ));
            buff.push_back(QPoint(-0.73*TILE_SIZE, -0.42*TILE_SIZE));
            buff.push_back(QPoint(-0.88*TILE_SIZE, -0.68*TILE_SIZE));
            buff.push_back(QPoint(-1.1*TILE_SIZE, -0.68*TILE_SIZE ));
            buff.push_back(QPoint(-1.1*TILE_SIZE, -0.71*TILE_SIZE ));
            buff.push_back(QPoint(-0.84*TILE_SIZE, -0.73*TILE_SIZE));
            buff.push_back(QPoint(-0.7*TILE_SIZE, -0.48*TILE_SIZE ));
            buff.push_back(QPoint(-0.58*TILE_SIZE, -0.57*TILE_SIZE));
            buff.push_back(QPoint(-0.33*TILE_SIZE, -0.44*TILE_SIZE));
            buff.push_back(QPoint(-0.22*TILE_SIZE, -0.51*TILE_SIZE));
            color = Qt::red;
        } break;
    }
    Convex asteroid(QPoint(X, Y), buff, color);
    asteroid.setType(type);
    asteroid.setV(randomBetween(-MOVE_STEP,MOVE_STEP),randomBetween(-MOVE_STEP,MOVE_STEP));
    asteroid.setGameField(QRect(0,0,SCREEN_TILE_WIDTH*TILE_SIZE,SCREEN_TILE_HEIGHT*TILE_SIZE));
    game_object.push_back(asteroid);
}

void Widget::addBullet(int num_player)  {
    myObject bullet;
    bullet.setXY(game_object[num_player].pointXY(0).x() -10.0f*sinf(game_object[num_player].angle()),
            game_object[num_player].pointXY(0).y()-10.0f*cosf(game_object[num_player].angle()));
    bullet.setField(SCREEN_TILE_WIDTH*TILE_SIZE,SCREEN_TILE_HEIGHT*TILE_SIZE);
    float vx = -10.0f*sinf(game_object[num_player].angle());
    float vy = -10.0f*cosf(game_object[num_player].angle());
    bullet.setV(vx,vy);
    game_bullet.push_back(bullet);
    sound_shoot->play();
}

bool Widget::collideAsteroidAndBullet(myObject bullet, Convex Asteroid) {
    bool collided = false;
    float x = bullet.x();
    float y = bullet.y();
    for(int i = 1; i < Asteroid.getBody().size();i++)   {
        float x1 = Asteroid.pointXY(i).x();
        float y1 = Asteroid.pointXY(i).y();
        float x2 = Asteroid.pointXY(i-1).x();
        float y2 = Asteroid.pointXY(i-1).y();
        if(((y1 <= y && y < y2) || (y2 <= y && y < y1)) &&
            (x > (x2 - x1)*(y - y1)/(y2 - y1) + x1))
        {
            collided = !collided;
        }
    }
    if(collided)    {
        if(x >= Asteroid.center().x() - Asteroid.radius() && x <= Asteroid.center().x() + Asteroid.radius() &&
           y >= Asteroid.center().y() - Asteroid.radius() && y <= Asteroid.center().y() + Asteroid.radius())    {
        } else {
            collided = false;
        }
    }
    return collided;
}

void Widget::CheckCollide() {

}

void Widget::setButton(int Code, bool enable)   {
    for(int i = 0; i < BUTTON_COUNT; i++)   {
        if(TouchKeys[i]->Code == Code)  {
            TouchKeys[i]->Enable = enable;
        }
    }
}

void Widget::gameover() {
    game_screen = GameScreens::DeadMenu;
    setButton(Qt::Key_W,true);
    setButton(Qt::Key_D,false);
    setButton(Qt::Key_S,true);
    setButton(Qt::Key_A,false);
    setButton(Qt::Key_Escape,false);
    setButton(Qt::Key_U,false);
    setButton(Qt::Key_J,true);
    game_play = false;
    switch(game_spawn_timer)    {
        case SPAWN_TIMER_EASY:
            if(game_results[0] < game_score) game_results[0] = game_score;
        break;
        case SPAWN_TIMER_MEDIUM:
            if(game_results[1] < game_score) game_results[1] = game_score;
        break;
        case SPAWN_TIMER_HARD:
            if(game_results[2] < game_score) game_results[2] = game_score;
        break;
        case SPAWN_TIMER_VERY:
            if(game_results[3] < game_score) game_results[3] = game_score;
        break;
        case SPAWN_TIMER_SUICIDE:
            if(game_results[4] < game_score) game_results[4] = game_score;
        break;
    }
    saveresults();
}

void Widget::saveresults() {
#ifdef Q_OS_ANDROID
    const QVector<QString> permissions({
                                        "android.permission.WRITE_EXTERNAL_STORAGE",
                                        "android.permission.READ_EXTERNAL_STORAGE"});
    for(const QString &permission : permissions){
        auto result = QtAndroid::checkPermission(permission);
        if(result == QtAndroid::PermissionResult::Denied){
            auto resultHash = QtAndroid::requestPermissionsSync(QStringList({permission}));
            if(resultHash[permission] == QtAndroid::PermissionResult::Denied)
                return;
        }
    }
    QStringList paths = QStandardPaths::standardLocations(QStandardPaths::DownloadLocation);
    for(int i = 0; i < paths.size(); i++)    {
        QString path = paths[i];
        QFile file(path + "/game_data.txt");
        file.open(QIODevice::WriteOnly);
        QTextStream out(&file);
        out << game_results[0] << "\n";
        out << game_results[1] << "\n";
        out << game_results[2] << "\n";
        out << game_results[3] << "\n";
        out << game_results[4] << "\n";
        file.close();
    }
    return;
#endif
#ifdef Q_OS_WIN
    QFile file("game_data.txt");
    file.open(QIODevice::ReadWrite);
    QTextStream out(&file);
    out << game_results[0] << "\n";
    out << game_results[1] << "\n";
    out << game_results[2] << "\n";
    out << game_results[3] << "\n";
    out << game_results[4] << "\n";
    file.close();
#endif

}

void Widget::loadresults()  {
#ifdef Q_OS_ANDROID
    const QVector<QString> permissions({
                                        "android.permission.WRITE_EXTERNAL_STORAGE",
                                        "android.permission.READ_EXTERNAL_STORAGE"});
    for(const QString &permission : permissions){
        auto result = QtAndroid::checkPermission(permission);
        if(result == QtAndroid::PermissionResult::Denied){
            auto resultHash = QtAndroid::requestPermissionsSync(QStringList({permission}));
            if(resultHash[permission] == QtAndroid::PermissionResult::Denied)
                return;
        }
    }
    QStringList paths = QStandardPaths::standardLocations(QStandardPaths::DownloadLocation);
    for(int i = 0; i < paths.size(); i++)    {
        QString path = paths[i];
        QFile file(path + "/game_data.txt");
        file.open(QIODevice::ReadOnly);
        QTextStream in(&file);
        int r = 0;
        while(!in.atEnd()) {
            QString line = in.readLine();
            game_results[r] = line.toInt();
            r++;
            if(r >= 5) break;
        }
        file.close();
    }
    return;
#endif
#ifdef Q_OS_WIN
    QFile file("game_data.txt");
    if(file.exists())   {
        file.open(QIODevice::ReadWrite);
        QTextStream in(&file);
        int i = 0;
        while(!in.atEnd()) {
            QString line = in.readLine();
            game_results[i] = line.toInt();
            i++;
            if(i >= 5) break;
        }
    }
    file.close();
#endif
}
