#include "Fairy.hpp"
#include "PicoLed.hpp"

namespace PicoLed {

    Fairy::Fairy(PicoLedController &controller, double fadeRate, double advance_time):
    Fade(controller, RGB(0, 0, 0), fadeRate)
    {
        
    }

    void Fairy::addBall(Color color, double length, double speed, double chaos) {
        uint numLeds = controller.getNumLeds();
        balls.push_back((struct FairyBall){
            .color = color, 
            .length = length, 
            .offset = (double)((double)random() / RAND_MAX) * (double) numLeds,  //Locate a fairy ball somewhere randomly
            .speed = speed,
            .chaos = chaos,
            .st = {
                    {
                        M_PI*0.9,                                            //Theta1 at fairly high angle, increases energy in the system
                        (double)((double)random() / RAND_MAX) * (double)M_PI ////Theta2 randomized
                    },  
                    {0, 0}                //Omega
                    },
            .ss = {{1.0, 1.0},     //Mass
                   {1.0, 1.0}      //Length
                }

        });
    }

    void Fairy::reset() {
        uint numLeds = controller.getNumLeds();
        // Iterate balls
        for (std::vector<FairyBall>::iterator it = balls.begin() ; it != balls.end(); ++it) {
            // Reset offsets and speeds
            it->offset = 0.0;
            it->speed = (double)numLeds * 0.5 - (double)(random() % 100) / 10.0;
        }
        // Reset update
        Fade::reset();
    }

    bool Fairy::update(uint32_t timeGone, uint32_t timeNow) {
        uint numLeds = controller.getNumLeds();
        // Fade tail
        fadePixels(timeNow);
        // Draw balls
        DrawMode prevMode = controller.getDrawMode();
        controller.setDrawMode(MODE_ADD);
        // Iterate balls
        for (std::vector<FairyBall>::iterator it = balls.begin() ; it != balls.end(); ++it) {
            // Draw body
            controller.fadeLine(it->color, it->offset, it->length, 1.0);
            
            it->st = dp::advance(it->st, it->ss, advance_time);
            
            dp::xy se = dp::second_edge(it->st, it->ss);
            //Y-coordinate of end of second arm
            //Normalized +- 1
            double x_norm = (se.first / (it->ss.length.first + it->ss.length.second));
            double x = (x_norm/2 + 0.5) * (double)numLeds; //Scale tot LEDs


            //Some randomization of direction, based on 'chaos'
            double chaos = it->chaos;            
            if (((double)random() / ((double) RAND_MAX+ 1) )*1000.0 < chaos) {
                it->speed *= -1.0;
            }

            //Speed is in pixel/second, timegone in ms
            it->offset += (double)timeGone * it->speed / 1000.0;
            //Reverse direction at ends of strip
            if (it->offset > (double)numLeds - 1.0) {
                it->speed *= -1.0;
                it->offset = (double)numLeds - 1.0;
            } else if (it->offset + it->length < 1.0) {
                it->speed *= -1.0;
                it->offset = 1.0 - it->length;
            }
            
            it->offset = x;
            
            
        }
        controller.setDrawMode(prevMode);
        return true;
    }

}