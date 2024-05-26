# RU VACANCY VIEWER
Проект содержит 2 программы:

1) Программа для просмотра вакансий
2) Программа для сбора датасета, обновления вакансий, обучения модели

Подключение к СУБД настраивается в config.json файле.

Если модель обучилась на нескольких специальностях, то необходимо добавить идентификаторы специальностей в файле classificator_config.json в массив available_ids.

Предоставлять доступ к базе данных с вакансиями другим лицам запрещено.

Для работы приложения обновления вакансий необходимо зарегистрировать свое приложение (https://dev.hh.ru/), получить одобрение на заявку, после чего ввести свой токен в файле network.cpp в конструкторе класса.

Требования:

1) Windows 10
2) Windows SDK 10
3) Boost.Python
4) Qt 6.2.*
5) Python 3.10.*
6) Libcurl
7) PostgreSQL 16+
8) C++ 20
9) MSVC 142

Модули Python:

1) transformers[pytorch]
2) numpy
3) pandas
4) nltk
5) datasets
6) evaluate
7) torch
8) psycopg2

![Поиск](https://github.com/KOJIMEISTER/RU_VACANCY_VIEWER/blob/main/preview/search.png)
![Проифль](https://github.com/KOJIMEISTER/RU_VACANCY_VIEWER/blob/main/preview/profile.png)
![История](https://github.com/KOJIMEISTER/RU_VACANCY_VIEWER/blob/main/preview/history.png)
![Сбор датасета](https://github.com/KOJIMEISTER/RU_VACANCY_VIEWER/blob/main/preview/dataset.png)
![Обучение](https://github.com/KOJIMEISTER/RU_VACANCY_VIEWER/blob/main/preview/train.png)
![Обновление вакансий](https://github.com/KOJIMEISTER/RU_VACANCY_VIEWER/blob/main/preview/refresh.png)
