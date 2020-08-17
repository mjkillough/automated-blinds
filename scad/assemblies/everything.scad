use <../parts/bracket.scad>
use <../parts/adaptor.scad>
use <../vitamins/nema17.scad>
use <../vitamins/blind.scad>

e = 10 * $t;

bracket();

color("cyan")
translate([20, 40, -62 + 22.75 - e])
nema17();

color("blue")
translate([20, 40, 10 + 2 + e])
blind();

color("purple")
translate([20, 40, 4])
adaptor();
