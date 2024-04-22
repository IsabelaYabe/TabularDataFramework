# TabularDataFramework
Framework desenvolvido para simplificar a criação e execução de pipelines de processamento de dados, com foco na execução concorrente e paralela.

O projeto está estruturado em torno de um pipeline de processamento de dados que incorpora cinco conceitos principais:

Dataframe: Esta é uma estrutura de dados tabular que armazena informações para serem processadas.

Handler: Uma função encarregada de manipular os dados contidos em um Dataframe.

Repository: Um repositório que mantém os Dataframes armazenados.

Trigger: Uma função que inicia a execução de um Handler.

Queue: Uma estrutura de dados que gerencia os Dataframes que entram e saem de um Handler.

Para otimizar a utilização dos recursos computacionais, o projeto utiliza a biblioteca de threads do C++, permitindo a paralelização dos Handlers. Isso torna o processamento dos dados mais eficiente e rápido. Em particular, as queues funcionam como buffers em um esquema de fila consumidor-produtor, uma configuração típica em problemas de computação paralela.

# Compilando o Projeto "Tabular Data Framework"

## Pré-requisitos

Certifique-se de ter o seguinte instalado em seu sistema:
- CMake 3.10 ou superior
- Um compilador C++ que suporte C++17
- Biblioteca Boost versão 1.65 ou superior

## Estrutura de Diretórios

O projeto está estruturado da seguinte forma:

```
Framework de Dados Tabulares/
│
├── CMakeLists.txt                  # Arquivo de configuração CMake raiz
├── README.md                       # Informações detalhadas do projeto
│
├── app/                            # Diretório de código-fonte da aplicação
│   ├── CMakeLists.txt              # Arquivo CMake para compilar o executável
│   └── main.cpp                    # Arquivo de código-fonte principal da aplicação
│
├── include/                        # Diretório contendo todos os cabeçalhos
│   ├── dataframe/                  # Headers relacionados a Dataframe
│   │   ├── dataframe.hpp
│   │   ├── row.hpp
│   │   ├── time.hpp
│   │   └── utils.hpp
│   ├── handler/                    # Headers relacionados aos tratadores
│   │   ├── handler.h
│   │   └── parser.hpp
│   ├── http/                       # Headers relacionados a HTTP
│   │   ├── http_server.hpp
│   │   └── http_session.hpp
│   ├── triggers/                   # Headers relacionados aos triggers
│   │   ├── request_trigger.hpp
│   │   └── timer_trigger.hpp
│   ├── utils/                      # Utilidades
│   │   ├── file_processor.hpp
│   │   └── watchdog.hpp
│   └── queue/                      # Cabeçalho de fila
│       └── queue.hpp
│
├── src/                            # Diretório de arquivos-fonte para a biblioteca
│   ├── CMakeLists.txt              # Arquivo CMake para compilar a biblioteca
│   ├── dataframe/                  # Arquivos-fonte relacionados ao dataframe
│   │   ├── dataframe.cpp
│   │   ├── row.cpp
│   │   ├── time.cpp
│   │   └── utils.cpp
│   ├── handler/                    # Arquivos-fonte relacionados a tratadores
│   │   ├── handler.cpp
│   │   └── parser.cpp
│   ├── http/                       # Arquivos-fonte relacionados a HTTP
│   │   ├── http_server.cpp
│   │   └── http_session.cpp
│   ├── triggers/                   # Arquivos-fonte relacionados a triggers
│   │   ├── request_trigger.cpp
│   │   └── timer_trigger.cpp
│   └── utils/                      # Arquivos-fonte de utilidades
│       ├── file_processor.cpp
│       └── watchdog.cpp
│
└── scripts/                        # Diretório de scripts
     └── mock/                      # Scripts relacionados a simulações
        └── mock.py
```

## Instruções de Compilação

1. **Clone o Repositório:**
   Certifique-se de ter uma cópia local do código do projeto.

2. **Navegue até o Diretório do Projeto:**
   Abra um terminal e mude para o diretório raiz do projeto.

3. **Crie um Diretório de Compilação:**
   Dentro do diretório raiz, crie um novo diretório para os arquivos de compilação.
   ```bash
   mkdir build
   cd build
   ```

4. **Execute o CMake:**
   Gere os Makefiles usando o CMake. Aponte o CMake para o diretório pai onde está localizado o `CMakeLists.txt` de nível superior.
   ```bash
   cmake ..
   ```

5. **Compile o Projeto:**
   Compile o projeto executando o comando `make`. Isso compilará tanto a biblioteca quanto o executável com base nas configurações especificadas no CMake.
   ```bash
   make
   ```

6. **Execute o Programa:**
   Uma vez que a compilação seja bem-sucedida, você pode encontrar seu executável no diretório `bin` sob o diretório de compilação. Você pode executá-lo a partir do terminal assim:
   ```bash
   cd bin/
   ./PipelineFramework
   ```

7. **Execute o Mock:**
   Uma vez que a compilação seja bem-sucedida, executar o mock para gerar os dados sintéticos. Da pasta raiz do projeto, rode:
   ```bash
   cd scripts/mock/
   python mock.py
   ```


## Informações Adicionais

- O projeto utiliza componentes do conjunto de funções Boost, então garanta que a instalação do Boost em seu sistema inclua esses componentes.
- Se a biblioteca Boost não estiver em um caminho padrão, talvez seja necessário especificar o `Boost_INCLUDE_DIR` e o `Boost_LIBRARY_DIR` no comando CMake:
  ```bash
  cmake -DBoost_INCLUDE_DIR=<caminho_para_include_boost> -DBoost_LIBRARY_DIR=<caminho_para_libs_boost> ..
  ```

## Solução de Problemas

Se você encontrar problemas relacionados à não localização do Boost ou outros componentes:
- Certifique-se de que o Boost está corretamente instalado, e a variável de ambiente `BOOST_ROOT` está configurada se o Boost não estiver em um local padrão.
- Verifique se seu compilador C++ suporta C++17.
- Garanta que todos os caminhos estejam corretamente especificados nas configurações do CMake e nas opções de linha de comando.
