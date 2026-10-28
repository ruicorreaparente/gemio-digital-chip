Write-Host "=== PASSO 1: Copiando arquivos ===" -ForegroundColor Cyan
docker cp "D:\dev\gemio-digital-chip\common\dtwin_ipc_protocol.h" qemu-build:/tmp/dtwin_ipc_protocol.h
docker cp "D:\dev\gemio-digital-chip\common\test_protocol.c" qemu-build:/tmp/test_protocol.c

Write-Host "=== PASSO 2: Verificando ===" -ForegroundColor Cyan
docker exec qemu-build bash -c "ls -la /tmp/dtwin_ipc_protocol.h /tmp/test_protocol.c"

Write-Host "=== PASSO 3: Compilando com GCC ===" -ForegroundColor Cyan
docker exec qemu-build bash -c "cd /tmp && gcc -I. -o test_protocol test_protocol.c && ./test_protocol"

Write-Host "=== PASSO 4: Compilando com G++ ===" -ForegroundColor Cyan
docker exec qemu-build bash -c "cd /tmp && g++ -I. -x c++ -o test_protocol_cpp test_protocol.c && ./test_protocol_cpp"

Write-Host "=== FIM ===" -ForegroundColor Cyan
