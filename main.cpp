#include <iostream>
#include "text_parser.h"
#include <algorithm>

int main(int argc, char* argv[]) {
    if(argc == 1) {
        std::cout << "Вам нужно передать в аргументы программы путь к файлу, в котором написан текст.\n";
        return 0;
    }
    if(!is_txt_format(argv[1])) {
        std::cout << "Файл должен быть формата txt.\n";
        return 0;
    }
    text_searcher pars(argv[1]);
    if(!pars.is_valid()) {
        std::cout << "Вашего файла не существует.\n";
        std::cout << "Проверьте, правильно ли вы ввели путь к файлу, и попробуйте снова.\n";
        return 0;
    }

    std::cout << "Введите количество строк, расположение которых вы хотите знать.\n";
    int N;
    std::cin >> N;
    std::vector<std::string> all_lines(N);

    std::cout << "Введите сами строки:\n";
    std::cin.ignore(1);
    for(int i = 0; i < N; i++) {
        char line[4096];
        std::cin.getline(line, sizeof(line));
        all_lines[i] = std::string(line);
    }

    std::cout << "Результат:\n";
    std::vector<std::vector<long long>> result = pars.get_all_occurrences(all_lines);
    for(size_t i = 0; i < N; i++) {
        std::cout << "Cтрока: " << all_lines[i] << " -> " << "Первые вхождения: ";
        if(result[i].size() == 0) {
            std::cout << "нет\n";
            continue;
        }
        std::sort(result[i].begin(), result[i].end());
        for(size_t j = 0; j < result[i].size(); j++) {
            std::cout << result[i][j] << ' ';
        }
        std::cout << '\n';
    }
}
