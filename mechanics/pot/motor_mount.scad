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

// ---------------------------------------------------------------- Peça

// Em pé, numa das pontas, pronto pra fatiar. As medidas vêm do motor
// (common/dimensions.scad) e do caminho A (pot_params.scad)
rotate([0, 90, 0])
    translate([-mount_x_max(tt_shaft_from_end, tt_tab_length,
                            mount_clearance, mount_wall), 0, 0])
        motor_mount(
            motor_length = tt_total_length,
            shaft_from_end = tt_shaft_from_end,
            tab_length = tt_tab_length,
            motor_width = max(tt_thickness, tt_can_thickness),
            motor_height = tt_height,
            raft_thickness = raft_thickness,
            shaft_z = shaft_z,
            inner_y = motor_inner_y - raft_edge_y,
            clearance = mount_clearance,
            wall = mount_wall,
            flange_length = mount_flange_length
        );

// ---------------------------------------------------------------- Funções

// Ponta da frente do suporte, do lado da orelha do motor: x em mm, com o 0
// no eixo. Os parâmetros são os de motor_mount.
function mount_x_max(shaft_from_end, tab_length, clearance, wall) =
    shaft_from_end + tab_length + clearance + wall;

// Ponta de trás do suporte, do lado do fundo do motor: x em mm, com o 0 no
// eixo. Os parâmetros são os de motor_mount.
function mount_x_min(motor_length, shaft_from_end, tab_length, clearance,
                     wall) =
    -(motor_length - shaft_from_end - tab_length) - clearance - wall;

// ---------------------------------------------------------------- Módulos

// Suporte do motor, na posição de uso: x ao longo do motor (0 no eixo), y pra
// fora da borda do isopor (0 na borda), z pra cima (0 no fundo do isopor).
//   motor_length       motor TT inteiro, do fundo à ponta da orelha (mm)
//   shaft_from_end     centro do eixo até a ponta da caixa, sem a orelha (mm)
//   tab_length         orelha de fixação na ponta da caixa (mm)
//   motor_width        espessura do motor, a maior entre caixa e motor (mm)
//   motor_height       altura da caixa, perpendicular ao eixo (mm)
//   raft_thickness     espessura do isopor (mm)
//   shaft_z            altura do eixo, a partir do fundo do isopor (mm)
//   inner_y            face de dentro do motor, a partir da borda (mm)
//   clearance          folga em volta do motor (mm)
//   wall               espessura das paredes (mm)
//   flange_length      aba que deita em cima do isopor (mm)
//   peg_hole_diameter  furo do palito de churrasco (mm)
//   strap_x_positions  onde passam as abraçadeiras, em x (mm)
//   strap_slot         rasgo da abraçadeira, [largura, espessura] (mm) —
//                      abraçadeira pequena, a medir
module motor_mount(motor_length, shaft_from_end, tab_length, motor_width,
                   motor_height, raft_thickness, shaft_z, inner_y, clearance,
                   wall, flange_length, peg_hole_diameter = 3.5,
                   strap_x_positions = [-30, 6], strap_slot = [4, 2.2]) {
    x_min = mount_x_min(motor_length, shaft_from_end, tab_length, clearance,
                        wall);
    x_max = mount_x_max(shaft_from_end, tab_length, clearance, wall);
    tray_bottom = shaft_z - motor_height / 2 - clearance - wall;
    difference() {
        translate([x_min, 0, 0])
            rotate([90, 0, 90])
                linear_extrude(height = x_max - x_min)
                    mount_profile(raft_thickness, inner_y, motor_width,
                                  tray_bottom, clearance, wall,
                                  flange_length);
        // Rasgo do eixo de dentro, aberto em cima pro motor descer
        translate([-4.5, -1, shaft_z - 4.5])
            cube([9, inner_y + 1.1, 2 * raft_thickness]);
        // Furos dos palitos que espetam no isopor
        for (x = [x_min + 10, x_max - 10])
            translate([x, -flange_length / 2, 0])
                cylinder(d = peg_hole_diameter, h = 2 * raft_thickness);
        // Rasgos das abraçadeiras: um de cada lado do motor
        for (x = strap_x_positions, y = [inner_y + 1.5,
                                         inner_y + motor_width - 1.5])
            translate([x - strap_slot[0] / 2, y - strap_slot[1] / 2,
                       tray_bottom - 1])
                cube([strap_slot[0], strap_slot[1], wall + 2]);
    }
}

// Perfil do suporte, em 2D (y, z): aba no topo do isopor, bloco na lateral e
// bandeja embaixo.
//   tray_bottom  altura da face de baixo da bandeja (mm)
//   os outros    os de motor_mount
module mount_profile(raft_thickness, inner_y, motor_width, tray_bottom,
                     clearance, wall, flange_length) {
    top = raft_thickness + wall;
    // Aba em cima do isopor
    translate([-flange_length, raft_thickness])
        square([flange_length + inner_y - clearance, wall]);
    // Bloco que encosta na lateral do isopor
    translate([0, tray_bottom])
        square([inner_y - clearance, top - tray_bottom]);
    // Bandeja do motor
    translate([0, tray_bottom])
        square([inner_y + motor_width, wall]);
}
