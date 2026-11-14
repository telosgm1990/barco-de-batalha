// Vista do conjunto do caminho A (pote e isopor) — não é peça de imprimir.
//
// Pote encaixado no isopor, motores nos suportes da borda, rodas de pá,
// carapaça colada na tampa e a cabeça na proa. Use -D explode=60 pra
// levantar a tampa com a carapaça e ver por dentro do pote.

include <pot_params.scad>
use <../common/shapes.scad>
use <../common/reference_parts.scad>
use <../paddle_wheel.scad>
use <../printed/head.scad>
use <motor_mount.scad>
use <carapace.scad>
use <raft_template.scad>

$fa = 3;
$fs = 0.8;

explode = 0;
show_water = true;

// ---------------------------------------------------------------- Cores

raft_color = "#F4F4EE";
pot_color = [0.86, 0.89, 0.92, 0.55];
lid_color = "#F2A7A0";
mount_color = "#F3C98B";
carapace_color = "#6BAF6B";
scute_color = "#4E8F57";
skin_color = "#7CC27C";
bandana_color = "#8E6CC9";
flipper_color = "#4E8F57";

// ---------------------------------------------------------------- Conjunto

// Isopor com o furo do pote
color(raft_color)
    linear_extrude(height = raft_thickness)
        difference() {
            raft_outline(raft_length, raft_width, raft_shape_n);
            pot_hole(pot_length, pot_width, pot_corner_radius, pot_taper,
                     pot_height, raft_thickness, pot_hole_clearance);
        }

// Pote (casca translúcida pra ver por dentro)
color(pot_color) pot_body();

// Tampa e carapaça colada nela
translate([0, 0, pot_height + explode]) {
    color(lid_color) pot_lid();
    translate([0, 0, lid_height]) {
        color(carapace_color) pot_carapace_body();
        color(scute_color) pot_carapace_scutes();
    }
}

// Motores, suportes e rodas: o do outro lado é o mesmo, girado
for (side = [-1, 1])
    translate([shaft_x, side * raft_edge_y, 0])
        rotate([0, 0, side > 0 ? 0 : 180]) {
            color(mount_color)
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
            translate([0, motor_center_y - raft_edge_y, shaft_z]) tt_motor();
            color(flipper_color)
                translate([0, wheel_inner_y - raft_edge_y, shaft_z])
                    rotate([-90, 0, 0])
                        paddle_wheel(radius = wheel_radius, width = wheel_width,
                                     shaft_diameter = tt_shaft_diameter,
                                     shaft_flat = tt_shaft_flat,
                                     clearance = shaft_clearance);
        }

// Dentro do pote: bateria no fundo, placa em cima
translate([0, 0, pot_wall]) {
    for (sy = [-1, 1])
        translate([-25, sy * (holder_width / 2 + 0.5), 0])
            battery_holder_with_cell();
    translate([10, 0, holder_height + 3]) populated_board();
}

// Cabeça na proa, colada na frente do isopor
translate([raft_length / 2 + 6, 0, raft_thickness / 2 + 2]) {
    color(skin_color) head_skin();
    color(bandana_color) head_bandana();
    color("#222222") head_eyes();
}

// Água até a linha d'água estimada
if (show_water && explode == 0)
    color([0.50, 0.72, 0.93, 0.30])
        translate([-200, -200, -20]) cube([400, 400, waterline_z + 20]);

// ---------------------------------------------------------------- Módulos

module pot_body() {
    difference() {
        linear_extrude(height = pot_height, scale = pot_taper)
            scale([1 / pot_taper, 1 / pot_taper])
                rounded_rect(pot_length, pot_width, pot_corner_radius);
        translate([0, 0, pot_wall])
            linear_extrude(height = pot_height, scale = pot_taper)
                scale([1 / pot_taper, 1 / pot_taper])
                    offset(delta = -pot_wall)
                        rounded_rect(pot_length, pot_width,
                                     pot_corner_radius);
    }
}

module pot_lid() {
    difference() {
        linear_extrude(height = lid_height)
            rounded_rect(pot_length + 2 * lid_overhang,
                         pot_width + 2 * lid_overhang,
                         pot_corner_radius + lid_overhang);
        translate([0, 0, -1])
            linear_extrude(height = lid_height - 1.5)
                rounded_rect(pot_length, pot_width, pot_corner_radius);
    }
}
