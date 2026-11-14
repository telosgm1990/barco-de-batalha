# CLAUDE.md — docs/

Regras gerais em [../CLAUDE.md](../CLAUDE.md). Aqui só o que é da documentação.

## Qual documento é o quê

- **[regulamento.md](regulamento.md) é a fonte da verdade das regras.** É a
  transcrição do banner. **Não reabra a imagem** pra consultar regra — leia o
  `.md`. Só volte à imagem se o texto parecer errado.
  - Só entra lá o que a organização disse. Palpite vai pro esboço.
  - Resposta nova da organização: suba pra seção certa e **tire a pergunta** da
    seção 6.
- **[esbocando-a-ideia.md](esbocando-a-ideia.md) é esboço declarado.** Decisão
  fechada é marcada **na própria D** (seção 11) com a data: "**Decidido em
  DD/MM/AAAA:** ...". Não implemente a partir de D aberta.
- **[lista-de-materiais.md](lista-de-materiais.md) é a conta.** Desde a compra
  de 14/11/2026, é o orçamento: o que foi comprado, o que vai no barco e quanto
  entra na conta. Preço vem da nota, que nunca vem pro git. Componente novo no
  barco entra aqui na mesma hora.
- **[plano-do-casco.md](plano-do-casco.md)** compara os dois caminhos do casco;
  a escolha vai pra D2 do esboço.
- **[montagem.md](montagem.md)** mostra como os módulos se ligam, com a
  ilustração. Ela **copia o esquemático, nunca o contrário**.

## Ilustração de montagem

O Capitão gosta do visual dos diagramas de montagem do Cirkit Designer. A gente
copia o visual, não a ferramenta (ela não é aberta nem tem CLI):

- **SVG em [imagens/](imagens/)** — o GitHub mostra, e o git versiona como
  texto. Dá pra editar à mão.
- **Fundo cinza claro; placas com cantos arredondados** e sombra leve; pinos
  como bolinha com **anel dourado**.
- **Fios grossos, com curva arredondada nas quinas**, em **cores pastel** mas
  legíveis. Cor fixa por função: V+ coral, GND grafite, sinais em azul, lilás,
  verde e laranja, leitura da bateria rosa, motores âmbar e marrom.
- **Fio que cruza outro** ganha uma borda da cor do fundo, pra parecer que
  passa por cima. **Junção** leva bolinha.
- **Etiquetas em pílula pastel** e uma legenda das cores.
- **Pino que ainda não está no esquemático** leva `*` e nota de rodapé.

## Como escrever aqui

- Informal e natural, com trocadilho quando couber (ver o CLAUDE.md da raiz).
  Informal não é bagunçado: tabela quando ajuda a achar número, prosa quando
  ajuda a entender a escolha.
- Número sem fonte ganha etiqueta: *estimativa*, *chute*, *a medir*.
- O dono do projeto é **o Capitão**. Ninguém mais tem nome aqui — quem imprime
  é "o dono da impressora", quem organiza é "a organização".
- Nome de arquivo de documento: `hyphen-case.md`, em português.
- Imagem vai em [imagens/](imagens/).

## Nunca aqui

- Nota fiscal, comprovante, foto de documento — têm CPF, nome e endereço.
  `notas-fiscais/` e `comprovantes/` estão no `.gitignore`.
- Caminho da máquina de ninguém, e-mail, telefone.
