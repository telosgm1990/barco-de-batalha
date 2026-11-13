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
| Hardware, software, mecânica | ❌ nada ainda |

## Estrutura

```
docs/          regulamento, esboço da ideia, imagens
hardware/      esquemático (KiCad) e pinagem
software/      firmware da ESP32-C3 (C++ / ESP-IDF) e a página de controle
mechanics/     modelos 3D (OpenSCAD) e as peças que foram impressas
scripts/       utilitários — por enquanto, o caçador de segredos
```

Cada pasta tem seu `README.md` (pra gente) e seu `CLAUDE.md` (pra IA).

## Documentação

| Documento | Conteúdo |
| --- | --- |
| [docs/regulamento.md](docs/regulamento.md) | 📜 As regras oficiais e o que falta perguntar pra organização |
| [docs/esbocando-a-ideia.md](docs/esbocando-a-ideia.md) | 🚧 Estratégia, a tartaruga, energia, controle, orçamento, fases e decisões |
| [CLAUDE.md](CLAUDE.md) | Instruções pra IA trabalhando aqui |

## Ambiente

Windows + PowerShell, num PC que também é de trabalho — então **nada se instala
no sistema**. Toolchain pesada roda em Docker; dependência Node vai pro
`node_modules` local; dependência Python, pro `scripts/.venv`:

```bash
powershell -ExecutionPolicy Bypass -File scripts/install-python-venv.ps1
```

## Git

| O quê | Como |
| --- | --- |
| Branches | `main` + uma por assunto, em inglês (`creating-dev-environment`, `implementing-flippers`) |
| Merge na `main` | Sempre 3-way: `git merge --no-ff` |
| Mensagem de commit | Em português: título + corpo curto com as principais mudanças |
| Antes de commitar | `scripts/.venv/Scripts/check-secrets` — ou ative o hook (ver [scripts/](scripts/README.md)) |
| Co-autoria | Commit feito pela IA leva `Co-Authored-By` — é vibado, afinal |

## Segredos

O repositório é tratado como público, sempre. Credencial fica em `.env` (ou
`secrets.*`), que está no `.gitignore`; no git vai o `.example`, com valor
falso. Nota fiscal vai pro juiz, nunca pro git — tem CPF, nome e endereço.
