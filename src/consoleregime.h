#ifndef CONSOLEREGIME
#define CONSOLEREGIME


/////////////////////////////////////////////////////////////////////////////////////////////////////////
///		consoleregime.h - функции для работы калькулятора в консольном режиме.
///
///						АРИФМЕТИЧЕСКИЙ КАЛЬКУЛЯТОР 1.0
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////

extern int exact;

void ConsoleRegime ()	// Функция консольного режима работы калькулятора
{
    srand ((unsigned) time (NULL)); // инициализация генератора случайных чисел
    rprin ("Введите арифметическое выражение:\n");

    struct complex current_result;
    char *p,
        str [MAX_EXPRESSION];  // строка, содержащая арифметическое выражение, вводится с клавиатуры или считывается из файла

    StringToNumber ariph;		// объект класса вычисления выражения

    for (;;) // главный цикл работы калькулятора
    {
        loop: error = ThisBoolValue = ThisComment = false;
        Write = true;

        gets (str); // ввести с клавиатуры выражение

#ifdef WINDOWS
        for (p = str; *p; ++p)
        {
            *p = convCyrStr1(*p);
        }
#else
        char *ptemp = get_windows1251(str);
        strcpy(str, ptemp);
        delete ptemp;
#endif
        p = str;

        // Блок выполнения команд
        {
            while( *p && isspace (*p++)); // пропустить пробелы
            --p;

            // установка точности
            if (tolower (*p) == 'e' && tolower (*(p+1)) == 'x' && tolower (*(p+2)) == 'a' && tolower (*(p+3)) == 'c' && tolower (*(p+4)) == 't')
            {
                p += 5;
                exact = atoi (p);
                rprin ("Результат отображается с точностью ");
                printf ("%i\n", exact);
                goto loop;
            }

            // вызов справки
            if (tolower (*p) == 'h' && tolower (*(p+1)) == 'e' && tolower (*(p+2)) == 'l' && tolower (*(p+3)) == 'p')
            {
                p += 5;
                system ("Инструкция.doc");
                goto loop;
            }

        } // Блок выполнения команд

        current_result = ariph.StrToNum (str); // вычислить значение выражения

        if (!error && !ThisComment) // если нет ошибок и не пустое выражение, отобразить результат
        {
            answer = current_result;
            printf (" = ");
            if (!ThisBoolValue) // если числовой ответ
                WriteComplexNumberInFile (current_result.Re, current_result.Im, stdout, exact);

            else // иначе если логический ответ
                (current_result.Re == 1.) ? printf ("true") : printf ("false");

                printf ("\n--------------------------------------------------------------------------------\n");

        }
        else
            printf ("\n");
    }
}
#endif // CONSOLEREGIME

