#ifndef MODULO3_SENSORES_H
#define MODULO3_SENSORES_H

#include "estruturas.h"

/* =========================================================
   MÓDULO 3 — MONITORIZAÇÃO DE SENSORES
   =========================================================
   Funcionalidades:
     1. Importar leituras a partir de sensores_rack.txt
     2. Guardar leituras na estrutura de dados (lista ligada)
     3. Listar leituras mais recentes
     4. Pesquisar por código de sensor
     5. Identificar leituras AVISO / CRITICO / FALHA_REDE
     6. Criar incidente automático para leituras anómalas
     7. Registar importação em log_sensores.txt
     8. Guardar / carregar leituras em ficheiro binário
   ========================================================= */

/* Ficheiros usados por este módulo */
#define FICHEIRO_SENSORES_TXT  "sensores_rack.txt"
#define FICHEIRO_SENSORES_DAT  "leituras_sensores.dat"
#define FICHEIRO_LOG_SENSORES  "log_sensores.txt"

/* --- Gestão da lista de sensores ------------------------- */

/** Inicializa a lista (deve ser chamado uma vez no arranque). */
void sensores_inicializar(ListaSensores *lista);

/** Liberta toda a memória da lista. */
void sensores_destruir(ListaSensores *lista);

/** Adiciona uma leitura no início da lista (mais recente primeiro). */
void sensores_adicionar(ListaSensores *lista, LeituraSensor leitura);

/* --- Importação / exportação ----------------------------- */

/**
 * Importa leituras do ficheiro de texto FICHEIRO_SENSORES_TXT.
 * Para cada leitura anómala chama o callback criar_incidente_sensor
 * (pode ser NULL se não houver integração com módulo 4).
 * Retorna o número de leituras importadas (≥0) ou -1 em erro.
 */
int sensores_importar_ficheiro(ListaSensores *lista,
                               ListaIncidentes *incidentes, /* pode ser NULL */
                               FilaIncidentes  *fila);      /* pode ser NULL */

/**
 * Guarda todas as leituras em ficheiro binário.
 * Retorna 0 em sucesso, -1 em erro.
 */
int sensores_guardar_binario(const ListaSensores *lista);

/**
 * Carrega leituras do ficheiro binário para a lista.
 * A lista deve estar inicializada mas vazia.
 * Retorna número de registos carregados ou -1 em erro.
 */
int sensores_carregar_binario(ListaSensores *lista);

/* --- Listagens / pesquisa -------------------------------- */

/** Lista todas as leituras (mais recentes primeiro). */
void sensores_listar_todos(const ListaSensores *lista);

/**
 * Lista as N leituras mais recentes.
 * Se n <= 0, lista todas.
 */
void sensores_listar_recentes(const ListaSensores *lista, int n);

/**
 * Pesquisa leituras pelo código do sensor.
 * Imprime todos os resultados encontrados.
 * Retorna o número de ocorrências.
 */
int sensores_pesquisar_por_codigo(const ListaSensores *lista,
                                  const char *codigo);

/** Lista apenas leituras com estado AVISO, CRITICO ou FALHA_REDE. */
void sensores_listar_anomalos(const ListaSensores *lista);

/* --- Utilitários ----------------------------------------- */

/** Converte EstadoSensor para string legível. */
const char *sensor_estado_para_str(EstadoSensor e);

/** Converte string do ficheiro para EstadoSensor. */
EstadoSensor sensor_str_para_estado(const char *s);

/** Imprime uma leitura formatada. */
void sensor_imprimir(const LeituraSensor *l);

/* --- Menu interativo ------------------------------------- */

/**
 * Menu principal do módulo 3.
 * Recebe ponteiros para os módulos 4 (incidentes / fila)
 * para criar incidentes automáticos quando necessário.
 */
void modulo3_menu(ListaSensores   *sensores,
                  ListaIncidentes *incidentes,
                  FilaIncidentes  *fila);

#endif /* MODULO3_SENSORES_H */
