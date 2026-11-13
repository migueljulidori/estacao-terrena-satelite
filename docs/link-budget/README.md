# Link budget — enlace geoestacionário

## Objetivo

Calcular a potência de transmissão (P_t) necessária para a estação terrestre fechar um enlace com um satélite geoestacionário. O ESP32 representa o satélite na demonstração prática; o cálculo usa os parâmetros de um enlace GEO real.

## Dados fornecidos pelo enunciado

| Parâmetro | Valor / observação |
|---|---|
| Altitude GEO | 35.786 km |
| Raio médio da Terra (R_e) | 6.378 km |
| Raio orbital (R_s) | 42.164 km |
| Elevação (E) | Definir para o cenário |
| Frequência (f) | Escolher banda/frequência |
| Diâmetro da antena (D) | Sugestão: 2,4 m |
| Figura de mérito do satélite (G/T) | Sugestão: 0 dB/K |
| Perdas atmosféricas (L_{atm}) | Sugestão: 2 dB |
| Margem | Sugestão: 3 dB |
| Taxa de bits (R_b) | Definir |
| (E_b/N_0) requerido | Depende da modulação adotada |

## Fórmulas do PI 3T

### 1. Geometria — distância inclinada

[
d=\sqrt{R_s^2-R_e^2\cos^2 E}-R_e\,sen E
]

### 2. Perda de espaço livre

[
L_{FSPL}(dB)=92,45+20\log_{10}(f_{GHz})+20\log_{10}(d_{km})
]

### 3. Ganho da antena da estação

[
G_t(dBi)\approx17,8+20\log_{10}(D_m)+20\log_{10}(f_{GHz})
]

Eficiência aproximada: 60%.

### 4. Relação portadora/densidade de ruído

[
C/N_0=EIRP+(G/T)_{sat}-L_{FSPL}-L_{atm}-k
]

[
EIRP=P_t+G_t-L_{alim}
]

[
k=-228,6\ dBW/(K\cdot Hz)
]

### 5. Condição para fechar o enlace

[
C/N_0\geq(E_b/N_0)_{req}+10\log_{10}(R_b)+margem
]

Resolver a desigualdade para (P_t), registrando todas as unidades e conversões.

## Próximos passos do cálculo

1. Definir frequência/banda.
2. Definir ângulo de elevação.
3. Confirmar diâmetro da antena e perda de alimentação.
4. Definir taxa de bits e modulação para obter (E_b/N_0).
5. Calcular (d), FSPL, ganho, (C/N_0) e (P_t).
6. Registrar fontes dos parâmetros e memória de cálculo.

Não preencher resultados numéricos antes de a equipe definir os parâmetros que o enunciado deixa em aberto.
