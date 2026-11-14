# software/

Todo o código que roda: um projeto por pasta, cada um com o seu README.

| Pasta | O quê | Estado |
| --- | --- | --- |
| [firmware/](firmware/README.md) | O código da placa (ESP32-C3, C++ com ESP-IDF) e a página de controle que ela serve pro celular | 🚧 Compila e passa nos testes do PC; falta a placa |
| `mobile/` | App do celular | Ainda não existe — decisão D11 do [esboço](../docs/esbocando-a-ideia.md) |

## Como os projetos conversam

O barco é o servidor: cria a rede Wi-Fi e atende quem entrar nela. O
joystick vai por **WebSocket**; o resto (estado e ajustes) é uma **API
REST** com JSON. A página que vem dentro do firmware é um cliente; um app de
celular seria outro, falando a mesma API. O contrato está no
[README do firmware](firmware/README.md#api).
