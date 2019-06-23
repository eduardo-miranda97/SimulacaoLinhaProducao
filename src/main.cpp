#include "./include/input.h"
#include <time.h>

using namespace std;

int main(int argc, char* argv[]){
    if (argc != 4){
        printf("\nErro parametro, usar segundo sintaxe:\n\n");
        printf(" %s [input file] [result file] [seed rand]\n\n", argv[0]);
        exit(ERRO_INPUT_FILE_ARGS);
    }

    srand(stoul(argv[3]));
    input_args(argv[1]);
    start_simulation();

    if (0){
        std::cout << '\n';

        for (int i=0; i < QUEUE_WAIT; i++){
            printf("Fila [%d] = %ld\n", i, SM_queue_vasos[i].size());
        }

        std::cout << '\n';

        for (Artesao* artesao: SM_Artesao) {
            printf("Artesao ID: %4u \tociosidade: %5d\n", artesao->get_id(), artesao->get_time_ociosity());
        }

        std::cout << '\n';

        for (Especialista* especialista: SM_Especialista) {
            printf("Especialista ID: %4u\t  ociosidade: %5d\n", especialista->get_id(), especialista->get_time_ociosity());
        }

        std::cout << '\n';

        for (Vaso* vaso: SM_vaso_finish){
            printf("Vaso ID: %4u\tType: %c \tTempo total: %5u\n", vaso->get_id(), vaso->get_type(),vaso->get_end_time()-vaso->get_start_time());
        }

        std::cout << '\n';

        std::cout << "Time Final:\t"        << SM_final_time_simulation << '\n';
        std::cout << "Time atual:\t"        << SM_time_simulation      << '\n';
        std::cout << '\n';
        std::cout << "Vasos Gerados:  \t"   << last_id_vaso            << '\n';
        std::cout << "Vasos Finalizados:\t" << SM_vaso_finish.size()    << '\n';
        std::cout << '\n';
        printf("Massa: %u, Pedra: %u, Espaço: %u\n\n", SM_massa, SM_pedra, SM_espaco_secagem);
    }
    double ociosidade_art = 0;
    double ociosidade_esp = 0;
    double produt_art     = SM_time_simulation*SM_quatd_art;
    double produt_esp     = SM_time_simulation*SM_quatd_esp;

    for (Artesao* artesao: SM_Artesao){
        ociosidade_art += artesao->get_time_ociosity();
    }

    for (Especialista* especialista: SM_Especialista){
        ociosidade_esp += especialista->get_time_ociosity();
    }

    produt_art -= ociosidade_art;
    produt_esp -= ociosidade_esp;

    FILE* result_file = fopen(argv[2], "r+");

    if (!result_file){
        result_file = fopen(argv[2], "w+");
        if (!result_file){
          printf("\n================================================================================\n");
          printf("\n[ERRO] Ao tentar abrir arquivo %s em main()\n", argv[2]);
          printf("\n================================================================================\n");
          exit(ERRO_INPUT_FILE);
        }
        fprintf(result_file, "TEMP_SIMULACAO\tUSO_MASSA\tUSO_PEDRA\tPROD_ART\tPROD_ESP\tOCIOSID_ART\tOCIOSID_ESP\tNUM_VASOS\tSEED\n");
    }else{
        result_file = fopen(argv[2], "a+");
    }

    if (!result_file){
        printf("\n================================================================================\n");
        printf("\n[ERRO] Ao tentar abrir arquivo %s em main()\n", argv[2]);
        printf("\n================================================================================\n");
        exit(ERRO_INPUT_FILE);
    }

    fprintf(result_file, "%.2lf\t%u\t%u\t%.2lf\t%.2lf\t%.2lf\t%.2lf\t%ld\t%ld\n", SM_time_simulation, SM_massa_usado, SM_pedra_usado, produt_art,
                        produt_esp, ociosidade_art, ociosidade_esp, SM_vaso_finish.size(), stoul(argv[3]));

    fclose(result_file);
    return 0;
}
