
/////////////////////////////////////////////////////////////////////////////////////////////////////////
///		main.cpp - главный файл
///
///						АРИФМЕТИЧЕСКИЙ КАЛЬКУЛЯТОР 1.0
///
///			- definitions.h							- препроцессорные утверждения и вспомогательные функции;
///
///
///			- complex.h								- описание класса complex для работы с
///													  комплексными числами;
///
///
///			- stringtonumber.h						- описание класса StringToNumber вычисления арифметического выражения,
///													  записанного в символьном массиве;
///
///			- consoleregime.h						- функции для работы калькулятора в консольном режиме;
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


//==============================================================================================================
// БЛОК ГЛОБАЛЬНЫХ ПЕРЕМЕННЫХ:

    // Флаги, характеризующие результат вычисленного выражения:

bool error = false, // равно true если в ходе вычисления строки обнаружена ошибка;

     Write = true,  // определяет необходимость вывода результата выражения на экран (например,
                    // если все выражение состоит из одного объявления новой переменной, то на экран
                    // не выводится никакой результат);

     ThisComment,	// определяет, состоит ли всё выражение из одного комментария;

     ThisBoolValue; // определяет, является ли значение выражения логическим (булевым).

int exact = 0;  // определяет число знаков после запятой, с ктоторым отобразится результат вычисленного
                // выражения. Может назначаться пользователем с помощью команды EXACT. Если равно 0, то
                // программа отображает результат по своему усмотрению.

// массив с названиями всех функций, операторов и констант, ни одна пользовательская переменная не должна
// совпадать с ними, имена могут иметь русские и английские символы и знаки _ кроме первого символа.
// Имена и значения пользовательских переменных хранятся в отдельном массиве, если лексема является переменной,
// т.е. ThisIs=VARIABLE, то IndexVar хранит номер элемента массива, в котором хранится информация о данной
// переменной - её имя и значение. Количесво переменных ограничено.
char ListNamesUnique [][10] =
{	// Новые записи добавлять обязательно в конец массива!
    // Записи не менять местами!

    // функции:
    "abs",		// модуль числа
    "acos",		// арккосинус
    "asin",		// арксинус
    "atg",		// арктангенс
    "actg",		// арккотангенс
    "exp",		// экспонента
    "ln",		// натур. логарифм
    "lg",		// десятичный логарифм
    "tg",		// тангенс
    "sin",		// синус
    "sqr",		// возведение в квадрат
    "sqrt",		// квадратный корень
    "cos",		// косинус
    "ctg",		// котангенс
    "cub",		// возведение в куб
    "cbrt",		// кубический корень
    "round",	// округление до ближайшего целого
    "mod",		// оператор: остаток по модулю от деления двух чисел
    "div",		// оператор: целая часть по модулю от деления двух чисел
    "hypot",	// гипотенуза по двум катетам
    "log",		// логарифм по произвольному основанию
    "rand",		// генерация вещественного случайного числа в заданных пределах
    "mid",		// среднее арифметическое двух чисел
    "max",		// наибольшее из двух чисел
    "min",		// наименьшее из двух чисел
    "nod",		// наибольший общий делитель двух чисел
    "pow",		// степень числа

    // константы:
    "pi",		// 3.14...
    "Eo",		// "эпсилон нулевое"
    "e",		// заряд электрона, Кл
    "c",		// скорость света, м/с
    "g",		// ускорение свободного падения
    "G",		// гравитационная постоянная
    "Vm",		// молярный объем идеального газа при н.у.
    "Na",		// число Авогадро
    "h",		// постоянная Планка
    "k",		// постоянная Больцмана
    "R",		// универсальная газовая постоянная
    "me",		// масса электрона, кг
    "mp",		// масса протона, кг
    "mn",		// масса нейтрона, кг
    "mo",		// "мю нулевое"

    "ans",		// метка ans - результат предыдущего выражения

    "i",		// мнимая единица

    // Функции:
    "arg",		// аргумент комплексного числа
    "module",	// модуль комплексного числа
    "conj",		// сопряженное комплексного числа
    "sh",		// гиперболический синус
    "ch",		// гиперболический косинус
    "th",		// гиперболический тангенс
    "ash",		// гиперболический арксинус
    "ach",		// гиперболический арккосинус
    "ath",		// гиперболический арктангенс
    "acth",		// гиперболический арккотангенс
    "cth",		// гиперболический котангенс
    "sec",		// секанс
    "csec",		// косеканс
    "torad",	// перевод из градусов в радианы
    "todeg",	// перевод из радиан в градусы

    "not",		// логический оператор НЕ
    "and",		// логический оператор И
    "or",		// логический оператор ИЛИ

    "true",		// константа true
    "false",	// константа false

    "integral",	// функция вычисления определенного интеграла (3 аргумента)
    "re",		// идентификатор действительной части значения переменной
    "im",		// идентификатор комплексной части значения переменной

    "getre",
    "getim",

    // TODO: сюда заносить новые записи

    ""			// пустая строка означает конец записей
};

//==============================================================================================================
// БЛОК ПОДКЛЮЧЕНИЯ ФАЙЛОВ:

#include "definitions.h"		// макроопределения, препроцессорные утверждения;
#include "complex.h"			// описания класса complex для работы с комплексными
                                //числами;

#include "stringtonumber.h"		// описание класса StringToNumber вычисления
                                // арифметического выражения;

#include "consoleregime.h"		// функции для работы калькулятора в консольном режиме;


//=============================================================

struct complex answer = {0,0}, // предыдущее вычисленное значение, определяет метку ans в строке-формуле, изначально равно 0
    temp_complex_number,
    null = {0,0};


// ГЛАВНАЯ ФУНКЦИЯ:

int main ()
{
    ConsoleRegime (); // консольный режим работы калькулятора - ввод с клавиатуры
    // арифметического выражения до нажатия ВВОД, вычисление его и отображение на консоли,
    // и так до нажатия ESC - выход из приложения.

    return 0;
}
