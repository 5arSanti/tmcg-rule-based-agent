#include <cstdlib>
#include <iostream>
#include <unistd.h> // usleep

const int FILAS = 12;
const int COLUMNAS = 10;
const int NUM_GATOS = 5;
const int NUM_TRAMPAS = 5;
const int NUM_QUESOS = 1;
const int POS_INICIAL_RATON_F = 0;
const int POS_INICIAL_RATON_C = 0;
const int PAUSA_MS = 1000;

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

void imprimir_tablero(int tablero[][COLUMNAS], bool visitado[][COLUMNAS],
                      int turno, int fila_raton, int col_raton) {
  // Limpiar consola (Unix)
  system("clear");

  std::cout << "Turno: " << turno << "\n\n";

  for (int fila = 0; fila < FILAS; ++fila) {
    for (int columna = 0; columna < COLUMNAS; ++columna) {
      char c = '.';
      int val = tablero[fila][columna];

      bool raton_en_celda = fila == fila_raton && columna == col_raton;

      if (raton_en_celda) {
        c = 'R';
      } else if (val == VALOR_VACIO) {
        bool celda_visitada = visitado[fila][columna];
        if (celda_visitada)
          c = '*';
        else
          c = '.';
      } else if (val == VALOR_GATO)
        c = 'G';
      else if (val == VALOR_TRAMPA)
        c = 'T';
      else if (val == VALOR_QUESO)
        c = 'Q';
      else if (val == VALOR_PELIGRO)
        c = '!';
      else
        c = '?';

      std::cout << c << ' ';
    }
    std::cout << '\n';
  }
}

struct Movimientos {
  int movs[4][2];
  int num_movs;
};

Movimientos obtener_movimientos_validos(int tablero[][COLUMNAS],
                                        bool visitado[][COLUMNAS], int fila,
                                        int col) {
  Movimientos resultado;
  resultado.num_movs = 0;

  // Arriba
  if (fila - 1 >= 0) {
    resultado.movs[resultado.num_movs][0] = fila - 1;
    resultado.movs[resultado.num_movs][1] = col;
    ++resultado.num_movs;
  }
  // Abajo
  if (fila + 1 < FILAS) {
    resultado.movs[resultado.num_movs][0] = fila + 1;
    resultado.movs[resultado.num_movs][1] = col;
    ++resultado.num_movs;
  }
  // Izquierda
  if (col - 1 >= 0) {
    resultado.movs[resultado.num_movs][0] = fila;
    resultado.movs[resultado.num_movs][1] = col - 1;
    ++resultado.num_movs;
  }
  // Derecha
  if (col + 1 < COLUMNAS) {
    resultado.movs[resultado.num_movs][0] = fila;
    resultado.movs[resultado.num_movs][1] = col + 1;
    ++resultado.num_movs;
  }

  return resultado;
}

int elegir_movimiento_aleatorio(int arr[], int n) {
  if (n <= 0)
    return -1;
  return arr[std::rand() % n];
}

// mover_raton refactorizado: más legible y con los mismos comportamientos.
void mover_raton(int tablero[][COLUMNAS], bool visitado[][COLUMNAS],
                 int &fila_raton, int &col_raton) {

  // Obtener movimientos posibles (máximo 4)
  Movimientos movs =
      obtener_movimientos_validos(tablero, visitado, fila_raton, col_raton);

  if (movs.num_movs == 0)
    return;

  // Arrays para almacenar los índices (k) de movs.movs que cumplen cada
  // categoría.
  int safe_unvisited[4], n_safe_unvisited = 0;
  int danger_unvisited[4], n_danger_unvisited = 0;
  int safe_visited[4], n_safe_visited = 0;
  int all_candidates[4], n_all = 0;

  // Clasificar cada movimiento en una (o más) categoría según la celda destino.
  for (int k = 0; k < movs.num_movs; ++k) {
    int rf = movs.movs[k][0];            // fila de la celda destino
    int rc = movs.movs[k][1];            // fila y columna de la celda destino
    int valor_destino = tablero[rf][rc]; // valor de la celda destino

    all_candidates[n_all++] = k;

    bool es_gato_o_trampa =
        (valor_destino == VALOR_GATO || valor_destino == VALOR_TRAMPA);
    bool es_peligro = (valor_destino == VALOR_PELIGRO);
    bool ya_visitado = visitado[rf][rc];

    // Prioridad 1: seguro (no peligro), no gato/trampa, no visitado
    if (!es_peligro && !es_gato_o_trampa && !ya_visitado) {
      safe_unvisited[n_safe_unvisited++] = k;
      continue;
    }

    // Prioridad 2: peligro y no visitado (arriesgarse)
    if (es_peligro && !ya_visitado && !es_gato_o_trampa) {
      danger_unvisited[n_danger_unvisited++] = k;
      continue;
    }

    // Alternativa: seguro pero ya visitado
    if (!es_peligro && !es_gato_o_trampa && ya_visitado) {
      safe_visited[n_safe_visited++] = k;
      continue;
    }
  }

  int indice_movimiento_elegido =
      elegir_movimiento_aleatorio(safe_unvisited, n_safe_unvisited);

  if (indice_movimiento_elegido == -1)
    indice_movimiento_elegido =
        elegir_movimiento_aleatorio(danger_unvisited, n_danger_unvisited);

  if (indice_movimiento_elegido == -1)
    indice_movimiento_elegido =
        elegir_movimiento_aleatorio(safe_visited, n_safe_visited);

  if (indice_movimiento_elegido == -1)
    indice_movimiento_elegido =
        elegir_movimiento_aleatorio(all_candidates, n_all);

  // Ejecutar movimiento elegido
  int nueva_fila = movs.movs[indice_movimiento_elegido][0];
  int nueva_columna = movs.movs[indice_movimiento_elegido][1];

  // Guardar lo que había antes en la celda destino (usado por verificar_estado)
  ultimo_valor_destino = tablero[nueva_fila][nueva_columna];

  // Marcar la celda anterior como vacía y registrada como visitada
  tablero[fila_raton][col_raton] = 0;
  visitado[fila_raton][col_raton] = true;

  // Colocar ratón en nueva posición (sobrescribe cualquier valor;
  // verificar_estado usará ultimo_valor_destino)
  tablero[nueva_fila][nueva_columna] = 1;

  // Actualizar coordenadas
  fila_raton = nueva_fila;
  col_raton = nueva_columna;
}

int verificar_estado(int tablero[][COLUMNAS], int fila, int col, int turno) {
  if (ultimo_valor_destino == VALOR_QUESO)
    return ESTADO_VICTORIA;
  if (ultimo_valor_destino == VALOR_GATO)
    return ESTADO_GATO;
  if (ultimo_valor_destino == VALOR_TRAMPA)
    return ESTADO_TRAMPA;

  return ESTADO_EN_CURSO;
}

void mostrar_resultado(int estado) {
  if (estado == ESTADO_VICTORIA) {
    std::cout << "\n¡El ratón encontró el queso!\n";
  } else if (estado == ESTADO_GATO) {
    std::cout << "\n¡El ratón fue atrapado por un gato!\n";
  } else if (estado == ESTADO_TRAMPA) {
    std::cout << "\n¡El ratón cayó en una trampa!\n";
  } else if (estado == ESTADO_EMPATE) {
    std::cout << "\nEl ratón no encontró una salida.\n";
  } else {
    std::cout << "\nJuego terminado.\n";
  }
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
    imprimir_tablero(tablero_real, visitado, turno, fila_raton, col_raton);

    mover_raton(tablero_real, visitado, fila_raton, col_raton);

    estado = verificar_estado(tablero_real, fila_raton, col_raton, turno);

    if (estado != ESTADO_EN_CURSO) {
      imprimir_tablero(tablero_real, visitado, turno, fila_raton, col_raton);
      mostrar_resultado(estado);
      break;
    }

    usleep(PAUSA_MS * 1000);

    ++turno;
  }
}