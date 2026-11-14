// Medidas compartilhadas da DonaTelos, em milímetros.
//
// Cada número diz de onde veio. Sem fonte, leva "a medir" ou "chute" — e
// vira medida de paquímetro assim que a peça chegar.

// ---------------------------------------------------------------- Impressora

// Mesa da mini do dono da impressora (informado em 13/11/2026)
bed_size = 180;
// Altura máxima de impressão — a confirmar com o dono da impressora
bed_height = 180; // a medir

// ---------------------------------------------------------------- Folgas

// Folga de encaixe entre peças — chute; calibrar com uma peça de teste
fit_clearance = 0.3;
// Folga do furo do eixo na roda de pá — chute; o eixo deve entrar justo
shaft_clearance = 0.15;

// ---------------------------------------------------------------- Motor TT

// Fonte: desenho do fabricante na página do motor TT 1:48 da Adafruit
// (produto 3777). Conferir com paquímetro quando o motor chegar.
tt_gearbox_length = 37.0; // caixa amarela, sem a orelha
tt_tab_length = 4.8; // orelha de fixação na ponta da caixa
tt_total_length = 70.0; // do fundo do motor até a ponta da orelha
tt_height = 22.44; // altura da caixa (perpendicular ao eixo)
tt_thickness = 18.6; // espessura da caixa (ao longo do eixo)
tt_can_height = 22.4; // motor prateado, mesma direção da altura
tt_can_thickness = 19.0; // motor prateado, mesma direção da espessura
tt_shaft_from_end = 14.0; // centro do eixo até a ponta da caixa (sem orelha)
tt_shaft_diameter = 5.4; // eixo, no redondo
tt_shaft_flat = 3.7; // eixo, entre as duas faces planas
tt_shaft_length = 8.7; // quanto o eixo sai da caixa (o lado mais curto)
tt_shaft_length_long = 9.3; // o outro lado do eixo duplo
tt_hole_diameter = 3.0; // furo de fixação

// ---------------------------------------------------------------- Roda de pá

// Igual nos dois caminhos do casco
wheel_radius = 28; // chute — afinar no teste da bacia (quanto a pá molha)
wheel_width = 16; // largura da pá, ao longo do eixo

// ---------------------------------------------------------------- Eletrônica

// Célula 18650 GoldHill — página da loja: Ø18 x 65 mm
cell_diameter = 18;
cell_length = 65;
// Suporte de 1 célula — chute; medir quando chegar
holder_length = 77; // a medir
holder_width = 21; // a medir
holder_height = 19; // a medir
// Placa fenolite 5x7 cm — página da loja
board_length = 70;
board_width = 50;
board_thickness = 1.6; // chute — fenolite comum
board_hole_inset = 2.5; // a medir — furo do canto até a borda
// Altura das peças em cima da placa (ESP, ponte H, bornes) — chute
board_parts_height = 15; // a medir
// Chave gangorra KCD11 — recorte no painel
switch_cutout_length = 19; // a medir
switch_cutout_width = 13; // a medir
