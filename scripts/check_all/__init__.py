"""check-all: roda todas as checagens do repositório de uma vez.

Antes de dizer que terminou, ou antes do push: o caçador de segredos, os
testes e o lint do Python, a formatação do C++, o ERC do esquemático e, no
Docker, os testes do firmware no PC, a compilação e o Doxygen.

Módulos:
    checks    a lista de checagens e quem roda cada uma
    __main__  a linha de comando
"""
