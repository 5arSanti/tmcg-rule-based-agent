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
const int VALOR_PELIGRO = 5;

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

void marcar_celdas_adyacentes_como_peligrosas(int tablero[][COLUMNAS], int fila,
                                              int columna) {
  if (fila - 1 >= 0 && tablero[fila - 1][columna] == 0)
    tablero[fila - 1][columna] = VALOR_PELIGRO;
  // abajo
  if (fila + 1 < FILAS && tablero[fila + 1][columna] == 0)
    tablero[fila + 1][columna] = VALOR_PELIGRO;
  // izquierda
  if (columna - 1 >= 0 && tablero[fila][columna - 1] == 0)
    tablero[fila][columna - 1] = VALOR_PELIGRO;
  // derecha
  if (columna + 1 < COLUMNAS && tablero[fila][columna + 1] == 0)
    tablero[fila][columna + 1] = VALOR_PELIGRO;
}

void calcular_peligros(int tablero[][COLUMNAS]) {
  for (int i = 0; i < FILAS; i++) {
    for (int j = 0; j < COLUMNAS; j++) {
      if (tablero[i][j] == VALOR_GATO) {
        marcar_celdas_adyacentes_como_peligrosas(tablero, i, j);
      }
    }
  }
}

int *colocar_raton(int tablero[][COLUMNAS], bool visitado[][COLUMNAS]) {
  int fila_raton = POS_INICIAL_RATON_F;
  int col_raton = POS_INICIAL_RATON_C;

  tablero[fila_raton][col_raton] = VALOR_RATON;
  visitado[fila_raton][col_raton] = true;

  return new int[2]{fila_raton, col_raton};
}

int main() {
  static int tablero_real[FILAS][COLUMNAS];
  static bool visitado[FILAS][COLUMNAS];

  inicializar_tableros(tablero_real, visitado);
  colocar_actores(tablero_real);
  calcular_peligros(tablero_real);

  int *posicion_raton = colocar_raton(tablero_real, visitado);
  int fila_raton = posicion_raton[0];
  int col_raton = posicion_raton[1];

  bool juego_activo = true;
  int estado = ESTADO_EN_CURSO;
  int turno = 1;

  while (juego_activo) {

  }
}