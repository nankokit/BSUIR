@echo off
chcp 65001 >nul
echo.
echo ╔═══════════════════════════════════════════════════════════╗
echo ║               ПРОСМОТР ВСЕХ BERKELEYDB БАЗ               ║
echo ╚═══════════════════════════════════════════════════════════╝
echo.

:: ВКЛЮЧАЕМ ЗАДЕРЖКУ ПЕРЕМЕННЫХ
setlocal enabledelayedexpansion

:: Автоопределение папки проекта
set "PROJECT_DIR=%~dp0"
set "DB_DIR=%PROJECT_DIR%berkeley_dbs"
set "IMAGE=lab2-converter:latest"

:: Проверка папки
if not exist "%DB_DIR%" (
    echo [ОШИБКА] Папка %DB_DIR% не найдена!
    echo Создайте её и запустите: docker compose up --build
    pause
    exit /b 1
)

:: Список таблиц
set "TABLES=booking equipment equipment_booking rehearsal_points rooms service service_booking staff users"

echo [ИНФО] Образ: %IMAGE%
echo [ИНФО] Папка: %DB_DIR%
echo.

:: Перебор таблиц
for %%t in (%TABLES%) do (
    set "DB_FILE=%DB_DIR%\%%t.db"
    if exist "!DB_FILE!" (
        echo.
        echo ════════════════════════════════════════════════════════════
        echo                %%t.db
        echo ════════════════════════════════════════════════════════════
        docker run --rm -v "%DB_DIR%:/output" %IMAGE% db_dump -p /output/%%t.db
        if errorlevel 1 (
            echo [ОШИБКА] Не удалось прочитать %%t.db
        )
    ) else (
        echo.
        echo [ПРЕДУПРЕЖДЕНИЕ] Файл %%t.db НЕ НАЙДЕН
    )
   echo.
)

echo ════════════════════════════════════════════════════════════
echo                    ГОТОВО! Все таблицы выведены.
echo ════════════════════════════════════════════════════════════
echo.
pause
endlocal