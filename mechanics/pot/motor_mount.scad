// Suporte do motor no caminho A — prende o motor TT na borda do isopor.
//
// Uma aba deita em cima do isopor (presa com palito de churrasco espetado e
// um pouco de cola), um bloco encosta na lateral e uma bandeja segura o motor
// pra fora da borda, com o eixo apontando pra fora. Duas abraçadeiras
// passam pela bandeja e abraçam o motor.
//
// O perfil é igual ao longo do comprimento: imprime em pé, deitado numa das
// pontas, sem suporte. São duas peças iguais — a do outro lado é a mesma,
// girada.

include <pot_params.scad>

$fa = 4;
$fs = 0.5;

// ---------------------------------------------------------------- Medidas

// Origem: x no eixo do motor, y = 0 na borda do isopor (pra fora é +y),
// z = 0 no fundo do isopor
mount_x_min = -(tt_total_length - tt_shaft_from_end - tt_tab_length)
              - mount_clearance - mount_wall;
mount_x_max = tt_shaft_from_end + tt_tab_length + mount_clearance
              + mount_wall;
motor_width = max(tt_thickness, tt_can_thickness);
inner_y = motor_inner_y - raft_edge_y; // face de dentro do motor
tray_top = shaft_z - tt_height / 2 - mount_clearance;
tray_bottom = tray_top - mount_wall;
peg_hole_diameter = 3.5; // palito de churrasco
peg_positions = [mount_x_min + 10, mount_x_max - 10];
strap_x_positions = [-30, 6]; // abraçadeiras, entre as partes do motor
strap_slot = [4, 2.2]; // abraçadeira pequena — a medir

// ---------------------------------------------------------------- Peça

// Em pé, numa das pontas, pronto pra fatiar
rotate([0, 90, 0]) translate([-mount_x_max, 0, 0]) motor_mount();

// ---------------------------------------------------------------- Módulos

module motor_mount() {
    difference() {
        translate([mount_x_min, 0, 0])
            rotate([90, 0, 90])
                linear_extrude(height = mount_x_max - mount_x_min)
                    mount_profile();
        // Rasgo do eixo de dentro, aberto em cima pro motor descer
        translate([-4.5, -1, shaft_z - 4.5])
            cube([9, inner_y + 1.1, 2 * raft_thickness]);
        // Furos dos palitos que espetam no isopor
        for (x = peg_positions)
            translate([x, -mount_flange_length / 2, 0])
                cylinder(d = peg_hole_diameter, h = 2 * raft_thickness);
        // Rasgos das abraçadeiras: um de cada lado do motor
        for (x = strap_x_positions, y = [inner_y + 1.5,
                                         inner_y + motor_width - 1.5])
            translate([x - strap_slot[0] / 2, y - strap_slot[1] / 2,
                       tray_bottom - 1])
                cube([strap_slot[0], strap_slot[1], mount_wall + 2]);
    }
}

// Perfil em (y, z): aba no topo do isopor, bloco na lateral, bandeja embaixo
module mount_profile() {
    top = raft_thickness + mount_wall;
    // Aba em cima do isopor
    translate([-mount_flange_length, raft_thickness])
        square([mount_flange_length + inner_y - mount_clearance, mount_wall]);
    // Bloco que encosta na lateral do isopor
    translate([0, tray_bottom])
        square([inner_y - mount_clearance, top - tray_bottom]);
    // Bandeja do motor
    translate([0, tray_bottom])
        square([inner_y + motor_width, mount_wall]);
}
