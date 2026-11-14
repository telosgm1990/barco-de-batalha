# barco-de-batalha

A **DonaTelos** — nosso barco, ou melhor, nossa tartaruga — pro **0º Campeonato
Internacional de Batalha Naval de São Carlos**: um campeonato amistoso de barcos
robóticos, com teto de R$ 100 por barco, quatro provas classificatórias e uma
final de guerra.

> 🚧 **Fase 1: boia e anda.** O estaleiro fechou: estrutura pronta e decisões
> D1–D9 tomadas, em [docs/esbocando-a-ideia.md](docs/esbocando-a-ideia.md).
> Agora é o barco feio que boia, anda e grava o vídeo.

## A ideia em um parágrafo

Uma tartaruga ninja — a da tecnologia, claro. Carapaça em domo, que desvia
pancada e desvira sozinha. Corpo
largo, que não capota. Um bico de cunha pra entrar por baixo do adversário e
virar ele. Rodas de pá com cara de nadadeira. Tudo que é oco cheio de isopor,
pra não afundar nem furada. Simples, barata e teimosa — *devagar se vai ao
fundo*. O do adversário, no caso.

## Como este projeto é feito

Na base do **vibe coding**, por regra do próprio campeonato: quem souber demais
do projeto perde ponto. Código, modelo 3D e esquemático são escritos por IA
(Claude Code). O Capitão decide, organiza, monta e pilota.

## Estado

| Item | Estado |
| --- | --- |
| Estrutura do repositório | ✅ pronta |
| Regulamento transcrito, com as perguntas pra organização | ✅ |
| Esboço da ideia | ✅ D1–D9 fechadas; D10 (identidade) parcial |
| Caçador de segredos (pré-commit) | ✅ |
| Ambiente de desenvolvimento (ESP-IDF, OpenSCAD, KiCad) | ✅ montado |
| [Lista de materiais da Fase 1](docs/lista-de-materiais.md) | 📦 comprada em 14/11; entrega estimada pra 24/11 |
| [Plano do casco](docs/plano-do-casco.md) | 🚧 caminho A (pote e isopor) por ora |
| [Mecânica](mechanics/README.md) | ✅ os dois caminhos projetados em OpenSCAD, com renders; medidas do pote e do motor *a medir* |
| [Esquemático e pinagem](hardware/README.md) | ✅ KiCad 10, ERC limpo |
| [Montagem ilustrada](docs/montagem.md) | ✅ com a pinagem do esquemático |
| [Firmware](software/firmware/README.md) | ✅ Fase 1: Wi-Fi, joystick no celular, failsafe, motores e bateria — compila e passa nos testes; falta a placa |

## Estrutura

```
docs/          regulamento, esboço da ideia, imagens
hardware/      esquemático (KiCad) e pinagem
software/      um projeto por pasta: firmware/ (ESP32-C3, C++ / ESP-IDF, com a
               página de controle)
mechanics/     modelos 3D (OpenSCAD) e as peças que foram impressas
scripts/       utilitários — por enquanto, o caçador de segredos
```

Cada pasta tem seu `README.md` (pra gente) e seu `CLAUDE.md` (pra IA).

## Documentação

| Documento | Conteúdo |
| --- | --- |
| [docs/regulamento.md](docs/regulamento.md) | 📜 As regras oficiais e o que falta perguntar pra organização |
| [docs/esbocando-a-ideia.md](docs/esbocando-a-ideia.md) | 🚧 Estratégia, a tartaruga, energia, controle, orçamento, fases e decisões |
| [docs/bancada.md](docs/bancada.md) | 🔌 A primeira vez que liga: fonte de bancada, motores, failsafe e o que medir |
| [CLAUDE.md](CLAUDE.md) | Instruções pra IA trabalhando aqui |

## Ambiente

Windows + PowerShell, num PC que também é de trabalho — então **nada se instala
no sistema** sem combinar antes. A preferência é por software aberto e com
linha de comando, que a IA consegue usar sozinha.

| Ferramenta | Pra quê | Como chega |
| --- | --- | --- |
| [Docker](https://www.docker.com/) | Roda o ESP-IDF v6.1, que compila o firmware | Imagem `espressif/idf:v6.1` |
| Python 3.10+ | `esptool` (grava a ESP), `pyserial` (lê a serial), `clang-format`, `ruff` e os nossos scripts | `scripts/.venv`, com o comando abaixo |
| [OpenSCAD](https://openscad.org/) | Modelos 3D, exportados em `.3mf` | Versão portátil (*snapshot*), sem instalador |
| [KiCad](https://www.kicad.org/) 10 | Esquemático | Instalador oficial |

O ambiente Python, uma vez por máquina:

```bash
powershell -ExecutionPolicy Bypass -File scripts/install-python-venv.ps1
```

## Git

| O quê | Como |
| --- | --- |
| Branches | `main` + uma por assunto, em inglês (`creating-dev-environment`, `implementing-flippers`) |
| Merge na `main` | Sempre 3-way e local: `git merge --no-ff` |
| Push | Só a `main`, depois do merge, com `check-secrets --all` antes. Branch de assunto fica local |
| Mensagem de commit | Em português: título + corpo curto com as principais mudanças |
| Antes de commitar | `scripts/.venv/Scripts/check-secrets` — ou ative o hook (ver [scripts/](scripts/README.md)) |
| Co-autoria | Commit feito pela IA leva `Co-Authored-By` — é vibado, afinal |

## Segredos

O repositório é tratado como público, sempre. Credencial fica em `.env` (ou
`secrets.*`), que está no `.gitignore`; no git vai o `.example`, com valor
falso. Nota fiscal vai pro juiz, nunca pro git — tem CPF, nome e endereço.
