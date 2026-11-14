// Roda de pá — a "nadadeira" da DonaTelos. Serve pros dois caminhos do casco.
//
// Imprime deitada: o disco no chão da mesa, as pás e o cubo pra cima, sem
// suporte. São duas iguais — a do outro lado é a mesma peça virada.

include <common/dimensions.scad>
use <common/shapes.scad>

$fa = 4;
$fs = 0.5;

// ---------------------------------------------------------------- Medidas

// Raio e largura: wheel_radius e wheel_width, em common/dimensions.scad
paddle_count = 6;
paddle_thickness = 2.0;
paddle_sweep = 28; // curvatura da pá, em graus — pá em concha empurra mais
hub_diameter = 12;
disc_thickness = 2.0;
disc_radius = 20; // disco de um lado só: firma as pás sem esconder a forma

// ---------------------------------------------------------------- Peça

paddle_wheel();

// ---------------------------------------------------------------- Módulos

module paddle_wheel() {
    difference() {
        union() {
            cylinder(r = disc_radius, h = disc_thickness);
            cylinder(d = hub_diameter, h = wheel_width);
            for (i = [0:paddle_count - 1])
                rotate([0, 0, i * 360 / paddle_count])
                    linear_extrude(height = wheel_width)
                        paddle_blade();
        }
        // Furo do eixo, justo
        translate([0, 0, -1])
            linear_extrude(height = wheel_width + 2)
                double_d_hole(tt_shaft_diameter + 2 * shaft_clearance,
                              tt_shaft_flat + 2 * shaft_clearance);
    }
}

// Pá curva, do cubo até a borda: uma corrente de círculos ao longo do arco
module paddle_blade() {
    steps = 10;
    r0 = hub_diameter / 2 - 1;
    for (k = [0:steps - 1])
        hull() {
            blade_point(r0, k / steps);
            blade_point(r0, (k + 1) / steps);
        }
}

module blade_point(r0, t) {
    r = r0 + t * (wheel_radius - r0);
    angle = paddle_sweep * t * t;
    rotate([0, 0, angle])
        translate([r, 0])
            circle(d = paddle_thickness * (t < 0.95 ? 1 : 1.3));
}
