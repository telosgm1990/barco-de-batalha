// Carapaça do caminho A — o domo que vai colado em cima da tampa do pote.
//
// Abriu a tampa, abriu a tartaruga. Colada com cola quente em volta, o ar
// preso entre ela e a tampa fica em cima do barco e faz ele desvirar sozinho
// (esboço, seção 3.1). Imprime de base no chão, sem suporte.

include <pot_params.scad>
use <../common/shapes.scad>

$fa = 3;
$fs = 0.8;

// ---------------------------------------------------------------- Peça

pot_carapace();

// ---------------------------------------------------------------- Módulos

module pot_carapace() {
    pot_carapace_body();
    pot_carapace_scutes();
}

module pot_carapace_body() {
    difference() {
        union() {
            shell_dome(0);
            linear_extrude(height = shell_base) shell_footprint(0);
        }
        // Oco, acima do anel da base
        intersection() {
            shell_dome(-shell_thickness);
            translate([-200, -200, shell_base]) cube(400);
        }
        // Abertura do anel: o anel cola na borda da tampa
        translate([0, 0, -1])
            linear_extrude(height = shell_base + 2)
                offset(delta = -shell_opening_margin) lid_outline();
    }
}

module pot_carapace_scutes() {
    intersection() {
        difference() {
            shell_dome(scute_ridge_height);
            shell_dome(0);
        }
        translate([0, 0, 5])
            linear_extrude(height = shell_height + 10)
                rotate([0, 0, 90])
                    hex_grid_lines(scute_size, scute_ridge_width);
    }
}

// Contorno da tampa vista de cima (a aba dela inclusa)
module lid_outline() {
    rounded_rect(pot_length + 2 * lid_overhang, pot_width + 2 * lid_overhang,
                 pot_corner_radius + lid_overhang);
}

module shell_footprint(grow) {
    hull_outline(shell_length + 2 * grow, shell_width + 2 * grow,
                 shell_shape_n);
}

// Domo de base em superelipse e perfil de elipse: fatias empilhadas, com
// casca convexa por cima
module shell_dome(grow) {
    steps = 24;
    h = shell_height + grow;
    hull()
        for (i = [0:steps])
            let(z = h * min(i / steps, 0.985), s = sqrt(1 - pow(z / h, 2)))
                translate([0, 0, z])
                    linear_extrude(height = 0.01)
                        scale([s, s]) shell_footprint(grow);
}
