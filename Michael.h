#include <kipr/wombat.h>

int k = 13.5; //Scale factor for distance to degrees
int rm = 0; //right motor
int lm = 3; //left motor
dif = 0;
//------------------------------Forward & Backward------------------------------------------
//f is defined (forward)
void f(int distance) {
  forward(distance);
}

//forward is defined
void forward(int distance) {
        v(1440, 1400, lm, distance);
}

//b is defined (backward)
void b(int distance) { //Distance in ticks
        backward(distance);
}

//backward is defined
void backward(int distance) {
        v(-1300, -1300, rm, distance); //velocity -1500 for both motors, tracking motor 0, distance
}

//------------------------------Left & Right Turns------------------------------
//l is defined (left turn)
void l(int degree) {
        turnL(degree);
}


//turnL is defined (left turn)
void turnL(int degree) {
        v(1000, -1000, rm, degree * k); //right motor 1000 velocity, left motor -1000 velocity, track motor 0, degrees*12 (scale factor)
}

//r is defined (right turn)
void r(int degree) {
turnR(degree);
}

//turnR is defined (right turn)
void turnR(int degree) {//12 might need to be changed
v(-1000, 1000, lm, degree * k); //Right motor -1000 speed, left motor 1000 speed, degrees (0-360) *12 (12 is scale factor for distance to degrees)
}


//UturnL is defined (180 degree left turn)///////////////////////////////////////////////////////
void UturnL() {
        v(1700, -1700, rm, 172 * k); //right motor 1000 velocity, left motor -1000 velocity, track motor 0, degrees*12 (scale factor)
}

//UturnR is defined (180 degree right turn)///////////////////////////////////////////////////////
void UturnR() {//12 might need to be changed
v(-1700, 1700, lm, 172 * k); //Right motor -1000 speed, left motor 1000 speed, degrees (0-360) *12 (12 is scale factor for distance to degrees)
}


//------------------------------------Extra------------------------------------
//v is defined (velocity)
void v(int right, int left, int driver, int distance){//(Right motor speed (-1000 to 1000), Left motor (speed -1000 to 1000), wheel to track(0,3), distance
    driver = clamp(driver, rm, lm);
    right = clamp(right, -1800, 1800);
    left = clamp(left, -1800, 1800);

    cmpc(driver); //clear position counter of driver (0 or 3)
    int majority = (distance * 5) / 6; //distance = 20%
    while (abs(gmpc(driver)) < majority) { //While absolute value of driver(0 or 3) motor position is less than 20% of original distance
    mav(rm,right); //motor 0 goes at mean average velocity of variable "right" (speed -1000 to 1000)
    mav(lm,left); //motor 3 goes at mean average velocity of variable "left" (speed -1000 to 1000)
    }


    while (abs(gmpc(driver)) < distance) { //While absolute value of driver(0 or 3) motor position is less than distance
        mav(0, right/2); //mean average velocity of 0 is variable "right"
        mav(3, left/2); //mean average velocity of 3 is variable "left"
        }
        mav(lm, -left); //mean average velocity is opposite of left (speed -1000 to 1000)
        ao(); //All off
    }



//Clamp is defined
//Clamp makes sure the value doesn't go over a certain threshold
int clamp(int var, int min, int max) { //Variable clamp(vaiable, minimum, maximum value)
    if (var > max) { // if variable is more than max then return value of max
        return max;
    }
   
    if (var < min) { //If variable is less then minimum then return value of min
        return min;
    }
    return var;
}
//------------------------------ Pause --------------------------------------------
//p defined(pause)
void p(double time) {
pause(time);
}

//pause defined
void pause(double time) { //double time is a type of variable. It holds either a decimal or interger. Meaning you could pause for half a second or 2 tenths, etc.
double sTime = seconds();
    while (seconds() - sTime < time) {
    ao();
    }
}

//------------------------------ Camera -------------------------------------------
//load_cam turns on camera

//lc defined (load_cam)
void lc(char* name, int logitech) { //for example load_cam("botball.conf",1) 1 is 1 or 0 for true or false
load_cam(name, logitech);
}

//load_cam defined
void load_cam(char* name, int logitech) {
    logitech = clamp(logitech, 0, 1);
camera_load_config(name);
    if (logitech) {
    camera_open_black();
    } else {
    camera_open();
    }
   
    for (int i = 0; i < 10; i++) { //(sets i to 0; while final value of i is less than 10; i equals i + 1)
    camera_update(); //Update cameras screen
        p(0.01);
    }
}


//Rotate_till
int rotate_till(int channel, int size, int resolution, int direction) { //direction is 1 (Right) or 0 (Left)
    cmpc(0);
    direction = clamp(direction, -1, 1);
while (1) {
        rectangle o = get_object_bbox(channel, 0);
        if (o.width * o.height > size) {
            int x = o.ulx + o.width/2;
            if (abs((x - resolution/2)) < resolution/10){
                return gmpc(0);
            }
        }
       
        double sTime = seconds();
        while (seconds() - sTime <= 0.2) {
        mav(0, -600 * direction);
            mav(3, 600 * direction);
        }
        p(0.01);
        camera_update();
    }
}

//------------------------------ Servos ------------------------------

//ss defined (slow servo)
void ss(int port, int position, int speed) { //(servo port, ending position of servo, speed)
slow_servo(port, position, speed);
}

//slow_servo defined (slow servo)
void slow_servo(int port, int position, int speed) { //(servo port, ending position, speed)
if (get_servo_position(port)  == position) { //If servo position of "servo port" equals "position"
    return; //Then end the code
    }
   
    int starting = get_servo_position(port); //"starting" position equals servo position of "port"
    int counter = 0;
   
    while((starting + counter)/speed != position/speed) { //While ("starting position" + "counter" (Which starts at 0 doesn't equal "position"(ex 200) /"speed" ex(50)->4)
    if (position - starting > 0) { //if current position - starting position is more than 0
    counter  += speed; // then counter equals counter + speed
    }
        if (position - starting < 0) { //if ending position - starting position is less than - then
        counter -= speed; //counter equals counter minus speed
        }
       
        set_servo_position(port, starting + counter);
        msleep(10);
    }
    set_servo_position(port, position);
    msleep(100);
}

//------------------------------ Color Detection ------------------------------

//detect_color defined (detect color)
int detect_color(int size) {
for (int i = 0; i < get_channel_count(); i++) { //(sets i to 0; final value of i is less than get_channel_count(); i+1)
    for (int j = 0; j < get_object_count(i); j++) {
        rectangle o = get_object_bbox(i, j);
            if (o.width * o.height > size) {
            return i;
            }
        }
    }
}

//------------------------------ Lineup ------------------------------
void lineup(int port1, int port2) {
    cmpc(0);///////////////////////
while ((digital(port1) == 0 || digital(port2) == 0)) {
        mav(3, digital(port1) == 0 ? -1000 : 500);
        mav(0, digital(port2) == 0 ? -1030 : 500);
    }
}
//while (digital(0) == 0 && digital(1) == 0) {


//Left sensor = 0
void lblack(int zdirection) {
    while ((analog(0) < 3800) || (analog(3) < 3700)) {
       mav(0, 600 * zdirection * (analog(0) < 3800));
       mav(3, 600 * zdirection * (analog(3) < 3700));
       printf("%d\n", analog(0) < 3800);

On Tue, Apr 14, 2026 at 4:40 PM Baez, Michael <baezmic000@mysbisd.org> wrote:

   
    //wait_for_light(1);
    shut_down_in(120);
    enable_servos();
    int arm = 2;
    int up = 1500; //Value for arm to be raise
    int closed = 200;
    int claw = 3;
    int down = 500;
    int open = 2000;
    lblack(1);
    /*
    set_servo_position(claw,open);
    set_servo_position(arm,1650);
   
    set_servo_position(claw,2100);
    forward(50);
   
    r(89);
    ss(arm,1700,10);
    msleep(10);
    lineup(0,1);
    r(20);
    forward(2000);
    l(20);
    lineup(0,1);
    msleep(10);
    r(0.5);
    ss(arm,1500,10);
    forward(5000);//going foward to cube
    r(1);
    forward(1200);
    ss(claw,900,10);
    forward(1500);
    p(1);
    backward(500);
    ss(arm,800,10);
    p(0.1);
    ss(claw,open,10);
    msleep(100);
    ss(arm,1600,10);
    lineup(0,1);
    forward(500);
    l(2);
   
   
   
   
    //starting getting pom poms
   
    ss(arm,1650,10);
    r(20);
    backward(1300);//backing up to get to lineup with pom poms
    lineup(0,1);
   
    forward(100);
    l(90);
    lineup(0,1);
    forward(1080);
    r(90);
    lineup(0,1);
   
    forward(1200);
    backward(1300);
    ss(2,down,6);
    forward(1100);
    ss(claw,0,5);
    ss(arm,1700,5);
    l(8);
   
lineup(0,1);
    forward(400);
    r(90);
    forward(1800);
    ss(2,1550,5);
    set_servo_position(claw,open);
    msleep(500);
   
    //now getting second round of putting pom poms in the first basket
    backward(1800);
    l(89);
    lineup(0,1);
    ao();
    ss(arm,down,6);
    forward(3600);
    ss(claw,0,5);
    l(5);
    lineup(0,1);
    forward(400);
    ss(arm,1650,10);
    msleep(100);
    r(90);
    forward(2200);
    set_servo_position(claw,open);
    msleep(500);
 
    //Now getting third set of poms
    backward(1900);
    l(87);
    lineup(0,1);
    ao();
    ss(arm,down,6);
    forward(4900);
    r(2);
    forward(1000);
    ss(claw,0,5);
    lineup(0,1);
    forward(400);
    ss(arm,1650,10);
    msleep(100);
    r(90);
    forward(2200);
    set_servo_position(claw,open);
    msleep(500);
 
    backward(1700);
    l(87);
lineup(0,1);
   
    forward(10000);
    */
