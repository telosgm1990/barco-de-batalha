// Plastrão — a casca de baixo da DonaTelos no caminho B (tudo impresso).
//
// Uma banheira com bico de cunha na proa e um flutuador de cada lado, onde o
// motor encaixa por cima. O vão de dentro é o cofre: baterias no fundo
// (lastro) e a placa em cima delas. A carapaça fecha por cima.
//
// Imprime de boca pra cima, sem suporte. Os flutuadores são maciços no
// modelo: o preenchimento do fatiador vira câmara de ar. Pedir paredes
// grossas (vedação) — ver docs/plano-do-casco.md.

include <hull_params.scad>
use <../common/shapes.scad>

$fa = 3;
$fs = 0.6;

// ---------------------------------------------------------------- Peça

plastron();

// ---------------------------------------------------------------- Módulos

module plastron() {
    difference() {
        union() {
            tub_outer();
            bow_wedge();
            for (side = [-1, 1])
                sponson(side);
        }
        tub_cavity();
        for (side = [-1, 1])
            motor_cuts(side);
    }
    rim_lip();
    board_posts();
    battery_rails();
}

// Contorno do fundo; o topo é ele aumentado de hull_flare
module bottom_outline() {
    hull_outline(hull_length / hull_flare, hull_width / hull_flare,
                 hull_shape_n);
}

module tub_outer() {
    linear_extrude(height = hull_height, scale = hull_flare)
        bottom_outline();
}

module tub_cavity() {
    translate([0, 0, hull_floor])
        intersection() {
            linear_extrude(height = hull_height, scale = hull_flare)
                offset(r = -hull_wall) bottom_outline();
            translate([-hull_length, -hull_width, 0])
                cube([2 * hull_length, 2 * hull_width, hull_height]);
        }
}

// Aba que sobe por dentro da borda e centraliza a carapaça
module rim_lip() {
    top_wall = hull_wall * hull_flare;
    translate([0, 0, hull_height - 4])
        linear_extrude(height = 4 + lip_height)
            difference() {
                offset(delta = -top_wall + 0.3)
                    hull_outline(hull_length, hull_width, hull_shape_n);
                offset(delta = -top_wall - lip_thickness)
                    hull_outline(hull_length, hull_width, hull_shape_n);
            }
}

// Bico de cunha: baixo e largo, pra entrar por baixo do adversário
module bow_wedge() {
    bow_x = hull_length / 2 / hull_flare;
    hull() {
        translate([bow_x - 8, -wedge_base_width / 2, 0])
            cube([0.1, wedge_base_width, wedge_height]);
        translate([bow_x + wedge_length, -wedge_tip_width / 2, 0])
            cube([0.1, wedge_tip_width, 1.6]);
    }
}

// Flutuador lateral: maciço, com o topo rente à borda e a lateral de fora
// inclinada, que nem o resto do casco
module sponson(side) {
    mirror([0, side < 0 ? 1 : 0, 0])
        hull() {
            translate([0, 0, hull_height - 0.01])
                linear_extrude(height = 0.01) sponson_outline(0);
            linear_extrude(height = 0.01) sponson_outline(sponson_taper);
        }
}

module sponson_outline(inset) {
    x_min = motor_back_x - motor_clearance - pod_outer_wall;
    x_max = motor_front_x + motor_clearance + pod_outer_wall;
    y_in = hull_width / 2 / hull_flare - 6; // entra no casco e funde
    hull() {
        translate([x_min + inset, y_in])
            square([x_max - x_min - 2 * inset, 1]);
        for (x = [x_min + pod_end_radius + inset,
                  x_max - pod_end_radius - inset])
            translate([x, pod_outer_y - pod_end_radius - inset])
                circle(r = pod_end_radius);
    }
}

// Berço do motor, rasgo do eixo de dentro, caixa da roda e furo dos fios
module motor_cuts(side) {
    c = motor_clearance;
    mirror([0, side < 0 ? 1 : 0, 0]) {
        // Berço: aberto em cima, o motor desce encaixando
        translate([motor_back_x - c, motor_inner_y - c,
                   shaft_z - tt_height / 2 - c])
            cube([motor_front_x - motor_back_x + 2 * c,
                  motor_outer_y - motor_inner_y + 2 * c, hull_height]);
        // Rasgo do eixo de dentro, pra ele girar livre
        translate([shaft_x - 4.5, motor_inner_y - tt_shaft_length_long - 1,
                   shaft_z - 4.5])
            cube([9, tt_shaft_length_long + 2, hull_height]);
        // Caixa da roda: a roda fica rente à caixa do motor
        translate([shaft_x, wheel_inner_y - 0.5, shaft_z])
            rotate([-90, 0, 0])
                cylinder(r = wheel_radius + wheel_well_margin, h = 40);
        // Furo dos fios, do berço pra dentro do casco, perto do motor
        translate([motor_back_x + 6, motor_inner_y + 1, shaft_z])
            rotate([90, 0, 0])
                cylinder(d = wire_hole_diameter, h = 20);
    }
}

// Colunas da placa, fora da área das baterias
module board_posts() {
    dx = board_length / 2 - board_hole_inset;
    dy = board_width / 2 - board_hole_inset;
    for (sx = [-1, 1], sy = [-1, 1])
        translate([board_center_x + sx * dx, sy * dy, hull_floor - 0.1])
            difference() {
                cylinder(d = board_post_diameter, h = board_post_height + 0.1);
                cylinder(d = board_post_hole, h = board_post_height + 1);
            }
}

// Trilhos baixos que seguram os dois suportes de bateria lado a lado
module battery_rails() {
    rail_length = holder_length - 16;
    for (sx = [-1, 1])
        translate([battery_center_x + sx * (holder_width + 1.6 / 2 + 0.5)
                       - 0.8,
                   -rail_length / 2, hull_floor - 0.1])
            cube([1.6, rail_length, 4.1]);
}
