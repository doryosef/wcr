////configuration
WHEEL_DIA=40;
WHEE_INNERL_HEIGHT=4;
WHEE_OUTTER_HEIGHT=12;
PLUS_SIZE=28;
PLUS_DIA=7;
PLUS_HEIGHT=WHEE_INNERL_HEIGHT-1;

SCREW_DIA=1;
SCREW_LENGTH=5;
SCREW_PREFIX=5;
$fn=150;

difference(){
    union(){
        cylinder(h=WHEE_INNERL_HEIGHT, d=WHEEL_DIA);
        translate([0,0,WHEE_INNERL_HEIGHT-0.1]){
            cylinder(h=WHEE_INNERL_HEIGHT, d=WHEEL_DIA-2);
        }
        translate([0,0,(2*WHEE_INNERL_HEIGHT)-0.1]){
                cylinder(h=WHEE_INNERL_HEIGHT, d=WHEEL_DIA-1);
        }

        translate([0,0,(2*WHEE_INNERL_HEIGHT)-0.1]){
                cylinder(h=WHEE_INNERL_HEIGHT, d=WHEEL_DIA-1);
        }
        difference(){
        translate([0,0,(3*WHEE_INNERL_HEIGHT)-(0.1)]){
                cylinder(h=5.5, d=6);
        }
        translate([0,0,(3*WHEE_INNERL_HEIGHT+3)-(0.1)]){
                rotate([90,0 ,0]){
                cylinder(h=5, d=3);
                }
        }
    }
    }
    translate([0,0,-2]){
        cylinder(h=20, d=2.5);
    }
        
}