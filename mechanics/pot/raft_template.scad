// Molde do isopor do caminho A, em tamanho real — pra imprimir em papel A4
// (paisagem, escala 100%) e recortar o isopor por cima.
//
// Linhas: o contorno do casco, o furo do pote, onde vão os suportes dos
// motores e os palitos, e uma seta pra proa. Exporte em SVG:
//   openscad.com -o out/raft_template.svg pot/raft_template.scad

include <pot_params.scad>
use <../common/shapes.scad>

$fa = 2;
$fs = 0.5;

// ---------------------------------------------------------------- Medidas

line_width = 0.6;

// ---------------------------------------------------------------- Peça

raft_template();

// ---------------------------------------------------------------- Módulos

// Contorno do isopor visto de cima
module raft_outline() {
    hull_outline(raft_length, raft_width, raft_shape_n);
}

// Furo do pote: a medida do pote na altura do topo do isopor
module pot_hole() {
    s = 1 + (pot_taper - 1) * raft_thickness / pot_height;
    offset(delta = pot_hole_clearance)
        scale([s / pot_taper, s / pot_taper])
            rounded_rect(pot_length, pot_width, pot_corner_radius);
}

module raft_template() {
    stroke() raft_outline();
    stroke() pot_hole();
    // Suportes dos motores (a aba que deita no isopor), do jeito que ficam
    // na montagem: o do outro lado é o mesmo, girado
    for (side = [-1, 1])
        translate([shaft_x, side * raft_edge_y])
            rotate([0, 0, side > 0 ? 0 : 180]) {
                stroke()
                    translate([-(tt_total_length - tt_shaft_from_end
                                 - tt_tab_length)
                                   - mount_clearance - mount_wall,
                               -mount_flange_length])
                        square([tt_total_length + 2 * mount_clearance
                                    + 2 * mount_wall,
                                mount_flange_length]);
                // Marca do eixo do motor
                translate([-0.5, -6]) square([1, 6]);
            }
    // Seta da proa, no meio (o furo do pote sai mesmo)
    translate([-17, 0])
        stroke() polygon([[0, -4], [22, -4], [22, -9], [34, 0], [22, 9],
                          [22, 4], [0, 4]]);
    translate([-17, -24])
        text("PROA", size = 9, font = "Liberation Sans:style=Bold");
    // Régua de conferência: imprimiu certo, isto mede 50 mm
    translate([-25, 24]) {
        square([50, line_width]);
        for (x = [0, 50]) translate([x - line_width / 2, -3])
            square([line_width, 6]);
        translate([0, 5])
            text("50 mm", size = 5, font = "Liberation Sans");
    }
}

module stroke() {
    difference() {
        offset(delta = line_width / 2) children();
        offset(delta = -line_width / 2) children();
    }
}
