# SectionsTest

Задача об отрезках в какой-то точке.  
Решение задачи разными способами на C++.

## Константы для тестов

В программе заданы константы, что ширина листа 210 мм (А4).  
Каждый отрезок длиной не более 1/10 ширины листа.  
Начальная точка и длина каждого отрезка задаются случайно.  
Общее число отрезков 100000.
            
## Результаты

Протестированные способы:

1. Линейный поиск по всем отрезкам и всему диапазону.
2. Отрезки отсортированы по точке начала и поиск прекращается после прохода искомой точки.
3. Битовые поля, соответствующие отрезкам, шириной в лист.
4. Приведение битовых полей к побитовым операциям размерностей x32 и x64 для сравнения.
5. Работа с массивом x64 вместо массива структур.
6. Прекращение поиска после прохода искомой точки.
7. Предварительное сужение полосы прохода по длине предварительного наибольшего отрезка.

Для контроля разных способов сравнивается число найденных отрезков.
                          
### По центру
                    
Для прогонов подставлена точка в центре ширины листа: 105.  
Найдено пересекающих отрезков: 5472.  
Время каждого способа в таблице ниже:

```
Enter a test point [0..209]:
105
Test1,    matches: 5472, time: 0.0049566s
Test2,    matches: 5472, time: 0.0012192s
Test3,    matches: 5472, time: 0.0063362s
Test4x32, matches: 5472, time: 0.0018483s
Test4x64, matches: 5472, time: 0.0013827s
Test5,    matches: 5472, time: 0.0007807s
Test6,    matches: 5472, time: 0.0005161s
Test7,    matches: 5472, time: 0.0002669s
```

### В начале

```
Enter a test point [0..209]:
0
Test1,    matches: 499, time: 0.001737s
Test2,    matches: 499, time: 1.21e-05s
Test3,    matches: 499, time: 0.0067402s
Test4x32, matches: 499, time: 0.0016699s
Test4x64, matches: 499, time: 0.0016441s
Test5,    matches: 499, time: 0.0015882s
Test6,    matches: 499, time: 2.03e-05s
Test7,    matches: 499, time: 2.08e-05s
```

### В конце

```
Enter a test point [0..209]:
209
Test1,    matches: 507, time: 0.0032643s
Test2,    matches: 507, time: 0.0021897s
Test3,    matches: 507, time: 0.0067324s
Test4x32, matches: 507, time: 0.0016257s
Test4x64, matches: 507, time: 0.0021194s
Test5,    matches: 507, time: 0.0007385s
Test6,    matches: 507, time: 0.0006449s
Test7,    matches: 507, time: 0.0001129s
```

## Програмная среда

Microsoft Visual Studio Community 2022 (64bit).  
Продукт: Microsoft Visual C++ 2022  
Тип шаблона проекта: Пустой проект, C++.
