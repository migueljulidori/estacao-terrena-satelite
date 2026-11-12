# Protocolo (payload ≤ 16 bytes) — precisa de responsável

**Status:** a definir.  
**Ação:** quem for ficar com esta parte, coloque seu nome em [`EQUIPE.md`](../../EQUIPE.md).

## Objetivo

Especificar a mensagem MQTT do satélite:

- Campos, tamanhos e ordem (soma ≤ **16 bytes**, incluindo HMAC/IV/tag se houver)
- Identificação do satélite e tipo de mensagem
- Exemplos hex e regras de decodificação

## Como começar

1. Assuma a parte em `EQUIPE.md`.
2. Crie um arquivo `especificarcao.md` com a tabela byte a byte (mesmo que provisória).
3. Alinhe com Miguel (firmware) e com quem fizer o `cliente-pc/`.
4. Atualize `CONTEXTO.md` + `ATUALIZACOES.md` e abra o PR.
