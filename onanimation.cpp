#include "widget.h"

void Widget::OnAnimation()  {

    // main app animaions in timer period 100ms (edit in defines.h)
    if(game_play)   {
        game_object[0].setColor(Qt::yellow);
    }
    if(game_flash) game_flash = false;
}
