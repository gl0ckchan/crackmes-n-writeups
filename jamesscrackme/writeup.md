jamesscrackme by jamesinuk
https://crackmes.one/crackme/5ab77f5e33c5d40ad448c748

Language: C/C++
Platform: Windows
Difficulty: 1.0

при запуске программа просит ввести имя и серийник. Если они неверные, то программа сообщает нам об этом и перезапускается.
```bash
Registration name: NAME
Registration serial: SERIAL

Registation failed!
But sinse ime a nice guy ime going to let you have another go!
```

strings нашла несколько любопытных вещей.
- %s-%d
- Sorry, name must be between 3-15 charictars!

второе, очевидно, параметры имени. 
а вот первое пока не совсем ясно. Но понятно, что это формат какой то строки.

запускаем x32dbg, вводим данные и смотрим как проходит выполнение.

интересный фрагмент находим. Здесь мы сравниваем длину введённого имени. На всякий ставим тут брейкпоинт и идём дальше.
```assembly
00401452 | 8D45 88                      | lea eax,dword ptr ss:[ebp-78]                     |
00401455 | 890424                       | mov dword ptr ss:[esp],eax                        | [esp]:"NAME"
00401458 | E8 33F40000                  | call <JMP.&strlen>                                |
```

далее идёт следующий набор команд. С каждым разом в EAX записывается более урезанная версия NAME. Очевидно, это цикл.

здесь есть пара интересных моментов. Во первых, программа явно как то использует наше имя. А также на каждом цикле производит вычитание 3C7F (15487 в десятич.)
```assembly
00401493 | 7D 2A                        | jge james'scrackme.4014BF                         |
00401495 | 8D45 F8                      | lea eax,dword ptr ss:[ebp-8]                      |
00401498 | 0385 2CFEFFFF                | add eax,dword ptr ss:[ebp-1D4]                    |
0040149E | 83E8 70                      | sub eax,70                                        | eax:"AME"
004014A1 | 0FBE00                       | movsx eax,byte ptr ds:[eax]                       | eax:"AME"
004014A4 | 0385 34FEFFFF                | add eax,dword ptr ss:[ebp-1CC]                    |
004014AA | 2D 7F3C0000                  | sub eax,3C7F                                      | eax:"AME"
004014AF | 8985 34FEFFFF                | mov dword ptr ss:[ebp-1CC],eax                    |
004014B5 | 8D85 2CFEFFFF                | lea eax,dword ptr ss:[ebp-1D4]                    |
004014BB | FF00                         | inc dword ptr ds:[eax]                            | eax:"AME"
004014BD | EB C8                        | jmp james'scrackme.401487                         |
004014BF | 8B85 34FEFFFF                | mov eax,dword ptr ss:[ebp-1CC]                    |
```

приблизительно код выглядит так:
```c
for (size_t i = 0; i < name_length; ++i) {
    sum += name[i];
    sum -= 15487;
}
```

сразу после цикла видим следующие инструкции (боже храни x32dbg, сразу отображающий используемые значения).
здесь, очевидно, происходит некая манипуляция со строками. Мы были правы в том, что найденная ранее строка "%s-%d" - это формат. 
также мы здесь видим строку "SR8", которая добавляется в начало серийника, по формату как раз. Второе же значение - вычисленное ранее в цикле.
```assembly
004014CF | 894424 08                    | mov dword ptr ss:[esp+8],eax                      | [esp+08]:"SR8"
004014D3 | C74424 04 8D004400           | mov dword ptr ss:[esp+4],james'scrackme.44008D    | 44008D:"%s-%d"
004014DB | 8D85 A8FEFFFF                | lea eax,dword ptr ss:[ebp-158]                    | [ebp-158]:wcstombs+70
004014E1 | 890424                       | mov dword ptr ss:[esp],eax                        | [esp]:"NAME"
004014E4 | E8 C7F50000                  | call <JMP.&wsprintfA>                             |
```

а далее, после преобразования строк, в стеке видим следующее. Это и есть сгенерированный серийник. Число явно ушло в минус и это используется в серийнике.
```assembly
1: [esp] 0064FDD0 0064FDD0 "SR8--61659"
```

таким образом, примерный код генерации серийника выглядит так.
```c
for (size_t i = 0; i < name_length; ++i) {
    sum += name[i];
    sum -= 15487;
}

char buff[14];
wsprintfA(buffer, "%s-%d", "SR8", sum);
```

проверяем найденный нами серийник и видим, что регистрация проходит.
```bash
Registration sucseeded!

Your serial was:  SR8--61659
And if you think your really good write a keygen!
If you have managed to make a keygen or just write a serial for your name please email me at Jamesdcockayne@hotmail.co.uk and i will send you a copy of the source code!
```
