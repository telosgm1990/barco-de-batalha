// Molde do isopor do caminho A, em tamanho real — pra imprimir em papel A4
// (paisagem, escala 100%) e recortar o isopor por cima.
//
// Linhas: o contorno do casco, o furo do pote, onde vão os suportes dos
// motores e os palitos, e uma seta pra proa. Exporte em SVG:
//   openscad.com -o out/raft_template.svg pot/raft_template.scad

include <pot_params.scad>
use <../common/shapes.scad>
use <motor_mount.scad>

$fa = 2;
$fs = 0.5;

// ---------------------------------------------------------------- Peça

// As medidas vêm do caminho A (pot_params.scad) e do motor
// (common/dimensions.scad); as pontas da aba do suporte, de motor_mount.scad
raft_template(
    raft_length = raft_length,
    raft_width = raft_width,
    raft_shape_n = raft_shape_n,
    raft_thickness = raft_thickness,
    pot_length = pot_length,
    pot_width = pot_width,
    pot_corner_radius = pot_corner_radius,
    pot_taper = pot_taper,
    pot_height = pot_height,
    pot_clearance = pot_hole_clearance,
    shaft_x = shaft_x,
    edge_y = raft_edge_y,
    mount_x_min = mount_x_min(tt_total_length, tt_shaft_from_end,
                              tt_tab_length, mount_clearance, mount_wall),
    mount_x_max = mount_x_max(tt_shaft_from_end, tt_tab_length,
                              mount_clearance, mount_wall),
    flange_length = mount_flange_length
);

// ---------------------------------------------------------------- Módulos

// Contorno do isopor visto de cima, em 2D.
//   length, width  tamanho total (mm)
//   shape_n        superelipse: 2 é elipse; maior, mais "quadrado"
module raft_outline(length, width, shape_n) {
    hull_outline(length, width, shape_n);
}

// Furo do pote, em 2D: a medida do pote na altura do topo do isopor (o pote
// é cônico e entra apertado).
//   pot_length, pot_width  boca do pote (mm)
//   pot_corner_radius      raio dos cantos (mm)
//   pot_taper              boca / fundo — quanto o pote abre pra cima
//   pot_height             altura do corpo do pote (mm)
//   raft_thickness         espessura do isopor (mm)
//   clearance              folga do furo, de cada lado (mm)
module pot_hole(pot_length, pot_width, pot_corner_radius, pot_taper,
                pot_height, raft_thickness, clearance) {
    s = 1 + (pot_taper - 1) * raft_thickness / pot_height;
    offset(delta = clearance)
        scale([s / pot_taper, s / pot_taper])
            rounded_rect(pot_length, pot_width, pot_corner_radius);
}

// O molde inteiro, em 2D, só com linhas.
//   raft_*         o isopor (ver raft_outline e pot_hole)
//   pot_*          o pote (ver pot_hole)
//   pot_clearance  folga do furo do pote (mm)
//   shaft_x        onde fica o eixo dos motores, ao longo do barco (mm)
//   edge_y         meia-largura do isopor no eixo dos motores (mm)
//   mount_x_min    ponta de trás da aba do suporte, a partir do eixo (mm)
//   mount_x_max    ponta da frente da aba do suporte (mm)
//   flange_length  quanto a aba entra no isopor (mm)
//   line_width     espessura das linhas (mm)
module raft_template(raft_length, raft_width, raft_shape_n, raft_thickness,
                     pot_length, pot_width, pot_corner_radius, pot_taper,
                     pot_height, pot_clearance, shaft_x, edge_y, mount_x_min,
                     mount_x_max, flange_length, line_width = 0.6) {
    stroke(line_width) raft_outline(raft_length, raft_width, raft_shape_n);
    stroke(line_width)
        pot_hole(pot_length, pot_width, pot_corner_radius, pot_taper,
                 pot_height, raft_thickness, pot_clearance);
    // Suportes dos motores (a aba que deita no isopor), do jeito que ficam
    // na montagem: o do outro lado é o mesmo, girado
    for (side = [-1, 1])
        translate([shaft_x, side * edge_y])
            rotate([0, 0, side > 0 ? 0 : 180]) {
                stroke(line_width)
                    translate([mount_x_min, -flange_length])
                        square([mount_x_max - mount_x_min, flange_length]);
                // Marca do eixo do motor
                translate([-0.5, -6]) square([1, 6]);
            }
    // Seta da proa, no meio (o furo do pote sai mesmo)
    translate([-17, 0])
        stroke(line_width)
            polygon([[0, -4], [22, -4], [22, -9], [34, 0], [22, 9], [22, 4],
                     [0, 4]]);
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

// Contorno dos filhos, em 2D: uma linha de largura `width` em cima da borda
module stroke(width) {
    difference() {
        offset(delta = width / 2) children();
        offset(delta = -width / 2) children();
    }
}
