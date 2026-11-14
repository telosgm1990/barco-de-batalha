// Modelos de referência — o que vai dentro do barco, mas não é impresso.
//
// Servem pra montar a vista do conjunto e conferir se tudo cabe. Medidas em
// dimensions.scad; caixa simples onde a forma não importa.

include <dimensions.scad>

// ---------------------------------------------------------------- Motor TT

// Origem no centro do eixo. Comprimento em X (a ponta da caixa, com a orelha,
// pra +X), eixo em Y, altura em Z.
module tt_motor() {
    gearbox_start = tt_shaft_from_end - tt_gearbox_length;
    can_length = tt_total_length - tt_gearbox_length - tt_tab_length;

    // Caixa de redução amarela
    color("#F6D877")
        translate([gearbox_start, -tt_thickness / 2, -tt_height / 2])
            cube([tt_gearbox_length, tt_thickness, tt_height]);
    // Orelha de fixação
    color("#F6D877")
        translate([tt_shaft_from_end, -tt_thickness / 2, -tt_height / 4])
            cube([tt_tab_length, tt_thickness, tt_height / 2]);
    // Motor prateado
    color("#CED2D8")
        translate([gearbox_start - can_length, -tt_can_thickness / 2,
                   -tt_can_height / 2])
            cube([can_length, tt_can_thickness, tt_can_height]);
    // Eixo duplo, branco
    color("#FFFFFF")
        rotate([90, 0, 0])
            linear_extrude(height = tt_thickness + 2 * tt_shaft_length_long,
                           center = true)
                intersection() {
                    circle(d = tt_shaft_diameter, $fn = 32);
                    square([tt_shaft_flat, tt_shaft_diameter], center = true);
                }
}

// ---------------------------------------------------------------- Bateria

// Suporte de uma célula, com a célula dentro. Origem no centro, em cima do
// chão; comprimento em X.
module battery_holder_with_cell() {
    color("#3B3C42")
        difference() {
            translate([-holder_length / 2, -holder_width / 2, 0])
                cube([holder_length, holder_width, holder_height]);
            translate([-cell_length / 2, -cell_diameter / 2 - 0.5, 2])
                cube([cell_length, cell_diameter + 1, holder_height]);
        }
    color("#A3A6EA")
        translate([0, 0, 2 + cell_diameter / 2])
            rotate([0, 90, 0])
                cylinder(d = cell_diameter, h = cell_length, center = true,
                         $fn = 32);
}

// ---------------------------------------------------------------- Placa

// Placa 5x7 com a ESP, a ponte H e o carregador. Origem no centro, embaixo.
module populated_board() {
    color("#C9A66B")
        translate([-board_length / 2, -board_width / 2, 0])
            cube([board_length, board_width, board_thickness]);
    translate([0, 0, board_thickness]) {
        // ESP32-C3 SuperMini (22,52 x 18 mm), em cima da barra de pinos
        color("#3F4148") translate([-30, 5, 8]) cube([22.5, 18, 3]);
        // Ponte H (medida a confirmar)
        color("#EE8E8A") translate([2, 4, 8]) cube([24, 21, 3]);
        // TP4056 (26 x 17 x 5 mm, página da loja)
        color("#8CC0EA") translate([-30, -22, 0]) cube([26, 17, 5]);
        // Bornes
        color("#7FB3E6")
            for (x = [8, 18, 28])
                translate([x - 4, -22, 0]) cube([8, 7.5, 10]);
        // Capacitores
        color("#4E5876")
            for (x = [-1, 4])
                translate([x, -2, 0]) cylinder(d = 5, h = 11, $fn = 16);
    }
}
