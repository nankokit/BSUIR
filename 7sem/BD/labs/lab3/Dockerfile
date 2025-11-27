# Dockerfile
FROM python:3.11-slim

# Установка BerkeleyDB + утилит
# db-util содержит db_dump
RUN apt-get update && \
    apt-get install -y libdb5.3-dev libdb5.3 gcc build-essential db-util && \
    rm -rf /var/lib/apt/lists/*

# Установка Python-пакетов
RUN pip install berkeleydb psycopg2-binary

# Копия кода
WORKDIR /app
COPY converter.py .

# Том для вывода
VOLUME /output

CMD ["python", "converter.py"]