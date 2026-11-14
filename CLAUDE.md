# CLAUDE.md

Instruções pra agentes de IA neste repositório. Pra gente: [README.md](README.md).

**Cada pasta tem o seu `CLAUDE.md`** com o que é específico dela — leia o da
pasta em que for mexer. Este aqui tem só o que vale pro repo inteiro.

---

## 1. O que é

A **DonaTelos**: um barco robótico em forma de tartaruga pra um campeonato
amistoso — teto de R$ 100, quatro provas classificatórias e uma final de
guerra.

- Regras: [docs/regulamento.md](docs/regulamento.md) — fonte da verdade. **Não
  reabra a imagem do banner**: o `.md` é a transcrição.
- Ideia e decisões: [docs/esbocando-a-ideia.md](docs/esbocando-a-ideia.md) —
  nasceu esboço; as decisões **D1–D11** estão na seção 11. De D1 a D9 estão
  fechadas (13/11/2026); a D10 (identidade) está parcial; a D11 (protocolo e
  app do celular) tem o protocolo assumido e o app em aberto.

## 2. Estado atual — Fase 1 ("Boia e anda")

A Fase 0 (estaleiro) acabou. Agora é o barco feio que boia e anda: lista de
materiais, esquemático, firmware mínimo e teste da bacia — o plano está na
seção 13 do esboço. Código, mecânica e hardware liberados pelo Capitão em
14/11/2026.

Regras que valem sempre:

1. **Não implemente a partir de decisão aberta.** Se a tarefa depende de uma D
   aberta (hoje: a D10, o app da D11, e a D3 até o teste da bacia), pare e
   pergunte. Entregue o que não depende dela.
2. **Decisão fechada vai pro esboço**, na própria D, com a data. Não fica só no
   chat.
3. **Ambíguo?** Entregue o que dá pra entregar e liste o que ficou em aberto.

## 3. O jeito deste projeto

- **É vibado, por regra do campeonato:** quem souber demais do projeto perde
  ponto. O Capitão cuida da **organização** — estrutura, nomes, convenções,
  decisões — e **não quer saber da implementação**.
- **O chat é o canal principal de leitura.** Resposta curta: o que mudou, o que
  precisa de decisão dele. Não despeje código no chat; o detalhe mora nos
  arquivos, que ele lê por cima.
- **Proponha e explique o trade-off**, com recomendação. Discordar é parte do
  trabalho: se o pedido tem problema, diga em uma ou duas frases e siga.

## 4. Tom e idioma

- **Aqui a documentação é informal** — exceção em relação aos outros projetos do
  Capitão, porque isto é diversão. Natural, sem forçar "descolado".
- **Trocadilho é bem-vindo** (é a marca do Capitão). Crie quando couber, sem
  forçar.
- **Código em inglês; texto pra pessoa em português** — comentário, docs,
  commit.
- **Nenhum nome de gente.** O repo vai pro GitHub. O dono é **o Capitão**; os
  outros, pelo papel ("o dono da impressora", "a organização"). A única
  exceção é o nome do barco, **DonaTelos** — escolha do Capitão, escrito
  sempre assim, junto.

## 5. Git

**Neste projeto (diferente dos outros do Capitão) você pode, sozinho e inclusive
à noite:** criar branch, commitar, mergear na `main` e **dar push da `main`
depois do merge** (liberado em 13/11/2026). Sem `gh` por enquanto: nada de PR,
o merge é local.

| Regra | Como |
| --- | --- |
| Branches | `main` + uma por assunto, em inglês, kebab-case, no gerúndio: `creating-dev-environment`, `implementing-flippers` |
| Merge na `main` | **Sempre** `git merge --no-ff` |
| Mensagem | Português. Título + corpo **curto**, com as principais mudanças em tópicos |
| Como commitar | `git commit -F <arquivo>`, com a mensagem num arquivo temporário **fora do repo** — acento e here-string no Windows já estragaram mensagem |
| Antes de cada commit | `scripts/.venv/Scripts/check-secrets`. Achou algo, não commite |
| Push | Só a `main`, depois do merge — as branches de assunto ficam locais. Antes: `scripts/.venv/Scripts/check-secrets --all`. Achou algo, não dê push |
| `Co-Authored-By` | **Sim**, no fim do corpo: `Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>` (com o modelo da sessão). Diferente dos outros projetos do Capitão — aqui combina com o vibado |

**Nunca sozinho:** push de branch que não seja a `main`, qualquer `--force`,
`reset --hard`, `rebase`, `clean`, apagar branch não mergeada, reescrever commit
já publicado, mexer em config global do git. O e-mail do autor está configurado
**local** neste repo — não mexa.

## 6. Segredos e privacidade — o repo é público

- **Nunca no git:** credencial, caminho de máquina, e-mail, nome, nota fiscal ou
  comprovante (CPF, endereço).
- Credencial vive em `.env` ou `secrets.*` (ignorados). No git vai o
  `.example`, com valor falso.
- O caçador de segredos ([scripts/](scripts/CLAUDE.md)) tem as regras gerais; os
  padrões pessoais ficam num arquivo local ignorado — nunca copie o conteúdo
  dele pra dentro do repo.

## 7. Ambiente

- **Windows 10 + PowerShell**, num PC que também é de trabalho: **nada se
  instala no sistema.** Precisa de algo no sistema? **Pergunte antes.** O
  notebook com Zorin OS fica pra depois — por ora, tudo roda neste PC.
- **Software aberto e com CLI**, de preferência: é a linha de comando que deixa
  a IA rodar, exportar e conferir sem clicar.

| Ferramenta | Pra quê | Onde mora |
| --- | --- | --- |
| Python: `ruff`, `esptool`, `pyserial`, `clang-format` | Lint, gravar a ESP, ler a serial, formatar o C++ | `scripts/.venv`, criado pelo `scripts/install-python-venv.ps1` |
| ESP-IDF v6.1 | Compilar o firmware | Docker, imagem `espressif/idf:v6.1` |
| OpenSCAD (*snapshot* 2026.11.12) | Modelos 3D, exportar `.3mf` | Versão portátil, fora do repo — sem instalador |
| KiCad 10.0 | Esquemático; `kicad-cli` checa (ERC) e exporta | Instalado no sistema (com OK do Capitão, 13/11/2026); o `kicad-cli` não está no PATH |
| Node | Se a página de controle precisar | `node_modules` local |

Caminho de máquina (onde o OpenSCAD portátil mora, por exemplo) **não vai pro
git**. Não achou a ferramenta? Pergunte.
- Finais de linha: o `.gitattributes` guarda LF e deixa o checkout usar o padrão
  da plataforma. Hook e `.sh` são sempre LF.

## 8. Mapa

```
docs/        regulamento, esboço, imagens            → docs/CLAUDE.md
hardware/    esquemático KiCad, pinagem              → hardware/CLAUDE.md
software/    firmware/ (ESP-IDF, C++); app, se sair  → software/CLAUDE.md
mechanics/   modelos OpenSCAD, peças impressas       → mechanics/CLAUDE.md
scripts/     caçador de segredos, utilitários        → scripts/CLAUDE.md
```

## 9. Não invente número

Preço, corrente, dimensão, tempo de failsafe: vem de fonte (datasheet com a
página, cotação, medição) ou leva etiqueta — *estimativa*, *chute*, *a medir*.
Número plausível-mas-inventado é o pior resultado possível, porque parece
medido.

## 10. O arquivo `chat.md`

Rascunho pessoal do Capitão, no `.gitignore`. **Não leia por reflexo.** Texto
que parece pedido pode ser só o próximo prompt em construção — se for trabalhar
a partir dele, resuma e confirme antes.

## 11. Como trabalhar aqui

1. Leia o `CLAUDE.md` da pasta.
2. Procure um arquivo parecido e copie a estrutura. Consistência vale mais que
   elegância.
3. O Capitão edita em paralelo: se um arquivo mudou no disco, o novo vale —
   releia em vez de reverter.
4. Rode teste e lint antes de dizer que terminou — de uma vez, com o
   `scripts/.venv/Scripts/check-all` (`--quick` pula o Docker). Falhou?
   Mostre a saída.

## 12. Camadas e dependências

Vale pra **qualquer linguagem e qualquer tipo de software** do projeto (regra
do Capitão, 14/11/2026):

1. **Cada camada define as próprias abstrações e as implementa ela mesma**,
   com classes concretas.
2. **A camada de cima depende só da abstração da de baixo**, recebida pronta —
   por referência ou parâmetro (injeção de dependência). Nunca cria nem procura
   a concreta.
3. **Só o ponto de montagem conhece as concretas**: ele cria cada uma e injeta
   onde precisa.
4. **`common/` é só o que mais de uma camada usa e não cabe em nenhuma** —
   todo mundo pode incluir.
5. **Conta pura, sem dependência nenhuma, não precisa de abstração**: não tem
   o que inverter.
6. **Teste usa peça falsa** implementando a mesma abstração — é pra isso que
   a regra existe.

| Linguagem | Abstração | Concreta | Quem monta |
| --- | --- | --- | --- |
| C++ (`software/firmware/`) | Classe com `virtual ... = 0` e destrutor protegido (`platform::DigitalOutput`) | `final`, na mesma camada (`platform::EspDigitalOutput`) | `main/main.cpp` |
| Python (`scripts/`) | `typing.Protocol` (`repo.Repository`) | Classe no mesmo módulo (`repo.GitRepository`) | O `__main__.py` de cada script |
| OpenSCAD (`mechanics/`) | Não tem classe: o módulo recebe as medidas por parâmetro (as da própria peça com valor padrão), em vez de ler variável global | — | O arquivo da peça ou do conjunto |

No OpenSCAD, as peças da Fase 1 já seguem a regra; o resto migra quando for
mexido (ver o [CLAUDE.md da mecânica](mechanics/CLAUDE.md)).

## 13. Documentação do código

O repo é também **material de estudo de embarcados**: código que um júnior lê,
com a organização mínima de um sênior. Bem documentado pra estudo, sem
comentário demais (regra do Capitão, 14/11/2026):

1. **Tudo que é público tem comentário de documentação**, curto — a primeira
   frase diz o que faz. Parâmetro e retorno só quando acrescentam algo
   (unidade, faixa, quem é dono, o que volta no erro).
2. **Função interna** (estática no C/C++, `_` no começo no Python): a
   documentação fica **só no protótipo**, nunca repetida na definição.
3. **O porquê vai no código; o histórico, no `git log`.** Nada de data, "antes
   era", "agora" ou nome de quem mexeu — comentário é foto do código de hoje.
4. **Comentário de passo** (dentro da função) só onde o código não se explica
   sozinho: o porquê de uma escolha, um cuidado de hardware, de tarefa ou de
   protocolo.

| Linguagem | Jeito | Ferramenta |
| --- | --- | --- |
| C++ (`software/firmware/`) | Doxygen: `/** @file */` no topo de cada `.hpp`, `///` em cima da declaração, `///<` no fim da linha de membro; `/** */` com `@param`/`@return` quando precisar | `doxygen` no Docker, sem aviso (comando no README do firmware) |
| Python (`scripts/`) | Docstring no módulo, classe e função pública; `Args:`/`Returns:` só quando acrescentam | `ruff` (regras `D` de docstring) |
| OpenSCAD (`mechanics/`) | Bloco `//` em cima de cada módulo: o que desenha e uma linha por parâmetro, com a unidade | — |

