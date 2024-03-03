#include "string.h"

char* index_page = "<html lang=\"en\">\n\
<head>\n\
    <style>\n\
        body {\n\
            background-color: #f2eddf;\n\
            font-family: \"Lucida Console\", monospace;\n\
            margin: 0; /* Убираем внешние отступы для body */\n\
        }\n\
\n\
        h1, p, #basic, #head-site, #menu-top {\n\
            font-family: \"Lucida Console\", monospace;\n\
        }\n\
\n\
        #head-site {\n\
            text-align: center;\n\
            background-color: #263d59;\n\
            padding: 20px; /* Добавляем внутренние отступы для красивого вида */\n\
        }\n\
\n\
        #head-site img {\n\
            max-width: 100%; /* Делаем изображение реагирующим на размеры контейнера */\n\
            height: auto; /* Для сохранения пропорций изображения */\n\
            display: block;\n\
            margin: 0 auto;\n\
        }\n\
\n\
        button {\n\
            border-radius: 15px;\n\
            padding: 8px 15px;\n\
            cursor: pointer;\n\
            font-family: 'Optima', sans-serif;\n\
        }\n\
\n\
        .round-button {\n\
            border-radius: 0;\n\
            padding: 15px 30px;\n\
            cursor: pointer;\n\
            font-family: \"Lucida Console\", monospace;\n\
            background-color: transparent;\n\
            border: 2px solid #000;\n\
            color: #000;\n\
            font-size: 18px;\n\
            margin: 0 5%; /* Используем проценты для отступов */\n\
            transition: background-color 0.3s, color 0.3s;\n\
            margin-bottom: 10px; /* Добавляем отступ снизу между кнопками */\n\
        }\n\
\n\
\n\
        .round-button.clicked {\n\
            background-color: #ffeb3b;\n\
            color: #263d59;\n\
            border: 2px solid transparent;\n\
        }\n\
\n\
        #menu-top {\n\
            text-align: center;\n\
            margin-top: 20px;\n\
        }\n\
\n\
        #colorPicker {\n\
            width: 100px;\n\
            height: 100px;\n\
            border: none;\n\
            background: none;\n\
            padding: 0;\n\
            cursor: pointer;\n\
        }\n\
\n\
        #colorPickerContainer {\n\
            margin-top: 20px;\n\
            text-align: center;\n\
         margin-left: 100px;\n\
        }\n\
\n\
        #colorValue {\n\
            margin-top: 10px;\n\
        }\n\
    </style>\n\
    <meta charset=\"UTF-8\">\n\
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\
    <title>Лампошки</title>\n\
    <link rel=\"icon\" href=\"https://i.postimg.cc/gc7m76CT/icon.png\" type=\"image/png\">\n\
</head>\n\
<body>\n\
    <div id=\"head-site\">\n\
        <img src=\"https://i.postimg.cc/gjkn7JYz/kot-header.png\" alt=\"Header Image\">\n\
    </div>\n\
    <div id=\"basic\">\n\
        <h1 align=\"center\">мур мур мур</h1>\n\
    </div>\n\
    <div id=\"menu-top\">\n\
        <a href=\"/LED=SWITCH\"><button class=\"round-button\" onclick=\"toggleButton(this)\">Ванна</button></a>\n\
        <a href=\"/LED=ON\"><button class=\"round-button\" onclick=\"toggleButton(this)\">Кухня</button></a>\n\
        <a href=\"/LED=ON\"><button class=\"round-button\" onclick=\"toggleButton(this)\">Спальня</button></a>\n\
    </div>\n\
\n\
    <div id=\"colorPickerContainer\">\n\
        <input type=\"color\" id=\"colorPicker\" onchange=\"updateColor(this.value)\">\n\
        <div id=\"colorValue\"></div>\n\
        <button onclick=\"sendColor()\">Ок</button>\n\
    </div>\n\
\n\
    <script>\n\
        function toggleButton(button) {\n\
            button.classList.toggle('clicked');\n\
        }\n\
\n\
        function updateColor(color) {\n\
            var colorValueElement = document.getElementById('colorValue');\n\
            var colorPickerElement = document.getElementById('colorPicker');\n\
\n\
            colorValueElement.style.marginTop = '0';\n\
            colorValueElement.style.position = 'static';\n\
\n\
            var rect = colorPickerElement.getBoundingClientRect();\n\
            colorValueElement.style.position = 'absolute';\n\
            colorValueElement.style.top = (rect.bottom + window.scrollY + 5) + 'px';\n\
            colorValueElement.style.left = rect.left + 'px';\n\
            colorValueElement.style.width = rect.width + 'px';\n\
            colorValueElement.style.textAlign = 'center';\n\
\n\
            colorValueElement.textContent = 'Выбранный цвет: ' + color;\n\
        }\n\
\n\
        function sendColor() {\n\
            var colorPicker = document.getElementById('colorPicker');\n\
            var selectedColor = colorPicker.value;\n\
\n\
         let response = fetch('/', {\n\
                                                  method: 'POST',\n\
                                                  headers: {\n\
                                                   'Content-Type': 'application/json;charset=utf-8'\n\
                                                  },\n\
                                                  body: JSON.stringify({\n\
                                                  color: selectedColor\n\
                                                  })\n\
                                                });\n\
            \n\
            // Здесь вы можете выполнить необходимые действия для отправки цвета\n\
            // Например, отправка через AJAX или перенаправление на другую страницу с параметром цвета\n\
        }\n\
    </script>\n\
</body>\n\
</html\n\
";

