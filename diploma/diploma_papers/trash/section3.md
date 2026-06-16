**3 Функциональное проектирование**

В данном разделе рассмотрено функционирование серверной части приложения. Проведен анализ входных и выходных данных, основных модулей программы и их зависимостей. Также подробно описаны все ключевые классы, реализованные в проекте, с указанием их полей, методов и особенностей реализации.

Серверная часть приложения построена на микросервисной архитектуре. Пять основных сервисов реализованы на Python с использованием фреймворка FastAPI, один сервис (frontend) и сервис аутентификации — на TypeScript с фреймворком NestJS. Внедрение зависимостей и конфигурация осуществляются через Pydantic Settings (singleton-паттерн). Логирование выполняется через structlog, корреляция запросов — через CorrelationMiddleware. Все сервисы общаются по HTTP с использованием JWT-токенов (через auth-service) и поддерживают асинхронную обработку.

Разрабатываемое приложение можно разделить на следующие модули (сервисы):

- модуль `extraction-service`;
- модуль `storage-service`;
- модуль `search-service`;
- модуль `orchestrator-service`;
- модуль `frontend`;
- модуль `auth-service`.

Диаграмма всех классов приложения представлена на чертеже ГУИР.400201.016 С1.

### 3.1 Анализ данных

В качестве входных данных система принимает два основных типа запросов: загрузку резюме (файл) и поисковый запрос рекрутера.

Требования ко входным данным приложения представлены в таблице 3.1.

**Таблица 3.1 – Требования ко входным данным**

| Категория          | Требование                                                                 |
|--------------------|----------------------------------------------------------------------------|
| Формат файла       | PDF, DOCX, DOC, PNG, JPG, JPEG (максимальный размер — 20 МБ)              |
| Заголовки          | `X-Correlation-ID` (обязательный), `Authorization: Bearer <token>`         |
| Поисковый запрос   | JSON-объект `SearchRequest` (query, filters, top_k, use_rerank)           |
| Аутентификация     | Валидный JWT-токен (роли: recruiter, admin, system)                       |

На вход пайплайна обработки поступают сырые файлы резюме, которые после парсинга преобразуются в структурированный профиль кандидата (`CandidateProfile`). Для поиска используется текстовый запрос на естественном языке с дополнительными фильтрами по навыкам, опыту, зарплате и локации.

В качестве выходных данных система формирует структурированные JSON-ответы. Требования к выходным данным представлены в таблице 3.2.

**Таблица 3.2 – Требования к выходным данным**

| Категория              | Требование                                                                 |
|------------------------|----------------------------------------------------------------------------|
| Формат ответа          | `ExtractionResponse` / `SearchResponse` / `OrchestratorResponse` (Pydantic) |
| Структура профиля      | `StructuredDocument` + `CandidateProfile` + чанки + эмбеддинги             |
| Формат поиска          | `RankedCandidate[]` с `final_score`, `matched_skills`, `explanation`       |
| Кодировка              | UTF-8                                                                      |
| Real-time              | WebSocket-события по `correlation_id` (обновления статуса)                |

### 3.2 Базовые классы моделей данных

Все сервисы используют общие Pydantic-модели v2 из библиотеки `libs/common`. Базовые классы обеспечивают единообразие структур данных, валидацию и сериализацию.

**3.2.1** Класс `ExperienceEntry` (файл `libs/common/models/candidate.py`) является вложенной Pydantic-моделью, описывающей один период трудовой деятельности кандидата.

Поле `company` типа `str` хранит название организации и не может быть пустым — это обеспечивается валидатором `non_empty_str`, который обрезает пробелы и выбрасывает ошибку при пустой строке.
Поле `position` типа `str` хранит должность кандидата и проходит ту же валидацию, что и `company`.
Поле `start_date` типа `Optional[str]` хранит дату начала работы в формате `YYYY` или `YYYY-MM`; допускает значение `None`.
Поле `end_date` типа `Optional[str]` хранит дату окончания в том же формате.
Поле `description` типа `Optional[str]` содержит произвольное текстовое описание обязанностей.
Поле `technologies` типа `list[str]` содержит список технологий, применявшихся на данной позиции; валидатор `strip_technologies` фильтрует пустые строки и обрезает пробелы.

Валидатор `validate_date_str` проверяет формат дат с помощью регулярного выражения `^\d{4}(-\d{2})?$`.
Валидатор модели `end_after_start` проверяет, что `end_date` не предшествует `start_date`, и выбрасывает `ValueError` при нарушении хронологии.

**3.2.2** Класс `SkillEntry` (тот же файл) описывает одиночный навык кандидата.

Поле `name` типа `str` содержит название навыка; валидатор `non_empty_name` запрещает пустые значения.
Поле `level` типа `Optional[Literal['junior', 'middle', 'senior']]` фиксирует уровень владения навыком; допускает `None`.
Поле `category` типа `Optional[str]` определяет категорию навыка (например, «бэкенд», «DevOps»); валидатор `strip_category` обрезает пробелы.

**3.2.3** Класс `EducationEntry` (тот же файл) описывает один период обучения кандидата.

Поле `institution` типа `str` хранит название учебного заведения и не может быть пустым.
Поле `degree` типа `Optional[str]` хранит степень или уровень образования.
Поле `field` типа `Optional[str]` хранит специальность.
Поле `graduation_year` типа `Optional[int]` хранит год окончания; валидатор `validate_graduation_year` проверяет допустимый диапазон от 1900 до текущего года плюс 10 лет.

**3.2.4** Класс `CandidateProfile` (тот же файл) является центральной моделью домена. Он наследуется от `BaseModel` и содержит полную структурированную информацию о кандидате.

Поле `id` типа `UUID` является уникальным идентификатором кандидата; генерируется автоматически через `uuid4()` при отсутствии явного значения.
Поле `full_name` типа `Optional[str]` хранит полное имя кандидата.
Поле `email` типа `Optional[str]` хранит адрес электронной почты; валидатор `validate_email` приводит значение к нижнему регистру и проверяет формат регулярным выражением `^[^@\s]+@[^@\s]+\.[^@\s]+$`.
Поле `phone` типа `Optional[str]` хранит номер телефона; валидатор `normalize_phone` удаляет пробелы, тире и скобки, после чего проверяет, что остаток содержит от 7 до 15 цифр с необязательным префиксом `+`.
Поле `location` типа `Optional[str]` хранит город или регион кандидата.
Поле `languages` типа `list[str]` хранит список языков; валидатор `strip_languages` отфильтровывает пустые элементы и обрезает пробелы.
Поле `experience` типа `list[ExperienceEntry]` хранит список записей об опыте работы.
Поле `skills` типа `list[SkillEntry]` хранит список навыков.
Поле `education` типа `list[EducationEntry]` хранит список записей об образовании.
Поле `salary_expectation` типа `Optional[int]` хранит ожидаемую зарплату; валидатор `validate_salary` запрещает отрицательные значения.
Поле `years_of_experience` типа `Optional[float]` хранит суммарный стаж в годах; валидатор `validate_years` запрещает отрицательные значения.
Поле `raw_text_hash` типа `str` хранит SHA-256 хеш исходного текста резюме в виде строки из 64 шестнадцатеричных символов; валидатор `validate_hash` проверяет соответствие шаблону `^[0-9a-f]{64}$` и допускает пустую строку.

**3.2.5** Классы `SuccessWrapper[T]` и `ErrorWrapper` (`libs/common/models/wrappers.py`) являются generic-обёртками ответов API.

Класс `SuccessWrapper` является обобщённым и наследуется от `BaseModel` и `Generic[T]`. Поле `success` имеет тип `Literal[True]` со значением по умолчанию `True`. Поле `data` типа `T` содержит полезную нагрузку ответа. Метод класса `of(data: T)` является фабричным методом, создающим экземпляр обёртки из данных.

Класс `ErrorWrapper` наследуется от `BaseModel`. Поле `success` имеет тип `Literal[False]` со значением по умолчанию `False`. Поле `error` типа `str` содержит краткое описание ошибки. Поле `detail` типа `Optional[str]` содержит расширенное описание. Метод класса `of(error, detail)` является фабричным методом для создания ошибочного ответа.

**3.2.6** Класс `ServiceException` (`libs/common/exceptions/base.py`) является базовым исключением для всех доменных ошибок. Поле `status_code` класса имеет значение `500` по умолчанию. Конструктор принимает `message` и опциональный `status_code`, переопределяющий значение класса. Метод `__repr__` возвращает строку с кодом статуса и сообщением.

От `ServiceException` наследуются специализированные классы: `BadRequestError` (400), `UnauthorizedError` (401), `ForbiddenError` (403), `NotFoundError` (404), `ConflictError` (409), `ValidationError` (422), `InternalError` (500), `ExternalServiceError` (502) и `ServiceUnavailableError` (503). Каждый класс задаёт собственный `status_code` и сообщение по умолчанию.

### 3.3 Классы модуля extraction-service

Модуль `extraction-service` (порт 8001) реализует блок агрегации, парсинга, извлечения сущностей и генерации эмбеддингов. Он является входным шлюзом системы и обеспечивает переход от неструктурированных файлов к векторным представлениям.

**3.3.1** Класс `AppConfig` (`core/config.py`) наследуется от `BaseSettings` и является singleton-объектом конфигурации сервиса. Экземпляр `settings` создаётся при импорте модуля и используется всеми остальными компонентами.

Поле `log_level` типа `str` определяет уровень логирования; по умолчанию — `'INFO'`.
Поле `s3_endpoint` типа `str` задаёт URL-адрес S3-совместимого хранилища.
Поле `s3_bucket` типа `str` задаёт имя бакета; по умолчанию — `'resumes'`.
Поле `s3_access_key` и `s3_secret_key` типа `str` содержат учётные данные для доступа к S3.
Поле `embedding_api_url` типа `str` задаёт URL embedding API.
Поле `embedding_model` типа `str` задаёт имя модели для генерации эмбеддингов; по умолчанию — `'text-embedding-3-small'`.
Поле `embedding_batch_size` типа `int` определяет размер батча при пакетной генерации; по умолчанию — `32`.
Поле `llm_api_url` и `llm_model` типа `str` задают URL и имя языковой модели для извлечения сущностей.
Поле `ocr_engine` типа `str` указывает основной OCR-движок; по умолчанию — `'tesseract'`.
Поле `max_file_size_mb` типа `int` задаёт максимальный размер загружаемого файла в мегабайтах; по умолчанию — `20`.
Поле `chunk_size` типа `int` определяет длину чанка в символах; по умолчанию — `512`.
Поле `chunk_overlap` типа `int` определяет перекрытие между чанками; по умолчанию — `64`.
Поле `ocr_llm_model` типа `str` задаёт имя vision-модели для OCR сложных изображений; пустая строка отключает LLM-режим.
Поле `ocr_confidence_threshold` типа `float` задаёт порог средней уверенности Tesseract, ниже которого подключается LLM; по умолчанию — `60.0`.
Поле `ocr_min_chars_per_kb` типа `float` задаёт минимальную плотность символов (символов на КБ), ниже которой подключается LLM; по умолчанию — `3.0`.
Поле `ocr_llm_timeout` типа `float` задаёт таймаут запроса к vision-API в секундах; по умолчанию — `60.0`.

**3.3.2** Класс `DocumentBlock` (`models/document.py`) наследуется от `BaseModel` и представляет один семантический блок внутри страницы документа.

Поле `block_type` типа `str` хранит тип блока согласно разметке Docling (например, `'text'`, `'title'`, `'table'`).
Поле `text` типа `str` содержит текстовое содержимое блока.
Поле `page` типа `int` указывает номер страницы, на которой расположен блок.
Поле `bbox` типа `Optional[list[float]]` содержит координаты ограничивающего прямоугольника блока в формате `[left, top, right, bottom]`; допускает `None`, если координаты недоступны.

**3.3.3** Класс `DocumentPage` (`models/document.py`) наследуется от `BaseModel` и представляет одну страницу документа.

Поле `page_number` типа `int` хранит номер страницы.
Поле `blocks` типа `list[DocumentBlock]` содержит список семантических блоков страницы.

**3.3.4** Класс `StructuredDocument` (`models/document.py`) наследуется от `BaseModel` и является сводным результатом обработки резюме, объединяющим все этапы пайплайна.

Поле `source_s3_key` типа `str` хранит ключ исходного файла в S3.
Поле `full_text` типа `str` содержит полный извлечённый текст резюме.
Поле `pages` типа `list[DocumentPage]` хранит постраничную структуру документа.
Поле `candidate_profile` типа `Optional[CandidateProfile]` содержит структурированный профиль кандидата, извлечённый LLM; допускает `None` при ошибке извлечения.
Поле `chunks` типа `list[str]` содержит список текстовых фрагментов, полученных нарезкой `full_text`.
Поле `embeddings` типа `list[list[float]]` содержит векторные представления, соответствующие каждому чанку.
Поле `metadata` типа `dict` хранит вспомогательные метаданные, такие как имя парсера и исходное имя файла.

**3.3.5** Модуль `s3_service` (`services/s3_service.py`) реализует слой взаимодействия с S3-совместимым объектным хранилищем через библиотеку `aioboto3`. Модуль использует единственную разделяемую сессию `_session = aioboto3.Session()` и внутреннюю функцию `_client()`, создающую контекстный клиент с параметрами из `settings`.

Функция `async upload_file(file_bytes: bytes, filename: str) → str` генерирует уникальный S3-ключ вида `<uuid>.<ext>`, загружает байты файла в бакет через `put_object` и возвращает ключ. После успешной загрузки фиксирует событие `s3.uploaded` с ключом и размером файла.

Функция `async download_file(s3_key: str) → bytes` скачивает объект из бакета по ключу через `get_object`, читает тело ответа и возвращает байты. Фиксирует событие `s3.downloaded` с ключом и размером.

**3.3.6** Класс `_OcrMetrics` (`services/parsers/image_parser.py`) является именованным кортежем (`NamedTuple`) и содержит результаты первичного OCR-прохода Tesseract.

Поле `text` типа `str` содержит распознанный текст.
Поле `mean_conf` типа `float` содержит среднюю уверенность Tesseract в диапазоне от 0 до 100.
Поле `char_density` типа `float` содержит плотность символов — отношение количества символов к размеру файла в килобайтах.

Функция `_ocr_sync_with_metrics(file_bytes: bytes) → _OcrMetrics` выполняется синхронно в пуле потоков. Она открывает изображение через Pillow, преобразует его в режим RGB при необходимости, вызывает `pytesseract.image_to_data` для получения уверенностей по каждому слову, затем `pytesseract.image_to_string` для получения полного текста. Возвращает `_OcrMetrics` с текстом, средней уверенностью и плотностью символов.

Функция `_pixel_entropy(file_bytes: bytes) → float` вычисляет информационную энтропию Шеннона по гистограмме пикселей в оттенках серого. Значение ниже 2.0 указывает на пустое или монохромное изображение, значение от 2 до 5 — на ограниченный контраст, от 5 до 8 — на полноценное содержимое.

Функция `_assess_quality(metrics, entropy, conf_threshold, density_threshold) → tuple[bool, str]` принимает метрики OCR и пороговые значения из конфигурации и возвращает пару: флаг необходимости LLM и читаемое описание причины. Возвращает `True` если энтропия ниже 2.0, средняя уверенность ниже порога или плотность символов ниже порога.

Функция `async _llm_vision_ocr(file_bytes: bytes, filename: str) → str` кодирует изображение в base64, формирует data-URL с MIME-типом и отправляет запрос к OpenAI-совместимому chat/completions API с системным промптом для распознавания резюме. Возвращает текст из первого варианта ответа.

Публичная функция `async parse_image(file_bytes: bytes, filename: str) → str` является входной точкой OCR-пайплайна. Выполняет валидацию расширения файла, запускает `_ocr_sync_with_metrics` и `_pixel_entropy` в пуле потоков, оценивает качество через `_assess_quality`. Если качество низкое и `ocr_llm_model` задан — вызывает `_llm_vision_ocr`. Возвращает более длинный из двух результатов (Tesseract или LLM).

**3.3.7** Модуль `docling_parser` (`services/parsers/docling_parser.py`) реализует разбор PDF и DOCX файлов с помощью библиотеки Docling. Использует ленивый singleton-конвертер `_converter`, который инициализируется при первом обращении через функцию `_get_converter()`.

Класс `ParseError` наследуется от `Exception` и выбрасывается при неудаче парсинга.

Функция `_build_converter() → DocumentConverter` создаёт конвертер Docling с включённым OCR для PDF через `PdfPipelineOptions(do_ocr=True)`.

Функция `_convert_sync(tmp_path: str) → DoclingDocument` вызывает `_get_converter().convert(tmp_path)` синхронно — она передаётся в `run_in_executor` для выполнения в пуле потоков.

Функция `_docling_to_structured(doc: DoclingDocument, s3_key: str) → StructuredDocument` обходит все элементы документа через `doc.iterate_items()`, извлекает текст, номер страницы и координаты bounding box, формирует объекты `DocumentBlock` и группирует их по страницам в `DocumentPage`. Возвращает `StructuredDocument` с полным текстом, постраничной структурой и метаданными парсера.

Публичная функция `async parse(file_bytes: bytes, filename: str, s3_key: str) → StructuredDocument` записывает байты во временный файл, запускает `_convert_sync` в пуле потоков и гарантирует удаление временного файла через блок `finally`. При ошибке парсинга выбрасывает `ParseError`.

**3.3.8** Модуль `entity_extractor` (`services/parsers/entity_extractor.py`) вызывает LLM для извлечения структурированного профиля кандидата из текста резюме.

Вспомогательные внутренние классы `_Message`, `_ResponseFormat`, `_ChatRequest` являются Pydantic-моделями для формирования тела запроса к chat/completions API: `_ChatRequest` содержит поля `model`, `messages: list[_Message]`, `temperature: float` со значением `0.0` и `response_format: _ResponseFormat` с типом `json_object` для получения гарантированного JSON.

Публичная функция `async extract(full_text: str) → CandidateProfile` форматирует промпт `ENTITY_EXTRACTION_PROMPT` с текстом резюме, отправляет POST-запрос на `llm_api_url/chat/completions`, парсит JSON из ответа и валидирует его через `CandidateProfile.model_validate`. При ошибке декодирования JSON выбрасывает `ValueError`. Успешное извлечение логируется с именем кандидата.

**3.3.9** Модуль `embedding_service` (`services/embedding_service.py`) реализует пакетную генерацию векторных эмбеддингов.

Внутренние классы `_EmbedRequest` (`model: str`, `input: list[str]`), `_EmbeddingObject` (`embedding: list[float]`, `index: int`) и `_EmbedResponse` (`data: list[_EmbeddingObject]`) формируют типизированный интерфейс к embedding API.

Функция `async _embed_single_batch(client, texts) → list[list[float]]` отправляет один батч текстов на `/embeddings`, десериализует ответ через `_EmbedResponse`, сортирует результаты по полю `index` и возвращает упорядоченный список векторов.

Публичная функция `async embed_batch(texts: list[str]) → list[list[float]]` нарезает входной список на батчи размером `embedding_batch_size`, последовательно вызывает `_embed_single_batch` для каждого батча в рамках одного `httpx.AsyncClient` и возвращает объединённый список векторов. При пустом входе возвращает пустой список.

**3.3.10** Модуль `extraction_service` (`services/extraction_service.py`) реализует главный пайплайн обработки резюме.

Функция `_chunk_text(text: str, chunk_size: int, overlap: int) → list[str]` разбивает текст на фрагменты фиксированного размера с перекрытием: каждый следующий чанк начинается со смещением `chunk_size - overlap` символов.

Публичная функция `async process_resume(file_bytes: bytes, filename: str, s3_key: str) → StructuredDocument` является оркестратором пайплайна. Определяет формат файла по расширению: для изображений вызывает `parse_image`, для PDF/DOCX — `docling_parser.parse` с откатом на `raw_parsers` при ошибке. Затем последовательно вызывает `extract_entities` для построения профиля, `_chunk_text` для нарезки текста и `embed_batch` для получения эмбеддингов. Каждый шаг обёрнут в `try/except` — при ошибке извлечения или эмбеддинга соответствующее поле устанавливается в `None` или пустой список. Возвращает `StructuredDocument` с заполненными полями и метаданными.

**3.3.11** Модуль `text_processing_service` (`services/text_processing_service.py`) является высокоуровневым фасадом, связывающим скачивание из S3 и запуск пайплайна.

Функция `async process_document_by_key(s3_key: str, filename: str) → StructuredDocument` скачивает файл из S3 по ключу через `s3_service.download_file`, затем передаёт байты в `process_resume`. Логирует размер скачанного файла, количество чанков и эмбеддингов, наличие профиля.

Функция `async reprocess_document(s3_key: str, filename: str) → StructuredDocument` является именованным алиасом для `process_document_by_key`, предназначенным для читаемости кода в точках повторной обработки.

### 3.4 Классы модуля storage-service

Модуль `storage-service` (порт 8002) реализует блок векторного хранилища и обеспечивает сохранение, поиск и удаление векторных точек в базе данных Qdrant.

**3.4.1** Класс `CandidateVectorPayload` (`models/vector.py`) наследуется от `BaseModel` и представляет метаданные, сохраняемые вместе с вектором в Qdrant.

Поле `candidate_id` типа `UUID` является идентификатором кандидата.
Поле `full_name` типа `str | None` хранит полное имя.
Поле `skills` типа `list[str]` хранит список навыков в нижнем регистре для фильтрации.
Поле `location` типа `str | None` хранит местоположение.
Поле `years_of_experience` типа `float | None` хранит суммарный стаж в годах.
Поле `salary_expectation` типа `int | None` хранит ожидаемую зарплату.
Поле `languages` типа `list[str]` хранит список языков.
Поле `source_s3_key` типа `str` хранит ключ исходного файла в S3.
Поле `chunk_index` типа `int` хранит индекс чанка, которому соответствует данная точка.
Поле `chunk_text` типа `str` хранит текстовое содержимое чанка.
Поле `indexed_at` типа `datetime` хранит временную метку индексации.
Поле `version` типа `int` хранит версию записи; по умолчанию — `1`.

**3.4.2** Класс `EmbeddingPoint` (`models/vector.py`) наследуется от `BaseModel` и представляет полную точку Qdrant, готовую к загрузке.

Поле `id` типа `UUID` является уникальным идентификатором точки.
Поле `vector` типа `list[float]` содержит векторное представление чанка.
Поле `payload` типа `CandidateVectorPayload` содержит метаданные точки.

**3.4.3** Класс `QdrantService` (`services/qdrant_service.py`) является оберткой над `AsyncQdrantClient` из библиотеки `qdrant-client`.

Конструктор инициализирует поле `_client` экземпляром `AsyncQdrantClient(url=settings.qdrant_url)` и поле `_collection` именем коллекции из конфигурации.

Метод `async upsert(point: EmbeddingPoint) → str` преобразует объект `EmbeddingPoint` в структуру `PointStruct`, вызывает `_client.upsert` с единственной точкой и возвращает строковое представление идентификатора точки. Фиксирует событие `qdrant.upsert` в журнале.

Метод `async batch_upsert(points: list[EmbeddingPoint]) → list[str]` преобразует весь список точек в `list[PointStruct]` и вызывает `_client.upsert` одним вызовом для эффективной пакетной загрузки. Возвращает список идентификаторов всех загруженных точек. Фиксирует событие `qdrant.batch_upsert` с количеством точек.

Метод `async delete(point_id: str) → None` удаляет точку по идентификатору через `_client.delete` с `PointIdsList`. Фиксирует событие `qdrant.delete`.

Метод `async get_by_id(point_id: str) → EmbeddingPoint | None` извлекает точку по идентификатору через `_client.retrieve` с флагами `with_vectors=True` и `with_payload=True`. При отсутствии результата возвращает `None`, при успехе восстанавливает объект `EmbeddingPoint` из записи.

**3.4.4** Класс `CollectionService` (`services/collection_service.py`) управляет жизненным циклом Qdrant-коллекции.

Конструктор инициализирует поля `_client` и `_collection` аналогично `QdrantService`.

Метод `async ensure_collection() → None` проверяет существование коллекции через `_client.get_collections()`. Если коллекция отсутствует — создаёт её с параметрами: метрика расстояния `COSINE`, размерность вектора из `settings.vector_size`, конфигурация HNSW с `m=16`, `ef_construct=100`, `full_scan_threshold=10000`. После создания коллекции создаёт payload-индексы: тип `KEYWORD` для полей `skills`, `location`, `languages`; тип `FLOAT` для `years_of_experience`; тип `INTEGER` для `salary_expectation`. При существующей коллекции логирует `collection.already_exists` и возвращает управление.

Метод `async list_collections() → list[dict]` возвращает список словарей с полем `name` для каждой коллекции в Qdrant.

### 3.5 Классы модуля search-service

Модуль `search-service` (порт 8003) реализует блок гибридного поиска, объединяющий векторный поиск, BM25-ранжирование, нейронный ре-ранкинг и бизнес-правила для итоговой оценки кандидатов.

**3.5.1** Класс `MetadataFilters` (`models/request.py`) наследуется от `BaseModel` и содержит параметры фильтрации поиска.

Поле `skills_required` типа `list[str]` задаёт обязательные навыки; кандидат должен владеть всеми указанными навыками (AND-семантика).
Поле `skills_preferred` типа `list[str]` задаёт желательные навыки; достаточно наличия хотя бы одного (OR-семантика).
Поле `location` типа `str | None` задаёт точное совпадение по местоположению.
Поле `min_experience_years` типа `float | None` задаёт минимальный стаж работы.
Поле `max_salary` типа `int | None` задаёт верхний порог зарплатных ожиданий кандидата.
Поле `languages` типа `list[str]` задаёт желаемые языки; OR-семантика.

**3.5.2** Класс `SearchRequest` (`models/request.py`) наследуется от `BaseModel` и является входным объектом поискового запроса.

Поле `query` типа `str` содержит текст поискового запроса на естественном языке.
Поле `filters` типа `MetadataFilters` содержит параметры фильтрации; по умолчанию — пустой объект без ограничений.
Поле `top_k` типа `int` определяет количество возвращаемых кандидатов; по умолчанию — `10`.
Поле `use_rerank` типа `bool` управляет включением нейронного ре-ранкинга; по умолчанию — `True`.
Поле `correlation_id` типа `str` передаёт идентификатор запроса для трассировки.

**3.5.3** Класс `RankedCandidate` (`models/response.py`) наследуется от `BaseModel` и представляет одного кандидата в ответе на поисковый запрос.

Поле `candidate_id` типа `UUID` является идентификатором кандидата.
Поле `full_name` типа `str | None` содержит имя кандидата.
Поле `vector_score` типа `float` содержит нормализованную оценку семантической близости.
Поле `rerank_score` типа `float` содержит нормализованную оценку CrossEncoder.
Поле `final_score` типа `float` содержит итоговую взвешенную оценку с учётом бизнес-бонусов.
Поле `matched_skills` типа `list[str]` содержит пересечение навыков кандидата с требуемыми.
Поле `explanation` типа `str` содержит человекочитаемое объяснение оценки в виде строки с компонентами.
Поле `chunk_text` типа `str` содержит текстовое резюме ключевых атрибутов кандидата.

**3.5.4** Класс `SearchResponse` (`models/response.py`) содержит поле `total` типа `int` с общим числом найденных кандидатов и поле `candidates` типа `list[RankedCandidate]` с отсортированным списком результатов.

**3.5.5** Класс `_BM25` (`services/fulltext_filter.py`) реализует алгоритм ранжирования BM25 без внешних зависимостей. Константы `_K1 = 1.5` и `_B = 0.75` соответствуют стандартным значениям из оригинальной статьи Окапи BM25.

Конструктор принимает `corpus: list[list[str]]` — список токенизированных документов. Вычисляет `n` (число документов), `avgdl` (среднюю длину документа), `tf` (частоту каждого терма в каждом документе) и `df` (частоту документов для каждого терма).

Метод `score(query_tokens: list[str], doc_index: int) → float` вычисляет BM25-оценку для одного документа: для каждого терма запроса вычисляет IDF по формуле `log((n - df + 0.5) / (df + 0.5) + 1)` и TF-компоненту с нормализацией по длине документа.

Функция `_tokenize(text: str) → list[str]` приводит текст к нижнему регистру и разбивает по нелитерным символам.

Публичная функция `bm25_rerank(query: str, payloads: list[dict]) → list[int]` принимает список payload-словарей из Qdrant, токенизирует их поле `chunk_text`, строит объект `_BM25` и возвращает список индексов, отсортированных по убыванию оценки.

Публичная функция `bm25_scores(query: str, payloads: list[dict]) → list[float]` возвращает BM25-оценки в исходном порядке документов.

**3.5.6** Модуль `qdrant_search` (`services/qdrant_search.py`) реализует векторный поиск с построением Qdrant-фильтра.

Глобальная переменная `_client` хранит lazy-инициализированный экземпляр `AsyncQdrantClient`; функция `get_client()` реализует паттерн singleton.

Функция `_build_filter(filters: MetadataFilters) → Filter | None` формирует объект фильтра Qdrant: для `skills_required` создаёт список `FieldCondition` с `MatchValue` (AND-семантика через поле `must`); для `skills_preferred` и `languages` — `FieldCondition` с `MatchAny` (OR-семантика через поле `should`); для `location` — точное совпадение; для `min_experience_years` — `Range(gte=...)`, для `max_salary` — `Range(lte=...)`. Возвращает `None` если фильтры не заданы.

Функция `async vector_search(vector, filters, top_n) → list[ScoredPoint]` вызывает `_client.search` с вектором запроса, построенным фильтром, лимитом `top_n`, флагами `with_payload=True` и `with_vectors=False`. Логирует факт поиска и количество возвращённых результатов.

**3.5.7** Модуль `reranker` (`services/reranker.py`) реализует нейронный ре-ранкинг на основе CrossEncoder из библиотеки `sentence-transformers`.

Функция `_get_model() → CrossEncoder` загружает модель из `settings.reranker_model` с кешированием через декоратор `@lru_cache(maxsize=1)`, обеспечивая однократную инициализацию.

Функция `_candidate_to_text(candidate: dict) → str` синтезирует текстовое представление кандидата для передачи в CrossEncoder: объединяет имя, местоположение, стаж, навыки, языки и опыт работы через разделитель `|`.

Функция `_predict(query, candidates) → list[float]` формирует пары `[query, candidate_text]` и вызывает `model.predict` синхронно.

Публичная функция `async rerank(query: str, candidates: list[dict]) → list[RerankScore]` запускает `_predict` в отдельном потоке через `asyncio.to_thread`, чтобы не блокировать event loop. Возвращает список объектов `RerankScore` с полями `index` и `score` для каждого кандидата.

**3.5.8** Модуль `search_service` (`services/search_service.py`) реализует главный поисковый пайплайн.

Функция `async _embed_query(query: str) → list[float]` отправляет текст запроса на embedding API и возвращает первый вектор из ответа.

Функция `_minmax(values: list[float]) → list[float]` нормализует список оценок в диапазон `[0, 1]` по формуле min-max. При равных значениях возвращает список из единиц.

Функция `_business_rules_score(payload: dict, filters: MetadataFilters) → float` вычисляет бонус за совпадение предпочтительных навыков (по `_PREFERRED_SKILL_BONUS = 0.02` за каждый) и языков (`_LANGUAGE_BONUS = 0.03`). Итоговый бонус ограничен константой `_BUSINESS_BONUS_CAP = 0.10`.

Функция `_matched_skills(payload, filters) → list[str]` возвращает отсортированное пересечение навыков кандидата с объединением `skills_required` и `skills_preferred`.

Функция `_explanation(payload, filters, vector_score, rerank_score, business_bonus) → str` формирует строку объяснения вида `"vector=0.812; rerank=0.743; bonus=0.040; skills: python, fastapi"`.

Публичная функция `async run_search(req: SearchRequest) → SearchResponse` выполняет шесть последовательных шагов: 1) получает вектор запроса через `_embed_query`; 2) выполняет векторный поиск в Qdrant с `prefetch_n = top_k * prefetch_multiplier` результатами; 3) при `use_rerank=True` вызывает `reranker.rerank`, иначе заполняет нулевыми оценками; 4) нормализует векторные и ре-ранк оценки через `_minmax`; 5) вычисляет `final_score = vector_weight * v_score + rerank_weight * r_score + bonus` для каждого кандидата; 6) сортирует по убыванию `final_score`, обрезает до `top_k` и возвращает `SearchResponse`.

### 3.6 Классы модуля orchestrator-service

Модуль `orchestrator-service` (порт 8004) является центральным блоком оркестрации агентов на основе LangGraph. Он координирует работу всех остальных сервисов через агентов с инструментами и управляет состоянием задач.

**3.6.1** Класс `OrchestratorState` (`models/state.py`) наследуется от `BaseModel` и является схемой состояния графов LangGraph.

Поле `user_query` типа `str` хранит исходный запрос пользователя.
Поле `query_type` типа `Literal['upload', 'search', 'batch', 'get_profile']` определяет тип задачи; по умолчанию — `'search'`.
Поле `correlation_id` типа `str` является идентификатором запроса для трассировки.
Поле `auth_token` типа `str` хранит JWT-токен для авторизации вызовов к другим сервисам.
Поле `extracted_document` типа `Optional[dict]` хранит результат извлечения данных из резюме.
Поле `candidate_profile` типа `Optional[dict]` хранит профиль кандидата после сохранения.
Поле `search_results` типа `Optional[list[dict]]` хранит список результатов поиска.
Поле `reflection` типа `Optional[dict]` хранит оценку качества результатов от LLM.
Поле `retry_count` типа `int` хранит текущее число попыток; по умолчанию — `0`.
Поле `revised_query` типа `Optional[str]` хранит скорректированный запрос, предложенный reflection-агентом.
Поле `max_retries` типа `int` определяет максимально допустимое число повторов; по умолчанию — `2`.
Поле `messages` типа `list` хранит накопленные сообщения агентов LangChain.
Поле `error` типа `Optional[str]` хранит сообщение об ошибке при её возникновении.
Поле `metadata` типа `dict` хранит вспомогательные данные (имя файла, байты, point_id).

**3.6.2** Класс `OrchestratorService` (`services/orchestrator_service.py`) является singleton-объектом, управляющим жизненным циклом графов LangGraph.

Конструктор инициализирует три приватных поля: `_search_graph`, `_upload_graph` и `_checkpointer` — все со значением `None`.

Метод `async initialize() → None` создаёт checkpointer через `make_checkpointer()`, компилирует граф поиска через `build_search_graph(checkpointer=...)` и граф загрузки через `build_upload_graph(checkpointer=...)`. Логирует событие инициализации.

Метод `async shutdown() → None` закрывает все HTTP-клиенты сервисов (`extraction_client`, `storage_client`, `search_client`, `auth_client`) параллельно через `asyncio.gather` с флагом `return_exceptions=True`.

Метод `async run(state: OrchestratorState) → OrchestratorState` выбирает граф через внутренний метод `_select_graph` по полю `query_type` (типы `'search'` и `'get_profile'` маршрутизируются в `_search_graph`, типы `'upload'` и `'batch'` — в `_upload_graph`). Запускает граф через `ainvoke` с таймаутом `settings.agent_timeout_sec`. При превышении таймаута выбрасывает `AgentTimeoutError`, при других исключениях — `OrchestratorError`.

Метод `async get_status(correlation_id: str) → dict | None` запрашивает последний снимок состояния из `_search_graph.aget_state`, при отсутствии — из `_upload_graph.aget_state`. Возвращает словарь с полями `correlation_id`, `values`, `next` (список следующих узлов) и `done` (флаг завершения).

**3.6.3** Функция `build_search_graph(checkpointer=None)` (`graphs/candidate_search_graph.py`) строит и компилирует граф поиска LangGraph.

Граф содержит пять узлов: `router` выполняет классификацию запроса через `route_query`; `search` вызывает `search_agent` и извлекает результаты из потока сообщений через `_parse_search_results`; `reflect` вызывает `reflection_node` для оценки качества; `format` добавляет итоговые метаданные; `error` фиксирует ошибку через `error_handler`. Рёбра: `router → search`; условный переход после `search` — в `reflect` или `error`; условный переход после `reflect` через `should_retry` — в `search` (повтор) или `format` (завершение). Узлы `format` и `error` ведут в `END`.

**3.6.4** Функция `build_upload_graph(checkpointer=None)` (`graphs/resume_upload_graph.py`) строит граф загрузки резюме.

Граф содержит четыре узла: `router` классифицирует запрос; `extract` запускает `extraction_agent`, передавая байты файла через `configurable`, и извлекает результат через `_parse_extracted_document`; `store` запускает `storage_agent` с профилем и эмбеддингами, извлекает `point_id` через `_parse_point_id`; `error` обрабатывает ошибки. Условный переход после `extract`: в `store` при наличии результата, в `error` при ошибке или пустом документе. Условный переход после `store`: в `END` при успехе, в `error` при ошибке.

**3.6.5** Модуль `common_nodes` (`graphs/common_nodes.py`) содержит разделяемые узлы обоих графов.

Функция `async reflection_node(state: OrchestratorState) → OrchestratorState` отправляет топ-3 результата поиска в LLM с промптом `REFLECTION_PROMPT` и парсит JSON-ответ с оценкой качества (`quality`) и опционально скорректированным запросом (`revised_query`). При ошибке парсинга устанавливает `reflection = {"quality": "ok"}`.

Функция `should_retry(state: OrchestratorState) → Literal["retry", "done"]` является условным ребром: возвращает `"retry"` если `reflection.quality == "low"` и `retry_count < max_retries`, иначе — `"done"`.

Функция `async error_handler(state: OrchestratorState) → OrchestratorState` фиксирует ошибку в логе, устанавливает `retry_count = max_retries` для предотвращения дальнейших повторов и `reflection = {"quality": "ok"}` для корректного выхода из условного ребра.

**3.6.6** Функция `async route_query(user_query: str)` (`services/agents/router_agent.py`) классифицирует запрос пользователя через LLM со structured output. Использует класс `RouterOutput` (`BaseModel` с полем `query_type: Literal["upload", "search", "batch", "get_profile"]`) и вызывает `llm.with_structured_output(RouterOutput).ainvoke(prompt)`. Возвращает значение `query_type`.

**3.6.7** Объект `search_agent` (`services/agents/search_agent.py`) создаётся функцией `build_search_agent()` через `create_react_agent` с моделью LLM и тремя инструментами: `hybrid_search`, `web_search`, `get_job_description`. Системный промпт указывает агенту использовать `hybrid_search` несколько раз с разными формулировками при необходимости.

**3.6.8** Объект `extraction_agent` (`services/agents/extraction_agent.py`) создаётся через `create_react_agent` с инструментом `extract_resume`.

**3.6.9** Объект `storage_agent` (`services/agents/storage_agent.py`) создаётся через `create_react_agent` с инструментом `store_candidate_profile`.

**3.6.10** Инструмент `extract_resume` (`services/tools/extraction_tools.py`) декорирован `@tool` и принимает `file_bytes: bytes` и `correlation_id: str`. Вызывает `extraction_client.process_resume` и возвращает словарь с результатом обработки.

**3.6.11** Инструмент `hybrid_search` (`services/tools/search_tools.py`) декорирован `@tool` и принимает `query: str`, `filters: dict` и `top_k: int`. Вызывает `search_client.hybrid_search` и возвращает список словарей с результатами.

Инструмент `web_search` декорирован `@tool` и принимает `query: str`. Выполняет POST-запрос на DuckDuckGo HTML-поиск (`https://html.duckduckgo.com/html/`), извлекает заголовки и сниппеты регулярными выражениями, очищает HTML-теги и возвращает до 5 результатов в виде нумерованного текста.

**3.6.12** Инструмент `store_candidate_profile` (`services/tools/storage_tools.py`) декорирован `@tool` и принимает `profile: dict` и `embeddings: list[float]`. Формирует структуру точки с идентификатором из профиля или новым UUID, вызывает `storage_client.upsert` и возвращает строку с `point_id`.

**3.6.13** Инструмент `get_job_description` (`services/tools/jd_tools.py`) декорирован `@tool` и принимает `job_title: str`. Возвращает описание вакансии из словаря `_JD_TEMPLATES` по ключу (нормализованное название должности) или шаблонное описание по умолчанию `_DEFAULT_JD`.

**3.6.14** Модуль `webhooks` (`routers/webhooks.py`) реализует in-memory реестр webhook-обратных вызовов.

Глобальный словарь `_registry: dict[str, str]` хранит соответствие `correlation_id → callback_url`.

Класс `WebhookRegisterRequest` содержит поля `correlation_id: str` и `callback_url: HttpUrl`. Класс `WebhookRegisterResponse` содержит поля `correlation_id`, `callback_url` и `registered: bool`. Класс `WebhookPayload` содержит поля `correlation_id`, `status` (строка `"success"` или `"error"`), `query_type`, `result` и `error`.

Маршрут `POST /register` сохраняет `callback_url` в `_registry` и возвращает `WebhookRegisterResponse`.
Маршрут `DELETE /register/{correlation_id}` удаляет запись или возвращает 404.
Маршрут `GET /register/{correlation_id}` возвращает зарегистрированный URL или 404.

Функция `async _deliver(url, payload)` отправляет POST-запрос с телом `WebhookPayload` на callback_url с таймаутом 10 секунд, поглощает все исключения и логирует результат.

Функция `notify(correlation_id, *, job_status, query_type, result, error)` является публичным API для вызова из orchestrator_service. Проверяет наличие URL в `_registry`, формирует `WebhookPayload` и запускает `_deliver` как фоновую задачу через `asyncio.create_task` (fire-and-forget).

### 3.7 Классы модуля frontend

Модуль `frontend` (порт 3000) состоит из двух частей: backend-for-frontend (NestJS) и React SPA с Module Federation. BFF проксирует запросы к orchestrator-service и storage-service, обеспечивает WebSocket-шлюз для real-time обновлений.

**3.7.1** Класс `OrchestratorProxy` (`backend-for-frontend/src/services/orchestrator.proxy.ts`) является Injectable-сервисом NestJS.

Поле `client` типа `AxiosInstance` является HTTP-клиентом, настроенным с `baseURL` из переменной окружения `ORCHESTRATOR_URL` (по умолчанию `http://localhost:8004`) и таймаутом `30000` мс.

Метод `async proxyUpload(fileBuffer, originalName, mimetype, jwt)` формирует объект `FormData` с файлом и его MIME-типом, отправляет POST на `/orchestrate/upload` с заголовком `Authorization: Bearer <jwt>` и возвращает объект `{ correlation_id: string }`. При ошибке выбрасывает `InternalServerErrorException`.

Метод `async proxySearch(body, jwt)` отправляет POST на `/orchestrate/search` с телом запроса в JSON и заголовком авторизации. Возвращает ответ оркестратора без трансформации.

**3.7.2** Класс `StorageProxy` (`backend-for-frontend/src/services/storage.proxy.ts`) является Injectable-сервисом NestJS.

Интерфейсы `CandidateVectorPayload` и `EmbeddingPoint` дублируют Python-модели хранилища для типизации TypeScript-кода.

Поле `client` типа `AxiosInstance` настроено с `baseURL` из переменной окружения `STORAGE_URL` (по умолчанию `http://localhost:8002`) и таймаутом `10000` мс.

Метод `async getEmbeddingPoint(pointId, jwt)` выполняет GET-запрос на `/storage/{pointId}`, извлекает поле `data` из обёртки `SuccessWrapper` и возвращает объект `EmbeddingPoint`. При статусе 404 выбрасывает `NotFoundException`, при других ошибках — `InternalServerErrorException`.

Метод `async getCandidate(pointId, jwt)` является алиасом для `getEmbeddingPoint`.

**3.7.3** Класс `UploadController` (`backend-for-frontend/src/controllers/upload.controller.ts`) обслуживает маршрут `POST /api/upload`. Использует декоратор `@UseInterceptors(FileInterceptor('file'))` для получения загружаемого файла. Извлекает JWT из куки `access_token` или `jwt` через приватный метод `extractJwt`. Передаёт буфер файла, оригинальное имя и MIME-тип в `proxy.proxyUpload`. При отсутствии файла выбрасывает `BadRequestException`, при отсутствии токена — `UnauthorizedException`.

**3.7.4** Класс `CandidatesController` (`backend-for-frontend/src/controllers/candidates.controller.ts`) обслуживает маршрут `GET /api/candidates/:id`. Извлекает JWT из куки и передаёт идентификатор в `storage.getCandidate`.

**3.7.5** Класс `WsGateway` (`backend-for-frontend/src/gateways/ws.gateway.ts`) реализует WebSocket-шлюз на базе Socket.io. Настроен на путь `/ws` с CORS `origin: '*'`.

Поле `server` типа `Server` является экземпляром Socket.io-сервера, внедряемым декоратором `@WebSocketServer()`.

Метод `handleConnection(client: Socket)` вызывается при подключении нового клиента; подписка выполняется отдельным сообщением.

Метод `handleDisconnect(client: Socket)` вызывается при отключении; очистка комнат выполняется Socket.io автоматически.

Метод `handleSubscribe(data: SubscribePayload, client: Socket)` обрабатывает сообщение `'subscribe'` с полем `correlation_id`, добавляет клиента в комнату с именем `correlation_id` через `client.join(correlation_id)` и возвращает `{ status: 'subscribed', correlation_id }`.

Метод `broadcast(payload: UpdatePayload)` принимает объект с полями `correlation_id`, `status` и `data`, и рассылает событие `'update'` всем клиентам в комнате с данным `correlation_id` через `server.to(correlation_id).emit('update', ...)`.

### 3.8 Классы модуля auth-service

Модуль `auth-service` (порт 3001, NestJS) реализует блок безопасности на основе Keycloak и обеспечивает проверку JWT-токенов, ролевой доступ и аудит событий.

**3.8.1** Класс `KeycloakStrategy` (`src/auth/strategies/keycloak.strategy.ts`) наследуется от `PassportStrategy(Strategy, 'jwt')` и реализует проверку JWT через JWKS-эндпоинт Keycloak.

Интерфейс `JwtPayload` описывает структуру payload токена: поле `sub` типа `string` — идентификатор пользователя; `preferred_username: string` — имя пользователя; `email?: string`; `realm_access?: { roles: string[] }` — роли из Keycloak-реалма; `iat: number` и `exp: number` — временные метки.

Интерфейс `AuthUser` описывает нормализованный объект пользователя: поля `sub`, `username`, `email` и `roles: string[]`.

Конструктор вызывает родительский конструктор с конфигурацией: `jwtFromRequest` — извлечение токена из заголовка `Authorization: Bearer`; `secretOrKeyProvider` — провайдер JWKS через библиотеку `jwks-rsa` с кешированием и ограничением 10 запросов в минуту, URI формируется из переменных окружения `KEYCLOAK_URL` и `KEYCLOAK_REALM`; алгоритм — `RS256`; `audience` — идентификатор клиента Keycloak.

Метод `validate(payload: JwtPayload): AuthUser` преобразует payload токена в объект `AuthUser`, извлекая роли из `realm_access?.roles ?? []`.

**3.8.2** Класс `AuditService` (`src/audit/audit.service.ts`) является Injectable-сервисом NestJS, реализующим сохранение событий аудита в базу данных через TypeORM.

Поле `logger` является экземпляром встроенного `Logger` NestJS, привязанным к классу `AuditService`.

Поле `repo` типа `Repository<AuditLog>` внедряется через декоратор `@InjectRepository(AuditLog)` и предоставляет TypeORM-репозиторий для работы с таблицей журнала аудита.

Метод `async logEvent(eventType, userId, correlationId, metadata)` создаёт запись `AuditLog` через `repo.create` и сохраняет её через `repo.save`. Параметр `eventType: string` задаёт тип события (например, `'login'`, `'upload'`); `userId: string | null` — идентификатор пользователя; `correlationId: string | null` — идентификатор запроса; `metadata?: Record<string, unknown>` — произвольные дополнительные данные. При ошибке сохранения логирует сообщение через `logger.error`, не пробрасывая исключение.

Таким образом, функциональная структура системы полностью соответствует требованиям дипломного проекта. Каждый сервис и класс имеет чётко определённую ответственность, что обеспечивает модульность, масштабируемость и высокую точность семантического подбора персонала.
