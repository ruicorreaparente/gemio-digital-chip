# Changelog

Todas as mudanças notáveis neste projeto serão documentadas neste arquivo.

O formato é baseado em [Keep a Changelog](https://keepachangelog.com/pt-BR/1.0.0/),
e este projeto adere ao [Versionamento Semântico](https://semver.org/lang/pt-BR/).

## [Sprint 1] - 2026-09-28

### Adicionado

- Dockerfile base com Ubuntu 22.04
- Ferramentas: GCC, G++, Make, CMake, Git
- Usuário não-root developer (segurança)
- Estrutura de pastas do projeto
- Exemplo Hello World em C++
- Makefile com targets build/run/clean
- Arquivo .dockerignore para otimizar build
- Documentação inicial (README.md)

### Validado

- ✅ Docker Desktop funcionando (v29.8.0)
- ✅ WSL2 + Ubuntu integrados
- ✅ Build da imagem Docker bem-sucedido
- ✅ Container executando corretamente
- ✅ Compilação C++ dentro do container
- ✅ Pipeline end-to-end validado

### Próximo Sprint

- Sprint 2: Adicionar SystemC 3.0.2 ao Dockerfile
- Sprint 2: Implementar modelo GPIO em SystemC
- Sprint 2: Testes unitários do GPIO