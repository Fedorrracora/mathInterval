# mathInterval
[![PyPI](https://img.shields.io/pypi/v/mathInterval)](https://pypi.org/project/mathInterval/)
[![License](https://img.shields.io/badge/license-Apache--2.0-blue.svg)](LICENSE)
[![C++](https://img.shields.io/badge/C++-20-blue.svg)]()
[![Python](https://img.shields.io/badge/Python-3.8+-green.svg)]()

[🇬🇧 Read in English](README.md)

Арифметика интервалов на C++, доступная в Python.  
Библиотеку также можно использовать напрямую в C++.

Модуль предоставляет классы и алгоритмы для работы
с математическими интервалами (множествами). Он поддерживает:

- Создание интервалов с конечными или бесконечными границами.
- Умные алгоритмы поиска с использованием пользовательских лямбда-функций.
- Преобразование и перенос данных множеств.
- Применение арифметических операторов между интервалами.

Все классы и функции содержат docstring’и в стиле Python,
которые можно просмотреть через `help()` в консоли Python.

В C++ описание каждой функции написано прямо в заголовочном файле
(`interval.h`) перед её объявлением.

## Установка

### C++

Скопируйте файл `interval.h` в свой проект и подключите его
стандартным способом (`#include "interval.h"`).

### Python

```bash
pip install mathInterval
```

Так же, вы можете собрать и установить библиотеку вручную:

> ⚠️ Перед сборкой убедитесь, что установлен модуль **build** для `pip`:
> ```bash
> pip install build
> ```

```bash
git clone https://github.com/Fedorrracora/mathInterval --recurse-submodules
cd mathInterval
python -m build
pip install dist/*.whl
```

### Совет:

Чтобы собрать универсальный wheel, можно использовать:

```bash
cibuildwheel --platform auto
```

## Удаление (Python)

```bash
pip uninstall mathInterval
```