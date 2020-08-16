use <../parts/bracket.scad>
use <../vitamins/nema17.scad>
use <../vitamins/blind.scad>

bracket();

color("cyan")
translate([20, 40, -62 + 22.75])
nema17();

color("blue")
translate([20, 40, 10])
blind();
