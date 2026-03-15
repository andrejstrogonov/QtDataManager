# Используем Ubuntu LTS для стабильности и широкого набора Qt6 пакетов
FROM ubuntu:22.04

# Устанавливаем необходимые пакеты для сборки Qt приложения
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    qt6-base-dev \
    qt6-declarative-dev \
    libqt6sql6 \
    libqt6sql6-psql \
    libpq-dev \
    xvfb \
    libxcb1-dev \
    libxcb-xkb-dev \
    libxkbcommon-dev \
    libxkbcommon-x11-dev \
    libx11-dev \
    libxext-dev \
    libxrender-dev \
    libxrandr-dev \
    libxcursor-dev \
    libxfixes-dev \
    libxi-dev \
    libxss-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev \
    fontconfig \
    dbus-user-session \
    locales \
 && rm -rf /var/lib/apt/lists/*

# Устанавливаем переменную окружения для локали
RUN locale-gen en_US.UTF-8
ENV LANG=en_US.UTF-8
ENV LANGUAGE=en_US:en
ENV LC_ALL=en_US.UTF-8

# Рабочая директория
WORKDIR /public/Work

# Копируем исходные файлы
COPY . .
ENV QT_QPA_PLATFORM=xcb
ENV DISPLAY=:99

# Скрипт запуска
RUN printf '%s\n' '#!/bin/sh' \
    'Xvfb :99 -screen 0 1024x768x24 &' \
    'sleep 2' \
    './build/bin/QtDataManagerApp' > /start.sh && \
    chmod +x /start.sh

CMD ["/start.sh"]
