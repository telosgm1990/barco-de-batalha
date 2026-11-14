// Medidas do caminho B (tudo impresso), compartilhadas entre plastrão,
// carapaça, cabeça e a vista do conjunto.
//
// O casco é dado no topo da borda; o fundo é menor (a lateral abre pra fora).

include <../common/dimensions.scad>

// ---------------------------------------------------------------- Plastrão

hull_length = 150; // no topo da borda
hull_width = 100; // no topo da borda
hull_height = 48; // chute — borda livre acima da linha d'água estimada
hull_flare = 1.12; // o topo é 12% maior que o fundo
hull_shape_n = 3; // superelipse: 2 é elipse, mais que isso fica "quadrado"
hull_wall = 2.4; // chute — parede grossa pra vedar melhor
hull_floor = 2.4;

// Encaixe da carapaça: aba que sobe por dentro da borda
lip_height = 5;
lip_thickness = 1.6;

// Bico de cunha na proa — o da tartaruga-de-pente, pra entrar por baixo
wedge_length = 24;
wedge_base_width = 44;
wedge_tip_width = 20;
wedge_height = 16;

// ---------------------------------------------------------------- Motores

// Os motores ficam em "flutuadores" laterais, com o topo rente à borda. O
// eixo de dentro gira num rasgo; o de fora leva a roda de pá.
motor_clearance = 0.6;
shaft_x = 16; // posição do eixo ao longo do barco (proa pra +X)
pod_outer_wall = 2.4;
pod_end_radius = 12; // ponta arredondada do flutuador
sponson_taper = 9; // o fundo do flutuador é menor que o topo
wheel_well_margin = 3; // folga entre a roda e o casco

// Altura do eixo: o motor fica com o topo rente à borda
shaft_z = hull_height - tt_height / 2 - motor_clearance;

// Motor visto de cima, em relação ao eixo
motor_back_x = shaft_x - (tt_total_length - tt_shaft_from_end - tt_tab_length);
motor_front_x = shaft_x + tt_shaft_from_end + tt_tab_length;

// Face de dentro do motor: o eixo de dentro (o mais longo) não encosta no
// casco, nem no topo da borda
motor_inner_y = hull_width / 2 + 1 + tt_shaft_length_long;
motor_center_y = motor_inner_y + max(tt_thickness, tt_can_thickness) / 2;
motor_outer_y = motor_center_y + max(tt_thickness, tt_can_thickness) / 2;
gearbox_outer_y = motor_center_y + tt_thickness / 2;
pod_outer_y = motor_outer_y + motor_clearance + pod_outer_wall;
// A roda encosta quase na caixa do motor, pra pegar o máximo de eixo
wheel_inner_y = gearbox_outer_y + 1;

// ---------------------------------------------------------------- Dentro

// Suportes de bateria atravessados (célula em Y), lado a lado em X
battery_center_x = -8;
// A placa fica em cima das baterias, presa em 4 colunas
board_center_x = -8;
board_post_height = holder_height + 4;
board_post_diameter = 6;
board_post_hole = 2.2; // parafuso auto-atarraxante pequeno ou abraçadeira
wire_hole_diameter = 5;

// ---------------------------------------------------------------- Carapaça

carapace_length = 172; // cabe na mesa de 18 cm, com folga pequena
carapace_width = 176;
carapace_height = 40;
carapace_shell = 2.0; // chute
carapace_flange = 2.0;
scute_size = 22; // tamanho dos "gomos" hexagonais do casco
scute_ridge_width = 1.6;
scute_ridge_height = 1.2;
switch_x = -52; // chave liga/desliga numa torrinha, na traseira

// ---------------------------------------------------------------- Cabeça

head_x = 82;
head_z = 34;
head_size = [30, 26, 22];

// ---------------------------------------------------------------- Água

// Linha d'água — estimativa: barco de ~330 g (impresso ~180 g fatiado,
// mais uma célula, dois motores e a placa) desloca isso com o casco afundado
// ~22 mm, pelo volume do modelo. Com duas células, ~25 mm. A bacia confirma.
waterline_z = 22; // estimativa
