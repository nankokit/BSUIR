# 4 РАЗРАБОТКА ПРОГРАММНЫХ МОДУЛЕЙ

Настоящий раздел описывает программную реализацию ключевых модулей системы семантического поиска кандидатов. Система построена по микросервисной архитектуре и включает четыре Python-сервиса на базе фреймворка FastAPI [17], два TypeScript-сервиса на базе NestJS [18], а также набор разделяемых библиотек. Каждый сервис имеет собственную зону ответственности, изолированную конфигурацию и взаимодействует с остальными компонентами посредством HTTP-вызовов. Разделение ответственности по сервисам позволяет масштабировать каждый компонент независимо, обновлять его без остановки остальных, а также тестировать в изоляции с помощью заглушек.

## 4.1 Модули сервиса извлечения данных (extraction-service)

### **4.1.1** Модуль инициализации и конфигурации

Точкой входа сервиса извлечения данных является файл `main.py`, отвечающий за создание экземпляра приложения FastAPI, настройку централизованного структурированного логирования и регистрацию маршрутизатора. Конфигурация выносится в отдельный модуль `core/config.py` на основе класса **AppConfig**, наследующего `BaseSettings` из библиотеки Pydantic v2 [19].

```python
# apps/extraction-service/main.py
from fastapi import FastAPI
from core.config import settings
from core.logging import configure_logging
from core.middleware import setup_middleware
from routers.extraction import router

configure_logging(settings.log_level)

app = FastAPI(title="extraction-service", version="1.0.0")
setup_middleware(app)
app.include_router(router, prefix="/extraction")
```

В данном фрагменте кода представлена минималистичная инициализация приложения. Функция `configure_logging` активирует библиотеку structlog [20] с заданным уровнем детализации до включения маршрутизаторов. В отличие от стандартного модуля `logging` Python, structlog формирует каждую запись как структурированный JSON-объект с именованными полями, что делает журналы машинно-читаемыми и совместимыми с системами агрегации логов (Loki, Elasticsearch). Благодаря контекстному связыванию (`structlog.contextvars.bind_contextvars`) значения `correlation_id` и `request_id` автоматически добавляются ко всем записям, создаваемым в рамках одного HTTP-запроса, без явной передачи параметров через слои приложения.

Вызов `setup_middleware` добавляет CORS-политики, сжатие ответов (`GZipMiddleware`) и встраивает `x-correlation-id` в контекст каждого запроса. Промежуточный слой корреляции реализует следующую логику: если заголовок `x-correlation-id` присутствует во входящем запросе, его значение используется без изменений; если заголовок отсутствует, автоматически генерируется новый идентификатор в формате UUID4. Это позволяет внешнему оркестратору инициировать сквозную трассировку запроса, а при прямых вызовах к сервису – иметь гарантированно заполненный идентификатор для диагностики.

Маршрутизатор подключается с префиксом `/extraction`, что обеспечивает версионируемое пространство имен эндпоинтов. Такой подход позволяет в будущем добавить версию `/v2/extraction` параллельно с существующей, не нарушая обратной совместимости.

Класс **AppConfig** описывает все параметры сервиса с возможностью их переопределения через переменные окружения, что соответствует методологии «двенадцатифакторного приложения» [21]:

```python
# apps/extraction-service/core/config.py
class AppConfig(BaseSettings):
    log_level: str = "INFO"
    s3_endpoint: str
    s3_bucket: str = "resumes"
    s3_access_key: str
    s3_secret_key: str
    embedding_api_url: str
    embedding_model: str = "text-embedding-3-small"
    embedding_batch_size: int = 32
    llm_api_url: str
    llm_model: str
    ocr_engine: str = "tesseract"
    chunk_size: int = 512
    chunk_overlap: int = 64
    # ...

settings = AppConfig()
```

Наследование от `BaseSettings` предоставляет два ключевых преимущества. Во-первых, Pydantic v2 автоматически считывает значения полей из переменных окружения с именами в верхнем регистре (например, поле `s3_endpoint` отображается на переменную `S3_ENDPOINT`), а также из файла `.env` при его наличии. Во-вторых, все значения проходят валидацию типов при запуске: если обязательное поле `s3_endpoint` не задано в окружении, приложение завершится с понятным сообщением об ошибке еще до обработки первого запроса.

Класс содержит следующие группы полей: параметры подключения к объектному хранилищу MinIO (поля `s3_endpoint`, `s3_bucket`, `s3_access_key`, `s3_secret_key`); параметры сервиса эмбеддингов (`embedding_api_url`, `embedding_model`, `embedding_batch_size`); параметры языковой модели (`llm_api_url`, `llm_model`); настройки OCR-движка (`ocr_engine`, `ocr_confidence_threshold`, `ocr_min_chars_per_kb`); параметры текстового чанкования (`chunk_size`, `chunk_overlap`). Значение `chunk_size = 512` выбрано эмпирически как компромисс между семантической полнотой фрагмента и ограничениями контекстного окна модели эмбеддингов. Значение `chunk_overlap = 64` обеспечивает перекрытие соседних фрагментов, благодаря которому ключевая информация, расположенная на границе двух фрагментов, попадает как минимум в один из них полностью.

### **4.1.2** Модуль маршрутизации запросов

Маршрутизатор сервиса реализует единственный эндпоинт для загрузки и обработки файла резюме. Принятый файл валидируется по расширению, сохраняется в объектном хранилище, после чего передается в сервисный слой для полной обработки.

```python
# apps/extraction-service/routers/extraction.py
router = APIRouter()
ALLOWED_EXTENSIONS = {".pdf", ".docx", ".doc", ".png", ".jpg", ".jpeg"}

@router.post("/process-resume", response_model=SuccessWrapper[ExtractionResponse])
async def process_resume_endpoint(
    file: UploadFile = File(...),
    x_correlation_id: str = Header(...),
) -> SuccessWrapper[ExtractionResponse]:
    ext = Path(file.filename).suffix.lower()
    if ext not in ALLOWED_EXTENSIONS:
        raise BadRequestError(f"Unsupported file format: {ext}")
    file_bytes = await file.read()
    s3_key = await upload_to_s3(file_bytes, file.filename)
    document = await process_resume(file_bytes, file.filename, s3_key)
    return SuccessWrapper.of(ExtractionResponse(document=document, correlation_id=x_correlation_id))
```

В данном фрагменте кода представлена реализация эндпоинта `POST /extraction/process-resume`. Множество `ALLOWED_EXTENSIONS` выполняет роль первого уровня защиты: файлы с неподдерживаемыми расширениями отклоняются еще до попытки чтения содержимого, что предотвращает напрасные аллокации памяти и вызовы к внешним сервисам. Фактический тип содержимого файла дополнительно проверяется на уровне сервиса синтаксического разбора путем анализа магических байтов, что защищает от переименованных файлов неверного формата.

Заголовок `x-correlation-id` объявлен обязательным параметром, что позволяет сквозным образом отслеживать запрос через всю цепочку сервисов. FastAPI автоматически преобразует имя заголовка из формата HTTP (`X-Correlation-Id`) в Python-параметр функции (`x_correlation_id`), поэтому имена синхронизированы без дополнительного кода. Ответ оборачивается в **SuccessWrapper**, обеспечивая единообразный формат ответа в соответствии с соглашением, принятым в системе. Исключение **BadRequestError** автоматически перехватывается глобальным обработчиком ошибок и преобразуется в HTTP 400 с телом **ErrorWrapper**.

Глобальный обработчик регистрируется один раз в `main.py` через декоратор `@app.exception_handler(ServiceException)`. Он перехватывает любой экземпляр подкласса `ServiceException`, формирует объект **ErrorWrapper** с полем `error` из сообщения исключения и возвращает HTTP-ответ с `status_code`, определенным в классе исключения. Благодаря этому ни один роутер не содержит явных блоков `try/except` для бизнес-ошибок – код маршрутизации остается линейным и читаемым.

### **4.1.3** Сервисный модуль обработки документов

Класс **ExtractionService** реализует многоэтапный конвейер обработки резюме: синтаксический анализ документа, извлечение структурированного профиля кандидата с помощью языковой модели, нарезку текста на фрагменты и пакетную генерацию эмбеддингов.

```python
# apps/extraction-service/services/extraction_service.py
# ...
log = structlog.get_logger()

async def process_resume(
    file_bytes: bytes,
    filename: str,
    s3_key: str,
) -> StructuredDocument:
    log.info("processing_resume", filename=filename, s3_key=s3_key)
    structured = await parse_document(file_bytes, filename)
    full_text = structured.full_text
    profile = await extract_candidate_profile(full_text)
    chunks = _chunk_text(full_text, settings.chunk_size, settings.chunk_overlap)
    embeddings = await generate_embeddings_batch(chunks)
    return StructuredDocument(
        source_s3_key=s3_key,
        full_text=full_text,
        pages=structured.pages,
        candidate_profile=profile,
        chunks=chunks,
        embeddings=embeddings,
    )

def _chunk_text(text: str, size: int, overlap: int) -> list[str]:
    chunks, start = [], 0
    while start < len(text):
        # ...
    return chunks
```

Функция `process_resume` реализует последовательный асинхронный конвейер из четырех этапов. На первом этапе вызывается `parse_document`, который делегирует разбор в зависимости от расширения файла: для форматов PDF и DOCX используется библиотека Docling [22], для растровых изображений – pytesseract [23] с настраиваемым порогом достоверности OCR. При обработке PDF-документов Docling анализирует внутреннюю структуру файла: выделяет блоки текста, заголовки, таблицы и их ограничивающие прямоугольники (`bbox`). Если при извлечении текста из PDF обнаруживается, что количество символов на килобайт файла ниже порога `ocr_min_chars_per_kb`, это свидетельствует о сканированном документе – в этом случае применяется OCR-режим поверх растеризованных страниц.

Второй этап реализует извлечение именованных сущностей посредством структурированного вывода языковой модели с использованием Pydantic-схемы **CandidateProfile** в качестве грамматики ответа. Языковая модель получает полный текст резюме и инструкцию выполнить разбор в режиме Function Calling, возвращая ответ в виде JSON-объекта, соответствующего схеме. Это исключает необходимость регулярных выражений или специализированных NER-моделей для извлечения имен, должностей и навыков – все делегируется универсальной языковой модели.

Третий этап – нарезка текста скользящим окном с перекрытием `chunk_overlap = 64` символа, обеспечивающим семантическую связность соседних фрагментов. Алгоритм `_chunk_text` реализован итеративным сдвигом позиции `start` на величину `size - overlap` при каждом шаге цикла. Последний фрагмент может быть короче `size` символов: Python-срез `text[start : start + size]` не выбрасывает исключение при выходе за границы строки, что делает реализацию безопасной без дополнительных проверок.

Четвертый этап – пакетная генерация векторных представлений размерностью 1536, соответствующей модели `text-embedding-3-small` [24]. Функция `generate_embeddings_batch` разбивает список фрагментов на подпакеты размером `embedding_batch_size = 32` и выполняет асинхронные HTTP-запросы к API эмбеддингов, что позволяет эффективно использовать пропускную способность сети и вычислительные ресурсы сервера эмбеддингов.

### **4.1.4** Модели данных сервиса

Для представления структуры разобранного документа в сервисе определен набор моделей Pydantic v2, отражающих иерархию страниц и блоков содержимого.

```python
# apps/extraction-service/models/document.py
class DocumentBlock(BaseModel):
    block_type: str
    text: str
    page: int
    bbox: Optional[list[float]] = None

class DocumentPage(BaseModel):
    page_number: int
    blocks: list[DocumentBlock] = []

class StructuredDocument(BaseModel):
    source_s3_key: str
    full_text: str
    pages: list[DocumentPage] = []
    candidate_profile: Optional[CandidateProfile] = None
    chunks: list[str] = []
    embeddings: list[list[float]] = []
    metadata: dict = {}
```

Класс **DocumentBlock** представляет атомарную единицу содержимого страницы и содержит поля: `block_type` – тип блока (заголовок, параграф, таблица, изображение), `text` – извлеченный текст блока, `page` – номер страницы, `bbox` – опциональный ограничивающий прямоугольник в координатах страницы, возвращаемый Docling. Поле `bbox` используется только для PDF-документов с выраженной структурой; для OCR-результатов и DOCX-файлов это поле остается `None`, поскольку указанные форматы не предоставляют координаты блоков в единой системе единиц.

Класс **StructuredDocument** является корневым объектом результата обработки. Поле `candidate_profile` ссылается на модель **CandidateProfile** из разделяемой библиотеки `libs/common`, что исключает дублирование схем данных между сервисами. Поля `chunks` и `embeddings` хранятся параллельными списками: элемент `embeddings[i]` является векторным представлением фрагмента `chunks[i]`. Такая структура выбрана намеренно вместо словаря или вложенных объектов, поскольку Qdrant ожидает раздельные списки идентификаторов, векторов и нагрузок при пакетной записи. Поле `metadata` зарезервировано для произвольных метаданных, специфичных для конкретного источника документа (например, идентификатора вакансии при загрузке через API ATS), без изменения основной схемы модели. Централизация классов в единой библиотеке `libs/common` исключает «дрейф схем», при котором сервисы независимо накапливают несовместимые определения одних и тех же сущностей.

---

## 4.2 Модули сервиса векторного хранилища (storage-service)

### **4.2.1** Модели векторного хранилища и маршрутизатор

Сервис хранения обеспечивает сохранение и фильтрацию векторных представлений кандидатов в базе данных Qdrant [25]. Qdrant выбран в качестве векторного хранилища благодаря нативной поддержке полезной нагрузки рядом с вектором, гибким фильтрам по метаданным, асинхронному Python-клиенту и открытой лицензии. В отличие от решений Pinecone или Weaviate, Qdrant разворачивается как самостоятельный контейнер без зависимостей от внешних сервисов, что соответствует требованиям к развертыванию системы в изолированной инфраструктуре.

Модуль `models/vector.py` определяет структуры данных, которыми обменивается сервис с остальными компонентами системы.

```python
# apps/storage-service/models/vector.py
class CandidateVectorPayload(BaseModel):
    candidate_id: UUID
    full_name: str | None = None
    skills: list[str] = []
    location: str | None = None
    years_of_experience: float | None = None
    salary_expectation: int | None = None
    languages: list[str] = []
    source_s3_key: str
    chunk_index: int
    chunk_text: str
    indexed_at: datetime
    version: int = 1

class EmbeddingPoint(BaseModel):
    id: UUID
    vector: list[float]
    payload: CandidateVectorPayload
```

Класс **CandidateVectorPayload** описывает метаданные, сохраняемые рядом с вектором в Qdrant. Поле `candidate_id` связывает все фрагменты одного резюме в единый логический объект, что позволяет при поиске сначала находить наиболее релевантные фрагменты, а затем группировать их по `candidate_id` для получения полных профилей кандидатов. Поле `chunk_index` позволяет воспроизвести оригинальный порядок фрагментов при реконструкции профиля – например, при формировании итогового отчета с развернутым текстом резюме. Поле `chunk_text` дублирует текст фрагмента непосредственно в полезной нагрузке, что позволяет кросс-энкодеру получать содержимое без дополнительных запросов к S3. Поле `version` зарезервировано для поддержки миграций схемы полезной нагрузки без перестройки коллекции.

Класс **EmbeddingPoint** является единицей обмена с Qdrant: поле `id` уникально идентифицирует точку, поле `vector` хранит эмбеддинг размерностью 1536. Выбор `UUID` в качестве типа идентификатора обоснован тем, что Qdrant поддерживает как целочисленные, так и UUID-идентификаторы точек, а UUID обеспечивает глобальную уникальность без централизованного счетчика.

```python
# apps/storage-service/routers/storage.py
router = APIRouter()

@router.post("/upsert", response_model=SuccessWrapper[UpsertResponse])
async def upsert(point: EmbeddingPoint, qdrant: QdrantDep):
    point_id = await qdrant.upsert(point)
    return SuccessWrapper.of(UpsertResponse(point_id=point_id))

@router.post("/batch-upsert", response_model=SuccessWrapper[list[str]])
async def batch_upsert(body: BatchUpsertRequest, qdrant: QdrantDep):
    ids = await qdrant.batch_upsert(body.points)
    return SuccessWrapper.of(ids)

@router.post("/filter", response_model=SuccessWrapper[list[EmbeddingPoint]])
async def filter_points(body: FilterRequest, qdrant: QdrantDep):
    results = await qdrant.filter(body.filters, body.limit)
    return SuccessWrapper.of(results)
```

Маршрутизатор экспортирует три эндпоинта. Зависимость `QdrantDep` объявлена как `Annotated[QdrantService, Depends(get_qdrant_service)]` и разрешается через механизм внедрения зависимостей FastAPI, что обеспечивает совместное использование соединения с Qdrant в рамках приложения. Эндпоинт `POST /storage/upsert` принимает одну точку и выполняет операцию «вставить или обновить» – семантика upsert гарантирует, что повторная загрузка того же резюме обновит существующие точки, а не создаст дубликаты. Эндпоинт `POST /storage/batch-upsert` оптимизирован для пакетного индексирования фрагментов резюме; Qdrant обрабатывает пакетные запросы значительно эффективнее последовательных одиночных операций благодаря внутренней буферизации записей перед коммитом в сегменты индекса. Эндпоинт `POST /storage/filter` реализует метаданные-фильтрацию без векторного поиска – применяется при доступе к профилю конкретного кандидата по его `candidate_id`.

### **4.2.2** Сервис инициализации коллекции

При запуске сервиса класс **CollectionService** проверяет наличие коллекции в Qdrant и создает ее при отсутствии. Настройка индексов на этом этапе критически влияет на производительность фильтрованного поиска.

```python
# apps/storage-service/services/collection_service.py
class CollectionService:
    def __init__(self, client: AsyncQdrantClient, collection: str):
        self._client = client
        self._collection = collection

    async def ensure_collection(self) -> None:
        exists = await self._client.collection_exists(self._collection)
        if exists:
            return
        await self._client.create_collection(
            collection_name=self._collection,
            vectors_config=VectorParams(size=1536, distance=Distance.COSINE),
            hnsw_config=HnswConfigDiff(m=16, ef_construct=100),
        )
        await self._create_payload_indexes()

    async def _create_payload_indexes(self) -> None:
        for field in ["skills", "location", "languages"]:
            await self._client.create_payload_index(self._collection, field, PayloadSchemaType.KEYWORD)
        await self._client.create_payload_index(self._collection, "years_of_experience", PayloadSchemaType.FLOAT)
        await self._client.create_payload_index(self._collection, "salary_expectation", PayloadSchemaType.INTEGER)
```

Метод `ensure_collection` реализует идемпотентную инициализацию: повторный вызов при уже существующей коллекции не производит никаких изменений. Это позволяет вызывать метод при каждом запуске сервиса, не рискуя потерять данные или получить ошибку конфликта.

В качестве метрики расстояния выбрана косинусная – `Distance.COSINE`. Косинусное сходство нечувствительно к длине вектора и оценивает угол между направлениями, что соответствует смысловой близости текстов: два семантически похожих фрагмента имеют близкие направления эмбеддингов независимо от их длины. Евклидово расстояние, напротив, учитывает абсолютную величину векторов, что делает его менее подходящим для текстовых эмбеддингов, нормализованных в единичную гиперсферу.

Параметры HNSW-индекса `m=16` и `ef_construct=100` выбраны как значения, обеспечивающие баланс между точностью приближенного поиска ближайших соседей и объемом потребляемой памяти. Параметр `m` задает число двунаправленных связей, создаваемых для каждого узла графа: большее значение улучшает полноту поиска (`recall`), но увеличивает расход памяти. Параметр `ef_construct` управляет шириной поиска при построении графа: большее значение повышает качество индекса, но замедляет вставку новых точек. Значения `m=16` и `ef_construct=100` соответствуют рекомендациям документации Qdrant для коллекций с вектором размерностью от 1000 до 2000.

Метод `_create_payload_indexes` создает индексы полезной нагрузки: поля `skills`, `location` и `languages` индексируются как тип `KEYWORD` для точного совпадения строк, поля `years_of_experience` и `salary_expectation` – как числовые типы, что позволяет применять диапазонные фильтры (`gte`, `lte`) в запросах поиска. Без явного создания индексов Qdrant выполняет фильтрацию методом полного перебора полезной нагрузки, что приемлемо для небольших коллекций, но неэффективно при десятках тысяч записей. Таким образом, инициализация коллекции является критически важным шагом для достижения целевой производительности системы при реальных объемах данных.

---

## 4.3 Модули сервиса семантического поиска (search-service)

### **4.3.1** Конфигурация и модели запросов

Сервис семантического поиска реализует гибридный конвейер, совмещающий векторный поиск в Qdrant с ре-ранжированием на основе кросс-энкодера. Класс **AppConfig** данного сервиса содержит весовые коэффициенты для итоговой агрегации оценок.

```python
# apps/search-service/core/config.py
class AppConfig(BaseSettings):
    qdrant_url: str
    embedding_api_url: str
    default_top_k: int = 10
    prefetch_multiplier: int = 3
    vector_weight: float = 0.6
    rerank_weight: float = 0.4
    reranker_model: str = "cross-encoder/ms-marco-MiniLM-L-6-v2"

settings = AppConfig()
```

```python
# apps/search-service/models/request.py
class MetadataFilters(BaseModel):
    skills: list[str] = []
    location: str | None = None
    preferred_skills: list[str] = []
    # ...

class SearchRequest(BaseModel):
    query: str
    filters: MetadataFilters = Field(default_factory=MetadataFilters)
    # ...
```

Поле `prefetch_multiplier = 3` определяет коэффициент предварительной выборки: при `top_k = 10` из Qdrant извлекается 30 кандидатов, которые затем ре-ранжируются кросс-энкодером. Такой подход позволяет компенсировать потери в точности приближенного поиска ближайших соседей: HNSW-алгоритм гарантирует высокую полноту (`recall`), но не абсолютную точность расстановки. Запрос расширенного множества кандидатов дает кросс-энкодеру возможность исправить неверную расстановку за счет более точной оценки релевантности пары «запрос – фрагмент».

Весовые коэффициенты `vector_weight = 0.6` и `rerank_weight = 0.4` задают относительный вклад каждого компонента оценки в итоговый ранг. Более высокий вес векторной оценки отражает то, что семантическое сходство является первичным критерием отбора, тогда как кросс-энкодер уточняет порядок уже отобранных кандидатов.

Класс **MetadataFilters** разделяет поля `skills` (обязательные навыки, формируют предусловие фильтрации в Qdrant) и `preferred_skills` (желаемые навыки, используются для добавления бонуса к оценке на этапе бизнес-правил). Такое разделение позволяет рекрутеру указать, какие требования являются обязательными, а какие – дополнительными, и система учтет оба типа требований по-разному: обязательные исключают несоответствующих кандидатов из выдачи, желаемые – повышают рейтинг кандидатов, обладающих дополнительными компетенциями. Флаг `use_rerank` позволяет клиенту отключить этап ре-ранжирования для ускорения ответа в сценариях, где скорость важнее точности.

### **4.3.2** Конвейер поиска и ранжирования

Функция `run_search` реализует полный семиэтапный конвейер обработки поискового запроса. Каждый этап является асинхронным, что обеспечивает неблокирующую обработку запросов.

```python
# apps/search-service/services/search_service.py
log = structlog.get_logger()

async def run_search(req: SearchRequest) -> SearchResponse:
    query_vector = await embed_query(req.query)
    prefetch_n = req.top_k * settings.prefetch_multiplier
    raw_hits = await vector_search(
        vector=query_vector,
        filters=req.filters,
        limit=prefetch_n,
    )
    if not raw_hits:
        return SearchResponse(total=0, candidates=[])
    rerank_scores = await rerank(req.query, raw_hits) if req.use_rerank else None
    candidates = _combine_scores(raw_hits, rerank_scores, req.filters)
    candidates.sort(key=lambda c: c.final_score, reverse=True)
    log.info("search_complete", query=req.query, total=len(candidates))
    return SearchResponse(total=len(candidates), candidates=candidates[: req.top_k])
```

```python
def _combine_scores(hits, rerank_scores, filters) -> list[RankedCandidate]:
    results = []
    for i, hit in enumerate(hits):
        v_score = _normalize(hit["score"], hits)
        r_score = rerank_scores[i] if rerank_scores else v_score
        bonus = _business_bonus(hit["payload"], filters)
        final = min(settings.vector_weight*v_score + settings.rerank_weight*r_score + bonus, 1.0)
        results.append(RankedCandidate(
            candidate_id=hit["payload"]["candidate_id"],
            final_score=round(final, 4),
            explanation=f"vec={v_score:.3f} re={r_score:.3f} bonus={bonus:.3f}",
        ))
    return results
```

Конвейер поиска начинается с преобразования текстового запроса в вектор той же размерности и той же моделью, что применялась при индексировании документов. Это критически важно: использование разных моделей для запроса и документов сделает косинусные расстояния бессмысленными. Функция `embed_query` отправляет запрос к тому же API эмбеддингов, что и extraction-service, адрес которого задается через конфигурацию.

После получения вектора запроса вызывается `vector_search`, передающий в Qdrant фильтры метаданных. Фильтры по обязательным навыкам, локации и диапазону зарплаты применяются до векторного поиска на уровне индекса Qdrant, что снижает объем данных, по которым выполняется поиск ближайших соседей. Ранняя проверка `if not raw_hits` предотвращает запуск дорогостоящего ре-ранжирования при пустой выборке.

Функция `_combine_scores` реализует агрегацию оценок по формуле: `final = 0.6 · v_score + 0.4 · r_score + bonus`, где `bonus` ограничен значением 0.10 и начисляется по правилам: `+0.02` за каждый совпавший предпочитаемый навык и `+0.03` при совпадении языка. Ограничение `min(final, 1.0)` гарантирует, что итоговая оценка не выйдет за пределы диапазона `[0, 1]`, сохраняя семантику вероятностной меры. Нормализация векторных оценок выполняется методом min-max по набору результатов текущего запроса, что обеспечивает сопоставимость оценок независимо от плотности распределения в конкретной коллекции: при малом числе близких кандидатов «сырые» косинусные оценки могут оказаться в узком диапазоне, и нормализация растягивает его до `[0, 1]`.

Поле `explanation` формирует текстовое разложение итоговой оценки, которое передается на клиент для объяснимости результатов поиска. Рекрутер видит не только итоговую оценку кандидата, но и то, за счет каких компонентов она сложилась, что повышает доверие к системе и облегчает ручную проверку граничных случаев.

### **4.3.3** Модуль ре-ранжирования

Модуль ре-ранжирования инкапсулирует модель **CrossEncoder** `cross-encoder/ms-marco-MiniLM-L-6-v2` из библиотеки sentence-transformers [26]. Использование декоратора `lru_cache` гарантирует однократную загрузку весов модели в оперативную память процесса.

```python
# apps/search-service/services/reranker.py
@lru_cache(maxsize=1)
def _get_model() -> CrossEncoder:
    return CrossEncoder(settings.reranker_model)

async def rerank(query: str, candidates: list[dict]) -> list[float]:
    pairs = [
        [query, c["payload"].get("chunk_text", "")]
        for c in candidates
    ]
    loop = asyncio.get_event_loop()
    raw_scores: list[float] = await loop.run_in_executor(
        None, _get_model().predict, pairs
    )
    min_s, max_s = min(raw_scores), max(raw_scores)
    span = max_s - min_s or 1.0
    return [(s - min_s) / span for s in raw_scores]
```

Кросс-энкодер оценивает пары «запрос – документ» совместно, а не независимо, как это делает би-энкодер при генерации эмбеддингов. Это позволяет модели учитывать взаимодействие между словами запроса и словами документа напрямую, что обеспечивает более высокую точность ранжирования по сравнению с простым косинусным сходством. Недостаток кросс-энкодера – вычислительная стоимость растет линейно с числом пар, поэтому его применяют не к полной коллекции, а только к предварительно отобранным кандидатам.

Метод `CrossEncoder.predict` является синхронным и потенциально ресурсоемким, поэтому его вызов делегируется в пул потоков через `loop.run_in_executor`. Это предотвращает блокировку цикла событий asyncio на время вывода нейронной сети, позволяя обрабатывать другие входящие запросы параллельно. Защита `span = max_s - min_s or 1.0` предотвращает деление на ноль в вырожденном случае, когда все оценки кросс-энкодера равны (например, при единственном кандидате в выборке). Нормализация «сырых» оценок кросс-энкодера в диапазон `[0, 1]` выполняется после предсказания методом min-max по набору текущего запроса, что обеспечивает совместимость шкал с нормализованными векторными оценками из Qdrant. Таким образом, модуль ре-ранжирования повышает качество итоговой выдачи, не влияя на этап первичного отбора по вектору.

---

## 4.4 Модули сервиса оркестрации (orchestrator-service)

### **4.4.1** Модуль инициализации и управления жизненным циклом

Сервис оркестрации координирует взаимодействие всех остальных компонентов системы посредством агентных графов на базе LangGraph [27]. Для корректного управления соединением с базой данных при инициализации и завершении работы используется механизм `lifespan` из FastAPI.

```python
# apps/orchestrator-service/main.py
from contextlib import asynccontextmanager
from fastapi import FastAPI
from services.orchestrator_service import orchestrator_service
from routers.orchestrator import router
from routers.webhooks import router as webhooks_router

@asynccontextmanager
async def lifespan(app: FastAPI):
    await orchestrator_service.initialize()
    yield
    await orchestrator_service.shutdown()

app = FastAPI(title="orchestrator-service", version="1.0.0", lifespan=lifespan)
app.include_router(router, prefix="/orchestrate")
app.include_router(webhooks_router, prefix="/webhooks")
```

Механизм `lifespan` является рекомендуемым способом управления ресурсами приложения начиная с FastAPI 0.93 и заменяет устаревшие обработчики событий `startup`/`shutdown`. Конструкция `async with` обеспечивает корректное выполнение блока `shutdown` даже при аварийном завершении сервера.

Метод `orchestrator_service.initialize()` выполняет три действия: создает чекпойнтер LangGraph на базе PostgreSQL и инициализирует схему таблиц через `checkpointer.setup()`; компилирует граф загрузки резюме `build_upload_graph` и граф поиска `build_search_graph` с передачей созданного чекпойнтера; устанавливает соединения с HTTP-клиентами для взаимодействия с extraction-service, storage-service и search-service. Компиляция графов выполняется один раз при запуске, а не при каждом запросе, что исключает накладные расходы на валидацию топологии графа в критическом пути обработки.

Использование `lifespan` в FastAPI обеспечивает единственный вызов инициализации при запуске и корректное освобождение ресурсов при завершении работы контейнера. Два маршрутизатора – `/orchestrate` и `/webhooks` – разнесены по разным модулям: первый отвечает за синхронные запросы пользователя, второй – за регистрацию и доставку асинхронных уведомлений.

### **4.4.2** Модель состояния агента

Класс **OrchestratorState** является центральным объектом передачи данных внутри графов LangGraph. Все узлы графа получают и возвращают экземпляр этого класса, обновляя только те поля, за которые отвечают.

```python
# apps/orchestrator-service/models/state.py
class OrchestratorState(BaseModel):
    user_query: str
    query_type: Literal["upload", "search", "batch", "get_profile"] = "search"
    correlation_id: str
    auth_token: str
    extracted_document: Optional[dict] = None
    candidate_profile: Optional[dict] = None
    search_results: Optional[list[dict]] = None
    reflection: Optional[dict] = None
    retry_count: int = 0
    revised_query: Optional[str] = None
    max_retries: int = 2
    messages: list = Field(default_factory=list)
    error: Optional[str] = None
    metadata: dict = Field(default_factory=dict)
```

Архитектурное решение об использовании единой модели состояния для всех графов обусловлено особенностями LangGraph: каждый узел принимает полное состояние и возвращает частичное обновление через `model_copy(update={...})`. Такой подход реализует паттерн «неизменяемое состояние с явными переходами», при котором каждый узел не мутирует объект, а создает новую версию с измененными полями.

Класс содержит следующие группы полей. Поля идентификации запроса: `user_query`, `query_type`, `correlation_id`, `auth_token`. Поля результатов этапов обработки: `extracted_document` заполняется узлом извлечения, `candidate_profile` – узлом хранения, `search_results` – узлом поиска. Поля управления повторными попытками: `reflection` содержит оценку качества результатов, `retry_count` отслеживает число выполненных попыток, `revised_query` хранит уточненный запрос, предложенный агентом рефлексии. Ограничитель `max_retries = 2` предотвращает бесконечные циклы в случае систематически низкого качества поисковой выдачи.

Поле `messages` используется LangGraph для сохранения истории сообщений ReAct-агентов в формате, совместимом с LangChain `BaseMessage`. Благодаря чекпойнтеру эта история персистируется в PostgreSQL, что позволяет возобновить сессию агента после перезапуска сервиса с сохранением полного контекста диалога. Поле `error` фиксирует описание ошибки при переходе в узел обработки исключений, чтобы информация об ошибке была доступна для формирования ответа пользователю и записи в журнал.

### **4.4.3** Граф обработки загрузки резюме

Рабочий процесс загрузки резюме реализован в виде направленного ациклического графа с тремя основными узлами. Функция `build_upload_graph` компилирует граф с опциональным чекпойнтером для персистентного сохранения промежуточных состояний.

```python
# apps/orchestrator-service/graphs/resume_upload_graph.py
def build_upload_graph(checkpointer=None):
    builder = StateGraph(OrchestratorState)
    builder.add_node("router",  _router_node)
    builder.add_node("extract", extract_node)
    builder.add_node("store",   store_node)
    builder.add_node("error",   error_handler)
    builder.set_entry_point("router")
    builder.add_edge("router",  "extract")
    builder.add_edge("extract", "store")
    builder.add_edge("store",   END)
    builder.add_edge("error",   END)
    return builder.compile(checkpointer=checkpointer)

async def _router_node(state: OrchestratorState) -> OrchestratorState:
    if state.query_type not in ("upload", "batch"):
        return state.model_copy(update={"error": "wrong_graph"})
    return state
```

Граф загрузки реализует линейный поток: `router → extract → store → END`. Узел `router` выполняет защитную проверку типа запроса, предотвращая попадание в граф запросов неверного типа. Это особенно важно в сценариях, когда сервис оркестрации маршрутизирует входящие запросы к соответствующим графам программно, а не через отдельные эндпоинты.

Узел `extract` реализован как ReAct-агент (Reasoning and Acting) на базе функции `create_react_agent` из LangGraph. Агент вооружен инструментом `extract_resume`, который выполняет HTTP-вызов к extraction-service и возвращает **StructuredDocument** в поле `extracted_document` состояния. Использование ReAct-агента вместо прямого HTTP-вызова позволяет агенту при необходимости вызвать инструмент повторно с измененными параметрами, например, при временной недоступности extraction-service.

Узел `store` получает из состояния извлеченные фрагменты и эмбеддинги, формирует список объектов **EmbeddingPoint** и выполняет пакетную запись в storage-service через эндпоинт `/storage/batch-upsert`. Параметр `checkpointer` обеспечивает персистентность состояния в PostgreSQL после каждого завершенного узла, что позволяет возобновить обработку после сбоя с точки последнего сохраненного шага, а не с начала пайплайна.

### **4.4.4** Граф семантического поиска кандидатов

Граф поиска реализует расширенный рабочий процесс с узлом рефлексии и условным ребром повторного поиска. Данная архитектура позволяет агенту самостоятельно оценивать качество результатов и уточнять запрос при недостаточном ранжировании.

```python
# apps/orchestrator-service/graphs/candidate_search_graph.py
def build_search_graph(checkpointer=None):
    builder = StateGraph(OrchestratorState)
    builder.add_node("router",     _router_node)
    builder.add_node("search",     search_node)
    builder.add_node("reflection", reflection_node)
    builder.add_node("format",     format_node)
    builder.add_node("error",      error_handler)
    builder.set_entry_point("router")
    builder.add_edge("router", "search")
    builder.add_edge("search", "reflection")
    builder.add_conditional_edges(
        "reflection",
        should_retry,
        {"retry": "search", "done": "format"},
    )
    builder.add_edge("format", END)
    builder.add_edge("error",  END)
    return builder.compile(checkpointer=checkpointer)
```

```python
# apps/orchestrator-service/graphs/common_nodes.py
async def reflection_node(state: OrchestratorState) -> OrchestratorState:
    results = state.search_results or []
    quality = "high" if len(results) >= 3 else "low"
    revised = state.revised_query
    if quality == "low":
        revised = await _llm_suggest_query(state.user_query, results)
    return state.model_copy(update={"reflection": {"quality": quality}, "revised_query": revised})

def should_retry(state: OrchestratorState) -> Literal["retry", "done"]:
    quality = (state.reflection or {}).get("quality", "high")
    if quality == "low" and state.retry_count < state.max_retries:
        return "retry"
    return "done"
```

Паттерн рефлексии (Reflection) является одним из базовых агентных паттернов: агент не только выполняет действие, но и оценивает его результат, принимая решение о продолжении или коррекции. В данной реализации узел `reflection_node` оценивает число найденных кандидатов как простейший индикатор качества: порог в три результата выбран как минимально приемлемое число для сравнительного анализа.

При выявлении низкого качества функция `_llm_suggest_query` передает языковой модели исходный запрос и текущие (недостаточные) результаты с просьбой предложить альтернативную формулировку запроса, охватывающую более широкий круг компетенций или использующую синонимичные термины. Уточненный запрос записывается в поле `revised_query` и на следующей итерации замещает исходный `user_query` в обращении к search-service.

Условный переход `add_conditional_edges` направляет выполнение из узла `reflection` либо обратно в узел `search` (ветка `"retry"`), либо в узел `format` (ветка `"done"`). Функция `should_retry` реализует логику принятия решения: повтор выполняется только при одновременном выполнении двух условий – низкое качество результатов (`quality == "low"`) и неисчерпанный лимит повторных попыток (`retry_count < max_retries`). Счетчик `retry_count` инкрементируется в начале узла `search_node` при обнаружении непустого `revised_query`, что предотвращает зачет начальной итерации в счетчик повторных попыток.

### **4.4.5** Модуль LLM-агентов

Агенты системы строятся с помощью функции `create_react_agent` из LangGraph, принимающей языковую модель, список инструментов и системную инструкцию. Языковая модель инициализируется один раз через `lru_cache` и используется совместно всеми агентами.

```python
# apps/orchestrator-service/services/agents/_llm.py
@lru_cache(maxsize=1)
def get_llm() -> ChatOpenAI:
    return ChatOpenAI(
        model=settings.llm_model,
        openai_api_base=settings.llm_api_url,
        openai_api_key="not-used",
        temperature=0,
    )
```

```python
# apps/orchestrator-service/services/agents/router_agent.py
class RouterOutput(BaseModel):
    query_type: Literal["upload", "search", "batch", "get_profile"]

async def route_query(user_query: str) -> Literal["upload", "search", "batch", "get_profile"]:
    structured_llm = get_llm().with_structured_output(RouterOutput)
    result: RouterOutput = await structured_llm.ainvoke(ROUTER_PROMPT.format(user_query=user_query))
    return result.query_type
```

Функция `get_llm` создает клиент **ChatOpenAI** с `temperature=0` для детерминированных ответов и подключается к собственному LLM-прокси по адресу `llm_api_url`, что позволяет независимо заменять модель без изменения кода агентов. Значение `openai_api_key="not-used"` является плейсхолдером, поскольку аутентификация к LLM-прокси выполняется на сетевом уровне, а не через заголовок `Authorization`. Параметр `temperature=0` устраняет случайность в ответах языковой модели для задач классификации и структурированного вывода: при фиксированном промпте система всегда возвращает одинаковый результат, что делает ее поведение предсказуемым и тестируемым.

Метод `with_structured_output` обязывает языковую модель возвращать ответ, соответствующий схеме Pydantic **RouterOutput**. Под капотом LangChain транслирует схему Pydantic в JSON Schema и передает ее в параметр `tools` запроса к языковой модели, используя механизм Function Calling. Это исключает необходимость синтаксического разбора неструктурированного текста и гарантирует принадлежность результата классификации одному из четырех допустимых типов. Метод `ainvoke` выполняет вызов асинхронно, не блокируя цикл событий в ожидании ответа от LLM API.

### **4.4.6** Система асинхронных уведомлений (webhooks)

Для поддержки асинхронного режима взаимодействия сервис оркестрации реализует систему вебхуков, позволяющую клиентам регистрировать URL обратного вызова и получать уведомления о завершении длительных операций.

```python
# apps/orchestrator-service/routers/webhooks.py
router = APIRouter()
_registry: dict[str, str] = {}   # correlation_id → callback_url

@router.post("/register")
async def register(correlation_id: str, callback_url: str) -> dict:
    _registry[correlation_id] = callback_url
    return {"registered": True}

async def notify(correlation_id: str, *, job_status: str,
                 query_type: str, result=None, error=None) -> None:
    url = _registry.pop(correlation_id, None)
    if not url:
        return
    payload = WebhookPayload(
        correlation_id=correlation_id,
        job_status=job_status,
        query_type=query_type,
        result=result,
        error=error,
    )
    asyncio.create_task(_deliver(url, payload))

async def _deliver(url: str, payload: WebhookPayload) -> None:
    async with httpx.AsyncClient(timeout=10.0) as client:
        await client.post(url, json=payload.model_dump())
```

Реестр `_registry` хранится в памяти процесса как словарь «correlation\_id → URL». Для текущей архитектуры в рамках единственного экземпляра сервиса это решение является достаточным; при необходимости горизонтального масштабирования реестр может быть перенесен в Redis с TTL-записями без изменения интерфейса модуля.

Метод `notify` извлекает запись из реестра с одновременным удалением (`dict.pop`), что гарантирует однократную доставку уведомления и автоматически освобождает память. Если соответствующий correlation\_id не найден в реестре (клиент не регистрировал вебхук или уведомление уже было доставлено), функция завершается без действий. Доставка производится асинхронно через `asyncio.create_task`, не блокируя возврат ответа вызывающему коду: граф завершает обработку, возвращает ответ в `notify`, после чего HTTP-запрос к клиентскому URL выполняется в фоне. Таймаут HTTP-клиента ограничен 10 секундами, после чего задача завершается без повторной попытки, что предотвращает накопление зависших соединений при недоступности клиентского сервера. Сигнатура `notify` использует keyword-only параметры (синтаксис `*`), что исключает случайное перемешивание аргументов при вызове.

---

## 4.5 Модули TypeScript-сервисов (NestJS)

### **4.5.1** Сервис аутентификации (auth-service)

Сервис аутентификации реализован на платформе NestJS [18] и делегирует проверку учетных данных серверу Keycloak [28] по протоколу OIDC. Keycloak выступает централизованным провайдером идентификации: он хранит учетные записи пользователей, определяет роли и выдает JWT-токены по стандарту RFC 7519. Сам auth-service не хранит паролей пользователей, что снижает поверхность атаки и упрощает соответствие требованиям безопасности.

Модуль `AppModule` конфигурирует подключение к PostgreSQL для хранения журнала аудита.

```typescript
// apps/auth-service/src/app.module.ts
@Module({
  imports: [
    TypeOrmModule.forRoot({
      type: 'postgres',
      host: process.env.DB_HOST ?? 'postgres',
      port: parseInt(process.env.DB_PORT ?? '5432', 10),
      username: process.env.DB_USER ?? 'postgres',
      password: process.env.DB_PASSWORD ?? 'postgres',
      database: process.env.DB_NAME ?? 'auth_db',
      entities: [AuditLog],
      synchronize: process.env.NODE_ENV !== 'production',
    }),
    AuthModule,
    AuditModule,
  ],
})
export class AppModule {}
```

Параметр `synchronize` деклараций TypeORM деактивируется в производственном окружении для предотвращения автоматических миграций схемы при каждом запуске: в режиме разработки это удобно для быстрой итерации, но в production приводит к риску непредвиденного изменения структуры таблиц. Для production-окружения миграции выполняются явно через CLI TypeORM до деплоя новой версии сервиса. Класс **AuditLog** является единственной сущностью данного сервиса и фиксирует события входа и выхода с указанием идентификатора пользователя, временной метки и IP-адреса клиента, обеспечивая соответствие требованиям ведения журнала безопасности.

```typescript
// apps/auth-service/src/auth/auth.controller.ts
@Controller('auth')
export class AuthController {
  constructor(private readonly authService: AuthService) {}

  @Post('login')
  @HttpCode(HttpStatus.OK)
  async login(@Body() dto: LoginDto) {
    return this.authService.login(dto.username, dto.password);
  }

  @Post('token/introspect')
  @HttpCode(HttpStatus.OK)
  introspect(@Body() dto: IntrospectDto) {
    return this.authService.introspect(dto.token);
  }

  @Get('roles')
  @UseGuards(JwtAuthGuard, RolesGuard)
  @Roles('recruiter', 'admin', 'system')
  getRoles(@Request() req: { user: AuthUser }) {
    return { roles: req.user.roles };
  }
}
```

Контроллер **AuthController** экспортирует три эндпоинта. Метод `login` принимает учетные данные и делегирует аутентификацию в Keycloak через endpoint `/realms/{realm}/protocol/openid-connect/token` по потоку `password grant`. Keycloak возвращает пару токенов `access_token` / `refresh_token`, которые передаются клиенту без сохранения в базе данных auth-service. Данный поток используется для машинного взаимодействия (M2M); для пользовательского браузерного доступа рекомендуется Authorization Code Flow, настройка которого выполняется на уровне Keycloak-клиента.

Метод `introspect` проверяет валидность переданного токена через Keycloak Introspection Endpoint – применяется сервисами бэкенда для верификации запросов без хранения сессий. Такой подход реализует «stateless»-аутентификацию: каждый сервис самостоятельно верифицирует токен, не обращаясь к централизованному хранилищу сессий, что упрощает горизонтальное масштабирование.

Метод `getRoles` защищен декораторами `@UseGuards(JwtAuthGuard, RolesGuard)` и `@Roles('recruiter', 'admin', 'system')`, демонстрируя реализацию контроля доступа на основе ролей (RBAC). Декоратор `@Roles` записывает список допустимых ролей в метаданные маршрута через `Reflector`, а `RolesGuard` считывает их при обработке запроса и сравнивает с ролями, извлеченными из JWT. Декоратор `@HttpCode(HttpStatus.OK)` для метода `login` явно задает код ответа 200, переопределяя умолчание NestJS для `POST`-методов (201 Created), поскольку login не создает новый ресурс.

### **4.5.2** Модуль BFF (Backend-for-Frontend)

Сервис **Backend-for-Frontend** выступает прокси-слоем между браузерным клиентом и микросервисами бэкенда. Паттерн BFF [30] решает проблему несоответствия между детализированными API микросервисов и потребностями конкретного клиента: BFF агрегирует данные из нескольких сервисов, адаптирует форматы ответов и берет на себя обработку JWT, освобождая браузерный клиент от прямого взаимодействия с микросервисами. Он принимает файлы и поисковые запросы, извлекает JWT из куки и перенаправляет вызовы в сервис оркестрации от имени аутентифицированного пользователя.

```typescript
// apps/frontend/backend-for-frontend/src/controllers/upload.controller.ts
@Controller('api')
export class UploadController {
  constructor(private readonly proxy: OrchestratorProxy) {}

  @Post('upload')
  @UseInterceptors(FileInterceptor('file'))
  async upload(
    @UploadedFile() file: Express.Multer.File,
    @Req() req: Request,
  ): Promise<{ correlation_id: string }> {
    return this.proxy.proxyUpload(file.buffer, file.originalname, file.mimetype, extractJwt(req));
  }
}
```

```typescript
// apps/frontend/backend-for-frontend/src/services/orchestrator.proxy.ts
@Injectable()
export class OrchestratorProxy {
  private readonly baseUrl =
    process.env.ORCHESTRATOR_URL ?? 'http://orchestrator-service:8004';

  async proxyUpload(buffer: Buffer, name: string, mimetype: string, jwt: string): Promise<{ correlation_id: string }> {
    const form = new FormData();
    form.append('file', buffer, { filename: name, contentType: mimetype });
    const { data } = await axios.post(
      `${this.baseUrl}/orchestrate/upload`,
      form,
      { headers: { ...form.getHeaders(), Authorization: `Bearer ${jwt}` } },
    );
    return data;
  }
}
```

Декоратор `@UseInterceptors(FileInterceptor('file'))` подключает Multer – промежуточный слой обработки `multipart/form-data` – и ограничивает размер принимаемого файла значением `max_file_size_mb` из конфигурации. Утилита `extractJwt` читает токен из HTTP-only куки `access_token`, установленной при входе в систему, тем самым исключая необходимость хранения токена в JavaScript-контексте браузера и снижая риск XSS-атак.

Класс **OrchestratorProxy** реализует паттерн «прокси-сервис» в архитектуре BFF. Метод `proxyUpload` упаковывает бинарные данные файла в объект **FormData** и передает их в orchestrator-service с заголовком `Authorization: Bearer <jwt>`. Вызов `form.getHeaders()` возвращает заголовки Multer, включая `Content-Type: multipart/form-data; boundary=...` с корректным значением boundary, автоматически сгенерированным для данного запроса. Адрес сервиса оркестрации разрешается из переменной окружения `ORCHESTRATOR_URL`, что позволяет без изменений кода переключаться между окружениями разработки и производства. Возвращаемый объект `{ correlation_id }` передается клиенту, который использует его для опроса статуса или регистрации вебхука. Инкапсуляция логики проксирования в отдельном injectable-сервисе упрощает тестирование контроллера: в тестах достаточно подставить мок `OrchestratorProxy` с фиксированным ответом.

---

## 4.6 Разделяемые библиотеки (libs)

### **4.6.1** Общие модели данных

Библиотека `libs/common` содержит канонические модели данных, разделяемые между всеми Python-сервисами системы. Согласно принятому соглашению, ни один сервис не переопределяет эти модели самостоятельно. Централизация схем данных решает проблему «дрейфа схем» (schema drift): без единого источника истины каждый сервис может незаметно накопить несовместимые определения одних и тех же сущностей, что приводит к ошибкам десериализации на границах сервисов.

```python
# libs/common/models/candidate.py
class SkillEntry(BaseModel):
    name: str
    level: Optional[Literal["junior", "middle", "senior"]] = None
    category: Optional[str] = None

class ExperienceEntry(BaseModel):
    company: str
    position: str
    start_date: Optional[str] = None
    end_date: Optional[str] = None
    technologies: list[str] = []

class CandidateProfile(BaseModel):
    id: UUID = Field(default_factory=uuid4)
    full_name: Optional[str] = None
    email: Optional[str] = None
    location: Optional[str] = None
    languages: list[str] = []
    experience: list[ExperienceEntry] = []
    skills: list[SkillEntry] = []
    salary_expectation: Optional[int] = None
    years_of_experience: Optional[float] = None
    raw_text_hash: str = ""

    @field_validator("email")
    @classmethod
    def validate_email(cls, v: str | None) -> str | None:
        if v and not re.match(r"^[\w.+-]+@[\w-]+\.[a-z]{2,}$", v, re.I):
            raise ValueError("invalid email")
        return v
```

Класс **CandidateProfile** является центральной схемой данных системы. Поле `id` генерируется автоматически с помощью `uuid4`, обеспечивая уникальность каждого профиля без внешней координации – разные экземпляры сервиса могут создавать профили независимо без обращения к централизованному счетчику или базе данных.

Класс **SkillEntry** содержит уровень владения навыком в виде перечисления с тремя значениями `Literal["junior", "middle", "senior"]`, что позволяет применять фильтрацию по грейду в поисковых запросах. Перечисление намеренно зафиксировано тремя значениями для упрощения индексирования: расширенные грейды (например, `"lead"` или `"principal"`) могут быть добавлены в будущем при обновлении схемы с инкрементом поля `version` в **CandidateVectorPayload**.

Класс **ExperienceEntry** хранит список используемых технологий (`technologies`) обособленно от текстового описания должности. Это разделение позволяет языковой модели при извлечении профиля точнее идентифицировать технический стек каждой должности, а поисковой системе – индексировать технологии как отдельные ключевые слова в составе полезной нагрузки Qdrant. Поле `raw_text_hash` содержит SHA-256 хэш исходного текста резюме, что позволяет storage-service определять, изменилось ли содержимое при повторной загрузке, и избегать дорогостоящей перегенерации эмбеддингов для неизмененных документов.

Валидатор `validate_email` применяется автоматически при создании объекта Pydantic v2, используя декоратор `@field_validator` с классовым методом. Декоратор `@classmethod` обязателен для валидаторов Pydantic v2 в отличие от Pydantic v1, где методы могли быть обычными. Валидатор допускает значение `None`, поскольку поле `email` является опциональным, но отклоняет синтаксически некорректные адреса электронной почты, предотвращая попадание в хранилище заведомо ошибочных данных.

### **4.6.2** Обертки ответов и иерархия исключений

Для обеспечения единообразного формата HTTP-ответов во всех сервисах определены универсальные обертки **SuccessWrapper** и **ErrorWrapper**, реализующие паттерн «конверт ответа».

```python
# libs/common/models/wrappers.py
T = TypeVar("T")

class SuccessWrapper(BaseModel, Generic[T]):
    success: Literal[True] = True
    data: T

    @classmethod
    def of(cls, data: T) -> "SuccessWrapper[T]":
        return cls(data=data)

class ErrorWrapper(BaseModel):
    success: Literal[False] = False
    error: str
    detail: Optional[str] = None
```

```python
# libs/common/exceptions/base.py
class ServiceException(Exception):
    status_code: int = 500
    def __init__(self, message: str, status_code: int | None = None):
        super().__init__(message)
        self.message = message
        if status_code is not None: self.status_code = status_code

class BadRequestError(ServiceException):   status_code = 400
class UnauthorizedError(ServiceException): status_code = 401
class ForbiddenError(ServiceException):    status_code = 403
class NotFoundError(ServiceException):     status_code = 404
class ConflictError(ServiceException):     status_code = 409
class ValidationError(ServiceException):   status_code = 422
class ExternalServiceError(ServiceException):    status_code = 502
class ServiceUnavailableError(ServiceException): status_code = 503
```

Класс **SuccessWrapper** является обобщенным (`Generic[T]`) и принимает тип тела ответа в качестве параметра типа. Параметризованная аннотация `SuccessWrapper[list[RankedCandidate]]` однозначно описывает схему ответа эндпоинта поиска в аргументе `response_model` декоратора маршрута. FastAPI использует эту информацию для генерации JSON Schema в документации OpenAPI, а также для сериализации ответа с исключением полей, помеченных как приватные. Фабричный метод `of` сокращает создание объекта до одной строки и исключает риск опечатки при ручном конструировании.

Дискриминирующий литерал `success: Literal[True]` позволяет клиентам (в том числе TypeScript-BFF) различать успешный и ошибочный ответ по единственному полю без анализа HTTP-статуса. При использовании `discriminatedUnion` в TypeScript компилятор автоматически сужает тип переменной: в ветке `if (response.success)` переменная типизируется как `SuccessWrapper`, а в ветке `else` – как `ErrorWrapper`.

Иерархия исключений **ServiceException** отображает HTTP-семантику: каждый подкласс фиксирует соответствующий код статуса как атрибут класса. Глобальный обработчик исключений в каждом сервисе регистрируется через декоратор `@app.exception_handler(ServiceException)` и реализует следующую логику: он перехватывает любой подкласс `ServiceException`, формирует ответ с кодом `exc.status_code` и телом **ErrorWrapper**, содержащим поле `error` с сообщением исключения и опциональное поле `detail` с трассировкой для неожиданных ошибок. Конструктор `ServiceException.__init__` позволяет переопределить `status_code` динамически при необходимости, хотя в большинстве случаев используется значение, заданное на уровне класса.

### **4.6.3** Утилиты LangGraph (libs/langchain-utils)

Библиотека `libs/langchain-utils` инкапсулирует повторно используемые примитивы для агентных графов: фабрику чекпойнтера и шаблоны промптов.

```python
# libs/langchain-utils/checkpointer.py
def make_checkpointer(dsn: str) -> AsyncPostgresSaver:
    return AsyncPostgresSaver.from_conn_string(dsn)
```

```python
# libs/langchain-utils/prompts/router.py
ROUTER_PROMPT = """\
Определи тип запроса пользователя и верни строго JSON без пояснений:
{{"query_type": "<upload|search|batch|get_profile>"}}

Правила классификации:
- upload      – пользователь загружает резюме или файл кандидата;
- search      – поиск кандидатов по описанию требований;
- batch       – массовая загрузка нескольких резюме;
- get_profile – получение профиля конкретного кандидата.
Запрос: {user_query}
"""
```

Функция `make_checkpointer` оборачивает конструктор **AsyncPostgresSaver** из пакета `langgraph-checkpoint-postgres` [29]. PostgreSQL выбран в качестве бэкенда чекпойнтера по двум причинам: во-первых, PostgreSQL уже присутствует в инфраструктуре системы как база данных auth-service, что исключает необходимость развертывания дополнительного компонента; во-вторых, PostgreSQL обеспечивает ACID-гарантии при записи промежуточных состояний, что критически важно для корректного восстановления после сбоя.

Функция возвращает неинициализированный объект. Ответственность за вызов `checkpointer.setup()` возлагается на вызывающий код (метод `OrchestratorService.initialize`), что явно разграничивает создание объекта и его подготовку к работе. Метод `setup()` создает таблицы `checkpoints`, `checkpoint_writes` и `checkpoint_blobs` в базе данных при их отсутствии, аналогично паттерну `ensure_collection` в storage-service.

Шаблон `ROUTER_PROMPT` содержит детерминированные правила классификации, снижающие вероятность неверной маршрутизации запроса. Двойные фигурные скобки `{{` и `}}` в строке – это экранирование, необходимое для форматирования через `str.format`: одиночная `{user_query}` является заменяемым местом, а `{{"query_type": ...}}` – литеральным JSON в тексте промпта. Инструкция «верни строго JSON без пояснений» совместно с методом `with_structured_output` языковой модели образует двухуровневую защиту: даже если модель вернет JSON с пояснительным текстом, механизм Function Calling извлечет нужные поля из структурированного ответа автоматически.
