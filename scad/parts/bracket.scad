module bracket() {
    module base() {
        module wallmount() {
            cube([4, 5, 16]);
            translate([-2, 2, 0])
            cube([8, 3, 17]);
        }

        difference() {
            // Base
            cube([40, 5, 20]);

            translate([9, 0, 0])
            wallmount();

            translate([27, 0, 0])
            wallmount();
        }
    }

    // Modified from nema17.scad
    module nema_screw_holes() {
        $fn = 60;
        d = 44;
        r = d / 2;
        for (i = [45:90:360])
            rotate([0, 0, i])
            translate([r, 0, 0]) {
                translate([0, 0, 2])
                cylinder(d = 5, h = 8);

                cylinder(d = 3, h = 2);
            }
    }

    module nema_shaft() {
        $fn = 60;
        cylinder(d = 5 + 1, h = 10);
    }

    module blinds_holder() {
        $fn = 60;
        d = 5 + 3;

        // Slot to allow blind to placed in from above.
        translate([-20, -(d / 2), 0])
        cube([20, d, 6]);

        cylinder(d = d, h = 6);
    }

    module backplate() {
        difference() {
            // Back-plate
            translate([0, 5, 0])
            cube([40, 55, 10]);

            // Nema 17's circular base
            translate([9 + 11, 60 - 9 - 11, 0])
            cylinder(r = 11, h = 1.75);

            // Nema 17's shaft
            translate([20, 40, 0])
            nema_shaft();

            // Nema 17's screw holes
            translate([20, 40, 0])
            nema_screw_holes();

            // Blinds holder
            translate([20, 40, 4])
            blinds_holder();
        }
    }

    base();
    backplate();
}

bracket();
