include<roundedcube.scad>
//conficguration
WIDTH = 72;
DEPTH = 125;
HEIGHT = 35;
SHELL_SIZE = 4;

BODY_SIZE = [WIDTH, DEPTH, HEIGHT];
BODY_SHELL = [WIDTH-SHELL_SIZE,DEPTH-SHELL_SIZE,HEIGHT];

SERVO_HEAD_WIDTH = 13;
SERVO_HEAD_DPETH = 10;
SERVO_HEAD_HEIGHT = 14;
SERVO_HEAD = [SERVO_HEAD_WIDTH,SERVO_HEAD_DPETH,SERVO_HEAD_HEIGHT];
SERVO_HANDLE = 5;
SERVO_PREFIX_DEPTH = 30;

WHEEL_HOLE_PREFIX=10;

MOTOR_OUTER_DIA = 18;
MOTOR_INNER_DIA = 7;

module screw_hole() {
    $fn=20;
    SCREW_DIA=2;
    rotate([90,0,90]) {
        hull() {
        cylinder(h=10,d=SCREW_DIA,center=true);
        translate([SERVO_HANDLE/2,0,0]) {
            cylinder(h=10,d=SCREW_DIA,center=true);
            }       
        }
    }
}

module wheel_hole(WHEEL_DIA=6) {
    $fn=20;
    WHEEL_DIA_HOLE=WHEEL_DIA+1;
    HOLE_DIA=5;
    LENGTH_DOWN=5;
    rotate([0,90,0]) {
        cylinder(h=5,d=WHEEL_DIA_HOLE,center=true);
        hull() {
        cylinder(h=5,d=HOLE_DIA,center=true);
        translate([LENGTH_DOWN,0,0]) {
            cylinder(h=5,d=HOLE_DIA,center=true);
            }       
        }
    }
}

module body() {
    difference(){
        roundedcube(BODY_SIZE, center=true);
        translate([0,0,SHELL_SIZE]) {
            roundedcube(BODY_SHELL, center=true);
            //LEFT
            translate([(WIDTH/2),
            (DEPTH/2)-SERVO_HANDLE-SERVO_PREFIX_DEPTH,
            -(HEIGHT/2.5)+(SERVO_HEAD_HEIGHT/2)]) {
                rotate([0, 90, 0]) {
                    translate([0,0,-2]) {
                        cylinder(h=2, d=MOTOR_OUTER_DIA, center=true);
                        
                    }
                    cylinder(h=5, d=MOTOR_INNER_DIA, center=true);
                }
                translate([0,SERVO_HEAD_DPETH/2+(SERVO_HANDLE/2)-1,0 ]) {
                    screw_hole();
                }
                translate([0,-SERVO_HEAD_DPETH/2-(SERVO_HANDLE)+1,0 ]) {
                    screw_hole();
                }
            }
            //RIGHT
            translate([-(WIDTH/2),
            (DEPTH/2)-SERVO_HANDLE-SERVO_PREFIX_DEPTH,
            -(HEIGHT/2.5)+(SERVO_HEAD_HEIGHT/2)]) {
                rotate([0, 90, 0]) {
                    translate([0,0,2]) {
                        cylinder(h=2, d=MOTOR_OUTER_DIA, center=true);
                        
                     }
                    cylinder(h=3, d=MOTOR_INNER_DIA, center=true);
                }
                translate([0,SERVO_HEAD_DPETH/2+(SERVO_HANDLE/2)-1,0 ]) {
                    screw_hole();
                }
                translate([0,-SERVO_HEAD_DPETH/2-(SERVO_HANDLE)+1,0 ]) {
                    screw_hole();
                }
            }
        }
    }
}
body();