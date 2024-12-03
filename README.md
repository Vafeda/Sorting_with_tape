# Сортировка N количество чисел при доступных M байт (M &lt; N)
## Задание
Устройство хранения данных типа лента (Tape) предназначено для последовательной записи и чтения 
данных. Считывающая/записывающая магнитная головка неподвижна во время чтения и записи, а 
лента имеет возможность двигаться в обоих направлениях. Запись и чтение информации возможны в 
ячейку ленты, на которой в данный момент находится магнитная головка. Перемещения ленты – 
затратная по времени операция – лента не предназначена для произвольного доступа.  
Имеется входная лента длины N (где N – велико), содержащая элементы типа integer (232). Имеется 
выходная лента такой же длины. Необходимо записать в выходную ленту отсортированные по 
возрастанию элементы с входной ленты. Есть ограничение по использованию оперативной памяти – не 
более M байт (M может быть < N, т.е. загрузить все данные с ленты в оперативную память не 
получится). Для реализации алгоритма можно использовать разумное количество временных лент, т.е. 
лент, на которых можно хранить какую-то временную информацию, необходимую в процессе работы 
алгоритма.

Необходимо создать проект С++, компилируемый в консольное приложение, которое реализует 
алгоритм сортировки данных с входной ленты на выходную. Необходимо сделать следующее:
- Определить интерфейс для работы с устройством типа лента.
- Написать класс, реализующий этот интерфейс и эмулирующий работу с лентой посредством 
обычного файла. Должно быть возможно сконфигурировать (без перекомпиляции – например, 
через внешний конфигурационный файл, который будет прочитан на старте приложения) 
задержки по записи/чтению элемента с ленты, перемотки ленты, и сдвига ленты на одну 
позицию.
- Файлы временных лент можно сохранять в директорию tmp.
- Написать класс, реализующий алгоритм сортировки данных с входной ленты на выходную.
- Консольное приложение должно принимать на вход имя входного и выходного файлов и 
производить сортировку.
- Желательно написать юнит-тесты.

## Интерфейс ленты
Для создания интерфейся был создан класс `Operation`, который в последствии был унаследованн классом `Tape`. В данном классе прописанны следующие виртуальные методы:
- Next (Сдвиг ленты вправа на одну позицию)
- Prev (Сдвиг ленты влево на одну позицию)
- Read (Чтение элемента с ленты)
- Write (Запись элемента на ленту)
- Rewinding_the_tape (Перемотка ленты)

Также стоит уделить внимания что в классе `Operation` прописана константная переменная `size`. Данная переменная устанавливает количество байт выделяемое на одну ячейку. В данном случае для визуального понимания работы алгоритма было выделено 12 байт (1 байт на `" "`, 11 байт на `unsigned int`). 
