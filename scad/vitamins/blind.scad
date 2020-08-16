$fn = 60;

module blind(length = 10) {
    // Inner radius
    r = 11.5;

    module tube() {
        difference() {
            circle(r = r + 0.5);
            circle(r = r);
        }
    }

    module fins () {
        inner_gap = 6;
        outer_gap = 8;
        h = 2;
        w = 1;

        angle = acos(((outer_gap - inner_gap) / 2) / 2);

        g = inner_gap / 2;
        offset = sqrt(r*r - g*g) - h;

        for (i = [0, 1]) {
            mirror([i, 0, 0])
            translate([g, offset])
            rotate([0, 0, -(90 - angle)])
            square([w, h]);
        }
    }

    linear_extrude(height = length) {
        fins();
        tube();
    }
}

blind();
