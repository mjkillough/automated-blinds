// NEMA 17 stepper motor
// Source: https://www.thingiverse.com/thing:2348650
// Author: gammagec
// License: CC-BY

// Slightly modified to add flat edge to shaft.

// See 'Product Detail' tab of:
// http://motechmotor.com/productDetail-0104-33.html

module nema17() {
    difference() {
        body();
        screw_holes();
    }
    shaft();

    module body() {
        difference() {
            union() {
                color("slategray")
                linear_extrude(39)
                    base_sketch();

                color("silver")
                translate([ 0, 0, 39 ])
                linear_extrude(0.2)
                    base_sketch();

                circular_base();
            }
            middle_cut();
        }
    }

    module screw_holes() {
        $fn = 60;
        d = 44;
        r = d / 2;
        for (i = [45:90:360])
            rotate([ 0, 0, i ])
            translate([ r, 0, 39.2 - 6 ])
                cylinder(d = 3, h = 7.2);
    }

    module shaft() {
        $fn = 60;
        translate([ 0, 0, 40.95 ])
        difference() {
            cylinder(d = 5, h = 21);

            translate([-2.5, 2, 6])
                cube([5, 5, 15]);
        }
    }

    module circular_base() {
        color("slategray")
        translate([ 0, 0, 39.2 ])
        linear_extrude(1.75)
            circle(d = 22);
    }

    module middle_cut() {
        translate([ 0, 0, 11.75 ])
        linear_extrude(18)
            chamfer(53);
    }

    module chamfer(amt) {
        rotate([ 0, 0, 45 ]) difference() {
            square(100, true);
            square(amt, true);
        }
    }

    module base_sketch() {
        difference() {
            square(42, true);
            chamfer(55);
        }
    }
}

nema17();
