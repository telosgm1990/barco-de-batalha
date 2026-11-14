// Vista do conjunto do caminho B (tudo impresso) — não é peça de imprimir.
//
// Monta plastrão, carapaça, cabeça, rodas, motores e eletrônica, pra ver como
// fica e conferir se tudo cabe. Use -D explode=60 pra levantar a carapaça e
// ver por dentro, e -D show_water=false pra tirar a água.

include <hull_params.scad>
use <plastron.scad>
use <carapace.scad>
use <head.scad>
use <../paddle_wheel.scad>
use <../common/reference_parts.scad>

$fa = 3;
$fs = 0.8;

explode = 0;
show_water = true;
show_carapace = true;

// ---------------------------------------------------------------- Cores

plastron_color = "#E9D8A6";
carapace_color = "#6BAF6B";
scute_color = "#4E8F57";
skin_color = "#7CC27C";
bandana_color = "#8E6CC9";
flipper_color = "#4E8F57";

// ---------------------------------------------------------------- Conjunto

color(plastron_color) plastron();

// Cabeça na proa
translate([head_x, 0, head_z]) {
    color(skin_color) head_skin();
    color(bandana_color) head_bandana();
    color("#222222") head_eyes();
}

if (show_carapace)
    translate([0, 0, hull_height + explode]) {
        color(carapace_color) carapace_body();
        color(scute_color) carapace_scutes();
    }

// Motores nos berços e rodas de pá nos eixos
for (side = [-1, 1])
    mirror([0, side < 0 ? 1 : 0, 0]) {
        translate([shaft_x, motor_center_y, shaft_z]) tt_motor();
        color(flipper_color)
            translate([shaft_x, wheel_inner_y, shaft_z])
                rotate([-90, 0, 0]) paddle_wheel();
    }

// Baterias atravessadas no fundo (lastro) e a placa em cima
for (sx = [-1, 1])
    translate([battery_center_x + sx * (holder_width / 2 + 0.5), 0,
               hull_floor])
        rotate([0, 0, 90]) battery_holder_with_cell();
translate([board_center_x, 0, hull_floor + board_post_height])
    populated_board();

// Água até a linha d'água estimada
if (show_water && explode == 0)
    color([0.50, 0.72, 0.93, 0.30])
        translate([-160, -160, -20]) cube([320, 320, waterline_z + 20]);
