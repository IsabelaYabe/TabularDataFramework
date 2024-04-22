# TabularDataFramework
Framework desenvolvido para simplificar a criação e execução de pipelines de processamento de dados, com foco na execução concorrente e paralela.

O projeto está estruturado em torno de um pipeline de processamento de dados que incorpora cinco conceitos principais:

Dataframe: Esta é uma estrutura de dados tabular que armazena informações para serem processadas.

Handler: Uma função encarregada de manipular os dados contidos em um Dataframe.

Repository: Um repositório que mantém os Dataframes armazenados.

Trigger: Uma função que inicia a execução de um Handler.

Queue: Uma estrutura de dados que gerencia os Dataframes que entram e saem de um Handler.

Para otimizar a utilização dos recursos computacionais, o projeto utiliza a biblioteca de threads do C++, permitindo a paralelização dos Handlers. Isso torna o processamento dos dados mais eficiente e rápido. Em particular, as queues funcionam como buffers em um esquema de fila consumidor-produtor, uma configuração típica em problemas de computação paralela.
