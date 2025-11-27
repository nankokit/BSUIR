import psycopg2
import json
from berkeleydb import db
import os
import sys

# === НАСТРОЙКИ ===
PG_CONFIG = {
    'dbname': os.getenv('PG_DB', 'repbase'),
    'user': os.getenv('PG_USER', 'postgres'),
    'password': os.getenv('PG_PASSWORD', ''),
    'host': os.getenv('PG_HOST', 'localhost'),
    'port': int(os.getenv('PG_PORT', 5432))
}

SCHEMA = os.getenv('PG_SCHEMA', 'main')
OUTPUT_DIR = '/output'

COMPOSITE_CONFIG = {}
CONFIG_PATH = 'composite_keys.json'
if os.path.exists(CONFIG_PATH):
    try:
        with open(CONFIG_PATH, 'r', encoding='utf-8') as f:
            COMPOSITE_CONFIG = json.load(f)
        print(f"[ИНФО] Загружен конфиг ключей: {CONFIG_PATH}")
    except Exception as e:
        print(f"[ОШИБКА] Не удалось загрузить {CONFIG_PATH}: {e}")

os.makedirs(OUTPUT_DIR, exist_ok=True)

def log(msg):
    print(msg)
    sys.stdout.flush()

def get_tables(conn):
    cur = conn.cursor()
    cur.execute(f"""
        SELECT table_name 
        FROM information_schema.tables 
        WHERE table_schema = %s AND table_type = 'BASE TABLE'
        ORDER BY table_name
    """, (SCHEMA,))
    return [row[0] for row in cur.fetchall()]

def get_columns(conn, table_name):
    cur = conn.cursor()
    cur.execute(f"""
        SELECT column_name 
        FROM information_schema.columns 
        WHERE table_schema = %s AND table_name = %s
        ORDER BY ordinal_position
    """, (SCHEMA, table_name))
    return [row[0] for row in cur.fetchall()]

def get_primary_key_columns(conn, table_name):
    cur = conn.cursor()
    cur.execute(f"""
        SELECT kcu.column_name
        FROM information_schema.table_constraints tc
        JOIN information_schema.key_column_usage kcu
          ON tc.constraint_name = kcu.constraint_name
        WHERE tc.constraint_type = 'PRIMARY KEY'
          AND tc.table_schema = %s AND tc.table_name = %s
        ORDER BY kcu.ordinal_position
    """, (SCHEMA, table_name))
    return [row[0] for row in cur.fetchall()]

def infer_composite_key(table_name, columns):
    """Автоопределение составного ключа для таблиц без PK"""
    id_cols = [c for c in columns if 'id' in c.lower()]
    if len(id_cols) >= 2:
        return id_cols[:2]
    if len(id_cols) == 1:
        return id_cols

    id_prefix = [c for c in columns if c.startswith('id_')]
    if len(id_prefix) >= 2:
        return id_prefix[:2]

    return columns[:2] if len(columns) >= 2 else columns

def convert_table(conn, table_name):
    log(f"Конвертируем: {table_name}")
    columns = get_columns(conn, table_name)
    pk_cols = get_primary_key_columns(conn, table_name)

    if pk_cols:
        log(f"  → PK: {pk_cols}")
    else:
        if table_name in COMPOSITE_CONFIG:
            pk_cols = COMPOSITE_CONFIG[table_name]
            log(f"  → Нет PK, из config: {pk_cols}")
        else:
            pk_cols = infer_composite_key(table_name, columns)
            log(f"  → Нет PK, авто: {pk_cols}")

        if not pk_cols:
            log(f"  → [ОШИБКА] Не удалось определить ключ для {table_name}")
            return

    db_path = os.path.join(OUTPUT_DIR, f"{table_name}.db")
    bdb = db.DB()
    try:
        bdb.open(db_path, None, db.DB_BTREE, db.DB_CREATE)
    except Exception as e:
        log(f"  → [ОШИБКА] Не удалось создать {db_path}: {e}")
        return

    cur = conn.cursor()
    cols_str = ', '.join(columns)
    cur.execute(f"SELECT {cols_str} FROM {SCHEMA}.{table_name}")
    rows = cur.fetchall()

    duplicates = 0
    for row in rows:
        row_dict = dict(zip(columns, row))

        if len(pk_cols) == 1:
            key_raw = row_dict[pk_cols[0]]
        else:
            key_raw = tuple(row_dict[col] for col in pk_cols)
        key = str(key_raw).encode('utf-8')

        value_dict = {k: v for k, v in row_dict.items() if k not in pk_cols}
        value = json.dumps(value_dict, ensure_ascii=False, default=str).encode('utf-8')

        if bdb.exists(key):
            duplicates += 1
        bdb.put(key, value)

    bdb.close()
    log(f"  → {db_path} ({len(rows)} записей)" + (f", {duplicates} дубликат(ов)" if duplicates else ""))

def main():
    log("Запуск конвертера PostgreSQL → BerkeleyDB")

    try:
        conn = psycopg2.connect(**PG_CONFIG)
        conn.cursor().execute(f"SET search_path TO {SCHEMA}")
        log(f"Подключено: {PG_CONFIG['dbname']}, схема: {SCHEMA}")
    except Exception as e:
        log(f"Ошибка подключения: {e}")
        return

    try:
        tables = get_tables(conn)
        if not tables:
            log("Таблиц не найдено!")
            return

        log(f"Найдено таблиц: {len(tables)} → {tables}")

        for table in tables:
            convert_table(conn, table)

        log("Конвертация завершена!")
        log(f"Файлы: {OUTPUT_DIR}")

    except Exception as e:
        log(f"Критическая ошибка: {e}")
    finally:
        conn.close()

if __name__ == "__main__":
    main()