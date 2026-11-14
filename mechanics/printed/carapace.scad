// Carapaça — o domo da DonaTelos no caminho B (tudo impresso).
//
// Meia elipsoide oca com gomos hexagonais em relevo, uma aba na base que
// assenta na borda do plastrão e em cima dos motores (prendendo-os no berço),
// dois arcos por onde as rodas de pá saem — as nadadeiras — e uma torrinha
// atrás com a chave liga/desliga.
//
// Fechada sobre o plastrão, ela é o volume de ar em cima que faz a tartaruga
// desvirar sozinha (esboço, seção 3.1). Imprime de aba no chão, sem suporte.

include <hull_params.scad>
use <../common/shapes.scad>

$fa = 3;
$fs = 0.8;

// ---------------------------------------------------------------- Peça

carapace();

// ---------------------------------------------------------------- Módulos

module carapace() {
    carapace_body();
    carapace_scutes();
}

module carapace_body() {
    difference() {
        union() {
            dome(0);
            flange();
            switch_turret();
        }
        // Oco do domo, acima da aba
        intersection() {
            dome(-carapace_shell);
            translate([-200, -200, carapace_flange]) cube(400);
        }
        // Abertura que encaixa por fora da aba do plastrão
        translate([0, 0, -1])
            linear_extrude(height = carapace_flange + 2)
                offset(delta = -hull_wall * hull_flare + 0.3 + fit_clearance)
                    hull_outline(hull_length, hull_width, hull_shape_n);
        common_cuts();
    }
}

// Gomos hexagonais em relevo, acompanhando a curva do domo
module carapace_scutes() {
    difference() {
        intersection() {
            difference() {
                dome(scute_ridge_height);
                dome(0);
            }
            translate([0, 0, 6])
                linear_extrude(height = carapace_height + 10)
                    rotate([0, 0, 90]) hex_grid_lines();
        }
        common_cuts();
    }
}

// Cortes que valem pro domo e pros gomos: arcos das rodas e furo da chave
module common_cuts() {
    for (side = [-1, 1])
        mirror([0, side < 0 ? 1 : 0, 0])
            translate([shaft_x, wheel_inner_y - 0.5, shaft_z - hull_height])
                rotate([-90, 0, 0])
                    cylinder(r = wheel_radius + wheel_well_margin, h = 60);
    translate([switch_x, 0, 0])
        cube([switch_cutout_width, switch_cutout_length, 2 * carapace_height],
             center = true);
}

// Meia elipsoide; grow > 0 engorda, grow < 0 afina
module dome(grow) {
    intersection() {
        resize([carapace_length + 2 * grow, carapace_width + 2 * grow,
                2 * (carapace_height + grow)])
            sphere(r = 1, $fn = 120);
        translate([-200, -200, 0]) cube(400);
    }
}

module flange() {
    linear_extrude(height = carapace_flange)
        scale([carapace_length / 2, carapace_width / 2])
            circle(r = 1, $fn = 160);
}

// Torrinha de topo plano pra chave gangorra
module switch_turret() {
    translate([switch_x, 0, 26])
        linear_extrude(height = 37 - 26)
            rounded_rect(switch_cutout_width + 8, switch_cutout_length + 8, 3);
}

// Linhas de uma grade hexagonal (colunas ao longo de Y antes de girar)
module hex_grid_lines() {
    r = scute_size / 2;
    w = scute_ridge_width / cos(30);
    for (i = [-6:6], j = [-6:6])
        translate([i * 1.5 * r,
                   j * sqrt(3) * r + (abs(i) % 2) * sqrt(3) * r / 2])
            difference() {
                circle(r = r + w / 2, $fn = 6);
                circle(r = r - w / 2, $fn = 6);
            }
}
