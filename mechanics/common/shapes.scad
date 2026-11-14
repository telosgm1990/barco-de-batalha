// Formas 2D reaproveitadas pelas peças. Use com `use <common/shapes.scad>`.

// Superelipse: n = 2 é elipse; quanto maior o n, mais "quadrada" a borda.
function superellipse_points(a, b, n, steps = 120) = [
    for (i = [0:steps - 1])
        let(t = 360 * i / steps, c = cos(t), s = sin(t))
            [a * sign(c) * pow(abs(c), 2 / n), b * sign(s) * pow(abs(s), 2 / n)]
];

// Contorno de casco: superelipse com comprimento e largura totais
module hull_outline(length, width, n) {
    polygon(superellipse_points(length / 2, width / 2, n));
}

// Retângulo de cantos arredondados, centrado
module rounded_rect(length, width, radius) {
    offset(r = radius) square([length - 2 * radius, width - 2 * radius],
                              center = true);
}

// Linhas de uma grade hexagonal — os gomos do casco da tartaruga. Colunas ao
// longo de Y; gire 90° pra deixar a fileira do meio ao longo do barco.
module hex_grid_lines(size, ridge_width) {
    r = size / 2;
    w = ridge_width / cos(30);
    for (i = [-7:7], j = [-7:7])
        translate([i * 1.5 * r,
                   j * sqrt(3) * r + (abs(i) % 2) * sqrt(3) * r / 2])
            difference() {
                circle(r = r + w / 2, $fn = 6);
                circle(r = r - w / 2, $fn = 6);
            }
}

// Furo de eixo "D duplo" do motor TT: redondo cortado por duas faces planas
module double_d_hole(diameter, flat) {
    intersection() {
        circle(d = diameter);
        square([flat, diameter], center = true);
    }
}
