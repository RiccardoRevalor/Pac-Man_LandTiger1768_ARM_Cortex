gameover_map = {
'map': [
        [0 for _ in range(48)] for _ in range(10)  # Prime 10 righe vuote
    ] + [
        # Pac-Man (colore giallo)
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] +
        [0 for _ in range(48 - 24)] for _ in range(3)
    ] + [
        # Pac-Man (bocca aperta, che ha perso)
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] +
        [0 for _ in range(48 - 24)] for _ in range(3)
    ] + [
        # Dettagli Pac-Man
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] +
        [0 for _ in range(48 - 24)] for _ in range(3)
    ] + [
        # Fantasma Rosso (Blinky)
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] +
        [0 for _ in range(48 - 24)] for _ in range(3)
    ] + [
        # Corpo Fantasma Rosso
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] +
        [0 for _ in range(48 - 24)] for _ in range(3)
    ] + [
        # Fantasma Blu (Inky)
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] +
        [0 for _ in range(48 - 24)] for _ in range(3)
    ] + [
        # Corpo Fantasma Blu
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] +
        [0 for _ in range(48 - 24)] for _ in range(3)
    ] + [
        # Resto della mappa riempito con zeri
        [0 for _ in range(48)] for _ in range(64 - 28)
    ]
}

# Legenda colori:
# 0 = Sfondo vuoto
# 1 = Giallo (Pac-Man)
# 2 = Nero (dettagli Pac-Man)
# 3 = Rosso (Blinky)
# 4 = Blu (Inky)

# Esempio di come stampare la mappa
def print_map(game_map):
    color_map = {
        0: ' ',  # Vuoto
        1: '🟨',  # Giallo
        2: '⬛',  # Nero
        3: '🟥',  # Rosso
        4: '🟦'   # Blu
    }
    
    for row in game_map['map']:
        print(''.join(color_map.get(cell, ' ') for cell in row))

# Stampa la mappa
print(str(gameover_map['map']).replace('[', '{').replace(']', '}'))