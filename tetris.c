#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define a capacidade máxima das estruturas de dados.
#define CAPACIDADE_FILA 5
#define CAPACIDADE_PILHA 3

// Estrutura de dados para uma Peca do Tetris.
typedef struct {
    char tipo; // I, O, T, S, Z, J, L
    int id;    // Identificador sequencial
} Peca;

// Estrutura de dados para a Fila Circular (buffer circular).
// Armazena as próximas peças a serem jogadas.
typedef struct {
    Peca itens[CAPACIDADE_FILA];
    int inicio;   // Índice do primeiro elemento
    int fim;      // Índice do último elemento
    int contagem; // Número de elementos na fila
} FilaCircular;

// Estrutura de dados para a Pilha Linear (LIFO).
// Armazena as peças de reserva.
typedef struct {
    Peca itens[CAPACIDADE_PILHA];
    int topo; // Índice do elemento no topo
} PilhaLinear;

// Variáveis globais para controle de geração de peças.
int id_sequencial = 1;
const char tiposPecas[7] = {'I', 'O', 'T', 'S', 'Z', 'J', 'L'};

// ----------------------------------------------------------------
// Funções Auxiliares
// ----------------------------------------------------------------

/**
 * @brief Gera uma nova Peca com tipo aleatório e ID sequencial.
 * @return A Peca recém-criada.
 */
Peca gerarPeca() {
    Peca p;
    p.tipo = tiposPecas[rand() % 7]; // Sorteia um dos 7 tipos
    p.id = id_sequencial++;
    return p;
}

// ----------------------------------------------------------------
// Funções da Fila Circular
// ----------------------------------------------------------------

/**
 * @brief Inicializa os ponteiros e a contagem da fila.
 */
void inicializarFila(FilaCircular *f) {
    f->inicio = 0;
    f->fim = -1;
    f->contagem = 0;
}

/**
 * @brief Verifica se a fila não possui elementos.
 * @return 1 (true) se vazia, 0 (false) caso contrário.
 */
int filaVazia(FilaCircular *f) {
    return f->contagem == 0;
}

/**
 * @brief Verifica se a fila atingiu sua capacidade máxima.
 * @return 1 (true) se cheia, 0 (false) caso contrário.
 */
int filaCheia(FilaCircular *f) {
    return f->contagem == CAPACIDADE_FILA;
}

/**
 * @brief Adiciona um elemento ao final da fila (enqueue).
 * Utiliza aritmética modular para garantir a circularidade.
 */
void enqueue(FilaCircular *f, Peca p) {
    if (filaCheia(f)) {
        printf("ERRO: Fila cheia. Nao foi possivel adicionar Peca %d.\n", p.id);
        return;
    }
    // Calcula o próximo índice 'fim' de forma circular
    f->fim = (f->fim + 1) % CAPACIDADE_FILA;
    f->itens[f->fim] = p;
    f->contagem++;
}

/**
 * @brief Remove e retorna o elemento do início da fila (dequeue).
 * @return A Peca removida. Retorna Peca com ID -1 se a fila estiver vazia.
 */
Peca dequeue(FilaCircular *f) {
    if (filaVazia(f)) {
        printf("ERRO: Fila vazia.\n");
        Peca vazia = {'X', -1}; // Retorna uma peça inválida
        return vazia;
    }
    Peca p = f->itens[f->inicio];
    // Calcula o próximo índice 'inicio' de forma circular
    f->inicio = (f->inicio + 1) % CAPACIDADE_FILA;
    f->contagem--;
    return p;
}

/**
 * @brief Exibe o estado atual da fila, respeitando a ordem de entrada.
 */
void mostrarFila(FilaCircular *f) {
    printf("Fila de Proximas Pecas (Frente -> Fim):\n");
    if (filaVazia(f)) {
        printf("[ Vazia ]\n");
        return;
    }

    printf("[ ");
    int i = f->inicio;
    int c;
    // Itera baseado na contagem, acessando os índices de forma circular
    for (c = 0; c < f->contagem; c++) {
        printf("ID:%d(%c) ", f->itens[i].id, f->itens[i].tipo);
        i = (i + 1) % CAPACIDADE_FILA; // Move para o próximo índice
    }
    printf("]\n");
}

// ----------------------------------------------------------------
// Funções da Pilha Linear
// ----------------------------------------------------------------

/**
 * @brief Inicializa o ponteiro do topo da pilha.
 */
void inicializarPilha(PilhaLinear *p) {
    p->topo = -1; // -1 indica pilha vazia
}

/**
 * @brief Verifica se a pilha não possui elementos.
 * @return 1 (true) se vazia, 0 (false) caso contrário.
 */
int pilhaVazia(PilhaLinear *p) {
    return p->topo == -1;
}

/**
 * @brief Verifica se a pilha atingiu sua capacidade máxima.
 * @return 1 (true) se cheia, 0 (false) caso contrário.
 */
int pilhaCheia(PilhaLinear *p) {
    return p->topo == CAPACIDADE_PILHA - 1;
}

/**
 * @brief Adiciona um elemento ao topo da pilha (push).
 */
void push(PilhaLinear *p, Peca peca) {
    if (pilhaCheia(p)) {
        printf("ERRO: Pilha cheia. Nao foi possivel adicionar Peca %d.\n", peca.id);
        return;
    }
    p->topo++;
    p->itens[p->topo] = peca;
}

/**
 * @brief Remove e retorna o elemento do topo da pilha (pop).
 * @return A Peca removida. Retorna Peca com ID -1 se a pilha estiver vazia.
 */
Peca pop(PilhaLinear *p) {
    if (pilhaVazia(p)) {
        printf("ERRO: Pilha de reserva vazia.\n");
        Peca vazia = {'X', -1}; // Retorna uma peça inválida
        return vazia;
    }
    Peca peca = p->itens[p->topo];
    p->topo--;
    return peca;
}

/**
 * @brief Exibe o estado atual da pilha (Topo -> Base).
 */
void mostrarPilha(PilhaLinear *p) {
    printf("Pilha de Reserva (Topo -> Base):\n");
    if (pilhaVazia(p)) {
        printf("[ Vazia ]\n");
        return;
    }

    printf("[ ");
    // Itera do topo até a base
    for (int i = p->topo; i >= 0; i--) {
        printf("ID:%d(%c) ", p->itens[i].id, p->itens[i].tipo);
    }
    printf("]\n");
}


// ----------------------------------------------------------------
// Funções de Integração (Fila <-> Pilha)
// ----------------------------------------------------------------

/**
 * @brief Troca a peça da frente da fila com a peça do topo da pilha.
 * Realiza a troca 'in-place' (diretamente nos arrays).
 */
void trocarFrenteTopo(FilaCircular *f, PilhaLinear *p) {
    // Validação das condições mínimas
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("AVISO: A fila e a pilha devem conter ao menos 1 peca para a troca.\n");
        return;
    }

    // A peça da frente da fila está em f->itens[f->inicio]
    // A peça do topo da pilha está em p->itens[p->topo]

    Peca temp = f->itens[f->inicio];
    f->itens[f->inicio] = p->itens[p->topo];
    p->itens[p->topo] = temp;

    printf("SUCESSO: Peca da frente da fila (ID:%d) trocada com topo da pilha (ID:%d).\n",
           p->itens[p->topo].id, f->itens[f->inicio].id);
}

/**
 * @brief Troca as 3 primeiras peças da fila com as 3 peças da pilha.
 * Exige que a pilha esteja cheia (3 peças) e a fila tenha ao menos 3 peças.
 */
void trocarTresComPilha(FilaCircular *f, PilhaLinear *p) {
    // Validação rigorosa das condições
    if (f->contagem < 3 || p->topo != (CAPACIDADE_PILHA - 1)) {
        printf("AVISO: A fila deve ter >= 3 pecas e a pilha deve estar CHEIA (3 pecas) para esta troca.\n");
        return;
    }

    Peca temp;
    int indiceFila;

    // A pilha é linear (índices 0, 1, 2)
    // A fila é circular (índices inicio, (inicio+1)%C, (inicio+2)%C)
    for (int i = 0; i < 3; i++) {
        // Calcula o índice da i-ésima peça da fila
        indiceFila = (f->inicio + i) % CAPACIDADE_FILA;

        // Realiza a troca
        temp = f->itens[indiceFila];
        f->itens[indiceFila] = p->itens[i];
        p->itens[i] = temp;
    }

    printf("SUCESSO: As 3 primeiras pecas da fila foram trocadas com a pilha.\n");
}


// ----------------------------------------------------------------
// Função Principal (Menu e Lógica)
// ----------------------------------------------------------------

int main() {
    FilaCircular fila;
    PilhaLinear pilha;
    int opcao;

    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Inicializa as estruturas
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila inicialmente (Nível Novato)
    printf("Iniciando o jogo... Gerando pecas iniciais...\n");
    for (int i = 0; i < CAPACIDADE_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    // Loop principal do menu
    while (1) {
        printf("\n--- Tetris Stack Control ---\n");
        mostrarFila(&fila);
        mostrarPilha(&pilha);
        printf("----------------------------\n");
        printf("Menu de Opcoes:\n");
        printf("1 - Jogar peca (Remove da frente, gera nova no fim)\n");
        printf("2 - Enviar peca da fila para a reserva (Fila -> Pilha)\n");
        printf("3 - Usar peca da reserva (Pilha -> Jogo)\n");
        printf("4 - Trocar peca da FRENTE (fila) com TOPO (pilha)\n");
        printf("5 - Trocar 3 PRIMEIROS (fila) com 3 (pilha)\n");
        printf("0 - Sair\n");
        printf("Escolha sua acao: ");
        
        if (scanf("%d", &opcao) != 1) {
            // Limpa o buffer de entrada em caso de input inválido (ex: char)
            while(getchar() != '\n');
            opcao = -1; // Força opção inválida
        }

        switch (opcao) {
            case 0:
                printf("Encerrando o jogo...\n");
                return 0; // Termina o programa

            case 1: // Jogar peça (Nível Novato)
                printf("\n[Acao: Jogar Peca]\n");
                Peca jogada = dequeue(&fila);
                if (jogada.id != -1) { // Verifica se o dequeue foi bem-sucedido
                    printf("Peca ID:%d (%c) foi jogada.\n", jogada.id, jogada.tipo);
                    // Repõe a fila com uma nova peça
                    Peca nova = gerarPeca();
                    enqueue(&fila, nova);
                    printf("Nova peca ID:%d (%c) entrou na fila.\n", nova.id, nova.tipo);
                }
                break;

            case 2: // Enviar da Fila para a Reserva (Nível Aventureiro)
                printf("\n[Acao: Fila -> Reserva]\n");
                if (pilhaCheia(&pilha)) {
                    printf("AVISO: Pilha de reserva esta cheia!\n");
                } else if (filaVazia(&fila)) {
                     printf("AVISO: Fila esta vazia! Nao ha o que reservar.\n");
                } else {
                    // Tira da fila
                    Peca movida = dequeue(&fila);
                    // Coloca na pilha
                    push(&pilha, movida);
                    printf("Peca ID:%d (%c) movida para a reserva.\n", movida.id, movida.tipo);
                    
                    // Repõe a fila (Regra: Fila sempre com 5)
                    Peca novaResposicao = gerarPeca();
                    enqueue(&fila, novaResposicao);
                    printf("Nova peca ID:%d (%c) entrou na fila.\n", novaResposicao.id, novaResposicao.tipo);
                }
                break;

            case 3: // Usar peça da Reserva (Nível Aventureiro)
                printf("\n[Acao: Usar Reserva]\n");
                Peca usada = pop(&pilha);
                if (usada.id != -1) { // Verifica se o pop foi bem-sucedido
                    printf("Peca da reserva ID:%d (%c) foi usada.\n", usada.id, usada.tipo);
                    // Nota: A fila não é reposta aqui, pois a peça não saiu dela.
                }
                break;

            case 4: // Trocar Frente(Fila) com Topo(Pilha) (Nível Mestre)
                printf("\n[Acao: Trocar Frente <-> Topo]\n");
                trocarFrenteTopo(&fila, &pilha);
                break;

            case 5: // Trocar 3 (Fila) com 3 (Pilha) (Nível Mestre)
                printf("\n[Acao: Trocar 3 (Fila) <-> 3 (Pilha)]\n");
                trocarTresComPilha(&fila, &pilha);
                break;

            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                break;
        }

        // Pausa para o usuário ler o resultado
        printf("\nPressione Enter para continuar...");
        // Limpa o buffer de entrada residual (do \n do scanf)
        while(getchar() != '\n'); 
        // Espera pelo próximo Enter
        getchar(); 
        system("clear || cls"); // Limpa a tela (Linux/Mac || Windows)
    }

    return 0;
}
