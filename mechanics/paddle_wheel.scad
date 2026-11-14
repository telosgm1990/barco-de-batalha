// Roda de pá — a "nadadeira" da DonaTelos. Serve pros dois caminhos do casco.
//
// Imprime deitada: o disco no chão da mesa, as pás e o cubo pra cima, sem
// suporte. São duas iguais — a do outro lado é a mesma peça virada.

include <common/dimensions.scad>
use <common/shapes.scad>

$fa = 4;
$fs = 0.5;

// ---------------------------------------------------------------- Peça

// As medidas compartilhadas (roda, eixo do motor, folga) vêm de
// common/dimensions.scad; as da própria roda são o padrão do módulo
paddle_wheel(
    radius = wheel_radius,
    width = wheel_width,
    shaft_diameter = tt_shaft_diameter,
    shaft_flat = tt_shaft_flat,
    clearance = shaft_clearance
);

// ---------------------------------------------------------------- Módulos

// Roda de pá: disco de um lado, cubo no meio e pás em concha, com o furo
// "D duplo" do eixo do motor TT.
//   radius            raio até a ponta da pá (mm)
//   width             largura da pá, ao longo do eixo (mm)
//   shaft_diameter    eixo do motor, no redondo (mm)
//   shaft_flat        eixo do motor, entre as faces planas (mm)
//   clearance         folga do furo do eixo, de cada lado (mm)
//   paddle_count      quantas pás
//   paddle_thickness  espessura da pá (mm)
//   paddle_sweep      curvatura da pá (graus) — pá em concha empurra mais
//   hub_diameter      cubo (mm)
//   disc_radius       disco de um lado só: firma as pás sem esconder a forma
//   disc_thickness    espessura do disco (mm)
module paddle_wheel(radius, width, shaft_diameter, shaft_flat, clearance,
                    paddle_count = 6, paddle_thickness = 2.0,
                    paddle_sweep = 28, hub_diameter = 12, disc_radius = 20,
                    disc_thickness = 2.0) {
    difference() {
        union() {
            cylinder(r = disc_radius, h = disc_thickness);
            cylinder(d = hub_diameter, h = width);
            for (i = [0:paddle_count - 1])
                rotate([0, 0, i * 360 / paddle_count])
                    linear_extrude(height = width)
                        paddle_blade(hub_diameter / 2 - 1, radius,
                                     paddle_thickness, paddle_sweep);
        }
        // Furo do eixo, justo
        translate([0, 0, -1])
            linear_extrude(height = width + 2)
                double_d_hole(shaft_diameter + 2 * clearance,
                              shaft_flat + 2 * clearance);
    }
}

// Pá curva, em 2D, do cubo até a borda: uma corrente de círculos ao longo do
// arco.
//   inner_radius  onde a pá começa (mm)
//   outer_radius  onde a pá termina (mm)
//   thickness     espessura da pá (mm)
//   sweep         curvatura (graus)
module paddle_blade(inner_radius, outer_radius, thickness, sweep) {
    steps = 10;
    for (k = [0:steps - 1])
        hull() {
            blade_point(inner_radius, outer_radius, thickness, sweep,
                        k / steps);
            blade_point(inner_radius, outer_radius, thickness, sweep,
                        (k + 1) / steps);
        }
}

// Um ponto da pá, em 2D: t vai de 0 (no cubo) a 1 (na borda); a ponta é um
// pouco mais grossa. Os outros parâmetros são os de paddle_blade.
module blade_point(inner_radius, outer_radius, thickness, sweep, t) {
    r = inner_radius + t * (outer_radius - inner_radius);
    angle = sweep * t * t;
    rotate([0, 0, angle])
        translate([r, 0])
            circle(d = thickness * (t < 0.95 ? 1 : 1.3));
}
