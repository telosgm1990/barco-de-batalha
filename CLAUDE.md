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
  nasceu esboço; as decisões **D1–D10** estão na seção 11. De D1 a D9 estão
  fechadas (13/11/2026); a D10 (identidade) está parcial.

## 2. Estado atual — Fase 1 ("Boia e anda")

A Fase 0 (estaleiro) acabou. Agora é o barco feio que boia e anda: lista de
materiais, esquemático, firmware mínimo e teste da bacia — o plano está na
seção 13 do esboço. Regras que valem sempre:

1. **Não implemente a partir de decisão aberta.** Se a tarefa depende de uma D
   aberta (hoje: a D10, e a D3 até o teste da bacia), pare e pergunte. Entregue
   o que não depende dela.
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
à noite:** criar branch, commitar e mergear na `main`.

| Regra | Como |
| --- | --- |
| Branches | `main` + uma por assunto, em inglês, kebab-case, no gerúndio: `creating-dev-environment`, `implementing-flippers` |
| Merge na `main` | **Sempre** `git merge --no-ff` |
| Mensagem | Português. Título + corpo **curto**, com as principais mudanças em tópicos |
| Como commitar | `git commit -F <arquivo>`, com a mensagem num arquivo temporário **fora do repo** — acento e here-string no Windows já estragaram mensagem |
| Antes de cada commit | `scripts/.venv/Scripts/check-secrets`. Achou algo, não commite |
| Antes de push | `scripts/.venv/Scripts/check-secrets --all` |
| `Co-Authored-By` | **Sim**, no fim do corpo: `Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>` (com o modelo da sessão). Diferente dos outros projetos do Capitão — aqui combina com o vibado |

**Nunca sozinho:** `push` (o primeiro vai ser junto), qualquer `--force`,
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
  instala no sistema.** Python → `scripts/.venv`, criado pelo
  `scripts/install-python-venv.ps1`; Node → `node_modules` local; toolchain
  (ESP-IDF, OpenSCAD headless) → Docker, que já está instalado. Precisa de algo
  no sistema? **Pergunte antes.**
- Finais de linha: o `.gitattributes` guarda LF e deixa o checkout usar o padrão
  da plataforma. Hook e `.sh` são sempre LF.

## 8. Mapa

```
docs/        regulamento, esboço, imagens            → docs/CLAUDE.md
hardware/    esquemático KiCad, pinagem              → hardware/CLAUDE.md
software/    firmware ESP-IDF em C++, página web     → software/CLAUDE.md
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
4. Rode teste e lint antes de dizer que terminou. Falhou? Mostre a saída.
