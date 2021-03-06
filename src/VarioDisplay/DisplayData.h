#pragma once

#include <VarioDisplay/VarioScreen/ScreenData.h>

ScreenData bootScreenData = {
    logo : {true, false, 0, 0, 128, 130, 94, 74, 99},
    txt1 : {true, false, 0, 120, 128, 21, 99},
    txt2 : {true, false, 0, 160, 128, 21, 99},
    txt3 : {true, false, 0, 180, 128, 21, 99},
    txt4 : {true, false, 0, 230, 128, 41, 99},
    txt5 : {true, false, 0, 270, 128, 41, 7},
    txt6 : {false, true, 20, 130, 100, 41, 99},
    txt7 : {false, true, 0, 270, 128, 41, 5},
    txt8 : {false, true, 20, 130, 100, 41, 99},
    alt : {false, true, 0, 170, 65, 40, 99},
    vario : {false, true, 64, 170, 64, 40, 99},
    toolbar : {false, true, 0, 210, 128, 51, 99},
    speed : {false, true, 0, 210, 128, 51, 99},
    bearing : {false, true, 0, 210, 128, 51, 99},
    time : {false, true, 0, 210, 128, 51, 99},
    flighttime : {false, true, 0, 210, 128, 51, 99}
};

ScreenData wifiScreenData = {
    logo : {true, false, 0, 0, 128, 240, 94, 74, 99},
    txt1 : {true, false, 0, 180, 128, 41, 99},
    txt2 : {true, false, 0, 220, 128, 41, 99},
    txt3 : {true, false, 0, 260, 128, 40, 99},
    txt4 : {false, true, 20, 130, 100, 41, 99},
    txt5 : {false, true, 20, 130, 100, 41, 99},
    txt6 : {false, true, 20, 130, 100, 41, 99},
    txt7 : {false, true, 20, 130, 100, 41, 99},
    txt8 : {false, true, 20, 130, 100, 41, 99},
    alt : {false, true, 0, 170, 65, 40, 99},
    vario : {false, true, 64, 170, 64, 40, 99},
    toolbar : {false, true, 0, 210, 128, 51, 99},
    speed : {false, true, 0, 210, 128, 51, 99},
    bearing : {false, true, 0, 210, 128, 51, 99},
    time : {false, true, 0, 210, 128, 51, 99},
    flighttime : {false, true, 0, 210, 128, 51, 99}
};

ScreenData calibrationScreenData = {
    logo : {true, true, 0, 0, 128, 296, 94, 74, 99},
    txt1 : {true, false, 0, 230, 128, 41, 99},
    txt2 : {false, true, 20, 130, 100, 41, 99},
    txt3 : {false, true, 20, 130, 100, 41, 99},
    txt4 : {false, true, 20, 130, 100, 41, 99},
    txt5 : {false, true, 20, 130, 100, 41, 99},
    txt6 : {false, true, 20, 130, 100, 41, 99},
    txt7 : {false, true, 20, 130, 100, 41, 99},
    txt8 : {false, true, 20, 130, 100, 41, 99},
    alt : {false, true, 0, 170, 65, 40, 99},
    vario : {false, true, 64, 170, 64, 40, 99},
    toolbar : {false, true, 0, 210, 128, 51, 99},
    speed : {false, true, 0, 210, 128, 51, 99},
    bearing : {false, true, 0, 210, 128, 51, 99},
    time : {false, true, 0, 210, 128, 51, 99},
    flighttime : {false, true, 0, 210, 128, 51, 99}
};

ScreenData vario1ScreenData = {
    logo : {false, true, 0, 0, 128, 91, 94, 74, 99},
    txt1 : {false, true, 0, 146, 128, 51, 99},
    txt2 : {true, true, 0, 196, 128, 51, 99},
    txt3 : {false, true, 0, 246, 128, 50, 99},
    txt4 : {false, true, 20, 130, 100, 41, 99},
    txt5 : {false, true, 20, 130, 100, 41, 99},
    txt6 : {false, true, 20, 130, 100, 41, 99},
    txt7 : {false, true, 20, 130, 100, 41, 99},
    txt8 : {false, true, 20, 130, 100, 41, 99},
    alt : {true, true, 0, 46, 128, 51, 99},
    vario : {true, true, 0, 96, 128, 51, 99},
    toolbar : {true, true, 0, 0, 128, 47, 99},
    speed : {false, true, 0, 210, 128, 51, 99},
    bearing : {true, true, 0, 246, 128, 50, 99},
    time : {true, true, 0, 146, 128, 51, 15},
    flighttime : {false, true, 0, 146, 128, 51, 14}
};

ScreenData vario2ScreenData = {
    logo : {false, true, 0, 246, 128, 50, 94, 74, 99},
    txt1 : {true, true, 0, 46, 128, 51, 99},
    txt2 : {true, true, 0, 96, 128, 51, 99},
    txt3 : {false, true, 20, 130, 100, 41, 99},
    txt4 : {false, true, 20, 130, 100, 41, 99},
    txt5 : {false, true, 20, 130, 100, 41, 99},
    txt6 : {false, true, 20, 130, 100, 41, 99},
    txt7 : {false, true, 20, 130, 100, 41, 99},
    txt8 : {false, true, 20, 130, 100, 41, 99},
    alt : {true, true, 0, 146, 128, 51, 99},
    vario : {true, true, 0, 196, 128, 51, 99},
    toolbar : {true, true, 0, 0, 128, 47, 99},
    speed : {true, true, 0, 246, 128, 50, 99},
    bearing : {false, true, 0, 210, 128, 51, 99},
    time : {false, true, 0, 210, 128, 51, 99},
    flighttime : {false, true, 0, 210, 128, 51, 99}
};

ScreenData vario3ScreenData = {
    logo : {true, true, 0, 0, 128, 296, 94, 74, 99},
    txt1 : {false, true, 0, 100, 128, 20, 99},
    txt2 : {false, true, 29, 210, 100, 21, 99},
    txt3 : {false, true, 20, 130, 100, 41, 99},
    txt4 : {false, true, 20, 130, 100, 41, 99},
    txt5 : {false, true, 20, 130, 100, 41, 99},
    txt6 : {false, true, 20, 130, 100, 41, 99},
    txt7 : {false, true, 20, 130, 100, 41, 99},
    txt8 : {false, true, 20, 130, 100, 41, 99},
    alt : {false, true, 10, 230, 100, 20, 99},
    vario : {false, true, 10, 190, 100, 20, 99},
    toolbar : {false, true, 0, 230, 128, 41, 99},
    speed : {false, true, 0, 210, 128, 51, 99},
    bearing : {false, true, 0, 210, 128, 51, 99},
    time : {false, true, 0, 210, 128, 51, 99},
    flighttime : {false, true, 0, 210, 128, 51, 99}
};

ScreenData soundScreenData = {
    logo : {true, true, 0, 0, 128, 296, 94, 74, 99},
    txt1 : {true, false, 0, 230, 128, 41, 99},
    txt2 : {false, true, 20, 130, 100, 41, 99},
    txt3 : {false, true, 20, 130, 100, 41, 99},
    txt4 : {false, true, 20, 130, 100, 41, 99},
    txt5 : {false, true, 20, 130, 100, 41, 99},
    txt6 : {false, true, 20, 130, 100, 41, 99},
    txt7 : {false, true, 20, 130, 100, 41, 99},
    txt8 : {false, true, 20, 130, 100, 41, 99},
    alt : {false, true, 0, 170, 65, 40, 99},
    vario : {false, true, 64, 170, 64, 40, 99},
    toolbar : {false, true, 0, 210, 128, 51, 99},
    speed : {false, true, 0, 210, 128, 51, 99},
    bearing : {false, true, 0, 210, 128, 51, 99},
    time : {false, true, 0, 210, 128, 51, 99},
    flighttime : {false, true, 0, 210, 128, 51, 99}
};

ScreenData statisticScreenData = {
    logo : {false, true, 0, 0, 128, 296, 94, 74, 99},
    txt1 : {true, false, 0, 0, 128, 21, 99},
    txt2 : {true, false, 0, 20, 128, 21, 99},
    txt3 : {true, false, 0, 40, 128, 21, 99},
    txt4 : {true, false, 0, 60, 128, 21, 99},
    txt5 : {true, false, 0, 80, 128, 21, 99},
    txt6 : {true, false, 0, 100, 128, 21, 99},
    txt7 : {true, false, 0, 120, 128, 21, 99},
    txt8 : {true, false, 0, 140, 128, 21, 99},
    alt : {false, true, 0, 170, 65, 40, 99},
    vario : {false, true, 64, 170, 64, 40, 99},
    toolbar : {false, true, 0, 210, 128, 51, 99},
    speed : {false, true, 0, 210, 128, 51, 99},
    bearing : {false, true, 0, 210, 128, 51, 99},
    time : {false, true, 0, 210, 128, 51, 99},
    flighttime : {false, true, 0, 210, 128, 51, 99}
};

ScreenData rebootScreenData = {
    logo : {true, true, 0, 0, 128, 296, 94, 74, 99},
    txt1 : {true, false, 0, 230, 128, 41, 99},
    txt2 : {false, true, 20, 130, 100, 41, 99},
    txt3 : {false, true, 20, 130, 100, 41, 99},
    txt4 : {false, true, 20, 130, 100, 41, 99},
    txt5 : {false, true, 20, 130, 100, 41, 99},
    txt6 : {false, true, 20, 130, 100, 41, 99},
    txt7 : {false, true, 20, 130, 100, 41, 99},
    txt8 : {false, true, 20, 130, 100, 41, 99},
    alt : {false, true, 0, 170, 65, 40, 99},
    vario : {false, true, 64, 170, 64, 40, 99},
    toolbar : {false, true, 0, 210, 128, 51, 99},
    speed : {false, true, 0, 210, 128, 51, 99},
    bearing : {false, true, 0, 210, 128, 51, 99},
    time : {false, true, 0, 210, 128, 51, 99},
    flighttime : {false, true, 0, 210, 128, 51, 99}
};
