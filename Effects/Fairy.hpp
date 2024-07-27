#ifndef PICOLEDEFFECT_FAIRY_H
#define PICOLEDEFFECT_FAIRY_H

#include <vector>
#include "Effects/Fade.hpp"
#include "Effects/double_pendulum.hpp"

using std::vector;

namespace PicoLed {

class Fairy: public Fade {

    public:
        Fairy(PicoLedController &controller, double fadeRate, double advance_time);

        //Fairies are oddballs...
        void addBall(Color color, double length, double speed, double chaos);
        void reset();
    protected:
        struct FairyBall {
            Color color;
            double length;
            double offset;
            double speed;
            double chaos; //Amount of 'chaos' the fairy has..erratic behaviour
            dp::state st;
            dp::system ss;
        };
        double advance_time;
        vector<FairyBall> balls;
        

        bool update(uint32_t timeGone, uint32_t timeNow);

};

}

#endif