import requests
import os
import pandas as pd
from faker import Faker
import multiprocessing  # Importando multiprocessing em vez de threading
import time
import json
import random
from datetime import datetime
from pathlib import Path


# Configuração do Faker
fake = Faker()

def criar_diretorio(diretorio):
    if not os.path.exists(diretorio):
        os.makedirs(diretorio)

# Gerador de dados para o serviço ContaVerde
def conta_verde():
    directory = "ContaVerde"
    
    start_time = time.time()
    timestamp = int(start_time)

    data_types = ["usuarios", "produtos", "estoque", "ordens"]
    columns = {
        "usuarios": ["ID", "Nome", "Sobrenome", "Endereço", "Data de Cadastro", "Data de Aniversário"],
        "produtos": ["ID", "Nome", "Imagem", "Descrição", "Preço"],
        "estoque": ["ID do Produto", "Quantidade Disponível"],
        "ordens": ["ID do Usuário", "ID do Produto", "Quantidade", "Data de Criação", "Data do Pagamento", "Data da Entrega"]
    }

    while True:
        
        if time.time() - start_time >= 60:
            start_time = time.time()
            timestamp = int(start_time)

        data_type = random.choice(data_types)
        criar_diretorio(f"mock/{directory}")
        csv_file = f"mock/{directory}/{data_type}_{timestamp}.csv"
        
        if not Path(csv_file).exists():
            pd.DataFrame([], columns=columns[data_type]).to_csv(csv_file, index=False)
        
        data = []
        
        switch = {
            "usuarios": [fake.uuid4(), fake.first_name(), fake.last_name(), fake.address(), fake.date(), fake.date_of_birth()],
            "produtos": [fake.uuid4(), fake.word(), fake.image_url(), fake.sentence(), fake.random_number(digits=5)],
            "estoque": [fake.uuid4(), fake.random_number(digits=3)],
            "ordens": [fake.uuid4(), fake.uuid4(), fake.random_int(min=1, max=10), fake.date_time_this_month(), fake.date_time(), fake.date_time()]
        }
        
        data = switch.get(data_type, [])
        pd.DataFrame([data], columns=columns[data_type]).to_csv(f"mock/{directory}/{data_type}_{timestamp}.csv", mode='a', header=False, index=False)

        time.sleep(1)

# Gerador de dados para o serviço DataCat
def data_cat():  
    inicio = time.time()
    timestamp = int(time.time())
    diretorio = "DataCat"
    
    while True:  
        
        if time.time() - inicio >= 60:  
            # Salva os dados de log acumulados no último minuto
            timestamp = int(time.time())
            # Reseta o tempo de início para o próximo ciclo de 1 minuto
            inicio = time.time()
        
        # A cada segundo, adicione um novo log
        arquivo_path = f"mock/{diretorio}/log_{timestamp}.txt"
        
        log_type = random.choice(["auditoria", "comportamento do usuário", "notificação de falhas", "depuração"])
        message = f"{datetime.now().strftime('%Y-%m-%d %H:%M:%S')} | {log_type} | {fake.sentence()}"
        if log_type == "auditoria":
            message += f" | User: {fake.uuid4()} | Action: {fake.word()}"
        elif log_type == "comportamento do usuário":
            message += f" | User: {fake.uuid4()} | Stimulus: {fake.word()} | Target: {fake.word()}"
        elif log_type == "notificação de falhas":
            message += f" | Target: {fake.word()} | File/Line: {fake.file_name()}:{fake.random_int(min=1, max=100)} | Severity: {random.choice(['low', 'medium', 'high'])}"
        criar_diretorio("mock/" + diretorio)
        with open(arquivo_path, "a+") as file:
            file.write(f"{message}\n")
        time.sleep(1)  # Espera 1 segundo antes de gerar um novo log

# Gerador de dados para o serviço CadêAnalytics
def cade_analytics():  
    eventos_acumulados = []  
    inicio = time.time()
    
    while True:  
        if time.time() - inicio >= 60:
            timestamp = int(time.time())
            criar_diretorio("mock/CadeAnalytics") 
            # Criação do arquivo simulando uma request HTTP
            with open(f"mock/CadeAnalytics/eventos_{timestamp}.json", "w") as file:
                # Escrevendo os headers da 'request'
                file.write("POST /api/eventos HTTP/1.1\n")
                file.write("Host: www.exemplo.com\n")
                file.write("Content-Type: application/json\n")
                file.write(f"Content-Length: {str(len(json.dumps(eventos_acumulados)))}\n")
                file.write("Connection: close\n\n")
                
                # Escrevendo o corpo da 'request' com os eventos acumulados
                json.dump(eventos_acumulados, file, indent=2)  # Formato bonito com 'indent'
            
            url = "http://localhost:8080"  # Substitua pela URL correta do seu servidor
            headers = {
                "Content-Type": "application/json"
            }
            data = json.dumps(eventos_acumulados)
            
            try:
                response = requests.post(url, headers=headers, data=data)
                print(f"Request enviada para o servidor. Status code: {response.status_code}")
            
            except requests.exceptions.RequestException as e:
                print(f"Erro ao enviar a request para o servidor: {e}")
            
            eventos_acumulados = []
            inicio = time.time()

        numero_aleatorio = random.randint(0, 100)
        print(f"Número aleatório: {numero_aleatorio}")
        if numero_aleatorio % 7 == 0:
            evento = {
                "data_notificacao": str(datetime.now().strftime('%Y-%m-%d %H:%M:%S')),
                "id_usuario": fake.uuid4(),
                "estimulo": fake.word(),
                "componente_alvo": fake.word()
            }
            eventos_acumulados.append(evento)

        time.sleep(1)


if __name__ == '__main__':  # É necessário proteger o ponto de entrada do script com essa verificação
    # Criando processos
    p1 = multiprocessing.Process(target=conta_verde)
    p2 = multiprocessing.Process(target=data_cat)
    p3 = multiprocessing.Process(target=cade_analytics)

    # Iniciando processos
    p1.start()
    p2.start()
    p3.start()

    # Aguardando os processos terminarem (opcional, dependendo do seu caso de uso)
    p1.join()
    p2.join()
    p3.join()