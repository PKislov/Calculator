#ifndef DEFINITIONS
#define DEFINITIONS

/////////////////////////////////////////////////////////////////////////////////////////////////////////
///		definitions.h - препроцессорные утверждения.
///
///						АРИФМЕТИЧЕСКИЙ КАЛЬКУЛЯТОР 1.0
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////


//================ ПОЖКЛЮЧЕНИЕ СТАНДАРТНЫХ ЗАГОЛОВОЧНЫХ ФАЙЛОВ =============================================
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <cmath>
#include <ctype.h>			 // функции обработки символов
#include <ctime>			 // системный таймер необходим для генерации случайных чисел функцией rand

//=============================================================================

// выбрать ОС:
#define LINUX
//#define WINDOWS

//=============================================================================
// ОПРЕДЕЛЕНИЕ ТИПОВ ДАННЫХ:
typedef int THIS_IS;
//=============================================================================
// МАКРООПРЕДЕЛЕНИЯ:

//  Нижеперечисленные константы пределяют, чем является лексема - функцией, числом или оператором.
// число, например 3, -1.5, 2e3 = 2000, 1e-2 = 0.01, 5e+1 = 50

// тип данных: число
#define NUMBER			1
// оператор + (унарный и бинарный)
#define PLUS			2
// знак или оператор - (унарный и бинарный)
#define MINUS			3
// оператор умножения *
#define MULTIPLICATION  4
// оператор деления /
#define DIVISION		5
// оператор степени ^, например 2^3 = 8
#define DEGREE			6
// открывающаяся скобка ( { [ - начертание скобок не имеет значения
#define BRECKET_OPEN	7
// закрывающаяся скобка ) } ]
#define BRECKET_CLOSE	8
// оператор факториал числа !, 3! = 6
#define FACTORIAL		9
// функция-экспонента (число Е в степени...) exp
#define EXP				10
// оператор остаток по модулю (если число вещественное, дробная часть отбрасывается)
#define MOD				11
// оператор частное по модулю (например, 15div4 = 3), если число вещественное, дробная часть отбрасывается
#define DIV				12
// функция натурального логарифма, ln (exp6) = 6
#define LN				13
// функция синуса, sin 30 = 0.5
#define	SIN	            14
// функция косинуса cos
#define COS				15
// функция тангенса, tg 45 = 1
#define TG				16
// функция котангенса, ctg 45 = 1
#define CTG				17
// модуль числа, например |-2| = 2, имеет одинаковый приоритет со скобками
#define MODULE			18
// функция абсолютного значения, т.е. модуль числа, abs(2) = abs(-2) = 2
#define ABSVALUE		19
// функция арксинуса, asin(0.5) = 30
#define ASIN			20
// функция арккосинуса, acos(0.5) = 60
#define ACOS			21
// функция арктангенса, atg(1) = 45
#define ATAN			22
// функция арккотангенса, actg(1) = 45
#define ACTAN			23
// функция квадратного корня числа, sqrt(4) = 2
#define SQRT			24
// функция квадрата числа, sqr 4 = 16
#define SQR				25
// функция куба числа, cub 4 = 64
#define CUB				26
// функция кубического корня числа, cbrt 27 = 3
#define CBRT			27
// функция нахождения гипотенузы (a*a + b*b)^0.5, имеет два аргумента
#define HYPOT			28
// оператор запятая, разделяет параметры в функциях от нескольких аргументов, например hypot (3, 4) = 5
#define COMMA			29
// оператор, определяющий дробь, 1#2#3 + 1#3 = 2
#define FRACTION		30
// функция округления числа, round(2,4)  2
#define ROUND			31
// функция десятичного логарифма lg
#define LG				32
// функция логарифма по произвольному основанию (два аргумента), log(49, 7) = 2
#define LOG				33
// функция генерации случайного числа в некотром диапазоне (два аргумента), rand (2,3) = 2.128
#define RAND			34
// функция среднего значения двух чисел (два аргумента), mid(4, 7) = 5.5
#define MIDDLE			35
// функция нахождения максимального числа из двух (два аргумента), мах(2, 5) = 5
#define MAX2			36
// функция нахождения минимального числа из двух (два аргумента), min(2, 5) = 2
#define MIN2			37
// функция нахождения наибольшего общего делителя (два аргумента), nod(100, 75) = 25
#define NOD_DEL			38
// функция возведения в степень (два аргумента), pow (3, 2) = 9
#define POW				39
// оператор присваивания =
#define EQUAL			40
// тип данных пользовательская переменная
#define VARIABLE		41
// функция аргумента комплексного числа, arg(i) = 90
#define ARG				42
// функция модуля комплексного числа, module(i) = 1
#define COMPLEX_MODULE	43
// функция нахождения комплексно-сопряженного, conj(1+i) = 1-i
#define CONJ			44
// функция гиперболического синуса
#define SH_				45
// функция гиперболического косинуса
#define CH				46
// функция гиперболического тангенса
#define TH				47
// функция гиперболического котангенса
#define CTH				48
// функция гиперболического арксинуса
#define ASH				49
// функция гиперболического арккосинуса
#define ACH				50
// функция гиперболического арктангенса
#define ATH				51
// функция гиперболического арккотангенса
#define ACTH			52
// функция секанса
#define SEC				53
// функция косеканса
#define CSEC			54
// функция перевода градусов в радианы
#define TORAD			55
// функция перевода радианов в градусы
#define TODEG			56
// логический оператор отрицания (унарный), not 1 = false
#define NOT				57
// логический оператор И (бинарный), 1 and 0 = false
#define AND				58
// логический оператор ИЛИ (бинарный), 1 or 0 = true
#define OR				59
// логический оператор равно (бинарный), 2 == 2 = true
#define EGALE			60
// логический оператор не равно , 2 <> 2 = false
#define NE_EGALE		61
// логический оператор больше >
#define GREATER_THAN	62
// логический оператор меньше <
#define LESS_THAN		63
// логический оператор >=
#define GREATER_OR_EQUAL	64
// логический оператор  <=
#define LESS_OR_EQUAL		65
// логический тип данных
#define BOOL				66
// строковый тип данных
#define STRING				67
// функция вычисления определенного интеграла (3 аргумента), Integral("x+1",0,1) = 1.5, где строка в кавычках - подинтегральная функция, и два числа - пределы интегрирования
#define INTEGRAL			68
// оператор точка, служит разделителем имени переменной и комплексной или действительной части значения переменной (как доступ к членам структуры в языке С++)
#define POINT				69
// идентификатор действительной части значения переменной
#define MEMBER_STRUCT_RE	70
// идентификатор комплексной части значения переменной
#define MEMBER_STRUCT_IM	71
// функция getre возвращает действительную часть числа, getre(3+2i) = 3
#define GETRE				81
// функция getim возвращает комплексную часть числа, getim(3+2i) = 2
#define GETIM				82
//------------------------------------------------------------------------------
// Все нижеперечисленные константы определяют основные вычислительные параметры калькулятора.

// Если счетчик итераций циклов раэбиения строки на лексемы или вычисления выражения
// достигает данного значения, функция вычисления завершается, флаг error устанавливается
// в true и выводится сообщение о синтаксической ошибке в выражении
#define LIMITE_ERROR 700

// максимальная длина пользовательской переменной
#define SIZE_UNIQUE_WORD_AND_VARIABLES 200

// максимальное количество переменных
#define QUANTITY_VARIABLES 200

// наибольшая длина арифметического выражения
#define MAX_EXPRESSION 11000

// параметры, определяющие размер массива строк:
// максимальное количество строк
#define QUANTITY_STRINGS 200

// максимальная длина строки
#define MAX_STRING 500

// максимальное время в секундах, отводимое на вычисление одного интеграла
#define TIME_CALCULATION_INTEGRALE 15



extern bool error;


//================ МАКРООПРЕДЕЛЕНИЯ ========================================================================

// наибольшее из двух чисел
#define MAX(a,b) ( ((a) > (b)) ? (a) : (b) )

// наименьшее из двух чисел
#define MIN(a,b) (((a) > (b)) ? (b) : (a))

// модуль числа
#define ABS(x) ((x) >= 0.) ? (x) : -1.*(x)

// Физические константы:
#define k_c		299.792458E6
#define k_g		9.80665
#define k_G		66.725985E-12
#define k_Vm	22.4141E-3
#define k_Na	602.2136736E21
#define k_e		160.2177335E-21
#define k_me	910.9389754E-33
#define k_mp	1.67262311E-27
#define k_mn	1.67492861E-27
#define k_h		662.607554E-36
#define k_k		13.8065812E-24
#define k_R		8.3145107
#define k_m0	1.256637061E-6
#define k_e0	8.854187818E-12

// Математические константы:
#define M_E     2.71828182845904523536
#define M_PI    3.14159265358979323846

//================ ПРОТОТИПЫ ФУНКЦИЙ =======================================================================


//-------------------------------------------------------------------------------
// МАТЕМАТИЧЕСКИЕ ФУНКЦИИ

// возвращает значение corner переведенное из градусов в радианы
double ToRadians (double corner);
// возвращает значение corner переведенное из радиан в градусы
double ToGrad	 (double corner);

// возвращает "бесконечность" 1.#INF
double getInfinity ();

// возвращает факториал х
double fact		 (double x);

// возвращает наибольший общий делитель х и y
int	   NOD		 (int x, int y);

// возвращает значение х округленное до ближайшего целого числа
double round	 (double x);

//-------------------------------------------------------------------------------
// ФУНКЦИИ ОБЩЕГО НАЗНАЧЕНИЯ:

// Функция завершает работу приложения выводя на экран message - собщение об ошибке
void FatalError (char *message);

// Функция записывает в файл out комплексное число xy с точностью exact знаков после запятой.
// Если exact равно 0, то программа устанавливает точность по своему усмотрению.
// Возвращает количество записаных байтов.
int WriteComplexNumberInFile (double x, double y, FILE *out, int exact = 0);

#ifdef WINDOWS
int  rputcstd (int c); // печатает на консоли русский символ с, возвращает результат функции putc

typedef class ConvLetter {
public:
        char    win1251;
        int             unicode;
        ConvLetter (char ch, int i)
        {
            win1251 = ch; unicode = i;
        }
} Letter;

static Letter g_letters[] = {
    ConvLetter(0x82, 0x201A), // SINGLE LOW-9 QUOTATION MARK
         ConvLetter(0x83, 0x0453), // CYRILLIC SMALL LETTER GJE
         ConvLetter(0x84, 0x201E), // DOUBLE LOW-9 QUOTATION MARK
         ConvLetter(0x85, 0x2026), // HORIZONTAL ELLIPSIS
         ConvLetter(0x86, 0x2020), // DAGGER
         ConvLetter(0x87, 0x2021), // DOUBLE DAGGER
         ConvLetter(0x88, 0x20AC), // EURO SIGN
         ConvLetter(0x89, 0x2030), // PER MILLE SIGN
         ConvLetter(0x8A, 0x0409), // CYRILLIC CAPITAL LETTER LJE
         ConvLetter(0x8B, 0x2039), // SINGLE LEFT-POINTING ANGLE QUOTATION MARK
         ConvLetter(0x8C, 0x040A), // CYRILLIC CAPITAL LETTER NJE
         ConvLetter(0x8D, 0x040C), // CYRILLIC CAPITAL LETTER KJE
         ConvLetter(0x8E, 0x040B), // CYRILLIC CAPITAL LETTER TSHE
         ConvLetter(0x8F, 0x040F), // CYRILLIC CAPITAL LETTER DZHE
         ConvLetter(0x90, 0x0452), // CYRILLIC SMALL LETTER DJE
         ConvLetter(0x91, 0x2018), // LEFT SINGLE QUOTATION MARK
         ConvLetter(0x92, 0x2019), // RIGHT SINGLE QUOTATION MARK
         ConvLetter(0x93, 0x201C), // LEFT DOUBLE QUOTATION MARK
         ConvLetter(0x94, 0x201D), // RIGHT DOUBLE QUOTATION MARK
         ConvLetter(0x95, 0x2022), // BULLET
         ConvLetter(0x96, 0x2013), // EN DASH
         ConvLetter(0x97, 0x2014), // EM DASH
         ConvLetter(0x99, 0x2122), // TRADE MARK SIGN
         ConvLetter(0x9A, 0x0459), // CYRILLIC SMALL LETTER LJE
         ConvLetter(0x9B, 0x203A), // SINGLE RIGHT-POINTING ANGLE QUOTATION MARK
         ConvLetter(0x9C, 0x045A), // CYRILLIC SMALL LETTER NJE
         ConvLetter(0x9D, 0x045C), // CYRILLIC SMALL LETTER KJE
         ConvLetter(0x9E, 0x045B), // CYRILLIC SMALL LETTER TSHE
         ConvLetter(0x9F, 0x045F), // CYRILLIC SMALL LETTER DZHE
         ConvLetter(0xA0, 0x00A0), // NO-BREAK SPACE
         ConvLetter(0xA1, 0x040E), // CYRILLIC CAPITAL LETTER SHORT U
         ConvLetter(0xA2, 0x045E), // CYRILLIC SMALL LETTER SHORT U
         ConvLetter(0xA3, 0x0408), // CYRILLIC CAPITAL LETTER JE
         ConvLetter(0xA4, 0x00A4), // CURRENCY SIGN
         ConvLetter(0xA5, 0x0490), // CYRILLIC CAPITAL LETTER GHE WITH UPTURN
         ConvLetter(0xA6, 0x00A6), // BROKEN BAR
         ConvLetter(0xA7, 0x00A7), // SECTION SIGN
         ConvLetter(0xA8, 0x0401), // CYRILLIC CAPITAL LETTER IO
         ConvLetter(0xA9, 0x00A9), // COPYRIGHT SIGN
         ConvLetter(0xAA, 0x0404), // CYRILLIC CAPITAL LETTER UKRAINIAN IE
         ConvLetter(0xAB, 0x00AB), // LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
         ConvLetter(0xAC, 0x00AC), // NOT SIGN
         ConvLetter(0xAD, 0x00AD), // SOFT HYPHEN
         ConvLetter(0xAE, 0x00AE), // REGISTERED SIGN
         ConvLetter(0xAF, 0x0407), // CYRILLIC CAPITAL LETTER YI
         ConvLetter(0xB0, 0x00B0), // DEGREE SIGN
         ConvLetter(0xB1, 0x00B1), // PLUS-MINUS SIGN
         ConvLetter(0xB2, 0x0406), // CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I
         ConvLetter(0xB3, 0x0456), // CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I
         ConvLetter(0xB4, 0x0491), // CYRILLIC SMALL LETTER GHE WITH UPTURN
         ConvLetter(0xB5, 0x00B5), // MICRO SIGN
         ConvLetter(0xB6, 0x00B6), // PILCROW SIGN
         ConvLetter(0xB7, 0x00B7), // MIDDLE DOT
         ConvLetter(0xB8, 0x0451), // CYRILLIC SMALL LETTER IO
         ConvLetter(0xB9, 0x2116), // NUMERO SIGN
         ConvLetter(0xBA, 0x0454), // CYRILLIC SMALL LETTER UKRAINIAN IE
         ConvLetter(0xBB, 0x00BB), // RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK
         ConvLetter(0xBC, 0x0458), // CYRILLIC SMALL LETTER JE
         ConvLetter(0xBD, 0x0405), // CYRILLIC CAPITAL LETTER DZE
         ConvLetter(0xBE, 0x0455), // CYRILLIC SMALL LETTER DZE
         ConvLetter(0xBF, 0x0457) // CYRILLIC SMALL LETTER YI
};

int convert_utf8_to_windows1251(const char* utf8, char* windows1251, size_t n); // перевод строки из кодировки UTF-8 в С1251
#endif

// Выводит на консоль строку с русским текстом:
int rprin (const char * const);
int rputs (const char * const); // еще вставляет символ новой строки

#endif // DEFINITIONS

