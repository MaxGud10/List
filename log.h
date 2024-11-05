#pragma once // защита от множественного включения 
int log_printf(const char* first, ...) __attribute__((format (printf, 1, 2)));
int log_printf_pause (const char* first, ...);
int creat_log ();
void fclose_log ();