// Medidas do caminho A (pote e isopor), compartilhadas entre o suporte do
// motor, a carapaça, o molde do isopor e a vista do conjunto.
//
// O pote é o da foto em docs/imagens/foto-pote.jpeg: as medidas dele são
// chute pela foto — trocar pelas da régua.

include <../common/dimensions.scad>

// ---------------------------------------------------------------- Pote

pot_length = 155; // chute pela foto — a medir
pot_width = 110; // chute pela foto — a medir
pot_height = 55; // corpo, sem a tampa — chute pela foto, a medir
pot_corner_radius = 15; // chute
pot_taper = 1.06; // a boca é um pouco maior que o fundo — chute
pot_wall = 1.2; // chute
lid_height = 8; // chute
lid_overhang = 4; // a aba da tampa passa da boca — chute

// ---------------------------------------------------------------- Isopor

raft_length = 230; // chute — depende da placa de isopor que aparecer
raft_width = 170; // chute
raft_thickness = 20; // a medir — placa comum de isopor
// Superelipse: forma de casco, com a lateral quase reta onde o suporte do
// motor encosta
raft_shape_n = 3.5;

// O pote encaixa num furo do isopor, com o fundo rente ao fundo do isopor.
// O furo tem a medida do pote na altura do topo do isopor: pela conicidade,
// o pote entra apertado.
pot_hole_clearance = 1;

// ---------------------------------------------------------------- Motores

// Suporte impresso na borda do isopor, de cada lado, no meio do comprimento
shaft_x = 0;
mount_clearance = 0.6;
mount_wall = 2.4;
mount_flange_length = 22; // aba que deita em cima do isopor
shaft_above_raft = 4; // o eixo fica um pouco acima do topo do isopor

// Meia-largura do isopor no meio do comprimento é raft_width / 2
raft_edge_y = raft_width / 2;
shaft_z = raft_thickness + shaft_above_raft;
// Face de dentro do motor: o eixo de dentro não encosta no isopor
motor_inner_y = raft_edge_y + 1 + tt_shaft_length_long;
motor_center_y = motor_inner_y + max(tt_thickness, tt_can_thickness) / 2;
gearbox_outer_y = motor_center_y + tt_thickness / 2;
wheel_inner_y = gearbox_outer_y + 1;

// ---------------------------------------------------------------- Carapaça

// Vai colada em cima da tampa do pote: abriu a tampa, abriu a tartaruga.
// O ar preso entre ela e a tampa é o que faz o barco desvirar.
shell_length = 176; // cabe na mesa de 18 cm
shell_width = 146;
shell_height = 34;
shell_shape_n = 3.5; // superelipse: cobre os cantos da tampa retangular
shell_thickness = 2.0; // chute
shell_base = 2.0; // anel da base, que cola na tampa
shell_opening_margin = 10; // o anel entra isso pra dentro da borda da tampa
scute_size = 22;
scute_ridge_width = 1.6;
scute_ridge_height = 1.2;

// ---------------------------------------------------------------- Água

// Linha d'água — estimativa: barco de ~310 g (impresso ~100 g, pote e tampa
// ~50 g de chute, isopor, uma célula, motores e placa) desloca isso com
// ~9 mm afundados, pelo volume do isopor e do pote no modelo. Sem a
// carapaça (Fase 1), ~7 mm. A bacia confirma.
waterline_z = 9; // estimativa
