# Tutorial: Análise Estática de Código com Cppcheck

Este repositório contém um guia prático e arquivos de exemplo para a utilização da ferramenta **Cppcheck** em projetos C/C++. O objetivo é demonstrar como identificar bugs, vazamentos de memória e comportamentos indefinidos sem a necessidade de executar o código.

-----

## Demonstração em Vídeo

Assista ao tutorial prático gravado passo a passo:

<p align="center">
  <a href="https://drive.google.com/file/d/1syqoeqWXVYbJwCbZGULdXqNPi_wbQjlH/view?usp=sharing">
    <img src="https://img.shields.io/badge/Assistir_Vídeo-Google_Drive-red?style=for-the-badge&logo=google-drive&logoColor=white" alt="Assistir Vídeo no Google Drive">
  </a>
</p>

-----

## Sobre a Ferramenta

O **Cppcheck** é uma ferramenta de análise estática (Static Analysis) focada em detectar bugs que compiladores tradicionais muitas vezes deixam passar.

### Diferenciais

  * **Não requer instrumentação:** Analisa o código fonte diretamente, sem precisar compilar ou injetar trechos de código no executável.
  * **Foco em Comportamento Indefinido:** Detecta Dead Pointers, Divisões por zero, Integer Overflows, etc.
  * **Segurança e Normas:** Possui suporte (na versão adequada) para normas industriais como **MISRA C 2012** e **AUTOSAR**, essenciais no setor automotivo.

> **Nota:** O Cppcheck é Open Source (GPL). Existe uma versão Premium ("Cppcheck Solutions") voltada para empresas que necessitam de certificação de segurança funcional profunda e suporte comercial.

-----

## Instalação e Configuração

### 1\. Download (Windows)

Baixe o instalador `.msi` oficial para Windows:

  * [Site Oficial do Cppcheck](https://cppcheck.sourceforge.io/)

### 2\. Variáveis de Ambiente (Passo Crítico)

Durante a instalação, certifique-se de marcar a opção **"Add to PATH"**.

Se você esqueceu de marcar ou o terminal não reconhece o comando, configure manualmente:

1.  Abra o Menu Iniciar e digite **"Variáveis de Ambiente"**.
2.  Em **Variáveis do Sistema** (ou de usuário), edite a variável `Path`.
3.  Adicione o caminho da pasta de instalação. Geralmente:
    `C:\Program Files\Cppcheck`

### 3\. Validação

Abra um **novo** terminal (PowerShell ou CMD) e digite:

```powershell
cppcheck --version
```

*Se retornar algo como `Cppcheck 2.16.0`, a instalação foi um sucesso.*

-----

## Código de Teste (`codigo_teste.c`)

Para este tutorial, foi utilizado um arquivo intencionalmente defeituoso para demonstrar as capacidades da ferramenta. Ele contém três erros clássicos de programação em C:

```c
#include <stdlib.h>

void vetor_estouro() {
    int vetor[5];
    // ERRO: Acesso fora dos limites (Index 5 é a 6ª posição)
    vetor[5] = 10;
}

void variavel_nao_iniciada() {
    int x;
    // ERRO: Uso de variável sem valor inicial (Lixo de memória)
    if (x == 10) {
        // ...
    }
}

void vazamento_memoria() {
    char *ptr = (char *)malloc(10);
    // ERRO: Alocação não verificada (pode ser NULL)
    ptr[0] = 'a';
    
    // ERRO: 'free(ptr)' nunca é chamado (Memory Leak)
}
```

-----

## Utilização via Linha de Comando

A verdadeira força do Cppcheck está na automação via terminal.

### 1\. Análise Básica

Para verificar um arquivo específico:

```powershell
cppcheck codigo_teste.c
```

*Resultado:* Mostrará apenas erros fatais.

### 2\. Análise Completa (Recomendado)

Para ativar todas as verificações (estilo, performance, portabilidade, aviso), use a flag `--enable=all`:

```powershell
cppcheck --enable=all codigo_teste.c
```

### 3\. Modo Verboso

Para obter mais detalhes sobre *onde* e *por que* um erro ocorreu:

```powershell
cppcheck --enable=all --verbose codigo_teste.c
```

-----

## 🔍 Interpretando os Resultados

O Cppcheck retorna erros no formato: `[arquivo]:[linha]: (tipo) [ID] Mensagem`. Abaixo, émostrar os erros encontrados no teste:

| ID do Erro (`errorId`) | Classificação | O que significa? |
| :--- | :--- | :--- |
| **`[arrayIndexOutOfBounds]`** | Erro Crítico | **Estouro de Buffer.** Tentativa de acessar uma posição de memória que não pertence ao vetor. Isso pode corromper dados vizinhos ou travar o programa. |
| **`[uninitvar]`** | Erro Crítico | **Variável não inicializada.** O código lê uma variável (`x`) que não teve valor atribuído. O comportamento do programa torna-se imprevisível (não-determinístico). |
| **`[memleak]`** | Erro Crítico | **Vazamento de Memória.** Memória foi alocada com `malloc` mas nunca liberada com `free`. Em sistemas embarcados, isso leva ao esgotamento da RAM e travamento do sistema. |
| **`[nullPointerOutOfMemory]`** | Aviso (Warning) | **Robustez.** O código usa o ponteiro do `malloc` sem verificar se ele é `NULL`. Se a memória do sistema estiver cheia, o `malloc` falha e o programa causa um *Crash*. |

-----

## Gerando Relatórios em HTML

Para apresentar os dados em um site interativo.

### Pré-requisitos

É necessário ter o **Python** instalado e o script `cppcheck-htmlreport.py`.

> *Dica: Se o script não vier na instalação do Windows, pode-se baixá-lo [Repositório Oficial](https://github.com/danmar/cppcheck/tree/main/htmlreport) deve-se salvá-lo na pasta do projeto.*

### Passo 1: Gerar o XML

O Cppcheck precisa exportar os dados em formato XML primeiro. A flag `2>` redireciona a saída de erro (onde o relatório sai) para um arquivo.

```powershell
cppcheck --xml --enable=all codigo_teste.c 2> relatorio.xml
```

### Passo 2: Converter para HTML

Usamos o script Python para ler o XML e criar uma pasta com o site.

```powershell
python cppcheck-htmlreport.py --file=relatorio.xml --report-dir=relatorio_html --source-dir=.
```

  * `--file`: O arquivo XML gerado no passo anterior.
  * `--report-dir`: Nome da pasta onde o site será criado.
  * `--source-dir`: Pasta onde está o código fonte (para que ele apareça colorido no relatório).

### Passo 3: Visualizar

Abra a pasta `relatorio_html` e execute o arquivo `index.html`.

-----

## Outras Funcionalidades

### Analisar um Diretório Inteiro

Para não verificar arquivo por arquivo, use o ponto (`.`) para indicar o diretório atual:

```powershell
cppcheck --enable=all .
```

### Integração com IDEs

O Cppcheck pode ser integrado ao **VS Code** (via extensões) ou ao **Visual Studio** (como Ferramenta Externa), permitindo clicar no erro e ir direto para a linha de código.

-----

## 🔗 Referências

  * [Manual do Cppcheck](https://cppcheck.sourceforge.io/manual.html)
  * [Lista de Verificações (Checks)](https://sourceforge.net/p/cppcheck/wiki/ListOfChecks/)
  * [GitHub Oficial](https://github.com/danmar/cppcheck)

-----

**Autor:** Izaac Moraes
**Contexto:** Residência em Software Automotivo (CIN/Stellantis)