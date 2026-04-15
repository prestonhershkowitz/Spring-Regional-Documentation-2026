void move(int vel0, int vel1, int vel2, int vel3);
void forward(int dist, int speed);
void backward(int dist, int speed);
void left(int dist, int speed);
void right(int dist, int speed);
void turn_right(int dist, int speed);
void turn_left(int dist, int speed);
void diag_r_forw(int dist, int speed);
void diag_l_forw(int dist, int speed);
void diag_l_back(int dist, int speed);
void diag_r_back(int dist, int speed);
void forsquare ();
void backsquare ();
void follow (int distance);
void s (int port, int pos);
void upslow (int arm, int initial, int final, int speed);
void downslow (int arm, int initial, int final, int speed);    



/*
Contributers: Taha, Liam, Nadia
Last Updated: 6 May 2025
To understand mecanum wheels, you can look at the wikipedia page for mecanum wheels
*/

#include <kipr/wombat.h>
#include <stdlib.h>

// variables for motors
const int back_r = 3;
const int front_r = 2;
const int front_l = 1;
const int back_l = 0;

// "up" means closer to the screen

// variables for sensors
const int lightSensor_l = 1;
const int lightSensor_r = 0;
const int buttonSensor_l = 0;
const int buttonSensor_r = 1;

// misc variables
const int grey = 973; // 973 = the year Cairo was founded

void move(int vel0, int vel1, int vel2, int vel3){
mav(back_r, vel0);
    mav(front_r, vel1);
    mav(front_l, vel2);
mav(back_l, vel3);
}

void forward(int dist, int speed){
    cmpc(back_r);
    while (gmpc(back_r) < dist){
        move(speed, speed, speed, speed);
    }
}

void backward(int dist, int speed){
    cmpc(back_r);
    while (abs(gmpc(back_r)) < dist){
        move(-speed, -speed, -speed, -speed);
    }
}

void right(int dist, int speed){
    cmpc(back_r);
    while (abs(gmpc(back_r)) < dist){
        move(speed, -speed, speed, -speed);
    }
}

void left(int dist, int speed){
    cmpc(back_r);
    while (abs(gmpc(back_r)) < dist){
        move(-speed, speed, -speed, speed);
    }
}

void turn_right(int dist, int speed){
    cmpc(back_r);
while (abs(gmpc(back_r)) < dist){
        move(-speed, -speed, speed, speed);
    }
}

void right_90(){
    turn_right(2100,800);
}

void right_180(){
    turn_right(4200,800);
}

void turn_left(int dist, int speed){
    cmpc(back_r);
while (abs(gmpc(back_r)) < dist){
        move(speed, speed, -speed, -speed);
    }
}

void left_90(){
    turn_left(2100,800);
}

void diag_r_forw(int dist, int speed){
    cmpc(back_r);
while (abs(gmpc(back_r)) < dist){
        move(speed, 0, speed, 0);
    }
}

void diag_l_forw(int dist, int speed){
    cmpc(front_r);
while (abs(gmpc(front_r)) < dist){
        move(0, speed, 0, speed);
    }
}

void diag_l_back(int dist, int speed){
    cmpc(back_r);
while (abs(gmpc(back_r)) < dist){
        move(-speed, 0, -speed, 0);
    }
}

void diag_r_back(int dist, int speed){
    cmpc(front_r);
while (abs(gmpc(front_r)) < dist){
        move(0, -speed, 0, -speed);
    }
}

void wait(int time)
{
    move(0, 0, 0, 0);
    msleep(time);
}

//square up on the black line - going forward
void squareUpLine_forw(int speed)
{
while ((analog(lightSensor_l) < grey) || (analog(lightSensor_r) < grey))
        {
            if ((analog(lightSensor_r) > 900) && (analog(lightSensor_l) < 900))
            { // right sees black, left sees white
                move(-speed/1.5, -speed/1.5, speed/1.5, speed/1.5); // turn left
            }
            else if ((analog(lightSensor_r) < 900) && (analog(lightSensor_l) > 900))
            { // right sees white, left sees black
                move(speed/1.5, speed/1.5, -speed/1.5, -speed/1.5); // turn right
            }
            else
            {
                move(speed, speed, speed, speed);
            }
        }
        wait(100);
}

//square up on the black line - going backwards
void squareUpLine_back(int speed)
{
while ((analog(lightSensor_r) < grey) || (analog(lightSensor_l) < grey))
        {
            if ((analog(lightSensor_r) > grey) && (analog(lightSensor_l) < grey))
            {
                move(speed/1.5, speed/1.5, -speed/1.5, -speed/1.5);
            }
            else if ((analog(lightSensor_r) < grey) && (analog(lightSensor_l) > grey))
            {
                move(-speed/1.5, -speed/1.5, speed/1.5, speed/1.5);
            }
            else
            {
                move(-speed, -speed, -speed, -speed);
            }
        }
        wait(100);
}

// square up using the touch sensors going backward
void squareUpTouch_back(int speed) {
while ((digital(buttonSensor_l) < 1) || (digital(buttonSensor_r) < 1))
        {
            if ((digital(buttonSensor_l) >= 1) && (digital(buttonSensor_r) < 1))
            {
                move(speed/1.5, speed/1.5, -speed/1.5, -speed/1.5);
            }
            else if ((digital(buttonSensor_l) < 1) && (digital(buttonSensor_r) >= 1))
            {
                move(-speed/1.5, -speed/1.5, speed/1.5, speed/1.5);
            }
            else
            {
                move(-speed, -speed, -speed, -speed);
            }
        }
   
        ao();
        msleep(200);
}

//follow the line
void lineFollow_forw (int distance)
{
int counter = 0;
while (counter < distance)
{
if (analog(lightSensor_l) > 1000)
{
move(600, 600, 0, 0);
counter += 1;
}
else
{
move(0, 0, 600, 600);
counter += 1;
}
}
}

// follow the line while going right
void lineFollow_right(int distance, int speed){
int counter = 0;
    while (counter < distance){
    if (analog(lightSensor_r) <= grey && analog(lightSensor_l) <= grey){
            // diag_r_back
            move(0, -speed, 0, -speed);
            counter++;
        }
        else if (analog(lightSensor_r) > grey && analog(lightSensor_l) <= grey){
            // turn left
            move(speed, speed, -speed, -speed);
            counter++;
        }
        else if (analog(lightSensor_r) <= grey && analog(lightSensor_l) > grey){
            // turn right
            move(-speed, -speed, speed, speed);
            counter++;
        }
        else if (analog(lightSensor_r) > grey && analog(lightSensor_l) > grey){
            // diag_r_forw
            move(speed, 0, speed, 0);
            counter++;
        }
        else {
            continue;
        }
    }
}

//move servo up slowly
void slowServo (int arm, int initial, int final, int speed)
{
    set_servo_position(arm, initial);
    msleep(500);
    int counter = initial;
   
    if (final > initial){
        while (get_servo_position(arm) < final)
        {
            set_servo_position(arm, counter);
            counter = counter + speed;
            msleep(20);
        }
    }
    else if (final < initial){
        while (get_servo_position(arm) > final)
        {
            set_servo_position(arm, counter);
            counter = counter - speed;
            msleep(20);
        }
    }
}

// set servo posisiton instantly
void ssp(int servo, int position)
{
    set_servo_position(servo,position);
    msleep(250);
}

void clear_motors(){
cmpc(0);
cmpc(1);
cmpc(2);
cmpc(3);
}

