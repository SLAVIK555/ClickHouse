#Загрузить образ убунту 18.04
FROM ubuntu:18.04

# Установим обновления
RUN apt-get update

#Для сборки кликхауса понадобиться компилятор gcc9
RUN apt install software-properties-common
RUN add-apt-repository ppa:ubuntu-toolchain-r/test
RUN apt install gcc-9 g++-9
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 90 --slave /usr/bin/g++ g++ /usr/bin/g++-9

#Скопируем необходисые файлы исходники для сборки Кликхаус
RUN apt install git
RUN git clone https://github.com/artpaul/clickhouse-cpp.git

#Выполняем сборку Кликхауса из исходников
RUN mkdir build
WORKDIR /newclickhouse/build
RUN cmake ../clickhouse-cpp
RUN make
