use <../vitamins/blind.scad>

$fn = 60;

module adaptor() {
    // Inner radius of blind:
    r = 12;
    // Height of this adaptor
    h = 25;
    // Height of circular part before flat bit to catch shaft:
    circular_height = 6;
    // Height of flat part of shaft:
    flat_height = h - circular_height;
    // Height of connector, which sits in bracket:
    connector_height = 6;
    // Height of lip, that sits between blind and bracket:
    lip_height = 2;
    // Diameter of shaft:
    shaft_clearance = 0.2;
    shaft_diameter = 5;

    module fin() {
        intersection() {
            rotate([0, 0, 60])
            translate([0, -1, 0])
            square([r, 2]);

            // Intersect with blind's inner radius to ensure edges of fin doesn't
            // overlap.
            circle(r = r);
        }
    }

    module inner() {
        circle(d = 18);

        for (y = [0, 1]) {
            mirror([0, y, 0])
            for (x = [0, 1]) {
                mirror([x, 0, 0])
                fin();
            }
        }
    }

    module shaft() {
        // Flat bit:
        translate([0, 0, circular_height])
        linear_extrude(height = flat_height)
        difference() {
            circle(d = shaft_diameter + shaft_clearance);

            translate([-(shaft_diameter / 2), 2])
            square([shaft_diameter + shaft_clearance, shaft_diameter]);
        }

        // Round bit:
        cylinder(d = shaft_diameter + shaft_clearance * 2, h = circular_height);
    }

    difference() {
        union() {
            // Inside the blind, connects to motor shaft:
            translate([0, 0, lip_height + connector_height])
            linear_extrude(h - lip_height - connector_height)
            inner();

            // Lip, caps end of blinds fabric:
            color("red")
            translate([0, 0, connector_height])
            cylinder(d = 40, h = lip_height);

            // Connector, into bracket:
            color("cyan")
            cylinder(d = 8, h = connector_height);
        }

        shaft();
    }
}

adaptor();
