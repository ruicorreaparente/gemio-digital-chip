// Sprint 1 - Hello World em C++
#include <iostream>
#include <vector>
#include <string>

int main() {
    std::vector<std::string> mensagens = {
        "[OK] Docker funcionando!",
        "[OK] GCC compilando!",
        "[OK] Ambiente EDA pronto para o Sprint 2!"
    };

    std::cout << "==========================================" << std::endl;
    std::cout << "  Gemeo Digital de Chip - Sprint 1" << std::endl;
    std::cout << "==========================================" << std::endl;

    for (const auto& msg : mensagens) {
        std::cout << "  " << msg << std::endl;
    }

    std::cout << "\n[SUCESSO] Pipeline validado!" << std::endl;
    return 0;
}
