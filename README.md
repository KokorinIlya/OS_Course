# Операционные системы, Университет ИТМО, 2018

Задачи взяты со [страницы курса](http://neerc.ifmo.ru/~os/hw.html)

## Домашнее задание №0: Hello, world:

Написать программу, выводящую на экран надпись "Hello, world". При этом функция, печатающая строку и функция main должны
находиться в разных единицах трансляции. Сборка должна осуществляться с помощью утилиты make.

## Домашнее задание №1: Shell:

Необходимо написать упрощенный командный интерпретатор(shell).

shell должен:

    1. В бесконечном цикле, как настоящий shell
    2. Считывать со стандартного ввода программу, которую необходимо запустить, вместе с ее аргументами. 
    Программа задается в виде полного пути до исполняемого файла.
    3. Выполнять заданную программу с заданными аргументами
    4. Дожидаться завершения программы и выводить в стандартный поток вывода код ее завершения
    5. Необходимо обрабатывать ошибки всех используемых системных вызовов
    6. Необходимо обрабатывать конец вводу со стандартного потока

[Решение](Shell/)

## Практическое задание № 1: Cat

Написать подмножество утилиты cat, выводящей на стандартный поток вывода содержимое файла или символы, вводимые со стандартного потока ввода.

## Домашнее задание №2?: Grep:

Назовём "каноническим" путём к файлу среди множества возможных такой путь, в котором количество компонент минимально, а при равенстве количества компонент путь минимален лексикографически.

Grep принимает первым аргументом строку s и рекурсивно ищет файлы, в которых встречается эта строка, начиная с текущей директории, выводя канонические пути относительно текущей директории ко всем уникальным файлам, в которых строка нашлась. Порядок, в котором выводятся пути, не важен.

Файлы с одинаковым inode считаются одним и тем же файлом.

Необходимо корректно обрабатывать симлинки, но игнорировать символьные устройства, FIFO-пайпы и т.д.

## Домашнее задание № 3: Кусочек JIT компилятора

Программа должна:

    1. Выделить память с помощью mmap(2)
    2. Записать в выделенную память машинный код, соответсвующий какой-либо функции
    3. Изменить права на выделенную память - чтение и исполнение. See: mprotect(2)
    4. Вызвать функцию по указателю на выделенную память
    5. Освободить выделенную память
    
Сильные духом призываются к возможности модификации кода выполняемой функции в runtime.

Записываемая в память программа представляет собой функцию *return_value*, не принимающую аргументов и возвращающую число 42.

Возможна модификация машинного кода этой фукнции во время выполнения числом, полученным в качестве аргумента вызова программы JitCompiler.

Варианты вызова:

    1. ./JitCompiler - вызов машинного кода, соответствующего функции return_vaue
    2. ./JitCompiler <число> - вызов машинного кода, соответсвующего изменённой версии 
    функции return_value. Вместо 42 она будет возвращать переданное программе в качестве 
    аргумента <число>

## Домашнее задание № 4: Библиотеки
Необходимо создать статическую, и две динамических библиотеки и программу, которая будет их использовать.

Помимо этого нужен Makefile, с помощью которого можно будет собрать все части.

Статическая библиотека должна:

    Собираться статически
    Предоставлять какие-то функции

Первая динамическая библиотека должна:

    Собираться динамически
    Динамически линковаться с программой на этапе линковки
    Предоставлять какие-то функции

Вторая динамическая библиотека должна:

    Собираться динамически
    Предоставлять какие-то функции

Программа должна

    Статически линковаться с статической библиотекой и вызывать предоставляемые ей функции
    Динамически линковаться с первой динамической библиотекой и вызывать предоставляемые ей функции
    Во время выполнения в явном виде загружать вторую динамическию библиотеку с помощью dlopen(3) 
    и вызывать какие-то функции из нее

### Замечания по сборке
Каждая из библиотек собирается независимо, с помощью запуска утилиты make в корне директории этой библиотеки.

Программа так же собирается запуском make в корне директории *utility*. В случае отсутствия собранных библиотек, запуск make в директории *utility* попробует собрать так же и их, но для этого должен быть разрешён запуск shell-скриптов *build_static.sh* и *build_shared.sh*. Разрешить их запуск можно с помощью команд

    $ chmod +x build_static.sh
    $ chmod +x build_shared.sh

После этоого библиотеки могут быть собраны с помощью запуска make в директории *utility*

## Сборка:

Каждое из домашних заданий находится в отдельной директории и собирается запуском утилиты make в корне этой директории.
