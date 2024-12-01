import smtplib
import json
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart

def notify_user():
    """
    Envia um e-mail com configurações carregadas de um arquivo JSON.
    """
    try:
        # Carrega as configurações do arquivo JSON
        with open("config.json", "r") as config_file:
            config = json.load(config_file)
        
        # Configurações de envio
        subject = "Aviso Importante"
        body = "Este é um teste de envio de e-mail pela função notify_user."
        from_email = config["from_email"]
        to_email = config["to_email"]
        password = config["password"]
        smtp_server = "smtp.gmail.com"
        smtp_port = 587

        # Cria a mensagem do e-mail
        msg = MIMEMultipart()
        msg['From'] = from_email
        msg['To'] = to_email
        msg['Subject'] = subject
        msg.attach(MIMEText(body, 'plain'))  # Define o corpo do e-mail como texto simples

        # Conecta ao servidor SMTP e envia o e-mail
        with smtplib.SMTP(smtp_server, smtp_port) as server:
            server.starttls()  # Ativa a criptografia TLS
            server.login(from_email, password)  # Faz login
            server.send_message(msg)  # Envia o e-mail
            print("E-mail enviado com sucesso!")
    except Exception as e:
        print(f"Falha ao enviar o e-mail: {e}")
