#include <cstdlib>

const int FILAS = 12;
const int COLUMNAS = 10;
const int NUM_GATOS = 5;
const int NUM_TRAMPAS = 5;
const int NUM_QUESOS = 1;
const int POS_INICIAL_RATON_F = 0;
const int POS_INICIAL_RATON_C = 0;
const int PAUSA_MS = 1000;
const int MAX_TURNOS = FILAS * COLUMNAS * 2;

const int ESTADO_EN_CURSO = 0;
const int ESTADO_VICTORIA = 1;
const int ESTADO_GATO = 2;
const int ESTADO_TRAMPA = 3;
const int ESTADO_EMPATE = 4;

int ultimo_valor_destino = 0;

const int VALOR_VACIO = 0;
const int VALOR_RATON = 1;
const int VALOR_GATO = 2;
const int VALOR_TRAMPA = 3;
const int VALOR_QUESO = 4;

void inicializar_tableros(int tablero[][COLUMNAS], bool visitado[][COLUMNAS]) {
  for (int i = 0; i < FILAS; i++) {
    for (int j = 0; j < COLUMNAS; j++) {
      tablero[i][j] = VALOR_VACIO;
      visitado[i][j] = false;
    }
  }
}

void randomizar_actores(int tablero[][COLUMNAS], int max_value, int state) {
  int colocados = 0;
  while (colocados < max_value) {
    int fila_aleatoria = rand() % FILAS;
    int columna_aleatoria = rand() % COLUMNAS;

    bool invalid_position = (fila_aleatoria == POS_INICIAL_RATON_F &&
                             columna_aleatoria == POS_INICIAL_RATON_C) ||
                            tablero[fila_aleatoria][columna_aleatoria] != 0;

    if (invalid_position)
      continue;

    tablero[fila_aleatoria][columna_aleatoria] = state;
    ++colocados;
  }
}

void colocar_actores(int tablero[][COLUMNAS]) {
  randomizar_actores(tablero, NUM_GATOS, VALOR_GATO);
  randomizar_actores(tablero, NUM_TRAMPAS, VALOR_TRAMPA);
  randomizar_actores(tablero, NUM_QUESOS, VALOR_QUESO);
}

int main() {
  static int tablero_real[FILAS][COLUMNAS];
  static bool visitado[FILAS][COLUMNAS];

  inicializar_tableros(tablero_real, visitado);
}