#include <stdio.h>
#include <stdlib.h> 
#include <stdarg.h>

#include "log.h"

FILE* Log_File = NULL;

int creat_log ()
{
    if (Log_File != 0)
        return 0;
    
    Log_File = fopen ("list.log.htm", "w");
    fprintf(Log_File, "<pre>\n");
    // fprintf(Log_File, "<pre>\n");

    atexit (fclose_log);    

    return 0;
}

void fclose_log ()
{
    if (Log_File != 0)
        fclose(Log_File);

    Log_File = NULL;
}

// int log_printf (const char ddlx[], ...)
// {
//     va_list arg;
//     va_start (arg, ddlx);

//     vprintf (ddlx, arg);

//     return 0;
// }

// int sum (int fist, ...)
// {
//     va_list arg;
//     va_start (arg, fist);

//     int result = 0;
//     int x = fist;

//     while (x != -1) 
//     {
//         result += x;
//         x = va_arg(arg, int);
//     }

//     printf("x = %d\n", x);

//     return result;
// }

// int sum(int first, ...)
// {
//     va_list args;
//     va_start(args, first);



//     int result = 0;
//     int x = first;

//     while (x != -1) 
//     {
//         if (isalpha(x)) // проверка на букву 
//         {
//             result += x; 
//         }

//         else
//         {
//             result += x; 
//         }
//         x = va_arg(args, int);
//     }

//     return result;
// }



int log_printf(const char* first, ...) __attribute__((format (printf, 1, 2)));

int log_printf(const char* first, ...) 
{
    va_list args;
    va_start(args, first);

    vfprintf  (stderr, first, args); // stdout - обычный printf 
    int count_symbol = vfprintf (Log_File, first, args); 

    fflush (Log_File); // данные сбросятся на диск и мы увидим в браузере самые свежие распечатки ( все что было напечатано )
    
    va_end(args);

    // char* ptr = first;
    // while (*ptr) 
    // { 
    //     if (*ptr == '%') 
    //     {
    //         ptr++;
    //         if (*ptr == 'd') 
    //         {
    //             int intValue = va_arg(args, int);
    //             if (intValue == -1) 
    //                 continue;  // Пропускает 
    //             printf("%d", intValue);
    //         } 

    //         else if (*ptr == 'c') 
    //         {
    //             int charValue = va_arg(args, int);
    //             putchar(charValue);
    //         } 

    //         else if (*ptr == 's') 
    //         {
    //             char *strValue = va_arg(args, char*);
    //             printf("%s", strValue);
    //         }
    //     } 
    //     else 
    //         putchar(*ptr);
    //     ptr++;
    // }

    return count_symbol;
}

int log_printf_pause (const char* first, ...)
{
    va_list args;
    va_start(args, first);

    log_printf(first, args);

    fprintf(stderr, "press any key\n");
    getchar();

    va_end(args);

    return 0;
}
// int main(void)
// {
//     int total = sum(6, 1, 'b', 3, 4, 'd', 1,-1); 
//     char bykva = 'b' , ddlx = 'd';
//     printf("%d | %d\n", bykva, ddlx); 
//     log_printf("Values: %d, %d, %c, %d, %p\n", 1, 2, 'd', 4, -1);
//     printf("sum: %d\n", total);       

//     return 0;
// }
