//configuration
WHEEL_DIA=40;
WHEE_INNERL_HEIGHT=3;
WHEE_OUTTER_HEIGHT=12;
PLUS_SIZE=28;
PLUS_DIA=7;
PLUS_HEIGHT=WHEE_INNERL_HEIGHT-1;

SCREW_DIA=1;
SCREW_LENGTH=5;
SCREW_PREFIX=5;
$fn=20;

module screw_hole() {
    $fn=20;
    SCREW_DIA=1;
        hull() {
        cylinder(h=PLUS_HEIGHT,d=SCREW_DIA,center=true);
        translate([SCREW_LENGTH,0,0]) {
            cylinder(h=PLUS_HEIGHT,d=SCREW_DIA,center=true);
            }       
        }
}

module plus() {
    $fn=20;
    hull(){
        cylinder(h=PLUS_HEIGHT,d=PLUS_DIA);
        translate([PLUS_SIZE/2,0,0]) {
            cylinder(h=PLUS_HEIGHT,d=PLUS_DIA-1);
        }
    }
    hull(){
    cylinder(h=PLUS_HEIGHT,d=PLUS_DIA);
        translate([-PLUS_SIZE/2,0,0]) {
            cylinder(h=PLUS_HEIGHT,d=PLUS_DIA-1);
        }
    }
    hull(){
        cylinder(h=PLUS_HEIGHT,d=PLUS_DIA);
        translate([0,PLUS_SIZE/2,0]) {
            cylinder(h=PLUS_HEIGHT,d=PLUS_DIA-1);
        }
    }
    hull(){
        cylinder(h=PLUS_HEIGHT,d=PLUS_DIA);
        translate([0,-PLUS_SIZE/2,0]) {
            cylinder(h=PLUS_HEIGHT,d=PLUS_DIA-1);
        }
    }
}

module wheel() {
    $fn=150;
    difference(){
        union() {
        cylinder(h=WHEE_INNERL_HEIGHT, d=WHEEL_DIA);
        
        translate([0,0,WHEE_INNERL_HEIGHT-0.1]){
            difference() {
            cylinder(h=WHEE_OUTTER_HEIGHT, d=WHEEL_DIA);
            cylinder(h=WHEE_OUTTER_HEIGHT+1, d=WHEEL_DIA/1.5);
            }
        }
    }
        translate([0,0,-1]){
            plus();
        }
        cylinder(h=WHEE_INNERL_HEIGHT-1.5, d=5);
        cylinder(h=WHEE_INNERL_HEIGHT+2, d=3);
    }
}
rotate([0,0,0]) {
    wheel();
}