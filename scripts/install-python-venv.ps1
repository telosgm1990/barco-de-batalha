<#
.SYNOPSIS
    Cria o ambiente virtual Python dos scripts do repositório.

.DESCRIPTION
    Cria o ambiente em 'scripts\.venv' e instala, em modo editável, os scripts
    declarados em 'scripts\pyproject.toml' junto com as ferramentas de
    desenvolvimento (ruff). Cada script ganha um executável próprio em
    '.venv\Scripts\' — o alias do [project.scripts].

    O ambiente não é versionado: rode uma vez em cada máquina. Rodar de novo só
    atualiza a instalação (necessário quando entra script novo no pyproject).

    Use -Force para recriar o ambiente do zero.

    Nada é instalado no sistema: tudo fica dentro de 'scripts\.venv'.

.EXAMPLE
    powershell -ExecutionPolicy Bypass -File scripts/install-python-venv.ps1

.EXAMPLE
    powershell -ExecutionPolicy Bypass -File scripts/install-python-venv.ps1 -Force
#>

param (
    # Caminho para o executável do Python. Sem ele, usa o 'python' do PATH.
    [string]$PythonPath,

    # Apaga o ambiente existente antes de criar o novo.
    [switch]$Force
)

# ------------------------------------------------ Constantes de configuração

$scriptDir = $PSScriptRoot
$venvDir = Join-Path $scriptDir '.venv'
$projectFile = Join-Path $scriptDir 'pyproject.toml'

# Mesma versão mínima do 'requires-python' do pyproject.toml.
$minimumVersion = [Version]'3.10'

# ------------------------------------------------------------------- Execução

if (-not (Test-Path -Path $projectFile -PathType Leaf)) {
    Write-Error "'pyproject.toml' não encontrado em '$scriptDir'."
    exit 1
}

# Encontre o Python: o do parâmetro ou o do PATH
if ($PythonPath) {
    if (-not (Test-Path -Path $PythonPath -PathType Leaf)) {
        Write-Error "Python não encontrado em '$PythonPath'."
        exit 1
    }
    $python = $PythonPath
}
else {
    $found = Get-Command python -ErrorAction SilentlyContinue
    if ($null -eq $found) {
        Write-Error "'python' não está no PATH. Informe o caminho com -PythonPath."
        exit 1
    }
    $python = $found.Source
}

# Confira a versão contra o mínimo do pyproject.toml
$versionText = & $python -c "import sys; print('{}.{}'.format(*sys.version_info[:2]))"
if ($LASTEXITCODE -ne 0) {
    Write-Error "Não deu pra rodar '$python'."
    exit 1
}
$version = [Version]$versionText
if ($version -lt $minimumVersion) {
    Write-Error "Python $version encontrado; os scripts pedem $minimumVersion ou mais recente."
    exit 1
}
Write-Host "Python $version em '$python'."

# Recrie do zero quando pedido
if ((Test-Path -Path $venvDir) -and $Force) {
    Write-Host 'Apagando o .venv existente (-Force)...' -ForegroundColor Yellow
    Remove-Item -Path $venvDir -Recurse -Force
}

# Crie o ambiente, se ainda não existe
if (Test-Path -Path $venvDir) {
    Write-Host '.venv já existe: só atualizando a instalação (-Force recria).'
}
else {
    Write-Host 'Criando o .venv...'
    & $python -m venv $venvDir
    if ($LASTEXITCODE -ne 0) {
        Write-Error 'Falha ao criar o .venv.'
        exit 1
    }
}

# O Python do ambiente muda de lugar conforme o sistema operacional
$venvPython = Join-Path $venvDir 'Scripts\python.exe'
if (-not (Test-Path -Path $venvPython -PathType Leaf)) {
    $venvPython = Join-Path $venvDir 'bin/python'
}
if (-not (Test-Path -Path $venvPython -PathType Leaf)) {
    Write-Error "Python do .venv não encontrado em '$venvDir'."
    exit 1
}

# Instale os scripts em modo editável, com as ferramentas de desenvolvimento
Write-Host 'Instalando os scripts e as ferramentas de desenvolvimento...'
& $venvPython -m pip install --disable-pip-version-check --quiet --editable "$scriptDir[dev]"
if ($LASTEXITCODE -ne 0) {
    Write-Error 'Falha ao instalar os scripts.'
    exit 1
}

Write-Host ''
Write-Host 'Ambiente pronto!' -ForegroundColor Green
Write-Host 'Cada script tem um executável em scripts\.venv\Scripts\ — por exemplo:'
Write-Host '    scripts\.venv\Scripts\check-secrets.exe'
