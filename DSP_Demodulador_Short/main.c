#include <stdio.h>
#include <string.h>
#include <cycles.h>

extern short demod_am(short entrada);

int main(int argc, char *argv[])
{
    cycle_stats_t stats;
    FILE *fin, *fout;
    short entrada, saida;

    printf("***************************************************************\n");
    printf("* DEMODULACAO AM PARA DSP BLACKFIN                            *\n");
    printf("***************************************************************\n\n");

    fin  = fopen("..\\am_input.pcm", "rb");
    if (!fin)
    {
        printf("Erro ao abrir entrada.\n");
        return 0;
    }

    fout = fopen("..\\am_output.pcm", "wb");
    if (!fout)
    {
        printf("Erro ao abrir saida.\n");
        return 0;
    }

    CYCLES_INIT(stats);

    printf("Processando...\n");

    while (fread(&entrada, sizeof(short), 1, fin) == 1)
    {
        CYCLES_START(stats);

        saida = demod_am(entrada);

        CYCLES_STOP(stats);

        fwrite(&saida, sizeof(short), 1, fout);
    }

    printf("Terminado.\n");

    CYCLES_PRINT(stats);

    fclose(fin);
    fclose(fout);

    return 0;
}
