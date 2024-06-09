# Web service for finding medical facilities with optimal route determination function
## (Веб-сервіс для пошуку медичних закладів з функцією визначення оптимального маршруту)

## Getting Started (Початок роботи)

1. **Clone the project (Склонуйте проект)**:
    ```bash
    git clone <repository URL>
    ```

2. **Install required software (Встановіть необхідне ПЗ)**:
    ```bash
    sudo apt install clang-format-15
    sudo apt install cmake
    sudo apt install libboost-all-dev
    sudo apt-get install nlohmann-json3-dev
    sudo apt install libpqxx-6.4
    ```

3. **Install libraries for the parser (Встановіть бібліотеки для парсера)**:
    ```bash
    pip install elementpath
    pip install geopy
    ```

4. **Download data from [OpenStreetMap](https://www.openstreetmap.org/export#map=15/50.4442/30.5297) (Завантажте дані з [OpenStreetMap](https://www.openstreetmap.org/export#map=15/50.4442/30.5297))**

5. **Parse the data using `parser.py` (Пропарсіть дані за допомогою `parser.py`)**

6. **Create your own database using this script (Створіть власну базу даних за допомогою цього скрипта)**:
    ```sql
    CREATE TABLE metadata(
        id BIGSERIAL PRIMARY KEY NOT NULL,
        facility_name CHAR(128),
        address CHAR(64),
        facility_type INT ARRAY,
        latitude DOUBLE PRECISION NOT NULL,
        longitude DOUBLE PRECISION NOT NULL
    );

    CREATE TABLE node(
        id BIGSERIAL PRIMARY KEY NOT NULL,
        meta_id BIGSERIAL REFERENCES metadata(id)
    );

    CREATE TABLE transport(
        id SERIAL PRIMARY KEY NOT NULL,
        transport_name CHAR(512),
        transport_type INT ARRAY
    );

    CREATE TABLE arc(
        id BIGSERIAL PRIMARY KEY NOT NULL,
        from_node BIGSERIAL REFERENCES node(id),
        to_node BIGSERIAL REFERENCES node(id),
        street CHAR(256),
        distance REAL NOT NULL,
        transport_id BIGSERIAL REFERENCES transport(id)
    );
    ```

7. **Fill the parsed data into the database (Заповніть базу даних отриманими даними)**

8. **Connect Google Maps API to the third page (Підключіть Google Maps API до третьої сторінки)**:
   Використовуйте цей [мануал](https://developers.google.com/maps/documentation/javascript/adding-a-google-map) для підключення Google Maps API до третьої сторінки.

9. **Connect the database to the C++ program (Підключити базу даних до програми на C++)**

    > I use PostgreSQL and the `libpqxx` library. If you want to use another DBMS, install the respective library and modify the database creation script accordingly. (Я використовую PostgreSQL і бібліотеку `libpqxx`. Якщо ви хочете використовувати іншу СУБД, встановіть відповідну бібліотеку і змініть скрипт створення бази даних.)

10. **Run the program (Запустити програму)** using the command:
    ```bash
    ./run.sh
    ```
    > Run this command in the directory where the cloned project is located. (Запустіть цю команду в директорії, в якій знаходиться клонований проект.)

11. **Open the web page `index.html` (Відкрити веб-сторінку `index.html`)**
