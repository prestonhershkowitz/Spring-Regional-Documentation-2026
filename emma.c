#include <kipr/wombat.h>
#include <mecanum_library.h>

int main()
{  
    //starting pos for servo 0 (arm) 321
    enable_servos();
    wait_for_light(0);
    shut_down_in(119);
    //made paper set starters next to robot
    set_servo_position(1,1879);
    set_servo_position(3,2047);
    msleep(400);

    turn_left(300,1000);
    diag_l_forw(1000,1000);
    diag_l_forw(800,1000);
    turn_left(900,1000);
    set_servo_position(1,870);
    msleep(400);
   
    set_servo_position(3,786);
    msleep(400);
    ao();
    msleep(500);
    turn_left(230,1000);
    backward(1200,1000);
    right(2500,1000);
    msleep(400);
    forward(13000,1000);
    turn_left(2100,1000);
   
    backward(4000,1000);
    right(1200,1000);
    set_servo_position(3,1853);
    msleep(400);
    ao();
    msleep(500);
    forward(3000,1000);
    msleep(400);
    set_servo_position(1,1339);
    msleep(200);
    ao();
    msleep(500);

    set_servo_position(3,1925);
    msleep(200);
    set_servo_position(3,1783);
    msleep(200);
    ao();
    msleep(200);
    right(2000,1000);
    msleep(200);
    set_servo_position(3,2029);
    msleep(200);
    backward(2700,1000);
    right(300,1000);
    msleep(200);
   
    disable_servos();
    return 0;
}
