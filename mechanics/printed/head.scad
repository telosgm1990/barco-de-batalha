// Cabeça — a DonaTelos olhando pra frente, de bandana.
//
// Cola na proa do plastrão, em cima do bico de cunha. A bandana é um relevo
// de 0,8 mm pra pintar de roxo (ou imprimir em roxo, se o dono da impressora
// trocar o filamento na camada certa). Imprime deitada na face plana de trás.

include <hull_params.scad>

$fa = 3;
$fs = 0.5;

// ---------------------------------------------------------------- Medidas

// Face plana de trás, onde a cabeça cola no casco (em relação ao centro)
head_back_cut = -9;
snout_size = [18, 16, 12];
bandana_bottom = 0; // em relação ao centro da cabeça
bandana_top = 6;
bandana_relief = 0.8;
eye_spacing = 14;

// ---------------------------------------------------------------- Peça

// Deitada na face de trás, pronta pra fatiar
rotate([0, -90, 0])
    translate([-head_back_cut, 0, 0])
        head();

// ---------------------------------------------------------------- Módulos

// Origem no centro da cabeça; o focinho aponta pra +X
module head() {
    head_skin();
    head_bandana();
}

module head_skin() {
    intersection() {
        head_volume(0);
        back_cut();
    }
}

module head_bandana() {
    intersection() {
        head_volume(bandana_relief);
        back_cut();
        translate([-50, -50, bandana_bottom])
            cube([100, 100, bandana_top - bandana_bottom]);
    }
}

// Só pra vista do conjunto: olhos dentro dos furos da bandana
module head_eyes() {
    for (sy = [-1, 1])
        translate([head_size[0] / 2 - 4, sy * eye_spacing / 2, 3])
            sphere(d = 4.5);
}

module head_volume(grow) {
    hull() {
        resize([head_size[0] + 2 * grow, head_size[1] + 2 * grow,
                head_size[2] + 2 * grow])
            sphere(r = 1, $fn = 64);
        translate([head_size[0] / 2 - 4, 0, -2])
            resize([snout_size[0] + 2 * grow, snout_size[1] + 2 * grow,
                    snout_size[2] + 2 * grow])
                sphere(r = 1, $fn = 48);
    }
}

module back_cut() {
    translate([head_back_cut, -50, -50]) cube([100, 100, 100]);
}
